#ifndef __ONVIF_LIB_H__
#define __ONVIF_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

//#include "stdinc.h"

#define ONVIF_DEV_NVT				(0x0)
#define ONVIF_DEV_NVD				(0x01)
#define ONVIF_DEV_NVS				(0x02)
#define ONVIF_DEV_NVA				(0x03)
#define ONVIF_DEV_ALL				(0xFFFFFFFF)

/**************************************************************************
* server-side interfaces
**************************************************************************/
/*
* onvif Server-side Context init/deint
* 1) ONVIF_SERVER_init
* parammeter:
*		-- device_type: optional values as below:
*			Network Video Transmitter(NVT)
*			Network Video Display(NVD)
*			Network Video Storage(NVS)
*			Network Video Analytics(NVA)
*		-- device_name : name of device
*/
extern void ONVIF_SERVER_init(int device_type, const char *device_name);
extern void ONVIF_SERVER_deinit();


/*
* Onvif server start
* parameter, port : server listen port
* note: 1) if you have a common listen daemon (like spook) and you want embed this onvif
*		daemon into your common listen daemon, ignore this interface, you can use another
*		two interfaces following.
* 		2) it would be stop by ONVIF_SERVER_deinit
*/
extern void ONVIF_SERVER_start(int port);

/*
* spook interfaces:
* 1) ONVIF_check_uri, check  whether a specific uri (int http request line) is onvif protocal or not,
*		yes return 0, else return -1
*	parameter :
*		-- uri, the URI in http request line, for example : /onvif/device
*		-- uri_size, the string length of uri
* 2) ONVIF_SERVER_daemon , onvif server-request-handler, if you accept a request (for example,
*  		probe a onvif request in spook, then you can run this interface to handle the request.
*	parameter :
*		--sock, the client socket return by accept() when a request arrived
*/
extern int ONVIF_check_uri(char *uri, int uri_size);
extern int ONVIF_SERVER_daemon(int sock);

/*
* event notification:
* when some system event occur, you should call this hook to notify ONVIF-module what's happen.
*  	parameter:
*		-- event, type of NVP_EVENT defined in env_common.h
*/
extern void ONVIF_notify_event(int event );

/**
*  get password
*	name [in]		user name
*	ret_pwd[out]		user password
*
*/
typedef int (*fONVIF_GET_PWD_BY_NAME)(const char *name, char *ret_pwd, int ret_size);
extern void ONVIF_set_auth_hook(fONVIF_GET_PWD_BY_NAME hook);

/*
*  open onvif password Authenticate
*  open [in]			true  open   ,false close,  default  false
*/
extern void ONVIF_open_use_password(bool open);

/**************************************************************************
* client-side interfaces
**************************************************************************/
/*
* onvif Server-side Context init/deint
* 	parameter:
*		-- timeout: unit is second
*		-- check_with_arp: devices found by WSDD keep alive by arp query
*		-- online_timeout: if disable check_with_arp, device found by WSDD-Hello would be deleted when 
*			given timeout elapse; if online_timeout <=0, disable online device list
*/
extern void ONVIF_CLIENT_init(int conn_timeout, int send_timeout, int recv_timeout,
	bool check_with_arp, int online_timeout);
extern void ONVIF_CLIENT_deinit();

/*
* Discovery Onvif Devices
	parameter :
		 -- 1) device_types, specific which devices do you want to search , available onvif device types as below:
				Network Video Transmitter(NVT)
				Network Video Display(NVD)
				Network Video Storage(NVS)
				Network Video Analytics(NVA)
		-- 2) add_hello_dev, whether add devices-list found by WSDD-Hello-Event
		-- 3) recv_timeout_s, receive timeout in second , if <=0 or > 60, use recvtimeout given 
				by ONVIF_CLIENT_init
		-- 4) hook, when search success, it would call this hook, so it you can specific hook function and know
				which device is found, the hook type is fOnvifSearchFoundHook
		-- 5) bind_host, if NULL, bind any address
				esle bind the specific host
	return value:
		-1 , search failed
		>= 0, the number of devices searched
*/
typedef void (*fOnvifSearchFoundHook)(const char *bind_host, unsigned char *mac, unsigned char *ip,unsigned short port,
	char *name, char *location, char *firmware,
	void *customCtx);
extern int ONVIF_search(int device_type, bool add_hello_dev, int recv_timeout_s, 
	fOnvifSearchFoundHook hook,  const char *bind_host, 
	void *customCtx);

/*
* onvif event daemon start
* if using Web sevice base notiofication server, it must create a listen proc to accept event-notification
* so if necessary , call this to start a event-listen daemon
* note: 1) if you have a common listen daemon (like spook) and you want embed this onvif-event
*		daemon into your common listen daemon, ignore this interface, you can use another
*		interfaces (ONVIF_event_daemon_start2/ONVIF_check_uri/ONVIF_SERVER_daemon).
* parameter:
*		-- bindip : which ip address would you want to bind
*		-- sock : which port do you want to listen event base-notification 
*		
*/
extern void ONVIF_event_daemon_start(char *bindip, int port);
extern void ONVIF_event_daemon_start2(char *bindip, int port);


/*
* WS-Discovery Server start
Note: you should check whether dev_type/xaddr/scopes  is null or not before useing it
* parameter:
*		-- hook : callback when got a wsdd event
*/
#define WSDD_EVENT_HELLO	(0)
#define WSDD_EVENT_BYE		(1)
#define WSDD_EVENT_PROBE	(2) // only for server-side, return  1 : probe response; 0: no response
typedef int (*fON_WSDD_EVENT)(char *dev_type, char *xaddr, char *scopes, int wsdd_event_type, void *customCtx);
extern void ONVIF_search_daemon_start(fON_WSDD_EVENT hook, void *customCtx);
extern void ONVIF_search_daemon_set_hook(fON_WSDD_EVENT hook, void *customCtx);
extern void ONVIF_search_daemon_stop();

#define NK_ONVIF_REVISION	(13U)

extern char* ONVIF_version();
/**
 * 获取 NkUtils 库版本号。\n
 * 在库该方法实现内部直接返回 @ref NK_UTILS_REVISION 宏定义的值，\n
 * 由于库发布时二进制已经确认，因此可以过在链接改库时判断此方法返回结果是否等于 @ref NK_UTILS_REVISION 判断头文件是否与二进制库文件匹配。\n
 * 通过次方法可以有效防止由于头文件与二进制文件不匹配所带来的致命问题。
 *
 * @return
 * 	返回 NkUtils 库的版本号，与 @ref NK_UTILS_REVISION 一致。\n
 *
 */
extern unsigned int
ONVIF_LibraryRevision();

#ifdef __cplusplus
}
#endif
#endif

