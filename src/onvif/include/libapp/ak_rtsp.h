#ifndef _AK_RTSP_H_
#define _AK_RTSP_H_

#include "ak_venc.h"

#define AK_RTSP_SUFFIX_NAME_LEN	 (10) //suffix name, just like "vs0" ans so on

/* rtst channel index id, DONT change its defination */
enum rtsp_channel {
	RTSP_CHANNEL_MAIN = 0,
	RTSP_CHANNEL_SUB = 1,
	RTSP_CHANNEL_NUM
};

struct rtsp_channel_cfg_t {
	enum rtsp_channel current_channel;	/* channel index */
	int width;					/* camera resolutions */
	int height;

	int fps;					/* video encode normally configurations */
	int max_kbps;
	int min_qp;
	int max_qp;
	int gop_len;

	int video_enc_type;						/* h.264 or h.265 */
	int target_ratio;		    			/* ratio of target kbps and max kbps */
	enum bitrate_ctrl_mode video_br_mode;	/* cbr or vbr */

	void *vi_handle;							/* video input handle */
	char suffix_name[AK_RTSP_SUFFIX_NAME_LEN];	/* suffix name */
	struct venc_smart_cfg smart;				/* smart config pointer */
};

struct rtsp_param {
	struct rtsp_channel_cfg_t rtsp_chn[RTSP_CHANNEL_NUM];
};

/**
 * ak_rtsp_get_version - get rtsp version
 * return: version string
 */
const char* ak_rtsp_get_version(void);

/**
 * ak_rtsp_init - 
 * @param: rtsp config param
 * return: 0 -> success, -1 -> failed
 */
int ak_rtsp_init(struct rtsp_param *param);

/**
 * ak_rtsp_start - start rtsp
 *                 run video thread to get stream
 *
 * @deprecated >= 1.9.00
 *
 * @index: [0, 1], sub, main channel
 * return: 0 -> success, -1 -> failed
 */
int ak_rtsp_start(int index);

/**
 * ak_rtsp_stop - stop rtsp
 *                stop video thread
 *
 * @deprecated >= 1.9.00
 *
 * @index: [0, 1], sub, main channel
 * return: 0 -> success, -1 -> failed
 */
int ak_rtsp_stop(int index);

/**
 * ak_rtsp_exit - exit rtsp app
 * @void
 * return: void
 */
void ak_rtsp_exit(void);

#endif
