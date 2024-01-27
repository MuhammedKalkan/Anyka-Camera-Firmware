#ifndef _AK_DEMUX_H_
#define _AK_DEMUX_H_

#include "ak_global.h"

/* demux media data type */
enum demux_data_type {
	DEMUX_DATA_UNKNOW = 0x00,
	DEMUX_DATA_VIDEO,
	DEMUX_DATA_AUDIO
};


enum video_demux_type {
	VIDEO_DEMUX_UNKNOWN = 0,
	VIDEO_DEMUX_H264,
	VIDEO_DEMUX_H265,
};

enum audio_demux_type {
	AUDIO_DEMUX_UNKNOWN = 0,
	AUDIO_DEMUX_MP3,
	AUDIO_DEMUX_AMR,
	AUDIO_DEMUX_AAC,
	AUDIO_DEMUX_WMA,
	AUDIO_DEMUX_PCM,
	AUDIO_DEMUX_PCM_ALAW,
	AUDIO_DEMUX_PCM_ULAW,
};

enum demux_direction {
	DEMUX_DIRECTION_NEAREST = 0,	//seek to nearest key frame
	DEMUX_DIRECTION_LAST,			//seek to last key frame
	DEMUX_DIRECTION_NEXT			//seek to next key frame
};

struct demux_stream {
	unsigned char *data;	/* demux stream data */
	unsigned int len; 		/* demux stream len in bytes */
	unsigned long long ts;	/* demux timestamp(ms) */
	unsigned long long start_uts;	/* video start unix timestamp(ms) */
	enum video_frame_type frame_type;	/* demux I or P frame */
};

struct ak_demux_media_info {
	//video
	unsigned int fps;
	unsigned long totaltime_ms;

	//audio
	enum audio_demux_type audio_type;
	unsigned int sample;
};

/* get current version string pointer */
const char* ak_demuxer_get_version(void);

/**
 * ak_demux_open - open dumxer lib
 * @file_path[IN]: media file path, including full name
 * @start[IN]: start flag
 * return: demux handle, NULL failed
 */
void* ak_demux_open(const char *file_path, int start);

/**
 * ak_demux_get_enc_type - get data encode type after open demux
 * @demux_handle[IN]: opened demux handle
 * @video_type[OUT]: demux video encode type
 * @audio_type[OUT]: demux audio encode type
 * return: 0 success, -1 failed
 */
int ak_demux_get_enc_type(void *demux_handle,
		int *video_type, int *audio_type);

/*
 * ak_demux_get_data - get data and which audio and video ts has 
 *                         been synchronized.
 * @demux_handle[IN]: opened demux handle
 * @type[IN]: demux type
 * return: the stream packet after demux, NULL failed
 */
struct demux_stream* ak_demux_get_data(void *demux_handle,
						enum demux_data_type *type);
/**
 * ak_demux_free_data: free the demux resource
 * @video[IN]: the stream packet after demux
 * return: void
 */
void ak_demux_free_data(struct demux_stream *video);

/**
 * ak_demux_get_total_time: get media file's total time
 * @file_path[IN]: media file path, including full name
 * return: total time in ms
 */
int ak_demux_get_total_time(const char *file_path);

/**
 * ak_demux_close: close dumxer lib
 * @demux_handle[IN]: opened demux handle
 * return: void
 */
void ak_demux_close(void *demux_handle);


/**
 * ak_demux_get_spec_keyframe: get speciffically key frame
 * @demux_handle[IN]: return by demux_open()
 * @ms[IN]: offset time base on file-begin
 * @direct[IN]: setting the direction
 * return: 0: get closely keyframe, else return -1; 
 */
int ak_demux_seek_to_keyframe(void *demux_handle, unsigned long ms, enum demux_direction direct);

/**
 * ak_demux_get_spec_keyframe: get speciffically key frame
 * @demux_handle[IN]: return by demux_open()
 * @info[OUT]: store media info
 * return: 0 on succeess, -1 failed
 */
int ak_demux_get_media_info(void *demux_handle, struct ak_demux_media_info *info);

struct demux_stream* ak_demux_get_video(void *demux_handle);

struct demux_stream* ak_demux_get_audio(void *demux_handle);

#endif
