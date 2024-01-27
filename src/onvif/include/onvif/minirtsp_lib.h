/******************************************************************************

  Copyright (C), 2014-2020, GuangZhou JUAN Electronics Co., Ltd.

 ******************************************************************************
  File Name    : minirtsp.h
  Version       : Initial Draft
  Author        : kejiazhw@gmail.com(kaga)
  Created       : 2014/11/3
  Last Modified : 2014/11/3
  Description   : rtsp  module  interfaces, reference to rfc2326
 
  History       : 
  1.Date        : 2014/11/3
    	Author      : kaga
 	Modification: Created file	
******************************************************************************/

#ifndef __MINIRTSP_H__
#define __MINIRTSP_H__

#ifdef __cplusplus
extern "C" {
#endif


struct minirtsp_ctx;
typedef struct minirtsp_ctx stMINIRTSP, *lpMINIRTSP;

/*
* minirtsp result of call ...
*/
#define MINIRTSP_OK						(0)
#define MINIRTSP_FAIL					(-1)
#define MINIRTSP_INVALID_PARAM			(-2)
#define MINIRTSP_NOT_ENOUGH_MEM			(-3)


/*
* rtsp transport mode
*/
enum {
	MINIRTSP_TRANSPORT_AUTO = 0, // try rtp-over-rtsp first
	MINIRTSP_TRANSPORT_UDP,
	MINIRTSP_TRANSPORT_OVER_RTSP,
};

/**********************************************************************************************
* client-side interfaces
**********************************************************************************************/

/*
* function: MINIRTSP_client_new
* description: rtsp client context new
* parameter:
*		-- streamUrl: stream formated as rtsp://[domain][:port][URI]
*		-- transport : transport, [auto|udp|rtp_over_rtsp]
*		-- userName : user name, null is allowed
*		-- userPwd : user password, null is allowed
*		-- autoReconnect: whether auto reconnect or not when disconnected, 0 for disable, else enable
*		-- id : stream ID [optional], you can ignore it
* return: failed return null, else return context of minirtsp
*/
lpMINIRTSP
MINIRTSP_client_new(const char *streamUrl, int transport, 
	const char *userName, const char *userPwd, 
	int enableAudio, int autoReconnect, int id);

/*
* function: MINIRTSP_set_buffersize
* description: rtsp client set buffer size of stream
* parameter:
*		-- thiz: minirtsp context
* return: none
*/

void MINIRTSP_set_buffersize(lpMINIRTSP thiz, unsigned int size);

/*
* function: MINIRTSP_delete
* description: rtsp client context delete
* parameter:
*		-- thiz: minirtsp context
* return: none
*/
void
MINIRTSP_delete(lpMINIRTSP thiz);

/*
* function: MINIRTSP_connect
* description: rtsp perform connect to server
* parameter:
*		-- thiz: minirtsp context
* return: success return 0, else return -1
*/
int
MINIRTSP_connect(lpMINIRTSP thiz);

/*
* function: MINIRTSP_play
* description: rtsp perform play (which mean to start receive stream data)
* parameter:
*		-- thiz: minirtsp context
* return: success return 0, else return -1
*/
int
MINIRTSP_play(lpMINIRTSP thiz);

/*
* function: MINIRTSP_pause
* description: rtsp perform pause
* parammeter:
*		-- thiz: minirtsp context
* return: success return 0, else return -1
*/
int
MINIRTSP_pause(lpMINIRTSP thiz);

/*
* function: MINIRTSP_disconnect
* description: rtsp perform disconnect
* parameter:
*		-- thiz: minirtsp context
* return: success return 0, else return -1
*/
int
MINIRTSP_disconnect(lpMINIRTSP thiz);


/*
* function: MINIRTSP_lookup_stream
* description: rtsp get stream properties given in rtsp describe
* parameter:
*		-- thiz: minirtsp context
*		-- dataType: data type , such as [h264, alaw, ulaw...]
*		-- dataProperties: structured memery location
* note:
* 	stMINIRTSP_DATA_PROPERTY: this a structure to store data properties, we don't ganrantee to get all properties,
* 	so check the data before use it(normally it would be zero if no specific property found), for example:
*	h264.fps = 0 means that no such property of framerate in SDP
* return: success return 0, else return -1
*/
typedef struct minirtsp_data_property
{
	char szip[64];
	char mediaName[32];// "H264", "ALAW" , "ULAW",...
	int chn_port;
	int mediaType;
	union
	{
		struct{
			int width, height, fps, bps;
			int spsSize, ppsSize;
			unsigned char u_sps[512];
			unsigned char u_pps[512];
		}h264;		
		struct{
			int sampleRate, sampleSize, channel;
		}g711;
	};
}stMINIRTSP_DATA_PROPERTY, *lpMINIRTSP_DATA_PROPERTY;

int
MINIRTSP_lookup_data(lpMINIRTSP thiz, int dataType, 
	lpMINIRTSP_DATA_PROPERTY dataProperties);

/*
* function: MINIRTSP_set_data_hook
* description: rtsp set a hook to handle data
* parameter:
*		-- thiz: minirtsp context
*		-- hook: type of fMINIRTSP_DATA_HOOK
*		-- customCtx: user-defined context
*		-- int CustomCtxSize: the size of customCtx Size, if size given ( > 0), it would malloc cache to store custome context,
*				else it would only store the pointer of custom context
* return: none
* note:
*		-- fMINIRTSP_DATA_HOOK:
*			--pdata: cache to store data
*			-- dataSize: size of data
*			--timestamp: timestamp of data in milisecond
*			-- dataType: type of data , detail see definition of media data type as follows
*			--customCtx: custom context given by MINIRTSP_set_event_hook
*/

/*
* media data type : corresponding to rtplib.h
*/
#define MINIRTSP_MD_TYPE_PS			(128)
#define MINIRTSP_MD_TYPE_TS			(129)
#define MINIRTSP_MD_TYPE_MPEG		(130)
#define MINIRTSP_MD_TYPE_H264		(131)
#define MINIRTSP_MD_TYPE_H265		(132)
#define MINIRTSP_MD_TYPE_ULAW		(0)
#define MINIRTSP_MD_TYPE_ALAW		(8)
#define MINIRTSP_MD_TYPE_AAC		(150)

typedef void (*fMINIRTSP_DATA_HOOK)(int streamId, void *pdata, unsigned int dataSize, unsigned int timestamp,
	int dataType, void *customCtx);
void
MINIRTSP_set_data_hook(lpMINIRTSP thiz, fMINIRTSP_DATA_HOOK hook, void *customCtx, int CustomCtxSize);

/*
* function: MINIRTSP_set_event_hook
* description: rtsp set a hook to handle rtsp inner event if you are interest with it 
* parameter:
*		-- thiz: minirtsp context
*		-- hook: type of fMINIRTSP_EVENT_HOOK
*		-- customCtx: user-defined context
*		-- int CustomCtxSize: the size of customCtx Size, if size given ( > 0), it would malloc cache to store custome context,
*				else it would only store the pointer of custom context
* return : none
* note:
*		-- fMINIRTSP_EVENT_HOOK:
*			--eventType: type of event
*			--lParam & rParam: some event, normally they nonsense, but for some event , they are significance (inner-module
*				try to tell you more info about this event with these parameters); the detail meanings see event defininition
*				above
*			--customCtx: custom context given by MINIRTSP_set_event_hook
*/

typedef void (*fMINIRTSP_EVENT_HOOK)(int eventType, int lParam, void *rParam, void *customCtx);
void
MINIRTSP_set_event_hook(lpMINIRTSP thiz, fMINIRTSP_EVENT_HOOK hook, void *customCtx, int CustomCtxSize);


/*
* additional interfaces for JCM
* new0 -> set_url -> do_loop(exited: cleanup) -> loop_exit-> destroy
*/
lpMINIRTSP
MINIRTSP_client_new0(int transport, 
	const char *userName, const char *userPwd, 
	int enableAudio, int autoReconnect, int id);
int
MINIRTSP_client_set_url(lpMINIRTSP thiz, const char *streamUrl);

void *
MINIRTSP_client_do_loop(lpMINIRTSP thiz);

void
MINIRTSP_client_loop_exit(lpMINIRTSP thiz);

void
MINIRTSP_client_cleanup(lpMINIRTSP thiz);

void
MINIRTSP_client_set_Iframe(lpMINIRTSP thiz);



/**********************************************************************************************
* server-side interfaces
* usage note:
* MINIRTSP_server_new->MINIRTSP_server_set_stream_func-> MINIRTSP_server_start-> ....{loop} ...MINIRTSP_delete
**********************************************************************************************/



/*
* function: MINIRTSP_server_new
* description: rtsp server context init
* parameter:
* return : success : context of rtsp server, else return NULL
*/

lpMINIRTSP
MINIRTSP_server_new(void);


/*
* function: MINIRTSP_server_set_stream_func
* description: rtsp set hooks to handle stream
* parameter:
*		-- func: hooks for stream api
* return : none
* note:
*		-- fMINIRTSP_STREAM_OPEN: open/init a stream specified by parameter of name
*		-- fMINIRTSP_STREAM_LOCK: lock stream if needed before read a frame from stream
*		-- fMINIRTSP_STREAM_NEXT: read a frame from stream
*		-- fMINIRTSP_STREAM_UNLOCK: unlock stream if needed after read done a frame
*		-- fMINIRTSP_STREAM_CLOSE: close stream
*		-- fMINIRTSP_STREAM_RESET: reset stream if needed
*		-- fMINIRTSP_STREAM_GET_AVC: get h264 sps/pps
*/

typedef struct MINIRTSP_STREAM {
    char name[64]; 				  // stream name
	unsigned long long timestamp; // us
	int isKeyFrame;				// key frame or not
	int type;					// media data type
    void *data;					// stream data
    unsigned long size;			// stream size
    void *param; 				// private context
    char username[64];		// username
}stMINIRTSP_STREAM, *lpMINIRTSP_STREAM;

typedef struct _h264avc
{
	unsigned char sps[256];
	unsigned char pps[256];
	int sps_size;
	int pps_size;
	int with_startcode;			//0 not have 00000001       1 have 00000001
}H264AVC_t;

typedef struct _venc
{
	int codeType;
	int resWidth;
	int resHeight;
	int frameRate;
}VENC_t;

#define rMINIRTSP_OK					(0)
#define rMINIRTSP_FAIL					(-1)
#define rMINIRTSP_NO_SUCH_STREAM		(-2)
#define rMINIRTSP_STREAM_BUSY			(-3)
#define rMINIRTSP_DATA_NOT_AVAILABLE	(-4)
#define rMINIRTSP_EOF					(-5)
typedef int (*fMINIRTSP_STREAM_OPEN)(lpMINIRTSP_STREAM stream, const char *name);
typedef int (*fMINIRTSP_STREAM_LOCK)(void *thiz);
typedef int (*fMINIRTSP_STREAM_NEXT)(void *thiz);
typedef int (*fMINIRTSP_STREAM_UNLOCK)(void *thiz);
typedef int (*fMINIRTSP_STREAM_CLOSE)(void *thiz);
typedef int (*fMINIRTSP_STREAM_RESET)(void *thiz);
typedef int (*fMINIRTSP_STREAM_GET_AVC)(char *name, H264AVC_t *data);

typedef struct MINIRTSP_STREAM_FUNC
{
	fMINIRTSP_STREAM_OPEN open;
	fMINIRTSP_STREAM_LOCK lock;
	fMINIRTSP_STREAM_NEXT next;
	fMINIRTSP_STREAM_LOCK unlock;
	fMINIRTSP_STREAM_CLOSE close;
	fMINIRTSP_STREAM_RESET reset;
	fMINIRTSP_STREAM_GET_AVC get_avc;
}stMINIRTSP_STREAM_FUNC, *lpMINIRTSP_STREAM_FUNC;

int MINIRTSP_server_set_stream_func(stMINIRTSP_STREAM_FUNC func);


/*
* function: MINIRTSP_server_start
* description: start rtsp server
* parameter:
*	-- listenPort: rtsp server port, if 0 , use default rtsp port(554)
* return : success  return 0, else return -1
*/

int MINIRTSP_server_start(int listenPort);

/*
* function: MINIRTSP_server_stop
* description: stop rtsp server
* parameter:
* return : none
* note: usually, you should call MINIRTSP_delete instead of this api
*/

void MINIRTSP_server_stop();

/*
* spook interfaces
*/
int
MINIRTSP_server_detect(char *msg, int msgSize);
void *
MINIRTSP_server_handler(void *param);

typedef int (*MINIRTSP_SERVER_GET_PWD_BY_NAME)(const char *name, char *ret_pwd, int ret_size);

void MINIRTSP_server_auth_hook(MINIRTSP_SERVER_GET_PWD_BY_NAME hook);

/*
	stream_name [in] 
	channal_venc [out] 
*/
typedef int (*MINIRTSP_SERVER_GET_VENC)(char *stream_name,VENC_t* channal_venc);   //  channal_venc { h264 : RTP_TYPE_DYNAMIC   h265:RTP_TYPE_DYNAMIC_H265}
void MINIRTSP_server_venc_hook(MINIRTSP_SERVER_GET_VENC hook);

/*
	stream_name [in]
*/
typedef int (*MINIRTSP_SERVER_SET_IFRAME)(char *stream_name,int code ,void* data);
void MINIRTSP_server_IFrame_hook(MINIRTSP_SERVER_SET_IFRAME hook);


/**********************************************************************************************
* server-side interfaces
**********************************************************************************************/

/*
* additional minirtsp interfaces
*/
int MINIRTSP_client_get_id(lpMINIRTSP thiz);

void 
MINIRTSP_set_loglevel(lpMINIRTSP thiz, int level);
void 
MINIRTSP_enable_rtcp(lpMINIRTSP thiz, int enable);
void
MINIRTSP_set_timestamp_mode(lpMINIRTSP thiz, int useRtpTimestamp);

#define MINIRTSP_LIB_VERSION        (13U)

int MINIRTSP_int_version();

/*
* function: MINIRTSP_server_version
*description: get rtsp server version
*return: version info ,string
*/
char * MINIRTSP_char_version();

#ifdef __cplusplus
}
#endif

#endif


