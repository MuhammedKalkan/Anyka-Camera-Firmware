#ifndef _AK_VIDEO_ENCODE_H_
#define _AK_VIDEO_ENCODE_H_

#include "ak_global.h"

/* encode group define */
enum encode_group_type {
	ENCODE_RECORD,   	//local record
	ENCODE_MAINCHN_NET, //main-chn net
	ENCODE_SUBCHN_NET,  //sub-chn net
	ENCODE_PICTURE,  	//picture
	ENCODE_GRP_NUM 		//total
};

/* encode use channel define */
enum encode_use_chn {
	ENCODE_MAIN_CHN,
	ENCODE_SUB_CHN
};

/* h.264 / h.265 encode control define */
enum bitrate_ctrl_mode {
	BR_MODE_CBR,
	BR_MODE_VBR
};

/* video encode output type define */
enum encode_output_type {
	H264_ENC_TYPE,
	MJPEG_ENC_TYPE,
	HEVC_ENC_TYPE
};

enum profile_mode {
	PROFILE_MAIN,
	PROFILE_HIGH,
	PROFILE_BASE,
	PROFILE_HEVC_MAIN,
	PROFILE_HEVC_MAIN_STILL
};

enum enc_method {
	METHOD_DEFAULT,	//no specifically method
	METHOD_ISIZE_CTRL,	//take I size under some value
	METHOD_SMART_H264	//enable smart h.264
};

struct venc_roi_param {
	int enable;	//1 enable, 0 disable
	long top;
	long bottom;
	long left;
	long right;
	long delta_qp;
};

struct encode_param {
	unsigned long width;		//real encode width, to be divisible by 4
	unsigned long height;		//real encode height, to be divisible by 4
	signed long   minqp;		//Dynamic bit rate parameter[20,25]
	signed long   maxqp;		//Dynamic bit rate parameter[45,50]
	signed long   fps;          //frame rate
	signed long   goplen;       //IP FRAME interval
	signed long   bps;	        //target bps
	enum profile_mode profile; 			//profile mode
	enum encode_use_chn use_chn;		//encode channel, 0: main, 1 sub
	enum encode_group_type enc_grp;		//encode group
	enum bitrate_ctrl_mode br_mode; 	//bitrate control mode, vbr or cbr
	enum encode_output_type enc_out_type;//encode output type, h264 or jpeg or h265
};

struct venc_rate_stat {
	int bps;    //unit: kbps
	float fps;
    int gop;
};

/* 
 * smart mode config, just work on VBR mode
 */

struct venc_smart_cfg {
	int smart_mode;			//0:disable smart, 1:mode of LTR, 2:mode of changing GOP length
	int smart_goplen;		//smart goplen
	int smart_quality; 		//smart quality
	int smart_static_value; //smart static value
};


/**
 * This Structure  Indicate an YUV Frame Data Structure. \n
 *
 * There are YUV, Y-Dem, UV-Dem, U-Dem, V-Dem to Describe All the Plane Data Memory Data in YUV Frame.\n
 * YUV, Y-Dem, UV-Dem, U-Dem, V-Dem are NOT All Valid at the Same Time.\n
 * For Example:\n
 * To Only Y Frame, Only Y-Dem is Valid, All Dem are Ignore.\n
 * To NV12, YUV, Y-Dem, UV-Dem are Valid, U-Dem, V-Dem are Ignore.\n
 *
 *
 */
struct venc_yuv_frame {

	int width;              ///< Frame Width (Unit: Bytes)
	int height;             ///< Frame Height (Unit: Bytes)

	int seqno;              ///< Frame Generation Sequence Number.
	unsigned long long ts;  ///< YUV Timestamp (Unit: ms)

	unsigned char *pixels;  ///< Raw Data Memory Address.
	int size;               ///< Raw Data Size.

	unsigned char *y;       ///< Y-D outof YUV Raw Data Offset, If Invalid It Should be NULL.
	int yStride;            ///< Y-D Line Strdie (Uint: Bytes)
	int ySize;              ///< Y-D Data Length

	unsigned char *u;      ///< U-D outof YUV Raw Data Offset, If Invalid It Should be NULL.
	int uStride;           ///< U-D Data Length
	int uSize;             ///< U-D Data Length

	unsigned char *v;      ///< V-D outof YUV Raw Data Offset, If Invalid It Should be NULL.
	int vStride;           ///< V-D Data Length
	int vSize;             ///< V-D Data Length

	unsigned char *uv;      ///< UV-D outof YUV Raw Data Offset, If Invalid It Should be NULL.
	int uvStride;           ///< UV-D Data Length
	int uvSize;             ///< UV-D Data Length

};



/**
 * ak_venc_get_version - get venc version
 * return: version string
 */
const char* ak_venc_get_version(void);

/**
 * Get Current Video Encoder Configuration File Path.
 *
 * @author luo_zhanzhao
 *
 * @param[OUT] stack
 *  File Path String Stored When Success.
 *
 * @param[IN] stacklen
 *  The Length of Memory @ref stack Indicated.
 *
 * @return
 *  Return the Configuration File Path If Success, the Path Would be Stored on Memory @stack Also.
 *  Failed Return -1.
 *
 */
const char *ak_venc_get_cfg_path (char *stack, int stacklen);


/**
 * Set Video Encoder Configuration File Path.\n
 * If Ignore this Function, The Configuration File Path Default </etc/jffs2/venc.cfg>.\n
 * If User Wanna to Change the Configuration File Path.\n
 * User MUST Call this Function to Setup a New Path before @ref ak_venc_open() Called.\n
 * Otherwires, the Configuration Would be Ignored.
 *
 * @author luo_zhanzhao
 *
 * @param[IN] path
 *  New Configuration File Path.
 *
 * @return
 *  Return 0 If Success, Else Return -1.
 *
 */
int ak_venc_set_cfg_path (const char *path);


/**
 * ak_venc_open - open encoder and set encode param
 * @param[IN]: encode param
 * return: on success return encode handle, failed return NULL.
 */
void *ak_venc_open(const struct encode_param *param);

/**
 * ak_venc_send_frame - encode single frame
 * @enc_handle[IN]: encode handle
 * @frame[IN]: frame which you want to encode
 * @frame_len[IN]: lenght of frame
 * @stream[OUT]: encode output buffer address
 * return: on success, return the length of output,
 *         on failed, -1 is return.
 * notes: when working by single mode, the memory of stream->data should
 * 		  free by user
 */
int ak_venc_send_frame(void *enc_handle, const unsigned char *frame,
		unsigned int frame_len, struct video_stream *stream);

/*
 * ak_venc_send_frame_ex - encode single frame
 * @enc_handle[IN]: encode handle
 * @frame[IN]: frame which you want to encode
 * @frame_len[IN]: lenght of frame
 * @stream[OUT]: encode output buffer address
 * return: 0 success; -1 failed
 * note: IMPORTANT-make sure your stream->data can contain encoded data
 */
int ak_venc_send_frame_ex(void *enc_handle, const unsigned char *frame,
		unsigned int frame_len, struct video_stream *stream);

/**
 * ak_venc_get_fps - get encode frames per second
 * @enc_handle[IN]: enc_handle return by 'ak_venc_open'
 * return: on success the fps, failed 0
 */
int ak_venc_get_fps(void *enc_handle);

/**
 * ak_venc_set_fps-reset encode fps
 * @enc_handle[IN]: encode handle return by 'ak_venc_open'
 * @fps[IN]: fps you want to set
 * return: 0 on success, -1 no effect
 * notes: the 'fps' should be careful do not out of range[1, sensor max fps]
 */
int ak_venc_set_fps(void *enc_handle, int fps);

/**
 * ak_venc_get_kbps - get encode kbps
 * @enc_handle[IN]:  enc_handle return by 'ak_venc_open'
 * return: on success return actually kbps ;-1:failed
 */
int ak_venc_get_kbps(void *enc_handle);

/*
 * ak_venc_get_actual_kbps - get encode target bps and max bps
 * @enc_handle[IN]:  enc_handle return by 'ak_venc_open'
 * @target_bps[OUT]: store target bps
 * @max_bps[OUT]:    store max bps
 * return: >=0 kbps ;-1:failed
 */
int ak_venc_get_actual_kbps(void *enc_handle, int *target_bps, int *max_bps);

/**
 * ak_venc_set_rc - reset encode bitpersecond
 * @enc_handle[IN]: encode handle return by 'ak_venc_open'
 * @bps[IN]: bps you want to set
 * return: 0 on success, -1 faield
 */
int ak_venc_set_rc(void *enc_handle, int bps);

/*
 * ak_venc_set_kbps - set encode bsp 
 * @enc_handle[IN]: encode handle return by 'ak_venc_open'
 * @target_bps[IN]: target bps you want to set
 * @max_bps[IN]: max bps you want to set
 * notes: target_bps should <= max_bps
 * return: 0 on success, others faield or no effect
 */
int ak_venc_set_kbps(void *enc_handle, int target_bps, int max_bps);

/**
 * ak_venc_set_iframe - set next encode frame to I frame
 * @enc_handle[IN], encode handle, return by 'ak_venc_open'
 * return: 0 on success, -1 failed
 */
int ak_venc_set_iframe(void *enc_handle);

/**
 * ak_venc_get_gop_len - get encode gop len
 * @enc_handle[IN]: encode handle
 * return: value of GOP len, -1 failed
 * notes:
 */
int ak_venc_get_gop_len(void *enc_handle);

/**
 * ak_venc_set_gop_len - set encode gop len
 * @enc_handle[IN]: encode handle
 * @gop_len[IN]: value of GOP len
 * return: 0 success, -1 failed
 * notes: set new gop len after you change encode fps
 */
int ak_venc_set_gop_len(void *enc_handle, int gop_len);

/**
 * ak_venc_set_roi - set ROI param
 * @enc_handle[IN]: encode handle
 * @roi[IN]: ROI param
 * return: 0 success, -1 failed
 */
int ak_venc_set_roi(void *enc_handle, struct venc_roi_param *roi);

/**
 * ak_venc_set_br - set video bitrate control mode, cbr or vbr
 * @enc_handle[IN]: encode handle
 * @mode[IN]: see define of enum bitrate_ctrl_mode
 * return: 0 success, -1 failed
 */
int ak_venc_set_br(void *handle, enum bitrate_ctrl_mode mode);

/**
 * ak_venc_set_profile - set video profile, main/high/base
 * @enc_handle[IN]: encode handle
 * @profile[IN]: see define of enum profile
 * return: 0 success, -1 failed
 */
int ak_venc_set_profile(void *handle, enum profile_mode profile);

/**
 * ak_venc_set_rc_weight - set video rate control weight
 * @enc_handle[IN]: encode handle
 * @weight[IN]: quality weight, [0, 100]
 * return: 0 success, -1 failed
 * notes:  quality weight 0 is the best quality, 100 is the lowest bitrate.
 */
int ak_venc_set_rc_weight(void *enc_handle, int weight);

/**
 * ak_venc_set_mjpeg_qlevel - set mjpeg quality level
 * @handle[IN]: encode handle return by ak_venc_open()
 * @level[IN]:quality level, 0-9
 * return: 0 success, specifically error number
 */
int ak_venc_set_mjpeg_qlevel(void *handle, int level);

/**
 * ak_venc_set_method - set video encode method
 * @enc_handle[IN]: encode handle return by ak_venc_open()
 * @method[IN]: encode method, for more ditail see defination of
 * 				enum enc_method.
 * return: 0 success, -1 failed
 * notes:
 */
int ak_venc_set_method(void *enc_handle, enum enc_method method);

/**
 * ak_venc_set_check_scene - switch scene check to change video encode param
 * @enc_handle[IN]: encode handle return by ak_venc_open()
 * @enable[IN]: switch flag, 0 close, 1 open.
 * return: 0 success, -1 failed
 * notes: the default switch flag is open.
 */
int ak_venc_set_check_scene(void *enc_handle, int enable);

/**
 * ak_venc_set_smart_config - enable or disable smart function
 * @enc_handle[IN]: encode handle return by ak_venc_open()
 * @cfg[IN]: pointer to venc_smart_cfg
 * return: 0 success, -1 failed
 * notes: call after open before request stream
 */
int ak_venc_set_smart_config(void *enc_handle, struct venc_smart_cfg *cfg);


/**
 * ak_venc_close - close video encode
 * @enc_handle[IN]: encode handle return by ak_venc_open()
 * return: 0 success, -1 failed
 * notes: if you request a stream, you should call this function
 *        after you call 'ak_venc_cancel_stream()' success.
 */
int ak_venc_close(void *enc_handle);

/**
 * ak_venc_request_stream - bind vi and venc handle start capture
 * @vi_handle[IN]: video input handle, return by ak_vi_open()
 * @enc_handle[IN]: encode handle, return by ak_venc_open()
 * return: on success return stream_handle, failed return NULL
 * notes:
 */
void* ak_venc_request_stream(void *vi_handle, void *enc_handle);

/**
 * ak_venc_get_stream - on stream-encode, get encode output stream
 * @stream_handle[IN]: stream handle
 * @stream[IN]: stream data
 * return: 0 success, failed return specifically error
 * notes: if you got a error, you can use 'ak_get_error_no()' or
 *        'ak_get_error_str()' to get more detail.
 */
int ak_venc_get_stream(void *stream_handle, struct video_stream *stream);

/**
 * ak_venc_release_stream - release stream resource
 * @stream_handle[IN]: stream handle
 * @stream[IN]: stream return by get()
 * return: 0 success, -1 failed
 * notes:
 */
int ak_venc_release_stream(void *stream_handle, struct video_stream *stream);

/**
 * ak_venc_get_rate_stat - on stream-encode, get encode rate stat info
 * @stream_handle[IN]: stream handle
 * @stat[IN]: stream rate stat info
 * return: 0 success, -1 failed
 * notes:
 */
int ak_venc_get_rate_stat(void *stream_handle, struct venc_rate_stat *stat);

/**
 * ak_venc_cancel_stream - cancel stream encode
 * @stream_handle[IN]: encode handle return by ak_venc_request_stream()
 * return: 0 success, specifically error number
 * notes: 1. make sure do not call ak_venc_get_stream when call this function.
 *      2. if you got a error, you can use 'ak_get_error_no()' or
 *       'ak_get_error_str()' to get more detail.
 */
int ak_venc_cancel_stream(void *stream_handle);

/**
 * Fetch Video Pre-Process YUV Raw Data, Which would Store on P-Memeory User Indicated.\n
 * User MUST Release this Frame with @ref ak_venc_release_yuv().
 *
 * Caution:\n
 *  The Interface Would Block till One Encode PreProcess Frame Captured.\n
 *  User Should Confirm this Interface Woud not be Called After the Video Encode Application Exit.
 *
 * @author luo_zhanzhao
 *
 * @param[IN] width
 *  The YUV Width User Indicated.
 *  If Pass -1, Sytem Would Use Default Image Width.
 *
 * @param[IN] height
 *  The YUV Height User Indicated.
 *  If Pass -1, Sytem Would Use Default Image Height.
 *
 *
 * @param[OUT] yuv_frame
 *  Return the Frame Structure Data Field When Fetch Success, See also @ref venc_yuv_frame.
 *
 * @return
 *  If Success Return 0, Failed Return -1.
 */
int ak_venc_get_yuv (int width, int height, struct venc_yuv_frame *yuv_frame);

/**
 * Release the YUV Frame Allocated by @ref ak_venc_get_yuv().
 *
 * @author luo_zhanzhao
 *
 */
int ak_venc_release_yuv (struct venc_yuv_frame *yuv_frame);


#endif
