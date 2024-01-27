#ifndef _AK_DVR_RECORD_H_
#define _AK_DVR_RECORD_H_

#include "ak_dvr_common.h"
#include "ak_venc.h"
#include "ak_mux.h"

typedef void (* FP_VIDEO_RECORD_ERROR)(int error_code, const char *error_desc);

struct record_param {
	void *vi_handle;		//video input handle
	void *ai_handle;		//audio input handle

	/* venc param */
	unsigned long width;	//real encode width, to be divisible by 4
	unsigned long height;	//real encode height, to be divisible by 4
	enum encode_use_chn enc_chn;	//encode use channel
	int file_fps;			//encode fps
	int minqp;
	int maxqp;
	int br_mode;
	int enc_grp;
	int file_bps;			//target bps
	int gop_len;			//GOP len
	int video_type;         //record video type
	int method;                  //0 default, bit[0-3]: smart h.264, skip, ctrl I size, high bitrate control

	/* aenc param */
	unsigned int sample_rate;
	int frame_interval;		//audio frame interval
	int audio_type;         //record audio type

	/* record file param */
	enum dvr_file_type file_type;//save file type
    time_t duration;		//record duration time(ms)

	int alarm_pre_time; //keeping video and aud
	int alarm_stop_time;//stop record after kee
	int alarm_limit_time;//record limit time	

    FP_VIDEO_RECORD_ERROR error_report;
};

/**
 * ak_dvr_record_get_version - get dvr version
 * return: version string
 * notes: make sure get the correct version before analyze problem
 */
const char* ak_dvr_record_get_version(void);

/**
 * ak_dvr_record_turn_on_trigger - start video pre_record.
 *                     get audio and video data and save in list.
 *                     do not save video & audio stream to file;
 * @trigger_time[IN]: pre_record time, [1, ~), unit: second
 * return: 0, success;  -1, failed
 * notes: 1. firstly, set prerecord flag on.
 *		2. if now no record, start pre_record.
 */
int ak_dvr_record_turn_on_trigger(int trigger_time);

/**
 * ak_dvr_record_off_trigger - stop video pre_record
 * @void:
 * return: 0, success;  -1, failed
 * notes: 1. should be called after ak_dvr_record_turn_on_trigger.
 *           2. firstly, set prerecord flag off.
 *		3. if now prerecord, stop pre_record.
 */
int ak_dvr_record_turn_off_trigger(void);


/**
 * ak_dvr_record_get_venc_handle - get the video encoder using handle
 * return: venc_handle, success;  NULL, failed
 */
void *ak_dvr_record_get_venc_handle(void);

/**
 * ak_dvr_record_start - start video record. prepare any resource,
 *          save video & audio stream to file;
 *          if on_trigger , save pre_record to record file.
 * @type[in]:  plan or alarm
 * return: 0, success;  -1, failed
 * notes: 1. should be called after ak_dvr_record_init or ak_dvr_record_stop
 */
int ak_dvr_record_start(enum dvr_record_type type);

/**
 * ak_dvr_record_stop - stop video record.
 *                              if on_trigger, do pre_record.
 * @void
 * return: 0, success;  -1, failed
 * notes: 1. should be called after ak_dvr_record_start
 */
int ak_dvr_record_stop(void);

/**
 * ak_dvr_record_init - init video record env and start video record thread.
 * @param: video record init param, make sure provide the correct param.
 * return: 0, success;  -1, failed
 * notes: 1. call this before use all other interface of dvr module
 *		2. if need reinit, should call record_exit first.
 */
int ak_dvr_record_init(const struct record_param *param);

/**
 * ak_dvr_record_exit: clear video record env and stop video record thread.
 * @void
 * return: 0, success;  -1, failed
 * notes: 1. stop record if still working.
 *		2. release any resource that create by ak_dvr_record_init.
 */
int ak_dvr_record_exit(void);

#endif
