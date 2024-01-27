#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>		/* getopt_long() */
#include <fcntl.h>		/* low-level i/o */
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

#include "ak_common.h"
#include "ak_thread.h"

#define  CMDSIZE            1024
#define  RECV_BUF_SIZE      1024

static int onvif_cmd_runflag = 0;
static ak_pthread_t cmd_listen_tid = 0;

/* analyze cmd string from buffer */
static int FindStrFromBuf(const char *buf, int buflen, const char *findingstr,
		int findinglen, int direction)
{
	int i, j;

	if(direction == 0){
		for (i = 0; i < buflen - findinglen + 1; i++){
	  		for (j = 0; j < findinglen; j++){
	      		if (buf[i + j] != findingstr[j])
					break;

	      	if (j == findinglen - 1)
				return i;
	    	}
		}
	}else{
		for (i = buflen - findinglen; i >= 0; i--){
			for (j = 0; j < findinglen; j++){
				if (buf[i + j] != findingstr[j])
					break;

	      		if (j == findinglen - 1)
					return i;
			}
		}
    }

	return -1;
}

/* execute appointed cmd by system */
static int do_command (char *tmpstr, int fd)
{
    printf("[%s]: [%s]\n", __func__, tmpstr);
	if (strcmp(tmpstr, "reboot") != 0)
		system(tmpstr);
	else
		printf("we don't reboot\n");

    return 0;
}

/* create TCP server for cmd handle using port 999 */
static void *cmd_listen(void *arg)
{
	int port = (int)arg;
	int fd = 0;

	ak_print_notice_ex("listen port=%d\n", port);
	/* 1. create TCP socket */
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (fd < 0){
		printf("[%s]: socket error.\n", __func__);
		goto exit;
	}

	int opt = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof (opt));

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons (port);
	addr.sin_addr.s_addr = htonl (INADDR_ANY);
	memset (&(addr.sin_zero), 0, sizeof (addr.sin_zero));

	/* 2. bind server ip address */
	if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) != 0){
		printf ("[%s]: Binding error.\n", __func__);
		close (fd);
		goto exit;
	}

	/* 3. set listen attribution */
	if (listen(fd, 100) != 0){
		printf("[%s]: Listen Error.\n", __func__);
	}
	printf("[%s]: Listen to %d\n", __func__, port);

	int pos;
	int addrlen;
	int length;
	int clientfd;
	int recv_len = 0;
	char recv_buf[RECV_BUF_SIZE*2];
	char tmpstr[CMDSIZE];
	int boundarylen = strlen ("$$boundary\r\n");
	struct sockaddr_in client_addr;
	
	while (onvif_cmd_runflag) {
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
			onvif_cmd_runflag = 0;
			goto exit;
			break;
		default:
			if (!FD_ISSET(fd, &readfds))
				continue;
			break;
		}

		addrlen = sizeof (client_addr);
		/* 4. accept client to connect */
		clientfd = accept(fd, (struct sockaddr *)&client_addr,
			(socklen_t *)&addrlen);
		if (clientfd != -1){
			/* 5. client connect OK */
			printf ("Client %s is connected.\n", inet_ntoa(client_addr.sin_addr));
			while (onvif_cmd_runflag) {
				/* 6. receive from client */
				length = recv(clientfd, recv_buf + recv_len, RECV_BUF_SIZE, 0);
				if (length <= 0){
					close (clientfd);
					break;
				} else {
					recv_len += length;
					while ((pos = FindStrFromBuf (recv_buf, recv_len,
						"$$boundary\r\n", boundarylen, 0)) > 0){
						memset(tmpstr, 0, CMDSIZE);
						memcpy(tmpstr,recv_buf, pos);
						memcpy(recv_buf, recv_buf + pos + boundarylen,
							recv_len - pos - boundarylen);
						recv_len = recv_len - pos - boundarylen;
						do_command(tmpstr, clientfd);
						close(clientfd);
						break;
					}
				}
			} /* end of recv-functione */
			break;
		} /* if (clientfd != -1)*/
	}

exit:
	if (fd)
		close(fd);
	ak_thread_exit();

	return NULL;
}

int onvif_cmd_init(void)
{
    signal(SIGPIPE, SIG_IGN);
	onvif_cmd_runflag = 1;
	ak_print_normal_ex("start onvif listen cmd service\n");
	ak_thread_create(&cmd_listen_tid, cmd_listen, (void *)999,
			ANYKA_THREAD_MIN_STACK_SIZE, -1);

    return 0;
}

void onvif_cmd_exit(void)
{
	ak_print_normal_ex("onvif listen cmd service stopping\n");
	onvif_cmd_runflag = 0;
	kill(cmd_listen_tid, SIGINT);
	ak_thread_join(cmd_listen_tid);
	ak_print_normal_ex("onvif listen cmd service exited\n");
}
