#ifndef _AK_AO_H_
#define _AK_AO_H_

#include "ak_global.h"

#define ak_ao_set_volume_by_aslc(__ao_handle, __volume) ak_ao_set_aslc_volume(__ao_handle, __volume)

enum ao_play_status {
	AO_PLAY_STATUS_RESERVED = 0x00,
	AO_PLAY_STATUS_READY,               //ready to write to da
	AO_PLAY_STATUS_DATA_NOT_ENOUGH,     //data is not enough that ao write to da
	AO_PLAY_STATUS_PLAYING,             //data is enough to play
	AO_PLAY_STATUS_FINISHED             //play finished
};

/* audio status: actual_rate resample */
struct ao_runtime_status {
	int actual_rate;
	int resample_enable;
};

/**
 * ak_ao_get_version - get audio out version
 * return: version string
 * notes:
 */
const char* ak_ao_get_version(void);

/**
 * ak_ao_open - open audio out device, DA
 * @param[IN]: open DA param
 * return: opened ao_handle, otherwize NULL
 * notes: sample_bits set 16 bit
 */
void* ak_ao_open(const struct pcm_param *param);

/**
 * ak_ao_get_params - get ao parameter
 * @handle[IN]: audio out opened handle
 * @param[OUT]: store ao params delivery by ao_open();
 * return: 0 success, -1 failed
 * notes: call after ao_open
 */
int ak_ao_get_params(void *ao_handle, struct pcm_param *param);

/**
 * ak_ao_get_handle - get ao handle
 * @dev_id[IN]: audio out device id
 * @ao_handle[OUT]: audio out opened handle
 * return: 0 success, -1 failed
 * notes: 
 */
int ak_ao_get_handle(int dev_id, void **ao_handle);

/**
 * ak_ao_send_frame - send frame to DA device
 * @ao_handle[IN]: audio out opened handle
 * @data[IN]: audio pcm data
 * @len[IN]: audio pcm data len
 * @ms[IN]: <0 block mode; =0 non-block mode; >0 wait time
 * return: real sent data len, -1 failed
 * notes:
 */
int ak_ao_send_frame(void *ao_handle, unsigned char *data, int len, long ms);

/**
 * ak_ao_notice_frame_end - notice send frame end
 * @ao_handle[IN]: audio out opened handle
 * return: 0 success; -1 failed
 * note: call this function after send frame OK.
 * notes:
 */
int ak_ao_notice_frame_end(void *ao_handle);

/**
 * ak_ao_clear_frame_buffer - frame buffer clean
 * @ao_handle[IN]: audio out opened handle
 * return: 0 success, -1 failed
 * notes: clean buffer after "ak_ao_open"
 */
int ak_ao_clear_frame_buffer(void *ao_handle);

/**
 * ak_ao_get_play_status - get audio play status
 * @ao_handle[in]: opened audio output handle
 * return: current audio play status
 * notes:
 */
enum ao_play_status ak_ao_get_play_status(void *ao_handle);

/**
 * ak_ao_set_play_status - set audio play status
 * @ao_handle[in]: opened audio output handle
 * @status[in]: new status
 * return: 0 success; -1 failed
 * notes:
 */
int ak_ao_set_play_status(void *ao_handle, enum ao_play_status status);

/**
 * @macro
 * ak_ao_set_volume - set ao volume
 * @param __handle[IN] audio out opened handle
 * @param __vol[IN] new volume value, [0, 12]: 0-mute, 12-max volume
 *
 */
#define ak_ao_set_volume(__handle, __vol) \
	((AK_SUCCESS == ak_ao_set_dac_volume (__handle, (__vol) >= 6 ? 6 : ((__vol) % 6)) \
				&& AK_SUCCESS == ak_ao_set_aslc_volume (__handle, (__vol) >= 6 ? (__vol) - 6 : 0)) ?\
						AK_SUCCESS : AK_FAILED)


/**
 * ak_ao_set_dac_volume - set ao dac volume
 * @ao_handle[IN]: audio out opened handle
 * @volume[IN]: new volume value, [0, 6]: 0-mute, 6-dac max volume
 * return: 0 success, -1 failed
 * notes:
 */
int ak_ao_set_dac_volume(void *ao_handle, int volume);

/**
 * ak_ao_set_volume_by_aslc - set ao volume by aslc
 * @ao_handle[IN]: audio out opened handle
 * @volume[IN]: aslc volume value, [0, 6]: 0-not use aslc , 6-aslc max volume
 * return: 0 success, -1 failed
 * notes:
 */
int ak_ao_set_aslc_volume(void *ao_handle, int volume);

/**
 * @macro
 * ak_ao_get_volume - get volume
 */
#define ak_ao_get_volume(__handle) \
	(ak_ao_get_dac_volume(__handle) + ak_ao_get_aslc_volume(__handle))

/**
 * ak_ao_get_volume - get volume
 * @handle[IN]: audio out opened handle
 * return: success return dac volume value, [0, 6]: 0-mute, 6-max dac volume,
 			-1 failed
 * notes:
 */
int ak_ao_get_dac_volume(void *handle);

/**
 * ak_ao_get_volume - get volume
 * @handle[IN]: audio out opened handle
 * return: success return volume value, [0, 6]: 0-not use aslc, 6-max aslc volume,
 			-1 failed
 * notes:
 */
int ak_ao_get_aslc_volume(void *handle);

/**
 * ak_ao_set_resample - set audio output resampling
 * @handle[IN]: audio out opened handle
 * @enable[IN]: 0 close, 1 open
 * return: 0 success, -1 failed
 */
int ak_ao_set_resample(void *ao_handle, int enable);

/**
 * ak_ao_enable_eq - enable eq
 * @handle[IN]: audio out opened handle 
 * @enable[IN]: 1 enable,0 disable
 * return: 0 success -1 failed
 * notes:
 */
int ak_ao_enable_eq(void *handle, int enable);

/**
 * ak_ao_set_eq_attr - set eq
 * @handle[IN]: audio out opened handle 
 * @eq_attr[IN]: eq attr
 * return: 0 success  -1 failed
 * notes:
 */
int ak_ao_set_eq_attr(void *handle, struct ak_audio_eq_attr *eq_attr);

/**
 * ak_ao_get_eq_attr - get eq attribute
 * @handle[IN]: audio out opened handle 
 * @eq_attr[OUT]: eq attribute
 * return: 0 success  -1 failed
 * notes:
 */
int ak_ao_get_eq_attr(void *handle, struct ak_audio_eq_attr *eq_attr);

/**
 * ak_ao_set_sample_rate - set new sample rate
 * @ao_handle[IN]: audio out opened handle
 * @sample_rate[IN]: ao sample rate.
 * return: 0 success, -1 failed
 * notes:
 */
int ak_ao_set_dev_buf_size(void *ao_handle, enum ak_audio_dev_buf_size dev_buf_size);

/**
 * ak_ao_get_dev_buf_size - get audio output device size
 * @ao_handle[IN]: audio out opened handle
 * @dev_buf_size[IN]: drvice DMA buffer size, [512, 4096], unit: byte
 * return: 0 success, -1 failed
 * notes: 1. if you do not call, the default frame size is 2048
 *      2. set drvice DMA buffer size before send frame.
 */
int ak_ao_get_dev_buf_size(void *ao_handle, int *dev_buf_size);


/**
 * ak_ao_enable_speaker - enable speaker
 * @ao_handle[IN]: audio out opened handle
 * @enable[IN]: 0 disable,1 enable
 * return: 0 success, -1 failed
 * notes:
 */
int ak_ao_enable_speaker(void *ao_handle, int enable);

/**
 * ak_ao_set_sample_rate - set new sample rate
 * @ao_handle[IN]: opened DA handle
 * @sample_rate[IN]: ao sample rate.
 * return: 0 success, -1 failed
 * notes:
 */
int ak_ao_set_sample_rate(void *ao_handle, int sample_rate);

/**
 * ak_ao_get_runtime_status - get ao run time status
 * @handle[IN]: audio out opened handle
 * @status[OUT]: get ao runtime status :nr&agc resample 
 * return: 0 success, -1 failed
 * notes: call after set all kind of DAC attr
 */
int ak_ao_get_runtime_status(void *handle, 
							struct ao_runtime_status *status);

/**
 * ak_ao_close - close audio output
 * @ao_handle[IN]: opened DA handle
 * return: 0 success, -1 failed
 * notes:
 */
int ak_ao_close(void *ao_handle);

#endif
