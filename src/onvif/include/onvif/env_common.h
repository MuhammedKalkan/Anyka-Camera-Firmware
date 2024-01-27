#ifndef __ENV_COMMON_H__
#define __ENV_COMMON_H__

//#include "stdinc.h"
//#include "cross.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
	NetSDK / Net Video Property  Definitions
	NOTE / WARNNING:
	1) index/id : start from 0 ; 0, 1, 2...
	2) *_nr : the number of ...
*/

/*********************************************************************************
** configure area....
**********************************************************************************/

#define NVP_MAX_NT_SIZE		(48)
#define NVP_MAX_URI_SIZE		(200)
#define NVP_MAX_CH			(1)
#define NVP_MAX_VIN			(NVP_MAX_CH)
#define NVP_MAX_VENC		(5)
#define NVP_MAX_VIN_IN_A_SOURCE	(5)
#define NVP_MAX_AIN			(NVP_MAX_CH)
#define NVP_MAX_AENC		(1)
#define NVP_MAX_RESOLUTION	(20)
#define NVP_MAX_PTZ_PRESET	(255)
#define NVP_MAX_PTZ_TOUR_ENTRY	(64)
#define NVP_MAX_PTZ_TOUR	(5)

#define NVP_MAX_OPTION_SIZE	(32)

//////////////////////////////////////
#ifndef NVP_CH_NR
#define NVP_CH_NR			(1)
#endif
#define NVP_VIN_NR			(NVP_CH_NR)
#ifndef NVP_VENC_NR
#define NVP_VENC_NR			(2)
#endif
#define NVP_VIN_IN_A_SOURCE	(NVP_VENC_NR)
#define NVP_AIN_NR			(1)
#define NVP_AENC_NR			(1)


#define NVP_SUPPORT_MD
#define NVP_SUPPORT_PROFILE

/*********************************************************************************
**
**********************************************************************************/

#define IN //input argument
#define OUT// output argument
#define INOUT // both used for input and output variables

#define NVP_SIZE_ARRAY(ARR) (sizeof(ARR) / sizeof(ARR[0]))

typedef enum{
	NVP_VENC_JPEG = 0,
	NVP_VENC_MPEG4,
	NVP_VENC_H264,
	NVP_VENC_H265,

	NVP_VENC_TYPE_NR
}NVP_VENC_TYPE;

typedef enum
{
	NVP_AENC_G711 = 0,
	NVP_AENC_G711_ALAW,
	NVP_AENC_G711_ULAW,
	NVP_AENC_G726,
	NVP_AENC_PCM,
	NVP_AENC_ADPCM,
	NVP_AENC_AAC,

	NVP_AENC_TYPE_NR
}NVP_AENC_TYPE;

typedef enum
{
	NVP_QUANT_AUTO = 0,
	NVP_QUANT_CBR,
	NVP_QUANT_VBR,
	NVP_QUANT_NR
}NVP_QUANT_MODE;

typedef enum{
	NVP_MPEG4_PROFILE_SP = 0,
	NVP_MPEG4_PROFILE_ASP,

	NVP_MPEG4_PROFILE_NR
}NVP_MPEG4_PROFILE;

typedef enum{
	NVP_H264_PROFILE_BASELINE = 0,
	NVP_H264_PROFILE_MAIN,
	NVP_H264_PROFILE_EXTENDED,
	NVP_H264_PROFILE_HIGH,

	NVP_H264_PROFILE_NR
}NVP_H264_PROFILE;

typedef struct NVP_RANGE
{
	int min;
	int max;
}stNVP_RANGE, *lpNVP_RANGE;

typedef struct NVP_FLOAT_RANGE
{
	float min;
	float max;
}stNVP_FLOAT_RANGE, *lpNVP_FLOAT_RANGE;

typedef struct NVP_SIZE
{
	int width;
	int height;
}stNVP_SIZE, *lpNVP_SIZE;
#define NVP_SET_SIZE(P,W,H) (P)->width = (W); (P)->height = (H)

typedef struct NVP_RECT
{
	int nX;
	int nY;
	int width;
	int height;
}stNVP_RECT, *lpNVP_RECT;
#define NVP_SET_RECT(P,X,Y,W,H) (P)->width = (W); (P)->height = (H); (P)->nX = (X); (P)->nY = (Y)

typedef struct NVP_OPTION
{
	int nr; 
	int list[NVP_MAX_OPTION_SIZE];
}stNVP_OPTION, *lpNVP_OPTION;

typedef struct NVP_IMAGE_OPTIONS
{
	IN char srcToken[NVP_MAX_NT_SIZE]; // video source token
	stNVP_FLOAT_RANGE sharpness;
	stNVP_FLOAT_RANGE hue;
	stNVP_FLOAT_RANGE contrast;
	stNVP_FLOAT_RANGE brightness;
	stNVP_FLOAT_RANGE saturation;

	stNVP_OPTION ircut_mode;
}stNVP_IMAGE_OPTIONS, *lpNVP_IMAGE_OPTIONS;


typedef struct NVP_COLOR_CONFIG
{
	// if given value is -1, this item would not be setting
	IN int index;   // source index
	IN char src_token[NVP_MAX_NT_SIZE]; // video source token
	float sharpness;
	float hue;
	float contrast;
	float brightness;
	float saturation;
}stNVP_COLOR_CONFIG, *lpNVP_COLOR_CONFIG;

typedef struct NVP_IRCUT
{
#define NVP_IRCUT_CONTROL_MODE_HARDWARE (0)
#define NVP_IRCUT_CONTROL_MODE_SOFTWARE (1)
		
#define NVP_IRCUT_MODE_AUTO (0)
#define NVP_IRCUT_MODE_DAYLIGHT (1)
#define NVP_IRCUT_MODE_NIGHT (2)

	int control_mode;
	int ircut_mode;
}stNVP_IRCUT_CONFIG, *lpNVP_IRCUT_CONFIG;

typedef struct NVP_MANUAL_SHARPNESS
{
	bool enabled;
	int sharpnessLevel;
}stNVP_MANUAL_SHARPNESS_CONFIG, *lpNVP_MANUAL_SHARPNESS_CONFIG;

typedef struct NVP_DENOISE3D
{
	bool enabled;
	int denoise3dStrength;
}stNVP_DENOISE3D_CONFIG,*lpNVP_DENOISE3D_CONFIG;

typedef struct NVP_WDR
{
	bool enabled;
	int WDRStrength;
}stNVP_WDR_CONFIG,*lpNVP_WDR_CONFIG;

typedef struct NVP_IMAGE
{
	char name[NVP_MAX_NT_SIZE];
	char token[NVP_MAX_NT_SIZE];

	char src_token[NVP_MAX_NT_SIZE];
	
	stNVP_COLOR_CONFIG color;
	stNVP_IRCUT_CONFIG ircut;
	stNVP_WDR_CONFIG wdr;
	stNVP_MANUAL_SHARPNESS_CONFIG manual_sharp;
	stNVP_DENOISE3D_CONFIG d3d;
	//
	stNVP_IMAGE_OPTIONS option;
}stNVP_IMAGE_CONFIG, *lpNVP_IMAGE_CONFIG;

typedef struct NVP_INPUT_ROTATE
{
	bool enabled;
#define NVP_ROTATE_MIRROR		(90)
#define NVP_ROTATE_FLIP			(180)
#define NVP_ROTATE_FLIP_MIRROR	(270)
	int degree;
}stNVP_VIN_ROTATE, *lpNVP_VIN_ROTATE;

typedef enum NVP_FISHEYE_SHOW_MODE{
	eNVP_FISHEYE_MODE_WALL_ORIGIN = 0x0,
	eNVP_FISHEYE_MODE_WALL_180,
	eNVP_FISHEYE_MODE_WALL_SPLIT,
	eNVP_FISHEYE_MODE_WALL_WALL_SPLIT,
	eNVP_FISHEYE_MODE_WALL_4R,
	eNVP_FISHEYE_MODE_WALL_KITR,
	eNVP_FISHEYE_MODE_WALL_KITO,	

	eNVP_FISHEYE_MODE_CELL_ORIGIN = 0x10,
	eNVP_FISHEYE_MODE_CELL_360,
	eNVP_FISHEYE_MODE_CELL_SPLIT,
	eNVP_FISHEYE_MODE_CELL_4R,
	eNVP_FISHEYE_MODE_CELL_WALL_SPLIT,
	eNVP_FISHEYE_MODE_CELL_180,
	eNVP_FISHEYE_MODE_CELL_KITR,
	eNVP_FISHEYE_MODE_CELL_KITO,

	eNVP_FISHEYE_MODE_TABLE_ORIGIN= 0x20,
	eNVP_FISHEYE_MODE_TABLE_360,
	eNVP_FISHEYE_MODE_TABLE_SPLIT,
	eNVP_FISHEYE_MODE_TABLE_4R,
	eNVP_FISHEYE_MODE_TABLE_VR,
	eNVP_FISHEYE_MODE_TABLE_KITR,
	eNVP_FISHEYE_MODE_TABLE_KITO,
}tNVP_FISHEYE_SHOW_MODE;

typedef enum NVP_FISHEYE_FIX_MODE{
	eNVP_FISHEYE_FIX_MODE_WALL=0,
	eNVP_FISHEYE_FIX_MODE_CELL,
	eNVP_FISHEYE_FIX_MODE_TABLE,
}tNVP_FISHEYE_FIX_MODE;

typedef struct NVP_V_SOURCE
{
	char name[NVP_MAX_NT_SIZE];
	char token[NVP_MAX_NT_SIZE];

	float fps;
	stNVP_SIZE resolution;
	stNVP_IMAGE_CONFIG image;
}stNVP_V_SOURCE, *lpNVP_V_SOURCE;

typedef struct NVP_VIN_CONFIG
{
	char name[NVP_MAX_NT_SIZE];
	char token[NVP_MAX_NT_SIZE];

	char src_token[NVP_MAX_NT_SIZE]; // reference to video source

	stNVP_RECT rect;
	bool enable_rect;
	stNVP_VIN_ROTATE rotate;
	bool enable_rotate;
	tNVP_FISHEYE_FIX_MODE fix_mode;
	tNVP_FISHEYE_SHOW_MODE show_mode;
	bool enable_mode;
}stNVP_VIN_CONFIG, *lpNVP_VIN_CONFIG;

typedef struct NVP_VIN_CONFIGS
{
	int nr;
	stNVP_VIN_CONFIG entry[NVP_MAX_VIN_IN_A_SOURCE];
}stNVP_VIN_CONFIGS, *lpNVP_VIN_CONFIGS;

typedef struct NVP_VENC_OPTIONS
{
	IN int index;
	IN char token[NVP_MAX_NT_SIZE];
	IN char enc_token[NVP_MAX_NT_SIZE];
	//
	stNVP_RANGE enc_quality;
	stNVP_RANGE enc_fps;
	stNVP_RANGE enc_bps; //kbps
	stNVP_RANGE enc_gov;
	stNVP_RANGE enc_interval;
	int resolution_nr;	// how many resolution supported
	stNVP_SIZE resolution[20];
	int enc_profile_nr;
	int enc_profile[5];
	//
	int coding_fmt_nr;
	int coding_fmt[5];
}stNVP_VENC_OPTIONS, *lpNVP_VENC_OPTIONS;

typedef struct NVP_VENC_CONFIG
{
	IN int index; //MAYBE USED FOR INPUT ARG WHEN GETTING
	//
	char name[NVP_MAX_NT_SIZE];
	char token[NVP_MAX_NT_SIZE];
	char enc_name[NVP_MAX_NT_SIZE];
	char enc_token[NVP_MAX_NT_SIZE];

	int user_count;
	int width;
	int height;
	NVP_VENC_TYPE enc_type;
	NVP_QUANT_MODE quant_mode;
	int enc_fps;
	int enc_bps; //kbps
	int enc_interval;  //???????1??????????2????????????????????
	int enc_quality;
	int enc_gov;  //key interval
	int enc_profile;

	stNVP_VENC_OPTIONS option;
}stNVP_VENC_CONFIG, *lpNVP_VENC_CONFIG;

typedef struct NVP_VENC_CONFIGS
{
	int nr;
	stNVP_VENC_CONFIG entry[NVP_MAX_VENC];
}stNVP_VENC_CONFIGS, *lpNVP_VENC_CONFIGS;


typedef struct NVP_AIN_CONFIG
{
	char name[NVP_MAX_NT_SIZE];
	char token[NVP_MAX_NT_SIZE];

}stNVP_AIN_CONFIG, *lpNVP_AIN_CONFIG;

typedef struct NVP_AENC_CONFIG
{
	int index;
	char name[NVP_MAX_NT_SIZE];
	char token[NVP_MAX_NT_SIZE];

	int user_count;
	NVP_AENC_TYPE enc_type;
	int channel;
	int sample_rate;
	int sample_size;
}stNVP_AENC_CONFIG, *lpNVP_AENC_CONFIG;

typedef struct NVP_VAN_CONFIG // video analytic
{
	char name[NVP_MAX_NT_SIZE];
	char token[NVP_MAX_NT_SIZE];

}stNVP_VAN_CONFIG, *lpNVP_VAN_CONFIG;


typedef struct NVP_ETHER_CONFIG
{
	char name[NVP_MAX_NT_SIZE];
	IN char token[NVP_MAX_NT_SIZE]; // if search with multicast , maybe we should use this to identify a device
	IN char nonce[NVP_MAX_NT_SIZE];
	//
	bool dhcp;
	unsigned char ip[4];
	unsigned char netmask[4];
	unsigned char gateway[4];
	unsigned char dns1[4];
	unsigned char dns2[4];
	unsigned char mac[6];
	//
	int http_port;
	int rtsp_port;
}stNVP_ETHER_CONFIG, *lpNVP_ETHER_CONFIG;
#define NVP_IP_INIT_FROM_STRING(IP, IPSTR) \
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
			IP[0] = 192; \
			IP[1] = 168; \
			IP[2] = 1; \
			IP[3] = 1; \
		}\
	}
#define NVP_MAC_INIT_FROM_STRING(MAC, MACSTR) \
	{ \
		int tmp[6]; \
		memset(MAC, 0, 6); \
		if(sscanf(MACSTR, "%x:%x:%x:%x:%x:%x", &tmp[0], &tmp[1], &tmp[2], &tmp[3], &tmp[4], &tmp[5]) == 6) \
		{ \
			MAC[0] = tmp[0]; \
			MAC[1] = tmp[1]; \
			MAC[2] = tmp[2]; \
			MAC[3] = tmp[3]; \
			MAC[4] = tmp[4]; \
			MAC[5] = tmp[5]; \
		}else{\
			printf("ERR: parse mac address failed!\n");\
			MAC[0] = 0; \
			MAC[1] = 0x9a; \
			MAC[2] = 0x11; \
			MAC[3] = 0x22; \
			MAC[4] = 0x33; \
			MAC[5] = 0x44; \
		}\
	}

#define NVP_IP_INIT(IP, VAL1, VAL2, VAL3, VAL4) \
	{ \
		IP[0] = VAL1; \
		IP[1] = VAL2; \
		IP[2] = VAL3; \
		IP[3] = VAL4; \
	}
#define NVP_MAC_INIT(MAC, VAL1, VAL2, VAL3, VAL4, VAL5, VAL6) \
	{ \
		MAC[0] = VAL1; \
		MAC[1] = VAL2; \
		MAC[2] = VAL3; \
		MAC[3] = VAL4; \
		MAC[4] = VAL5; \
		MAC[5] = VAL6; \
	}


typedef struct NVP_DEV_INFO
{
	char manufacturer[64];
	char devname[64];
	char model[64];
	char firmware[64];
	char sn[64];  // serial number
	char hwid[64];//hardware id
	char sw_builddate[32];
	char sw_version[32];
	char hw_version[32];
}stNVP_DEV_INFO, *lpNVP_DEV_INFO;

// date time value is similar to struct tm,
typedef struct NVP_TIME
{
	unsigned char hour; // 0 ~ 23
	unsigned char minute; // 0 ~ 59
	unsigned char second; // 0 ~ 59
}stNVP_TIME, *lpNVP_TIME;

typedef struct NVP_DATE
{
	unsigned char year; // the number of years since 1900
	unsigned char month; // 0 ~ 11
	unsigned char day; // 1 ~ 31
}stNVP_DATE, *lpNVP_DATE;


typedef struct NVP_DATE_TIME 
{
	stNVP_DATE date;
	stNVP_TIME time;
}stNVP_DATE_TIME, *lpNVP_DATE_TIME;

typedef struct NVP_SYS_TIME
{
	stNVP_DATE_TIME local_time;
	stNVP_DATE_TIME gm_time;
	/*
	  tzone example: 
	  	330 => GMT+03:30
	  	-530 => GMT-05:30
	  	400 => GMT+04:00
	*/
	int tzone;
	bool daylightsaving;
	char stzone[32];
	//
	bool ntp_enable;
	char ntp_server[NVP_MAX_URI_SIZE];
}stNVP_SYS_TIME, *lpNVP_SYS_TIME;

typedef struct NVP_RTSP_STREAM
{
	IN int main_index;//0~N
	IN int sub_index; // 0~N
	char main_uri[NVP_MAX_URI_SIZE];
	char sub_uri[NVP_MAX_URI_SIZE];
}stNVP_RTSP_STREAM, *lpNVP_RTSP_STREAM;

typedef struct NVP_PTZ_PRESET
{
	int index;
	char name[NVP_MAX_NT_SIZE];
	char token[NVP_MAX_NT_SIZE];
	bool in_use;
}stNVP_PTZ_PRESET, *lpNVP_PTZ_PRESET;

typedef struct NVP_PTZ_TOUR_ENTRY
{
	int index;
	int preset;
	stNVP_TIME duration;
}stNVP_PTZ_TOUR_ENTRY, *lpNVP_PTZ_TOUR_ENTRY;

typedef struct NVP_PTZ_TOUR
{
	int index;
	char name[NVP_MAX_NT_SIZE];
	char token[NVP_MAX_NT_SIZE];
	int entry_nr;
	stNVP_PTZ_TOUR_ENTRY ptz_tour_entry[NVP_MAX_PTZ_TOUR_ENTRY];
}stNVP_PTZ_TOUR, *lpNVP_PTZ_TOUR;


typedef struct NVP_PTZ_CONFIG
{
	char name[NVP_MAX_NT_SIZE];
	char token[NVP_MAX_NT_SIZE];
	char node_name[NVP_MAX_NT_SIZE];
	char node_token[NVP_MAX_NT_SIZE];

	int protocal;
	int databit;
	int stopbit;
	bool parity;
	// status
	bool pantile_ismoving;
	bool zoom_ismoving;
	float x, y, z; // position: x->pan, y -> tilt, z -> zoom
	// speed
	float default_pan_speed;
	float default_tilt_speed;
	float default_zoom_speed;
	// space
	char default_absolute_pantilt_apace[NVP_MAX_URI_SIZE];
	char default_absolute_zoom_apace[NVP_MAX_URI_SIZE];
	char default_relative_pantilt_apace[NVP_MAX_URI_SIZE];
	char default_relative_zoom_apace[NVP_MAX_URI_SIZE];
	char default_continue_pantilt_apace[NVP_MAX_URI_SIZE];
	char default_continue_zoom_apace[NVP_MAX_URI_SIZE];
	//
	int preset_nr;
	stNVP_PTZ_PRESET preset[NVP_MAX_PTZ_PRESET];
	int tour_nr;
	stNVP_PTZ_TOUR tour[NVP_MAX_PTZ_TOUR];
}stNVP_PTZ_CONFIG, *lpNVP_PTZ_CONFIG;

typedef enum
{
	NVP_PTZ_CMD_NULL = -1,
	NVP_PTZ_CMD_UP,
	NVP_PTZ_CMD_DOWN,
	NVP_PTZ_CMD_LEFT,
	NVP_PTZ_CMD_RIGHT,
	NVP_PTZ_CMD_LEFT_UP,
	NVP_PTZ_CMD_RIGHT_UP,
	NVP_PTZ_CMD_LEFT_DOWN,
	NVP_PTZ_CMD_RIGHT_DOWN,
	NVP_PTZ_CMD_AUTOPAN,
	NVP_PTZ_CMD_IRIS_OPEN,
	NVP_PTZ_CMD_IRIS_CLOSE,
	NVP_PTZ_CMD_ZOOM_IN,
	NVP_PTZ_CMD_ZOOM_OUT,
	NVP_PTZ_CMD_FOCUS_FAR,
	NVP_PTZ_CMD_FOCUS_NEAR,
	NVP_PTZ_CMD_STOP,
	NVP_PTZ_CMD_WIPPER_ON,
	NVP_PTZ_CMD_WIPPER_OFF,
	NVP_PTZ_CMD_LIGHT_ON,
	NVP_PTZ_CMD_LIGHT_OFF,
	NVP_PTZ_CMD_POWER_ON,
	NVP_PTZ_CMD_POWER_OFF,
	NVP_PTZ_CMD_GOTO_PRESET,
	NVP_PTZ_CMD_SET_PRESET,
	NVP_PTZ_CMD_CLEAR_PRESET,
	NVP_PTZ_CMD_TOUR,
	NVP_PTZ_CMD_CNT,
}NVP_PTZ_CMD;

typedef struct NVP_PTZ_CONTROL
{
	IN NVP_PTZ_CMD cmd;
	IN int index; // which preset / tour 
	IN float speed; // 0~ 1.0
	IN int step;
	IN bool repeat;	
}stNVP_PTZ_CONTROL, *lpNVP_PTZ_CONTROL;

typedef enum{
	NVP_BLACK = 0,
	NVP_WHITE,
	NVP_RED,
	NVP_GREEN,
	NVP_BLUE,
	NVP_PINK,
	NVP_GRAY,
	NVP_DIMGRAY,
	NVP_DARKGRAY,
	NVP_SILVER,
	
}NVP_COLOR;

typedef struct NVP_POINT{
	float nX;
	float nY;
}stNVP_POINT, *lpNVP_POINT;

typedef enum{
	NVP_LEFT = 0,
	NVP_RIGHT,
	NVP_TOP,
	NVP_BOTTOM,
	NVP_TOPLEFT,
	NVP_TOPRIGHT,
	NVP_BOTTOMLEFT,
	NVP_BOTTOMRIGHT,
	NVP_CENTER,
	NVP_COUNT,
}NVP_LOCATION;

typedef struct NVP_TITLE_OVERLAY
{
	bool enable; // required
	char alias[64]; //required IF enabled, otherwise optional
	float nX;		// option  max 1; min 0;
	float nY;		// option  max 1;min 0;
	char where_type[16]; // optional , "location" or "point"
	union{
		NVP_LOCATION location;
		stNVP_POINT  point;
	}where;
	char color_type[16]; // optional, "argb" or "name"
	union{
		unsigned int argb;
		NVP_COLOR name;
	}TextColor;// option
	union{
		unsigned int argb;
		NVP_COLOR name;
	}BgColor;// option
}stNVP_TITLE_OVERLAY, *lpNVP_TITLE_OVERLAY;

typedef struct NVP_TIME_OVERLAY
{
	bool enable; // required
	float nX;		// option  max 1 ; min 0
	float nY;		// option  max 1 ; min 0
	int date_type;  //  1:YYYY/MM/DD;  2:MM/DD/YYYY  3:DD/MM/YYYY 4:YYYY-MM-DD  5:MM-DD-YYYY  6:DD-MM-YYYY
	int time_type;  // 1: 12     2:24
	char where_type[16]; // optional , "location" or "point"
	union{
		NVP_LOCATION location;
		stNVP_POINT  point;
	}where;
	char color_type[16]; // optional, "argb" or "name"
	union{
		unsigned int argb;
		NVP_COLOR name;
	}TextColor;// option
	union{
		unsigned int argb;
		NVP_COLOR name;
	}BgColor;// option
}stNVP_TIME_OVERLAY, *lpNVP_TIME_OVERLAY;

typedef struct NVP_DEVICE_ID_OVERLAY
{
	bool enable; // required
	float nX;		// option
	float nY;		// option
	char where_type[16]; // optional , "location" or "point"
}stNVP_DEVICE_ID_OVERLAY, *lpNVP_DEVICE_ID_OVERLAY;

typedef enum
{
	NVP_IO_NOPEN = 0,
	NVP_IO_NCLOSE,
	NVP_IO_DISABLED,
}NVP_IO_MODE;

typedef struct NVP_INPUT_CONFIG
{
	int index; 
	NVP_IO_MODE work_mode;
}stNVP_INPUT_CONFIG, *lpNVP_INPUT_CONFIG;

typedef struct NVP_INPUT_CONFIGS
{
	int nr; // number of input
	stNVP_INPUT_CONFIG entry[16];
}stNVP_INPUT_CONFIGS, *lpNVP_INPUT_CONFIGS;


typedef struct NVP_INPUT_STATUS
{
	IN int index;
	OUT bool trigger;
}stNVP_INPUT_STATUS, *lpNVP_INPUT_STATUS;

typedef struct NVP_OUTPUT
{
	int index;
	bool trigger;
}stNVP_OUTPUT, *lpNVP_OUTPUT;

typedef struct NVP_MD_GRANULARITY
{
	int rowGranularity;
	int columnGranularity;
	unsigned int granularity[64];
	float sensitivity; /* 1 ~ 100 */
	float threshold; /* 1~ 100 */
}stNVP_MD_GRANULARITY, *lpNVP_MD_GRANULARITY;

typedef struct NVP_MD_REGION_CH
{
	int index; 
	char name[NVP_MAX_NT_SIZE];
	int enable;
	float nX, nY, nWidth, nHeight;
	float sensitivity;
	float threshold;
}stNVP_MD_REGION_CH, *lpNVP_MD_REGION_CH;

typedef struct NVP_MD_REGION
{
	stNVP_MD_REGION_CH ch[10];
}stNVP_MD_REGION, *lpNVP_MD_REGION;

typedef struct NVP_MD_CONFIG
{
	int index;
	int enable;
	char rule_name[NVP_MAX_NT_SIZE];
	char module_name[NVP_MAX_NT_SIZE];
	//
	int delay_on_alarm; // unit : ms
	int delay_off_alarm;

#define NVP_MD_TYPE_GRID		( 1 << 0)
#define NVP_MD_TYPE_REGION	(1 << 1)
	int type;
	stNVP_MD_GRANULARITY grid;
	stNVP_MD_REGION	region;
}stNVP_MD_CONFIG, *lpNVP_MD_CONFIG;

typedef struct NVP_PRIVACYMASK_CONFIG{
	int id;
	bool enable;
	stNVP_POINT point1;		// min -1 ,max 1;
	stNVP_POINT point2;		// min -1 ,max 1;
	stNVP_POINT point3;		// min -1 ,max 1;
	stNVP_POINT point4;		// min -1 ,max 1;
	char color_type[16]; // optional, "argb" or "name"
	union{
		unsigned int argb;
		NVP_COLOR name;
	}TextColor;// option
}stNVP_PRIVACYMASK_CONFIG,*lpNVP_PRIVACYMASK_CONFIG;

typedef struct NVP_PRIVACYMASKS_CONFIG
{
	int privacyMaskNum ;
	stNVP_PRIVACYMASK_CONFIG privacyMask[4];
}stNVP_PRIVACYMASKS_CONFIG,*lpNVP_PRIVACYMASKS_CONFIG;




typedef struct NVP_PROFILE_CHN
{
	/* profile_nr ==  venc_nr */
	int index;
	int profile_nr; 
	char name[NVP_MAX_VENC][NVP_MAX_NT_SIZE];
	char token[NVP_MAX_VENC][NVP_MAX_NT_SIZE];
	char ain_name[NVP_MAX_VENC][NVP_MAX_NT_SIZE];
	char ain_token[NVP_MAX_VENC][NVP_MAX_NT_SIZE];

	stNVP_V_SOURCE v_source;
	int vin_conf_nr;
	stNVP_VIN_CONFIG vin[NVP_MAX_VIN_IN_A_SOURCE];
	int venc_nr;
	stNVP_VENC_CONFIG venc[NVP_MAX_VENC];
	stNVP_AIN_CONFIG ain;
	int aenc_nr;
	stNVP_AENC_CONFIG aenc[NVP_MAX_AENC];
	stNVP_VAN_CONFIG van;
	stNVP_MD_CONFIG md;
	stNVP_PTZ_CONFIG ptz;
	stNVP_TITLE_OVERLAY title_ol;
	stNVP_TIME_OVERLAY time_ol;
	stNVP_PRIVACYMASKS_CONFIG privMask;
}stNVP_PROFILE_CHN, *lpNVP_PROFILE_CHN;

typedef struct NVP_PROFILE
{
	int chn;
	stNVP_PROFILE_CHN profile[NVP_MAX_CH];
}stNVP_PROFILE, *lpNVP_PROFILE;


typedef enum
{
	NVP_EVENT_ALL = 0,
	NVP_EVENT_MD = 1,      // 
	NVP_EVENT_MD_EX,	// for image videosource/motion
	NVP_EVENT_IO_IN,
	NVP_EVENT_IO_OUT,
	NVP_EVENT_VIDEO_LOSS,
	NVP_EVENT_TOO_BLURRY,
	NVP_EVENT_TOO_DARK,
	NVP_EVENT_TOO_BRIGHT,
	NVP_EVENT_SCENE_CHANGE,
	NVP_EVENT_PROFILE_CHANGED,
	NVP_EVENT_VENC_CONF_CHANGED,
	NVP_EVENT_VSOURCE_CONF_CHANGED,
	NVP_EVENT_VOUT_CONF_CHANGED,
	NVP_EVENT_AENC_CONF_CHANGED,
	NVP_EVENT_ASOURCE_CONF_CHANGED,
	NVP_EVENT_AOUT_CONF_CHANGED,
	NVP_EVENT_VAN_CONF_CHANGED,
	NVP_EVENT_PTZ_CONF_CHANGED,

	NVP_EVENT_CNT
}enNVP_EVENT;

typedef struct NVP_EVENT
{
	IN int type; // required
	IN int index;
	bool status;
}stNVP_EVENT, *lpNVP_EVENT;

typedef void (*fNVPEventHook)(int event, int lparam, void* rparam, void *custom /* top-level-param */);

typedef struct NVP_SUBSCRIBE
{
	int event;
	fNVPEventHook hook;
	void *hook_custom; // hock custom paramter
	
	unsigned int keeplive_time; 
	int subs_user;
	int subs_id;
}stNVP_SUBSCRIBE, *lpNVP_SUBSCRIBE;

typedef struct NVP_USER
{
	char snapshot_url[256];	
}stNVP_USER, *lpNVP_USER;

typedef struct NVP_RTSP_URL{
	int save_load_status;
	char rtsp_url_main[256];
	char rtsp_url_sub[256];
}stNVP_RTSP_URL,*lpNVP_RTSP_URL;

typedef struct NVP_ENV
{
	stNVP_DEV_INFO devinfo;
	stNVP_SYS_TIME systime;
	stNVP_ETHER_CONFIG ether;
	stNVP_PROFILE profiles;
	stNVP_USER user;
	stNVP_RTSP_URL rtsp;
}stNVP_ENV, *lpNVP_ENV;

typedef struct NVP_CMD
{
	stNVP_PTZ_CONTROL ptz;
	
}stNVP_CMD, *lpNVP_CMD;

typedef enum{
	NVP_SNAP_SHOT_LINK = 0,
	NVP_SNAP_SHOT_FILE = 1,
	NVP_SNAP_SHOT_MEM = 2,
	NVP_SNAP_SHOT_COUNT,
}NVP_SNAP_SHOT_TPYE;

#define MAX_SNAP_SHOT_LINK		(256)

typedef struct NVP_SNAP_SHOT
{
	NVP_SNAP_SHOT_TPYE type;
	char Link[MAX_SNAP_SHOT_LINK];
}stNVP_SNAP_SHOT, *lpNVP_SNAP_SHOT;

typedef enum{
	NVP_WIRELESS_MODE_AP = 0,
	NVP_WIRELESS_MODE_STA,
	NVP_WIRELESS_MODE_REPEATER,
	NVP_WIRELESS_MODE_COUNT,
}NVP_WIRELESS_MODE;

#define NVP_WIRELESS_MAX_MODE_NUM (8)

typedef struct NVP_WIRELESS_STA_MODE{
	char wirelessApEssId[NVP_MAX_NT_SIZE];
	char wirelessApPsk[NVP_MAX_NT_SIZE];
}stNVP_WIRELESS_STA_MODE, *lpNVP_WIRELESS_STA_MODE;

typedef struct NVP_WIRELESS_AP_INFO{
	char wirelessEssId[NVP_MAX_NT_SIZE];
	char wirelessPsk[NVP_MAX_NT_SIZE];
	int channel;
}stNVP_WIRELESS_AP_MODE, *lpNVP_WIRELESS_AP_MODE;

typedef struct NVP_WIRELESS_REPEATER_INFO_OPT{
	int wiredBandWidthOptNum;
	int repeaterModeOptNum;
	int repeaterWorkModeOptNum;
	int wiredMaxConn;
	int wirelessMaxConn;
	char wiredBandWidthOpt[NVP_WIRELESS_MAX_MODE_NUM][NVP_MAX_NT_SIZE];
	char repeaterModeOpt[NVP_WIRELESS_MAX_MODE_NUM][NVP_MAX_NT_SIZE];
	char repeaterWorkModeOpt[NVP_WIRELESS_MAX_MODE_NUM][NVP_MAX_NT_SIZE];
}stNVP_WIRELESS_REPEATER_MODE_OPT, *lpNVP_WIRELESS_REPEATER_MODE_OPT;

typedef struct NVP_WIRELESS_REPEATER_INFO{
	char wiredBandWidth[NVP_MAX_NT_SIZE];
	char repeaterMode[NVP_MAX_NT_SIZE];
	char repeaterWorkMode[NVP_MAX_NT_SIZE];
	int wiredCurMaxConn;
	int wirelessCurMaxConn;
}stNVP_WIRELESS_REPEATER_MODE, *lpNVP_WIRELESS_REPEATER_MODE;

typedef struct NVP_WIRELESS_INFO
{
	char wireless_mode[NVP_MAX_NT_SIZE/2];
	char repeater_id[NVP_MAX_NT_SIZE*2];
	char wireless_mode_opt[NVP_WIRELESS_MAX_MODE_NUM][NVP_MAX_NT_SIZE/2];
	stNVP_WIRELESS_STA_MODE staMode;
	stNVP_WIRELESS_STA_MODE staModeBackup;
	stNVP_WIRELESS_AP_MODE apMode;
	stNVP_WIRELESS_REPEATER_MODE repeater;
	stNVP_WIRELESS_REPEATER_MODE_OPT repeaterOpt;
	int wireless_mode_opt_num;
	int dhcp_enable;//ap dhcp server
	int dhcp_auto_set;//sta dhcp client
}stNVP_WIRELESS_INFO, *lpNVP_WIRELESS_INFO;

#define NVP_WIRELESS_MAX_STA_NUM (8)
typedef struct NVP_WIRELESS_STA_INFO{
	int num;
	unsigned char mac[NVP_WIRELESS_MAX_STA_NUM][6];
	unsigned int rate[NVP_WIRELESS_MAX_STA_NUM];
}stNVP_WIRELESS_STA_INFO, *lpNVP_WIRELESS_STA_INFO;

typedef struct NVP_REPEATER_INFO
{
	stNVP_WIRELESS_STA_MODE staMode;
	stNVP_WIRELESS_AP_MODE apMode;
	int wiredCurConn;
	int wirelessCurConn;
	int wiredMaxConn;
	int wirelessMaxConn;
	char currentWorkMode[NVP_MAX_NT_SIZE];
	char swVersion[NVP_MAX_NT_SIZE*2];
	char deviceId[NVP_MAX_NT_SIZE*2];
}stNVP_REPEATER_INFO, *lpNVP_REPEATER_INFO;

#define OM_ALL		"all"
#define OM_INFO		"system_info"
#define OM_PROFILE	"chn_profile"		// single channel profile
#define OM_PROFILES	"profiles"			// all channels profiles
#define OM_DTIME		"date_time"
#define OM_NET		"network"
#define OM_VENC		"video_encode"
#define OM_VSRC		"video_source"
#define OM_VINC		"video_source_conf"
#define OM_IMG		"image"
#define OM_COLOR		"color"
#define OM_AENC		"audio_encode"
#define OM_AIN		"audio_source"
#define OM_MD		"md"
#define OM_AND		"|"
#define OM_OSD		"osd"
#define OM_PRIVMASK	"privmask"
//
#define OM_REBOOT	"reboot"
#define OM_SYS_RESET	"factory_default"
#define OM_PTZ		"ptz"
#define OM_IFRAME   "key_frame"

//for snapshot
#define OM_USER		"user"

#define OM_RTSP		"rtsp_url"

#define OM_MATCH(src, target) (strncmp(src, target, strlen(target)) == 0)

/*
keyid: chn * 100 + stream_index
*/
extern int NVP_env_load(lpNVP_ENV nvp, const char *module, int keyid);
extern int NVP_env_save(lpNVP_ENV nvp, const char *module, int keyid);
extern int NVP_env_cmd(lpNVP_CMD cmd, const char *module, int keyid);

#ifdef __cplusplus
}
#endif
#endif


