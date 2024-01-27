#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "dvs.h"
#include "onvif_service.h"

#include "ak_common.h"
#include "ak_net.h"
#include "ak_global.h"
#include "ak_venc.h"
#include "ak_vi.h"
#include "ak_osd.h"
#include "ak_cmd_exec.h"
#include "onvif_demo.h"

static struct onvif_param *service_param = NULL;
static struct video_handle_t onvif_handle = {0};

#define IP_INIT_FROM_STRING(IP, IPSTR) \
	{ \
		int tmp[4]; \
		IP[0] = 0; \
		IP[1] = 0; \
		IP[2] = 0; \
		IP[3] = 0; \
		if(sscanf(IPSTR, "%d.%d.%d.%d", &tmp[0], &tmp[1], &tmp[2], &tmp[3]) == 4) \
		{ \
			IP[0] = tmp[0]; \
			IP[1] = tmp[1]; \
			IP[2] = tmp[2]; \
			IP[3] = tmp[3]; \
		}else{\
			printf("ERR: parse ip address failed!\n");\
		}\
	}


/**
 * NAME        IniReadString
 * @BRIEF	read config ini file string

 * @PARAM	const char *section, section name
 const char *indent,	indent name
 const char *defaultresult,  default result when not found the string
 char *result, pointer to result
 int resultlen, lengh of result
 char *inifilename, INI file name
 * @RETURN	int
 * @RETVAL	if return 0 success, otherwise failed
 */
int IniReadString(const char *section, const char *indent,
		const char *defaultresult, char *result, int resultlen,
		char *inifilename)
{
	FILE *fp;
	struct stringlist *head, *node, *p, *t;
	char tempstr[MAXSTRLEN];
	char sectionstr[MAXSTRLEN];
	int i, n, len;
	int sectionfinded;
	bzero (sectionstr, MAXSTRLEN);
	sprintf (sectionstr, "[%s]", section);
	fp = fopen (inifilename, "rt");
	if (fp == NULL) {
		strcpy (result, defaultresult);
		return 0;
	}
	head = (struct stringlist *)calloc(1, sizeof (struct stringlist));
	p = head;
	bzero (tempstr, MAXSTRLEN);
	while (fgets (tempstr, MAXSTRLEN, fp) != NULL) {
		node = (struct stringlist *)calloc(1, sizeof (struct stringlist));
		node->next = NULL;
		bzero (node->string, MAXSTRLEN);
		strcpy (node->string, tempstr);
		p->next = node;
		p = p->next;
		bzero (tempstr, MAXSTRLEN);
	}
	fclose (fp);
	p = head;
	while (p->next != NULL) {
		t = p->next;
		len = strlen (t->string);
		bzero (tempstr, MAXSTRLEN);
		n = 0;
		for (i = 0; i < len; i++) {
			if ((t->string[i] != '\r') && (t->string[i] != '\n')
					&& (t->string[i] != ' ')) {
				tempstr[n] = t->string[i];
				n++;
			}
		}
		if (strlen (tempstr) == 0) {
			p->next = t->next;
			free(t);
		} else {
			bzero (t->string, MAXSTRLEN);
			strcpy (t->string, tempstr);
			p = p->next;
		}
	}
	p = head;
	sectionfinded = 0;
	while (p->next != NULL) {
		p = p->next;
		if (sectionfinded == 0) {
			if (strcmp (p->string, sectionstr) == 0)
				sectionfinded = 1;
		} else {
			if (p->string[0] == '[') {
				strcpy (result, defaultresult);
				goto end;
			} else {
				if (strncmp (p->string, indent, strlen (indent)) == 0) {
					if (p->string[strlen (indent)] == '=') {
						strncpy (result, p->string + strlen (indent) + 1, resultlen);
						goto end;
					}
				}
			}
		}
	}
	strcpy (result, defaultresult);
end:
	while (head->next != NULL) {
		t = head->next;
		head->next = t->next;
		free(t);
	}
	free(head);
	return 0;
}

/**
 * NAME        IniReadInteger
 * @BRIEF	read config ini file interger

 * @PARAM	const char *section, section name
 const char *indent,	indent name
 const char *defaultresult,  default result when not found the string
 char *inifilename, INI file name
 * @RETURN	int
 * @RETVAL	if return 0 success, otherwise failed
 */
int IniReadInteger(const char *section, const char *indent, int defaultresult,
		char *inifilename)
{
	int i, len;
	char str[1024];
	IniReadString (section, indent, "NULL", str, 1024, inifilename);
	if (strcmp (str, "NULL") == 0)
	{
		return defaultresult;
	}
	len = strlen (str);
	if (len == 0)
	{
		return defaultresult;
	}
	for (i = 0; i < len; i++)
	{
		if ((*(str + i) < '0') || (*(str + i) > '9'))
		{
			return defaultresult;
		}
	}
	return atoi (str);
}

/**
 * NAME        IniWriteString
 * @BRIEF	write config ini file string

 * @PARAM	const char *section, section name
 const char *indent,	indent name
 const char *value, value which you want to write
 char *inifilename, INI file name
 * @RETURN	int
 * @RETVAL	if return 0 success, otherwise failed
 */
int IniWriteString(const char *section, const char *indent, const char *value,
		char *inifilename)
{
	FILE *fp;
	struct stringlist *head, *node, *p, *t;
	char tempstr[MAXSTRLEN];
	char sectionstr[MAXSTRLEN];
	int i, n, len;
	int sectionfinded;
	bzero (sectionstr, MAXSTRLEN);
	sprintf (sectionstr, "[%s]", section);
	fp = fopen (inifilename, "rt");
	if (fp == NULL)
	{
		fp = fopen (inifilename, "wt");
		fputs (sectionstr, fp);
		fputs ("\r\n", fp);
		bzero (tempstr, MAXSTRLEN);
		sprintf (tempstr, "%s=%s", indent, value);
		fputs (tempstr, fp);
		fputs ("\r\n", fp);
		fclose (fp);
		return 0;
	}
	head = (struct stringlist *)calloc(1, sizeof (struct stringlist));
	p = head;
	while (!feof (fp))
	{
		node = (struct stringlist *)calloc(1, sizeof (struct stringlist));
		node->next = NULL;
		bzero (node->string, MAXSTRLEN);
		fgets (node->string, MAXSTRLEN, fp);
		p->next = node;
		p = p->next;
	}
	fclose (fp);
	p = head;
	while (p->next != NULL)
	{
		t = p->next;
		len = strlen (t->string);
		bzero (tempstr, MAXSTRLEN);
		n = 0;
		for (i = 0; i < len; i++)
		{
			if ((t->string[i] != '\r') && (t->string[i] != '\n')
					&& (t->string[i] != ' '))
			{
				tempstr[n] = t->string[i];
				n++;
			}
		}
		if (strlen (tempstr) == 0)
		{
			p->next = t->next;
			free(t);
		}
		else
		{
			bzero (t->string, MAXSTRLEN);
			strcpy (t->string, tempstr);
			p = p->next;
		}
	}
	p = head;
	sectionfinded = 0;
	while (p->next != NULL)
	{
		t = p;
		p = p->next;
		if (sectionfinded == 0)
		{
			if (strcmp (p->string, sectionstr) == 0)
			{
				sectionfinded = 1;
				node = (struct stringlist *)calloc(1, sizeof (struct stringlist));
				bzero (node->string, MAXSTRLEN);
				sprintf (node->string, "%s=%s", indent, value);
				node->next = p->next;
				p->next = node;
				p = p->next;
			}
		}
		else
		{
			if (p->string[0] == '[')
			{
				goto end;
			}
			else
			{
				if (strncmp (p->string, indent, strlen (indent)) == 0)
				{
					if (p->string[strlen (indent)] == '=')
					{
						node = p;
						t->next = p->next;
						free(node);
						goto end;
					}
				}
			}
		}
	}
end:
	fp = fopen (inifilename, "wt");
	p = head;
	while (p->next != NULL)
	{
		p = p->next;
		fputs (p->string, fp);
		fputs ("\r\n", fp);
	}
	if (sectionfinded == 0)
	{
		fputs (sectionstr, fp);
		fputs ("\r\n", fp);
		bzero (tempstr, MAXSTRLEN);
		sprintf (tempstr, "%s=%s", indent, value);
		fputs (tempstr, fp);
		fputs ("\r\n", fp);
	}
	fclose (fp);
	while (head->next != NULL)
	{
		t = head->next;
		head->next = t->next;
		free(t);
	}
	free(head);
	return 0;
}

/**
 * NAME        IniWriteInteger
 * @BRIEF	write config ini file interger

 * @PARAM	const char *section, section name
 const char *indent,	indent name
 const char *value, value which you want to write
 char *inifilename, INI file name
 * @RETURN	int
 * @RETVAL	if return 0 success, otherwise failed
 */
int IniWriteInteger(const char *section, const char *indent, int value,
		char *inifilename)
{
	char str[20];
	memset (str, 0, 20);
	sprintf (str, "%d", value);
	IniWriteString (section, indent, str, inifilename);
	return 0;
}

int is_valid_ipaddr(unsigned char *ip)
{
	if((NULL == ip)
		|| (4 > ip[0]) || (127 == ip[0]) || (224 <= ip[0]) || (0 > ip[1]) || (255 < ip[1])
		|| (0 > ip[2]) || (255 < ip[2]) || (0 >= ip[3]) || (255 <= ip[3])) {
		ak_print_error_ex("invalid ip, %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
		return 0;
	}

	return 1;
}

int is_valid_netmask(unsigned char *ipaddr)
{
	unsigned int b = 0, i;

	if (NULL != ipaddr) {
		unsigned char n[4] = {ipaddr[3], ipaddr[2], ipaddr[1], ipaddr[0]};

		for (i = 0; i < 4; ++i)
			b += n[i] << (i * 8);
		b = ~b + 1;
		if ((b & (b - 1)) == 0)
			return 1;
	}

	return 0;
}

int check_net_parm(unsigned char *ip, unsigned char *mask, unsigned char *gw)
{
    if (NULL == ip || NULL == mask || NULL == gw ) {
        ak_print_error_ex("parm null\n");
        return 0;
    }

    if (!is_valid_ipaddr(ip)) {
        return 0;
    }


    if ((0 == mask[0]) && (0 == mask[1]) && (0 == mask[2]) && (0 == mask[3])) {
        ak_print_error_ex("mask is all 0\n");
        return 0;
    }

    if ((255 == mask[0]) && (255 == mask[1]) &&
			(255 == mask[2]) && (255 == mask[3])) {
        ak_print_error_ex("mask is all 255\n");
        return 0;
    }

	if (!is_valid_netmask(mask)) {
        ak_print_error_ex("invalid netmask: %d.%d.%d.%d\r\n",
			   	mask[0], mask[1], mask[2], mask[3]);
        return 0;
	}

	if (!is_valid_ipaddr(gw)) {
        ak_print_error_ex("invalid gateway: %d.%d.%d.%d\r\n",
				gw[0], gw[1], gw[2], gw[3]);
		return 0;
	}


    if (((ip[0] & mask[0]) != (gw[0] & mask[0]))
    || ((ip[1] & mask[1]) != (gw[1] & mask[1]))
    || ((ip[2] & mask[2]) != (gw[2] & mask[2]))
    || ((ip[3] & mask[3]) != (gw[3] & mask[3]))) {
        ak_print_error_ex("ip and gw are not in the same network section\n");

        gw[0] = (mask[0] == 255) ? ip[0]: gw[0];
		gw[1] = (mask[1] == 255) ? ip[1]: gw[1];
		gw[2] = (mask[2] == 255) ? ip[2]: gw[2];

		ak_print_error_ex("modify gw to %d.%d.%d.%d\n", gw[0], gw[1], gw[2], gw[3]);
    }

    return 1;
}

unsigned int get_dhcp_status(void)
{
	char buf[10] = {0};

	int fd = open("/tmp/dhcp_status", O_RDONLY);
	if(fd < 0)
		return 0;

	read(fd, buf, 1);
	close(fd);
	ak_print_normal_ex("dhcp: %d\n", atoi(buf));

	return atoi(buf);
}

/* set onvif net param to config file */
static void set_net_param_to_file(struct netinfo_t *resp)
{
    if (service_param->net_cb) {
        struct onvif_net_param param = {0};

    	param.dhcp = resp->IpType;
    	strcpy(param.ipaddr, resp->cIp);
    	strcpy(param.netmask, resp->cNetMask);
    	strcpy(param.gateway, resp->cGateway);
    	strcpy(param.firstdns, resp->cMDns);
    	strcpy(param.backdns, resp->cSDns);

    	service_param->net_cb(&param);
    }
}

/**
 * NAME        GetNet
 * @BRIEF	get net data

 * @PARAM	struct  netinfo_t *resp, strore info
 * @RETURN	int
 * @RETVAL	if return 0 success, otherwise failed
 */
int GetNet(struct  netinfo_t *resp)
{
	/* for readmac in libonvif.a */
	system("ifconfig > /tmp/eth_msg");

	unsigned int dhcp = get_dhcp_status();
	resp->IpType = dhcp;
	resp->DnsType = dhcp;

	char ip[16] = {0}, netmask[16] = {0}, route[16] = {0},
		 mdns[16] = {0}, sdns[16] = {0};

	if (ak_net_get_ip("wlan0", ip)) {
		ak_print_error_ex("get wlan0's ip failed\n");
		return -1;
	}
	if (ak_net_get_netmask("wlan0", netmask)) {
		ak_print_error_ex("get wlan0's netmask failed\n");
		return -1;
	}
	if (ak_net_get_route("wlan0", route)) {
		ak_print_error_ex("get route failed\n");
		return -1;
	}
	if (ak_net_get_dns(0, mdns)) {
		strcpy(mdns, route);
	}
	if (ak_net_get_dns(1, sdns)) {
		strcpy(sdns, mdns);
	}

	ak_print_normal_ex("ip: %s, netmask: %s, route: %s, dns1: %s, dns2: %s\n",
			ip, netmask, route, mdns, sdns);

	sprintf(resp->cIp, "%s", ip);
	sprintf(resp->cNetMask, "%s", netmask);
	sprintf(resp->cGateway, "%s", route);
	sprintf(resp->cMDns, "%s", mdns);
	sprintf(resp->cSDns, "%s", sdns);

	resp->chttpPort = 80;

	return 0;
}

/**
 * NAME  SetNet
 * @BRIEF	set net data
 * @PARAM	struct  netinfo_t *resp, info you want to set
 * @RETURN	int
 * @RETVAL  0
 */
int SetNet(struct netinfo_t *resp)
{
	unsigned char ip[4] = {0};
	unsigned char mask[4] = {0};
	unsigned char gw[4] = {0};

	ak_print_normal_ex("dhcp: %d, ip: %s, netmask: %s, gateway: %s,"
			" dns1: %s, dns2: %s\n",
			resp->IpType,
			resp->cIp,
			resp->cNetMask,
			resp->cGateway,
			resp->cMDns,
			resp->cSDns);

	IP_INIT_FROM_STRING(ip, resp->cIp);
	IP_INIT_FROM_STRING(mask, resp->cNetMask);
	IP_INIT_FROM_STRING(gw, resp->cGateway);

	if (!check_net_parm(ip, mask, gw))
		return -1;

    /* set onvif net param to config file */
    set_net_param_to_file(resp);

	/*
	 * set net on run-time
	 */
	char iface[10] = {0};

	/* get run interface */
	if (ak_net_get_cur_iface(iface)) {
		ak_print_error_ex("net was not working\n");
		return -1;
	}

	/* set ip and netmask */
	ak_net_set_ip(iface, resp->cIp);
	ak_net_set_netmask(iface, resp->cNetMask);

	/* set default gataway */
	if (ak_net_set_default_gateway(resp->cGateway))
		ak_print_error_ex("set default gw failed\n");
	else
		ak_print_notice_ex("set default gw success\n");

	ak_cmd_exec("killall discovery; discovery &", NULL, 0);

	return 0;
}

/**
 * NAME        init_network
 * @BRIEF	init net

 * @PARAM	dvs_t * dvs, strore info
 * @RETURN	int
 * @RETVAL	0
 */
int init_network(dvs_t * dvs)
{
	dvs->video_width = 1280;
	dvs->video_height = 720;//camera_get_ch1_height();
	dvs->video_width_small = 640;
	dvs->video_height_small = 480;//camera_get_ch2_height();
	return 0;
}

/**
 * NAME        hal_set_brightness
 * @BRIEF	set brightness

 * @PARAM	dvs_t *dvs,
 int index,
 int brightness
 * @RETURN	int
 * @RETVAL	0
 */
int hal_set_brightness(dvs_t *dvs, int index, int brightness)
{
	// SetBrightness(brightness / 15);//SetBrightness(0);
	//camera_set_brightness(brightness);
	return 0;
}

/**
 * NAME        hal_set_contrast
 * @BRIEF	set contrast

 * @PARAM	dvs_t *dvs,
 int index,
 int brightness
 * @RETURN	int
 * @RETVAL	0
 */
int hal_set_contrast(dvs_t *dvs, int index, int contrast)
{
	// SetGAMMA(contrast / 15);//SetGAMMA(0);
	//camera_set_gamma(contrast);
	return 0;
}

/**
 * NAME        hal_set_saturation
 * @BRIEF	set saturation

 * @PARAM	dvs_t *dvs,
 int index,
 int brightness
 * @RETURN	int
 * @RETVAL	0
 */
int hal_set_saturation(dvs_t *dvs, int index, int saturation)
{
	// SetSATURATION(saturation / 15);//SetSATURATION(0);
	//camera_set_saturation(saturation);
	return 0;
}

/* set onvif video param to config file */
static void set_video_param_to_file(int flage)
{
    if (service_param->video_cb) {
        struct onvif_video_param param = {0};

        param.channel = flage;
        if (VIDEO_CHN_MAIN == param.channel) {
            param.main_kbps = service_param->main_kbps;
    		param.main_fps = service_param->main_fps;
    		param.main_width = service_param->main_width;
    		param.main_height = service_param->main_height;
        } else {
            param.sub_kbps = service_param->sub_kbps;
    		param.sub_fps = service_param->sub_fps;
    		param.sub_width = service_param->sub_width;
    		param.sub_height = service_param->sub_height;
        }

    	service_param->video_cb(&param);
    }
}

/**
 * NAME        GetVideoPara
 * @BRIEF	get vadie para

 * @PARAM	int *fps,  fps
     int *bitrate, bitrate
     int *quality, quality
     int *flage, flage
     int *width, width
     int *height, height
     int main_small, main or small
 * @RETURN	int
 * @RETVAL	0
 */
int GetVideoPara(int *fps, int *bitrate, int *quality, int *flage,
		int *width, int *height, int main_small)
{
	if (main_small) {
	    *fps = service_param->sub_fps;
		*bitrate = service_param->sub_kbps;
		int sub_width = service_param->sub_width;
		int sub_height = service_param->sub_height;

		ak_print_normal_ex("onvif get video param w: %d, h: %d\n",
			sub_width, sub_height);

		IniWriteInteger ("small", "width", sub_width, "/etc/jffs2/profile.ini");
		IniWriteInteger ("small", "height", sub_height, "/etc/jffs2/profile.ini");
		//quality && flage saved to profile.ini
		*quality = IniReadInteger ("small", "quality", 2, "/etc/jffs2/profile.ini");
		*flage = IniReadInteger ("small", "flage", 1, "/etc/jffs2/profile.ini");
		*width = IniReadInteger ("small", "width", 640, "/etc/jffs2/profile.ini");
		*height = IniReadInteger ("small", "height", 480, "/etc/jffs2/profile.ini");
	} else {
        *fps = service_param->main_fps;
		*bitrate = service_param->main_kbps;
		int main_width = service_param->main_width;
		int main_height = service_param->main_height;

		ak_print_normal_ex("onvif get video param w: %d, h: %d\n",
			main_width, main_height);

		IniWriteInteger ("main", "width", main_width, "/etc/jffs2/profile.ini");
		IniWriteInteger ("main", "height", main_height, "/etc/jffs2/profile.ini");

		//quality && flage saved to profile.ini
		*quality = IniReadInteger ("main", "quality", 2, "/etc/jffs2/profile.ini");
		*flage = IniReadInteger ("main", "flage", 0, "/etc/jffs2/profile.ini");
		*width = IniReadInteger ("main", "width", 1280, "/etc/jffs2/profile.ini");
		*height = IniReadInteger ("main", "height", 720, "/etc/jffs2/profile.ini");
	}

	return 0;
}

/**
 * NAME        SetVideoPara
 * @BRIEF	set vadie para

 * @PARAM	int *fps,  fps
 int *bitrate, bitrate
 int *quality, quality
 int *flage, flage
 int *width, width
 int *height, height
 int main_small, main or small
 * @RETURN	int
 * @RETVAL	0
 */
int SetVideoPara(int fps, int bitrate, int quality,
		int width, int height, int flage)
{
	if (!onvif_handle.onvif_server_runflag)
		return -1;

	struct video_channel_attr attr;

	/* flage indicate main or sub channel */
	ak_print_normal_ex("fps: %d, bitrate: %d, quality: %d,"
			" w: %d, h: %d, flage: %d\n",
			fps,
			bitrate,
		   	quality,
		   	width,
			height,
		   	flage);

	/* main channel */
	if (flage == 0) {
		service_param->main_kbps = bitrate;
		service_param->main_fps = fps;
		if (onvif_handle.main_chn) {
			ak_venc_set_rc(onvif_handle.main_chn, bitrate);
			ak_venc_set_fps(onvif_handle.main_chn, fps);
		}

		service_param->main_width = width;
		service_param->main_height = height;

		//quality && flage saved to profile.ini
		IniWriteInteger ("main", "quality", quality,"/etc/jffs2/profile.ini");
		IniWriteInteger ("main", "flage", flage,"/etc/jffs2/profile.ini");
		IniWriteInteger ("main", "width", width, "/etc/jffs2/profile.ini");
		IniWriteInteger ("main", "height", height, "/etc/jffs2/profile.ini");
	} else {
		service_param->sub_kbps = bitrate;
		service_param->sub_fps = fps;
		if (onvif_handle.sub_chn) {
			ak_venc_set_rc(onvif_handle.sub_chn, bitrate);
			ak_venc_set_fps(onvif_handle.sub_chn, fps);
		}

		service_param->sub_width = width;
		service_param->sub_height = height;

		//quality && flage saved to profile.ini
		IniWriteInteger ("small", "quality", quality,"/etc/jffs2/profile.ini");
		IniWriteInteger ("small", "flage", flage,"/etc/jffs2/profile.ini");
		IniWriteInteger ("small", "width", width, "/etc/jffs2/profile.ini");
		IniWriteInteger ("small", "height", height, "/etc/jffs2/profile.ini");
	}

	/* vi resolution change */
	memset(&attr, 0, sizeof(struct video_channel_attr));
	ak_vi_get_channel_attr(onvif_handle.vi_handle, &attr);
	if (attr.res[flage].width != width || attr.res[flage].height != height) {
		ak_print_notice_ex("\n\tWant change resolution w: %d, h: %d\n\n",
				width, height);
		if (onvif_handle.vi_handle) {
			if (onvif_handle.stop_func) {
				ak_print_notice_ex("going to stop onvif\n");
				onvif_handle.stop_func();
			}

			attr.res[flage].width = width;
			attr.res[flage].height = height;

			int ret = ak_vi_change_channel_attr(onvif_handle.vi_handle, &attr);
			/* change resolution ok, store config to ini */
			if (!ret) {
				if (flage == 0) {
					service_param->main_width = width;
					service_param->main_height = height;
					ak_print_notice_ex("store to ini, w: %d, h: %d\n",
					    width, height);
				} else {
					service_param->sub_width = width;
					service_param->sub_height = height;
					ak_print_notice_ex("store to ini, w: %d, h: %d\n",
					    width, height);
				}
			}

			if (onvif_handle.start_func) {
				ak_print_notice_ex("going to start onvif\n");
				onvif_handle.start_func(onvif_handle.vi_handle);
			}
		}
	}

    set_video_param_to_file(flage);

	return 0;
}

/**
 * NAME        config_load_videocolor
 * @BRIEF	load video recorder config

 * @PARAM	config_videocolor_t * videocolor, store info
 int index, intex
 * @RETURN	int
 * @RETVAL	0
 */
int config_load_videocolor(config_videocolor_t * videocolor, int index)
{
	char tmpstr[MAX_NAME_LEN] = {0};
	sprintf(tmpstr, "%d", index + 1);
	videocolor->brightness = IniReadInteger(tmpstr,
		   	"brightness", 50, "/etc/jffs2/videocolor.ini");
	videocolor->contrast = IniReadInteger(tmpstr,
		   	"contrast", 50, "/etc/jffs2/videocolor.ini");
	videocolor->saturation =IniReadInteger(tmpstr,
		   	"saturation", 50, "/etc/jffs2/videocolor.ini");
	videocolor->hue = IniReadInteger(tmpstr,
		   	"hue", 50, "/etc/jffs2/videocolor.ini");
	return 0;
}

/**
 * NAME        get_onvif_utf8
 * @BRIEF	change word to utf-8 type word

 * @PARAM	char *out,  out buf pointer
 char *in, input buf pointer

 * @RETURN	void
 * @RETVAL
 */
void get_onvif_utf8(char *out, char *in)
{
	int i, j;
	for(i = 0, j = 0; i < strlen(in); )
	{
		if(in[i] == '%')
		{
			i ++;
			char tmp = in[i ++];
			if(tmp >= 'A')
			{
				out[j ++] = tmp - 'A' + 10;
			}
			else
			{
				out[j ++] = tmp - '0';
			}
			if(in[i] != '%')
			{
				tmp = in[i ++];
				out[j - 1] = (out[j - 1] << 4) ;
				if(tmp >= 'A')
				{
					tmp = tmp - 'A' + 10;
				}
				else
				{
					tmp = tmp - '0';
				}
				out[j - 1] |= tmp;
			}
		}
		else
		{
			out[j ++] = in[i++];
		}
	}
	out[j] = 0;
}


/**
 * NAME        onvif_format_gb_to_Jovision__code
 * @BRIEF	change gb code to jovision code

 * @PARAM	unsigned char *GB_code, pointer to GB code
 int gb_len, gb code len
 unsigned char *Jovisio_code, out put buf pointer

 * @RETURN	void
 * @RETVAL
 */
void onvif_format_gb_to_Jovision__code(unsigned char *GB_code,
	   	int gb_len,unsigned char *Jovision_code)
{
	int i;
	unsigned char *ptr = Jovision_code;

	for(i = 0; i < gb_len; i++)
	{
		if(*GB_code < 0x80)
		{
			*ptr = *GB_code;
		}
		else if(*GB_code < 0xC0)
		{
			*ptr = 0xc2;
			ptr ++;
			*ptr = *GB_code;
		}
		else
		{
			*ptr = 0xc3;
			ptr ++;
			*ptr = *GB_code - 0x40;
		}
		ptr ++;
		GB_code ++;
	}
	*ptr = 0;
}


/**
 * NAME        onvif_format_Jovision_to__gb_code
 * @BRIEF	changejovision code  to gb code

 * @PARAM	unsigned char *GB_code, pointer to GB code
 unsigned char *Jovisio_code, input buf pointer
 int Jovision_len, jovision code len

 * @RETURN	void
 * @RETVAL
 */
void onvif_format_Jovision_to__gb_code(unsigned char *GB_code,
		unsigned char *Jovision_code, int Jovision_len)
{
	int i;

	for(i = 0; i < Jovision_len; i++)
	{
		if(*Jovision_code == 0xc2)
		{
			Jovision_code ++;
			*GB_code = *Jovision_code;
		}
		else if(*Jovision_code == 0xc3)
		{
			Jovision_code ++;
			*GB_code = *Jovision_code + 0x40;
		}
		else
		{
			*GB_code = *Jovision_code;
		}
		Jovision_code ++;
		GB_code ++;
	}
	*GB_code = 0;
}

/**
 * NAME        GetOnvifData
 * @BRIEF	get onvfi config

 * @PARAM	onvif_data_t *onvif_data

 * @RETURN	void
 * @RETVAL
 */
int GetOnvifData(onvif_data_t *onvif_data)
{
	unsigned char gb_code[100];

	//Pcamera_disp_setting camera_info;
	//camera_info = anyka_get_camera_info();

	//font_unicode_to_gb(camera_info->osd_unicode_name, gb_code);

	onvif_format_gb_to_Jovision__code(gb_code, strlen((char *)gb_code),
		   	(unsigned char *)onvif_data->identification_name);
	//IniReadString ("onvif_data", "if_name", "H264_IPCAM",
	//onvif_data->identification_name, 256,"/etc/jffs2/onvif_data.ini");
	IniReadString ("onvif_data", "if_location", "shenzhen",
		   	onvif_data->identification_location, 256, "/etc/jffs2/onvif_data.ini");

	return 0;
}

/**
 * NAME        SetOnvifData
 * @BRIEF	set onvfi config

 * @PARAM	onvif_data_t *onvif_data

 * @RETURN	void
 * @RETVAL
 */
int SetOnvifData(onvif_data_t *onvif_data)
{
	unsigned char gb_code[100];
#if 0
	//unsigned short un_code[100];
	//int un_len;

	//Pcamera_disp_setting camera_info;
	//camera_info = anyka_get_camera_info();
	//char *osd_name = "Test onvif";

	//get_onvif_utf8(camera_info->osd_name, onvif_data->identification_name);
#endif
	onvif_format_Jovision_to__gb_code(gb_code,
			(unsigned char *)onvif_data->identification_name,
		   	strlen(onvif_data->identification_name));
#if 0
	un_len = font_gb_to_unicode(gb_code, un_code);
	font_unicode_to_utf8(un_code, un_len, (unsigned char *)camera_info->osd_name, 50);
	ak_print_normal("camera_info->osd_name osd name:%s\n", camera_info->osd_name);
	anyka_set_camera_info(camera_info);
	IniWriteString ("onvif_data", "if_name",
			onvif_data->identification_name,
			"etc/jffs2/onvif_data.ini");
#endif
	IniWriteString("onvif_data", "if_location",
			onvif_data->identification_location,
			"/etc/jffs2/onvif_data.ini");

	return 0;
}

static void show_onvif_video_param(void)
{
    ak_print_normal("*** %s ***\n", __func__);
	ak_print_normal("main: width=%d, height=%d\n",
		service_param->main_width, service_param->main_height);
	ak_print_normal("sub: width=%d, height=%d\n",
		service_param->sub_width, service_param->sub_height);

	ak_print_normal("main_fps=%d, main_kbps=%d\n",
		service_param->main_fps, service_param->main_kbps);
	ak_print_normal("sub_fps=%d, sub_kbps=%d\n",
		service_param->sub_fps, service_param->sub_kbps);
	ak_print_normal("*** %s End ***\n\n", __func__);
}

int onvif_server_init_video_param(struct onvif_param *param)
{
    service_param = param;
    show_onvif_video_param();

    return AK_SUCCESS;
}

int onvif_server_register_handle(struct video_handle_t *video)
{
	if (video && video->vi_handle)
		onvif_handle.vi_handle = video->vi_handle;
	else {
		ak_print_error_ex("Invalid arguments\n");
		return AK_FAILED;
	}

	onvif_handle.main_chn = video->main_chn;
	onvif_handle.sub_chn = video->sub_chn;
	onvif_handle.start_func = video->start_func;
	onvif_handle.stop_func = video->stop_func;
	onvif_handle.onvif_server_runflag = 1;

	ak_print_notice_ex("register onvif handle ok\n");
	return AK_SUCCESS;
}

void onvif_server_unregister_handle(void)
{
	ak_print_notice_ex("unregister onvif handle\n");
	onvif_handle.onvif_server_runflag = 0;
}
