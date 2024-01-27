#ifndef _AK_AUDIO_ENCODE_H_
#define _AK_AUDIO_ENCODE_H_

#include "list.h"
#include "ak_global.h"

enum aenc_aac_attr{
	AENC_AAC_RESERVED = 0,
    AENC_AAC_SAVE_FRAME_HEAD,
    AENC_AAC_CUT_FRAME_HEAD
};

/* audio encode attr */
struct aenc_attr {
    enum aenc_aac_attr aac_head;	//AAC head attr
};

/* audio encode entry */
struct aenc_entry {
    struct audio_stream stream;		//real data stream
    struct list_head list;
};

/** 
 * ak_aenc_print_codec_info - print audio codec version & support functions
 * notes: encode such as: MP3 encode, AAC encode and so on
 *		decode such as: MP3 decode, AAC decode and so on
 */
void ak_aenc_print_codec_info(void);

/**
 * ak_aenc_get_version - get audio encode version
 * return: version string
 * notes: 
 */
const char* ak_aenc_get_version(void);

/**
 * ak_aenc_open - open anyka audio encode
 * @param[IN]: audio pcm data encode param
 * return: audio encode handle, NULL failed
 * notes: 
 */
void* ak_aenc_open(const struct audio_param *param);

/**
 * ak_aenc_get_params - get aenc params
 * @handle[IN]: aenc opened handle
 * @param[OUT]: aenc params by aenc_open()
 * return: 0 success, -1 failed
 * notes: call after aenc_open()
 */
int ak_aenc_get_params(void *handle, struct audio_param *param);

/**
 * ak_aenc_set_attr - set aenc attribution after open
 * @enc_handle[IN]: encode handle
 * @attr[IN]: audio encode attribution
 * return: 0 success, -1 failed
 */
int ak_aenc_set_attr(void *enc_handle, const struct aenc_attr *attr);

/**
 * ak_aenc_send_frame - send pcm data(frame) to encode 
 * @enc_handle[IN]: encode handle
 * @frame[IN]: the audio pcm raw data info
 * @stream[OUT]: encode out stream info
 * return: >=0 encoded size, -1 failed 
 * notes: 
 */
int ak_aenc_send_frame(void *enc_handle, 
					const struct frame *frame, 
					struct audio_stream *stream);

/**
 * ak_aenc_set_frame_default_interval - set audio frame default interval, unit: ms
 * @enc_handle[IN]: encode handle
 * @frame_interval[IN]: audio frame interval, [10, 125], unit: ms
 * return: 0 success, -1 failed
 * note: just for g711/pcm encode, 
 */
int ak_aenc_set_frame_default_interval(void *enc_handle, int frame_interval);

/**
 * ak_aenc_close - close anyka audio encode
 * @handle[in]: opened encode handle
 * return: 0 success, -1 failed 
 * notes: 
 */
int ak_aenc_close(void *enc_handle);

/**
 * ak_aenc_request_stream - request audio data stream according to handle
 * @ai_handle[IN]: audio input handle
 * @enc_handle[IN]: audio encode handle
 * return: stream handle, NULL failed
 * notes: after use "ak_aenc_request_stream", if not set "ak_aenc_cancel_stream",
 		 use "ak_aenc_send_frame" will failed  
 */
void* ak_aenc_request_stream(void *ai_handle, void *enc_handle);

/**
 * ak_aenc_get_stream - get audio encoded data, stream
 * @stream_handle[IN]: opened stream handle
 * @stream_head[IN]: stream list head, we'll add audio stream to tail.
 * return: 0 success, -1 failed
 * notes: IMPORTANT, you must call INIT_LIST_HEAD to init stream head firstly.
 */
int ak_aenc_get_stream(void *stream_handle, struct list_head *stream_head);

/**
 * ak_aenc_release_stream -  release audio data stream
 * @entry[IN]: audio stream entry from ak_aenc_get_stream
 * return: 0 success, -1 failed
 * notes: 
 */
int ak_aenc_release_stream(struct aenc_entry *entry);

/**
 * ak_aenc_cancel_stream -  cancel audio data stream according to stream handle
 * @stream_handle[IN]: opened stream handle
 * return: 0 success, -1 failed 
 * notes: 
 */
int ak_aenc_cancel_stream(void *stream_handle);

#endif
