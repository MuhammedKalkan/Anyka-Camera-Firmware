#ifndef _AK_AUDIO_DECODE_H_
#define _AK_AUDIO_DECODE_H_

#include "ak_global.h"

/**
 * ak_adec_get_version - get audio decode version
 * return: version string
 * notes:
 */
const char* ak_adec_get_version(void);

/**
 * ak_adec_open - open anyka audio decode
 * @param[in]: audio stream decode param
 * return: opened decode handle
 * notes:
 */
void* ak_adec_open(const struct audio_param *param);

/**
 * ak_adec_get_params - get adec parameter
 * @handle[IN]: adec opened handle
 * @param[OUT]: store adec params delivery by adec_open();
 * return: 0 success, -1 failed
 * notes: call after set all kind of DAC attr
 */
int ak_adec_get_params(void *handle, struct audio_param *param);

/**
 * ak_adec_get_free_size - get audio decode buffer free size
 * @dec_handle[in]: opened decode handle
 * return: -1 failed; otherwize decode buffer free size
 * notes:
 */
int ak_adec_get_free_size(void *dec_handle);

/**
 * ak_adec_send_stream - decode audio stream
 * @dec_handle[in]: opened decode handle
 * @data[in]: data add to decode buffer
 * @len[in]: add to decode buffer data len
 * @ms[in]: <0 block mode; =0 non-block mode; >0 wait time
 * return: >=0 len added to decode buffer; -1 failed
 * notes:
 */
int ak_adec_send_stream(void *dec_handle,
						const unsigned char *data,
						unsigned int len,
						long ms);

/**
 * ak_adec_get_frame - after decode, get frame data
 * @dec_handle[in]: opened decode handle
 * @frame[OUT]: audio decode frame out buffer
 * return: >0 decode frame len; -1 failed
 * notes:
 */
int ak_adec_get_frame(void *dec_handle, unsigned char *frame);

/**
 * ak_adec_set_frame_size - set frame size that send to ao after decode
 * @dec_handle[IN]: opened audio encode handle
 * @frame_size[IN]: appointed frame size, [256, 4096], unit: byte
 * return: 0 success, -1 failed
 * notes: 1. if you do not call, the default frame size is 2048
 * 		2. only work in stream mode
 */
int ak_adec_set_frame_size(void *dec_handle, unsigned int frame_size);

/**
 * ak_adec_notice_stream_end - notice send stream end
 * @dec_handle[in]: opened decode handle
 * return: 0 success; -1 failed
 * notes:
 */
int ak_adec_notice_stream_end(void *dec_handle);

/**
 * ak_adec_close - close anyka audio decode
 * @handle[in]: opened decode handle
 * return: 0 success; -1 failed
 * notes:
 */
int ak_adec_close(void *dec_handle);

/**
 * ak_adec_request_stream - request audio data stream according to handle
 * @ao_handle[in]: audio output handle
 * @dec_handle[in]: audio decode handle
 * return: stream handle, NULL failed
 * notes:
 */
void* ak_adec_request_stream(void *ao_handle, void *dec_handle);

/**
 * ak_adec_cancel_stream - cancel audio data stream according to stream handle
 * @dec_handle[in]: opened stream handle
 * return: 0 success, -1 failed
 * notes:
 */
int ak_adec_cancel_stream_no_wait(void *stream_handle);



/**
 * ak_adec_cancel_stream - reference to @ref ak_adec_cancel_stream, this interface would wait until stream finished.
 *
 */
int ak_adec_cancel_stream (void *stream_handle);


#endif
