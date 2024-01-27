#ifndef _AK_MUX_H_
#define _AK_MUX_H_

#include "ak_global.h"

enum video_mux_type {
	VIDEO_MUX_TYPE_UNKNOWN,
	VIDEO_MUX_TYPE_MPEG4,
	VIDEO_MUX_TYPE_H263,
	VIDEO_MUX_TYPE_WMV,
	VIDEO_MUX_TYPE_FLV263,
	VIDEO_MUX_TYPE_H264,
	VIDEO_MUX_TYPE_RV,
	VIDEO_MUX_TYPE_MJPEG,
	VIDEO_MUX_TYPE_MPEG2,
	VIDEO_MUX_TYPE_H265,
};

enum audio_mux_type {
	AUDIO_MUX_TYPE_UNKNOWN,
	AUDIO_MUX_TYPE_AMR,
	AUDIO_MUX_TYPE_MP3,
	AUDIO_MUX_TYPE_AAC,
	AUDIO_MUX_TYPE_PCM,
	AUDIO_MUX_TYPE_WMA,
	AUDIO_MUX_TYPE_MIDI,
	AUDIO_MUX_TYPE_ADPCM,
	AUDIO_MUX_TYPE_APE,
	AUDIO_MUX_TYPE_FLAC,
	AUDIO_MUX_TYPE_RA,
	AUDIO_MUX_TYPE_VORBIS,
	AUDIO_MUX_TYPE_AC3,
	AUDIO_MUX_TYPE_G711,
	AUDIO_MUX_TYPE_SPEEX
};

enum record_type {
	RECORD_TYPE_AVI_NORMAL,
	RECORD_TYPE_AVI_CYC,
	RECORD_TYPE_3GP,
	RECORD_TYPE_AVI_SEGMENT
};
#define RECORD_TYPE_MP4		RECORD_TYPE_3GP

enum mux_status {
	MUX_STATUS_OPEN,
	MUX_STATUS_STOP,
	MUX_STATUS_DOING,
	MUX_STATUS_SYSERR,
	MUX_STATUS_MEMFULL,
	MUX_STATUS_SYNERR,
	MUX_STATUS_WAITING
};

enum format_tag {
	FORMAT_TAG_UNKNOWN = 0,
    FORMAT_TAG_PCM_ALAW = 0x6,
    FORMAT_TAG_PCM_ULAW = 0x7,
};

struct mux_input {
	/* video */
	enum record_type media_rec_type;
	enum video_mux_type video_type;
	unsigned short width;
	unsigned short height;
	int file_fps;					//save file fps
	unsigned long long start_ts;	//mux start ts(ms)

	/* audio */
	unsigned char capture_audio;
	enum audio_mux_type audio_type;
	unsigned long sample_rate;
	int frame_interval;
	int format_tag;
	unsigned int record_second;
};

/**
 * ak_muxer_get_version - get muxer version
 * return: version string
 */
const char* ak_muxer_get_version(void);

/**
 * ak_mux_open - open mux lib
 * @mux_param[IN]: mux input param
 * @record_fp[IN]: save file pointer
 * @index_fp[IN]: tmp file pointer
 * return: mux handle, NULL failed
 */
void* ak_mux_open(const struct mux_input *mux_param,
				FILE *record_fp, FILE *index_fp);

/**
 * ak_mux_add_audio - add audio to mux
 * @mux_param[IN]: opened mux handle
 * @audio[IN]: audio stream info
 * return: 0 success, -1 failed, >0 other mux status
 */
int ak_mux_add_audio(void *mux_handle, const struct audio_stream *audio);

/**
 * ak_mux_add_video - add video to mux
 * @mux_param[IN]: opened mux handle
 * @video[IN]: video stream info
 * return: 0 success, -1 failed, >0 other mux status
 */
int ak_mux_add_video(void *mux_handle, const struct video_stream *video);

/**
 * ak_mux_fix_file - release mux lib resource
 * @record_fp[IN]: save file pointer
 * return: 0 success, -1 failed
 * notes: we just support fixed avi file.
 */
int ak_mux_fix_file(FILE *record_fp);

/**
 * ak_mux_free - release mux lib resource
 * @mux_handle[IN]: opened mux handle
 * return: 0 success, -1 failed
 * notes: we just free the unused handle resources. Don't execute flush and
 *		close operations.
 */
int ak_mux_free(void *mux_handle);

/**
 * ak_mux_close - close mux lib
 * @mux_handle[IN]: opened mux handle
 * return: 0 success, -1 failed
 */
int ak_mux_close(void *mux_handle);

#endif
