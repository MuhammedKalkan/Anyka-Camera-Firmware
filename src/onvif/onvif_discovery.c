#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>		/* getopt_long() */
#include <fcntl.h>		/* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <asm/types.h>		/* for videodev2.h */
#include <linux/videodev2.h>
#include <sys/statfs.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>

#include "ak_common.h"
#include "ak_thread.h"
#include "ak_net.h"

#define MAXBUF (8192*2)
#define TEMP_STR_LEN 1024
#define PUERTO 3702
#define GRUPO "239.255.255.250"
#define GET_FAB_CODE     0x00
#define GET_CARD_CODE    0x01
#define GET_ID_CODE_H    0x02
#define GET_ID_CODE_L    0x03

static char *xml_version = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
static char *soap_head = "<env:Envelope xmlns:env=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:soapenc=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" xmlns:timg=\"http://www.onvif.org/ver20/imaging/wsdl\" xmlns:tev=\"http://www.onvif.org/ver10/events/wsdl\" xmlns:tptz=\"http://www.onvif.org/ver20/ptz/wsdl\" xmlns:tan=\"http://www.onvif.org/ver20/analytics/wsdl\" xmlns:tst=\"http://www.onvif.org/ver10/storage/wsdl\" xmlns:ter=\"http://www.onvif.org/ver10/error\" xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:tns1=\"http://www.onvif.org/ver10/topics\" xmlns:tmd=\"http://www.onvif.org/ver10/deviceIO/wsdl\" xmlns:wsdl=\"http://schemas.xmlsoap.org/wsdl\" xmlns:wsoap12=\"http://schemas.xmlsoap.org/wsdl/soap12\" xmlns:http=\"http://schemas.xmlsoap.org/wsdl/http\" xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:wsadis=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:xop=\"http://www.w3.org/2004/08/xop/include\" xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" xmlns:wsa=\"http://www.w3.org/2005/08/addressing\" xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" xmlns:wsrf-bf=\"http://docs.oasis-open.org/wsrf/bf-2\" xmlns:wsntw=\"http://docs.oasis-open.org/wsn/bw-2\" xmlns:wsrf-rw=\"http://docs.oasis-open.org/wsrf/rw-2\" xmlns:wsaw=\"http://www.w3.org/2006/05/addressing/wsdl\" xmlns:wsrf-r=\"http://docs.oasis-open.org/wsrf/r-2\" xmlns:tnshik=\"http://www.hikvision.com/2011/event/topics\">";

static int onvif_discovery_runflag = 0;
static ak_pthread_t discovery_th_tid = 0;

static int readip (char *addr)
{
	if (ak_net_get_ip("wlan0", addr))
		return -1;
	printf("ip=%s, len=%d\r\n", addr, strlen(addr));

	return 0;
}

static int readmac(char *addr)
{
	if (!addr)
		return -1;

	char mac_addr[18] = {0};
	char mac[13] = {0};
	/*
	 * here we get wlan0's mac addr.
	 * if success, mac_addr will store format like 00:11:22:33:44:55
	 * then we should strip : and store to 'addr'.
	 */
	if (ak_net_get_mac("wlan0", mac_addr, sizeof(mac_addr))) {
		printf("get net mac fail\n");
		return -1;
	} else {
		int r = sscanf(mac_addr, "%2c:%2c:%2c:%2c:%2c:%2c",
				&mac[0], &mac[2], &mac[4], &mac[6], &mac[8], &mac[10]);
		if (r != 6) {
			return -1;
		} else {
			memcpy(addr, mac, 13);
			printf("mac=%s, len=%d\r\n", addr, strlen(addr));
		}
	}
	return 0;
}

static int find_str_from_buf (const char *buf, int buflen,
		const char *findingstr, int findinglen)
{
	int i, j;
	int len = buflen - findinglen + 1;

	for (i = 0; i < len; i++){
		for (j = 0; j < findinglen; j++) {
			if (buf[i + j] != findingstr[j])
				break;
			if (j == findinglen - 1)
				return i;
		}
	}

	return -1;
}

static int get_message_id(char *buf, char *result)
{
	int buflen = strlen(buf);
	int pos = find_str_from_buf(buf,  buflen,
			"uuid:", strlen("uuid:"));
	if (pos < 0) {
		return -1;
	}

	int pos1 = find_str_from_buf(buf + pos, buflen - pos,
			"</", strlen("</"));
	if (pos1 < 0) {
		return -1;
	}
	memcpy(result, buf + pos + strlen("uuid:"),
			pos1-strlen("uuid:"));

	return 0;
}

static void *discovery_th(void*arg)
{
	int fd = 0, n, r;
	struct sockaddr_in srv, cli;
	struct ip_mreq mreq;
	char buf[MAXBUF];
	char buf1[MAXBUF];
	char msg_id_str[TEMP_STR_LEN];
	char uuid_str[TEMP_STR_LEN];
	char mac_str[TEMP_STR_LEN];
	char ip_str[TEMP_STR_LEN];
	int buflen;

	memset (&srv, 0, sizeof (struct sockaddr_in));
	memset (&cli, 0, sizeof (struct sockaddr_in));
	memset (&mreq, 0, sizeof (struct ip_mreq));

	srv.sin_family = AF_INET;
	srv.sin_port = htons (PUERTO);
	srv.sin_addr.s_addr=htonl(INADDR_ANY);

	if ((fd = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
		perror ("socket");
		goto exit;
	}
	if (bind (fd, (struct sockaddr *) &srv, sizeof (srv)) < 0){
		perror ("bind");
		goto exit;
	}

	if (inet_aton (GRUPO, &mreq.imr_multiaddr) < 0){
		perror ("inet_aton");
		goto exit;
	}

	memset(mac_str, 0, TEMP_STR_LEN);
	if (readmac (mac_str) < 0){
		sprintf (mac_str, "000011112222");
	}

	memset(ip_str, 0, TEMP_STR_LEN);
	readip(ip_str);

	memset(uuid_str, 0, TEMP_STR_LEN);
	sprintf(uuid_str, "316d4de6-a7b4-438b-8374-%s", mac_str);
	inet_aton (ip_str, &(mreq.imr_interface));
	if (setsockopt (fd, SOL_IP, IP_ADD_MEMBERSHIP,
				&mreq, sizeof (mreq)) < 0) {
		perror ("setsockopt");
		goto exit;
	}

	n = sizeof (cli);

	/*
	  FILE *fp;
	  fp = fopen("1.xml", "wb");
	 */
	while (onvif_discovery_runflag) {
		/* for select */
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);
		struct timeval tvout = {0, 500*1000};

		int sel_ret = select(fd + 1, &readfds, NULL, NULL, &tvout);
		switch (sel_ret) {
		case 0:
			/* time out */
			continue;
			break;
		case -1:
			/* error */
			onvif_discovery_runflag = 0;
			goto exit;
			break;
		default:
			if (!FD_ISSET(fd, &readfds))
				continue;
			break;
		}
		/* end of select */

		if ((r = recvfrom(fd, buf, MAXBUF, 0, 
						(struct sockaddr *)&cli, 
						(socklen_t *)&n)) < 0) {
			perror ("recvfrom");
		} else {
			buf[r] = 0;
			// fprintf (stdout, "Mensaje desde %s: %s", inet_ntoa (cli.sin_addr), buf);
			// printf ("sin_port=%d,r=%d\r\n", cli.sin_port, r);
			memset(msg_id_str, 0, TEMP_STR_LEN);
			get_message_id(buf, msg_id_str);
			//printf("msg_id=[%s]\r\n",msg_id_str);
			buflen = strlen(buf);

			if (find_str_from_buf (buf, buflen, ":NetworkVideoTransmitter",
						strlen(":NetworkVideoTransmitter"))>0){
				//printf("[%s]\r\n", "NetworkVideoTransmitter");
				memset(buf1, 0, MAXBUF);
				sprintf(buf1, "%s%s<env:Header><wsadis:MessageID>urn:uuid:%s"
						"</wsadis:MessageID><wsadis:RelatesTo>uuid:%s"
						"</wsadis:RelatesTo><wsadis:To>http://schemas.xmlsoap.org/"
						"ws/2004/08/addressing/role/anonymous</wsadis:To>"
						"<wsadis:Action>http://schemas.xmlsoap.org/ws/2005/04/"
						"discovery/ProbeMatches</wsadis:Action><d:AppSequence "
						"InstanceId=\"1372862456\" MessageNumber=\"1\"/>"
						"</env:Header><env:Body><d:ProbeMatches><d:ProbeMatch>"
						"<wsadis:EndpointReference><wsadis:Address>urn:uuid:%s"
						"</wsadis:Address></wsadis:EndpointReference><d:Types>"
						"dn:NetworkVideoTransmitter tds:Device</d:Types>"
						"<d:Scopes>onvif://www.onvif.org/type/video_encoder "
						"onvif://www.onvif.org/type/ptz onvif://www.onvif.org/"
						"type/audio_encoder onvif://www.onvif.org/location/city/"
						"shenzhen onvif://www.onvif.org/hardware/"
						"IPCAM_FARADAY_01 onvif://www.onvif.org/name/"
						"H.264_IPCamera onvif://www.onvif.org/Profile/Streaming"
						"</d:Scopes><d:XAddrs>http://%s/onvif/device_service"
						"</d:XAddrs><d:MetadataVersion>10</d:MetadataVersion>"
						"</d:ProbeMatch></d:ProbeMatches></env:Body>"
						"</env:Envelope>\r\n",
					xml_version,
					soap_head,
					uuid_str,
					msg_id_str,
					uuid_str,
					ip_str);
			} else if(find_str_from_buf (buf, buflen, ":Device",
						strlen(":Device"))>0){
				//printf("[%s]\r\n", "Device");
				sprintf(buf1, "%s%s<env:Header><wsadis:MessageID>urn:uuid:%s"
						"</wsadis:MessageID><wsadis:RelatesTo>uuid:%s</wsadis:RelatesTo>"
						"<wsadis:To>http://schemas.xmlsoap.org/ws/2004/08/addressing/"
						"role/anonymous</wsadis:To><wsadis:Action>http://"
						"schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches"
						"</wsadis:Action><d:AppSequence InstanceId=\"1372862456\" "
						"MessageNumber=\"2\"/></env:Header><env:Body><d:ProbeMatches>"
						"<d:ProbeMatch><wsadis:EndpointReference><wsadis:Address>"
						"urn:uuid:%s</wsadis:Address></wsadis:EndpointReference>"
						"<d:Types>dn:NetworkVideoTransmitter tds:Device</d:Types>"
						"<d:Scopes>onvif://www.onvif.org/type/video_encoder onvif://"
						"www.onvif.org/type/ptz onvif://www.onvif.org/type/"
						"audio_encoder onvif://www.onvif.org/location/city/"
						"shenzhen onvif://www.onvif.org/hardware/IPCAM_FARADAY_01 "
						"onvif://www.onvif.org/name/H.264_IPCamera onvif://"
						"www.onvif.org/Profile/Streaming</d:Scopes><d:XAddrs>http://"
						"%s/onvif/device_service</d:XAddrs><d:MetadataVersion>10"
						"</d:MetadataVersion></d:ProbeMatch></d:ProbeMatches>"
						"</env:Body></env:Envelope>\r\n",
						xml_version,
						soap_head,
						uuid_str,
						msg_id_str,
						uuid_str,
						ip_str);
			}
			else if(find_str_from_buf (buf, buflen, "Types />",
						strlen("Types />"))>0){
				//printf("[%s]\r\n", "Types />");
				sprintf(buf1, "%s%s<env:Header><wsadis:MessageID>urn:uuid:%s"
						"</wsadis:MessageID><wsadis:RelatesTo>uuid:%s</wsadis:RelatesTo>"
						"<wsadis:To>http://schemas.xmlsoap.org/ws/2004/08/addressing/"
						"role/anonymous</wsadis:To><wsadis:Action>http://schemas."
						"xmlsoap.org/ws/2005/04/discovery/ProbeMatches</wsadis:Action>"
						"<d:AppSequence InstanceId=\"1372862456\" MessageNumber=\"1\"/>"
						"</env:Header><env:Body><d:ProbeMatches><d:ProbeMatch>"
						"<wsadis:EndpointReference><wsadis:Address>urn:uuid:%s"
						"</wsadis:Address></wsadis:EndpointReference><d:Types>"
						"dn:NetworkVideoTransmitter tds:Device</d:Types><d:Scopes>"
						"onvif://www.onvif.org/type/video_encoder onvif://www.onvif.org/"
						"type/ptz onvif://www.onvif.org/type/audio_encoder "
						"onvif://www.onvif.org/location/city/shenzhen onvif://"
						"www.onvif.org/hardware/IPCAM_FARADAY_01 onvif://"
						"www.onvif.org/name/H.264_IPCamera onvif://www.onvif.org/"
						"Profile/Streaming</d:Scopes><d:XAddrs>http://%s/onvif/"
						"device_service</d:XAddrs><d:MetadataVersion>10"
						"</d:MetadataVersion></d:ProbeMatch></d:ProbeMatches>"
						"</env:Body></env:Envelope>\r\n",
						xml_version,
						soap_head,
						uuid_str,
						msg_id_str,
						uuid_str,
						ip_str);
			}
			sendto (fd, buf1, strlen(buf1), 0,
					(struct sockaddr *) &cli, sizeof (cli));
		}
	}
exit:
	if (fd)
		close(fd);
	ak_thread_exit();
	return NULL;
}

void onvif_discovery_init(void)
{
	onvif_discovery_runflag = 1;
	ak_print_normal_ex("start onvif discovery service\n");
	ak_thread_create(&discovery_th_tid, discovery_th,
			NULL, ANYKA_THREAD_MIN_STACK_SIZE, -1);
}

void onvif_discovery_exit(void)
{
	ak_print_normal_ex("onvif discovery service stopping\n");
	onvif_discovery_runflag = 0;
	kill(discovery_th_tid, SIGINT);
	ak_thread_join(discovery_th_tid);
	ak_print_normal_ex("onvif discovery service exited\n");
}
