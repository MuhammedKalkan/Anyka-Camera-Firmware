

#ifndef __JA_TYPES_H__
#define __JA_TYPES_H__
	
#ifdef __cplusplus
extern "C" {
#endif

typedef char   JA_CHAR;
typedef int   JA_BOOL;

typedef float   JA_FLOAT;
typedef double  JA_DOUBLE;

typedef signed char JA_INT8;
typedef unsigned char JA_UINT8;
typedef signed short JA_INT16;
typedef unsigned short JA_UINT16;
typedef signed int JA_INT32;
typedef unsigned int JA_UINT32;
#ifndef WIN32
typedef signed long long JA_INT64;
typedef unsigned long long JA_UINT64;
#else
typedef __int64 JA_INT64;
typedef __int64 JA_UINT64;
#endif

typedef JA_UINT64 JA_CHANNELS;

typedef void *JA_POINTER;

/*
* small case form
*/
typedef JA_CHAR		jchar;
typedef JA_BOOL		jbool;
typedef JA_FLOAT 	jfloat;
typedef JA_DOUBLE 	jdouble;
typedef JA_INT8 	jint8;
typedef JA_UINT8 	juint8;
typedef JA_INT16 	jint16;
typedef JA_UINT16 	juint16;
typedef JA_INT32 	jint32;
typedef JA_UINT32 	juint32;
typedef JA_INT64 	jint64;
typedef JA_UINT64 	juint64;

typedef JA_POINTER	jpointer;

typedef JA_UINT16	jword;
typedef JA_UINT32	jdword;

/* Define min and max constants for the fixed size numerical types */
#define JA_MININT8	((JA_INT8)  0x80)
#define JA_MAXINT8	((JA_INT8)  0x7f)
#define JA_MAXUINT8	((JA_UINT8) 0xff)

#define JA_MININT16	((JA_INT16)  0x8000)
#define JA_MAXINT16	((JA_INT16)  0x7fff)
#define JA_MAXUINT16	((JA_UINT16) 0xffff)

#define JA_MININT32	((JA_INT32)  0x80000000)
#define JA_MAXINT32	((JA_INT32)  0x7fffffff)
#define JA_MAXUINT32	((JA_UINT32) 0xffffffff)

#define JA_INT64_CONSTANT(val)	(val##LL)
#define JA_INT64_MODIFIER "ll"
#define JA_INT64_FORMAT "lli"
#define JA_UINT64_FORMAT "llu"

#define JA_MININT64	JA_INT64_CONSTANT(0x8000000000000000)
#define JA_MAXINT64	JA_INT64_CONSTANT(0x7fffffffffffffff)
#define JA_MAXUINT64	JA_INT64_CONSTANT(0xffffffffffffffffU)

#define JA_POINTER_TO_INT(p)	((JA_INT32)   (p))
#define JA_POINTER_TO_UINT(p)	((JA_UINT32)  (p))

#define JA_INT_TO_POINTER(i)	((JA_POINTER)  (i))
#define JA_UINT_TO_POINTER(u)	((JA_POINTER)  (u))


enum {
	JA_FALSE = 0,
	JA_TRUE = 1,
};

enum {
	jfalse = 0,
	jtrue = 1,
};


/*
* event & error-code
*/

typedef enum {
   	JA_EVENT_ALL = -1,
	JA_EVENT_NULL = 0,
	JA_EVENT_IDLE,
	JA_EVENT_TIMER,
	JA_EVENT_ERROR_OCCUR,
	JA_EVENT_CONNECT_OK,
	JA_EVENT_DISCONNECT_OK,
	JA_EVENT_LOGIN_OK,
	JA_EVENT_LOGOUT_OK,
	JA_EVENT_AUTH_REQUIRE,
	JA_EVENT_REQ_PAUSE,
	JA_EVENT_REQ_PLAY,
	JA_EVENT_DESTROYED,
	JA_EVENT_PLAY,
	JA_EVENT_DATA_RECEIVED,
	JA_EVENT_RTCP_SENDER_REPORT,
	JA_EVENT_RTCP_RECEIVER_REPORT,
	JA_EVENT_MD,
	JA_EVENT_MD_EX,
	JA_EVENT_VIDEO_LOSS,
	JA_EVENT_SENSOR_IN,
	JA_EVENT_ALARM_OUT,
	JA_EVENT_TOO_BLURRY,			
	JA_EVENT_TOO_DARK,				
	JA_EVENT_TIME_CHANGED,
	JA_EVENT_NET_CONF_CHANGED,
	JA_EVENT_TOO_BRIGHT,			
	JA_EVENT_SCENE_CHANGE,				
	JA_EVENT_PROFILE_CHANGED,			
	JA_EVENT_VENC_CONF_CHANGED,			
	JA_EVENT_VSOURCE_CONF_CHANGED,		
	JA_EVENT_VOUT_CONF_CHANGED,		
	JA_EVENT_AENC_CONF_CHANGED,			
	JA_EVENT_ASOURCE_CONF_CHANGED,		
	JA_EVENT_AOUT_CONF_CHANGED,
	JA_EVENT_PTZ_CONF_CHANGED,

	JA_EVENT_NR
}nJA_EVENT_TYPE;


typedef enum{
	JA_ERROR_OK = 0,
	JA_ERROR_FAULT,
	JA_ERROR_INVALID_ARG,
	JA_ERROR_CONNECT_FAILED,
	JA_ERROR_SOCK_ERROR,
	JA_ERROR_AUTH_FAILED,
	JA_ERROR_SERVER_NOT_FOUND,
	JA_ERROR_LOST_HEARTBREAK,
	JA_ERROR_PEER_ABORT_SERVER,
	JA_ERROR_HTTP_PARSE_FAIL,
	JA_ERROR_XML_PARSE_FAIL,
	JA_ERROR_JSON_PARSE_FAIL,
	JA_ERROR_NO_MEM,
	JA_ERROR_RES_BUSY,
	JA_ERROR_STREAM_RESOL_TOO_BIG,
	JA_ERROR_STREAM_FRAME_TOO_BIG,
	JA_ERROR_MAIN_STREAM_RESOL_TOO_BIG,
	JA_ERROR_MAIN_STREAM_FRAME_TOO_BIG,
	JA_ERROR_SUB_STREAM_RESOL_TOO_BIG,
	JA_ERROR_SUB_STREAM_FRAME_TOO_BIG,
	JA_ERROR_3RD_STREAM_RESOL_TOO_BIG,
	JA_ERROR_3RD_STREAM_FRAME_TOO_BIG,
	JA_ERROR_DEC_FAIL,
	JA_ERROR_ENC_FMT_NOT_SUPPORT,
	JA_ERROR_THREAD_CREATE_FAIL,
	JA_ERROR_NO_SUCH_STREAM,
	JA_ERROR_DATA_NOT_AVAILABLE,
	JA_ERROR_EOF,
	JA_ERROR_INNER_LOGIC_ERROR,
	JA_ERROR_OUTER_ERROR,
	JA_ERROR_VENC_STANDARD_NOT_SUPPORT,
	JA_ERROR_VENC_PROPERTY_NOT_SUPPORT,
	JA_ERROR_AENC_STANDARD_NOT_SUPPORT,
	JA_ERROR_AENC_PROPERTY_NOT_SUPPORT,
	JA_ERROR_DATA_EMPTY,
	JA_ERROR_REFUSE_FOR_TOO_MUCH_CONNECTION,
	JA_ERROR_GOP_TOO_LONG,
	JA_ERROR_MEDIAPOOL_EXHAUSTED,
		
	JA_ERROR_NR
}nJA_ERROR_TYPE;

/*
* JA Media Data Type
*/
#define JA_MD_TYPE_PS		(128)
#define JA_MD_TYPE_TS		(129) /* MPEG-2 transport stream */
#define JA_MD_TYPE_MPEG		(130) /* MPV, MPEG-1 and MPEG-2 video */
#define JA_MD_TYPE_H264		(131)
#define JA_MD_TYPE_H265		(132)
#define JA_MD_TYPE_VP8		(133)
#define JA_MD_TYPE_VP9		(134)
#define JA_MD_TYPE_V_RAW	(135)
#define JA_MD_TYPE_JPEG		(26)
#define JA_MD_TYPE_ULAW		(0)
#define JA_MD_TYPE_ALAW		(8)
#define JA_MD_TYPE_AAC		(150)
#define JA_MD_TYPE_G711		(151)
#define JA_MD_TYPE_G723		(152)
#define JA_MD_TYPE_G726		(153)
#define JA_MD_TYPE_G729		(154)
#define JA_MD_TYPE_ILIBC	(155)
#define JA_MD_TYPE_AMR		(156)
#define JA_MD_TYPE_SMV		(157)
#define JA_MD_TYPE_A_RAW	(158)
#define JA_MD_TYPE_QCELP	(12)
#define JA_MD_TYPE_T140		(200) /* text over ip */
#define JA_MD_TYPE_TCMD		(201) /* juan text CMD over ip , reversed */
#define JA_MD_TYPE_SUBTITLE		(203) /* SUBTITLES over ip , reversed */

#define JA_MD_IS_VIDEO(_type) (((_type) == JA_MD_TYPE_PS) || ((_type) == JA_MD_TYPE_TS) || ((_type) == JA_MD_TYPE_MPEG) || ((_type) == JA_MD_TYPE_JPEG) \
	|| ((_type) == JA_MD_TYPE_H264) || ((_type) == JA_MD_TYPE_H265) || ((_type) == JA_MD_TYPE_V_RAW) \
	|| ((_type) == JA_MD_TYPE_VP8) || ((_type) == JA_MD_TYPE_VP9))
#define JA_MD_IS_AUDIO(_type) (((_type) == JA_MD_TYPE_G711) || ((_type) == JA_MD_TYPE_ULAW) || ((_type) == JA_MD_TYPE_ALAW) \
		|| ((_type) == JA_MD_TYPE_G723) || ((_type) == JA_MD_TYPE_G726) || ((_type) == JA_MD_TYPE_G729)  || ((_type) == JA_MD_TYPE_AAC)\
		|| ((_type) == JA_MD_TYPE_ILIBC) || ((_type) == JA_MD_TYPE_AMR) || ((_type) == JA_MD_TYPE_SMV) || ((_type) == JA_MD_TYPE_A_RAW))
#define JA_MD_IS_TEXT(_type) (((_type) == JA_MD_TYPE_T140) || ((_type) == JA_MD_TYPE_SUBTITLE) || ((_type) == JA_MD_TYPE_TCMD))

#if !(defined (JA_STMT_START) && defined (JA_STMT_END))
#  if defined (__GNUC__) && !defined (__STRICT_ANSI__) && !defined (__cplusplus)
#    define JA_STMT_START	(void) __extension__ (
#    define JA_STMT_END		)
#  else
#    if (defined (sun) || defined (__sun__))
#      define JA_STMT_START	if (1)
#      define JA_STMT_END	else (void)0
#    else
#      define JA_STMT_START	do
#      define JA_STMT_END	while (0)
#    endif
#  endif
#endif

#define JA_EXIT(n)	JA_STMT_START {exit(n); }JA_STMT_END

#ifdef __cplusplus
}
#endif
#endif /* __JA_TYPES_H__ */




