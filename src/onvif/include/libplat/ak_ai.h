#ifndef _AK_AI_H_
#define _AK_AI_H_

#include "ak_global.h"

#define AUDIO_DEFAULT_INTERVAL		100	//100ms one frame
#define AMR_FRAME_INTERVAL		    20	//AMR frame interval 20ms

enum ai_source {
	AI_SOURCE_AUTO,		//pcm driver decide adc source automatically,
						//and it is default configuration in pcm driver
	AI_SOURCE_MIC,		//set adc mic source manually
	AI_SOURCE_LINEIN	//set adc linein source manually
};

/* audio status: nr&agc resample aec nr_max*/
struct ai_runtime_status {
	int actual_rate;
	int nr_agc_enable;
	int nr_max_enable;
	int resample_enable;
	int aec_enable;
};

struct ak_audio_aec_attr 
{
    unsigned long   audio_out_threshold; // max amplitude of far signal, 1~32767(Q15), 0: use default. see AK32Q15
    signed short   	audio_out_digi_gain; // digtal gain for far signal, Q10, 0: use default. see AK16Q10
    signed short   	audio_in_digi_gain; // digtal gain for near signal, Q10, 0: use default. see AK16Q10
    int crc;
    unsigned short  tail;             // tail size in samples
    int enable;// v300 not use
};

struct ak_audio_agc_attr 
{
    unsigned long   agc_level;  // 1~32767(Q15), 0: use default. see AK32Q15
    signed short    agc_max_gain;   // agc's max_gain, Q0
    signed short    agc_min_gain;          // agc's min_gain, use AK16Q10
    signed short    near_sensitivity;// sensitivity of near-end speech [1:100], 0: use default
    int crc;
    int enable;// v300 not use
};

struct ak_audio_nr_attr 
{
    signed short  noise_suppress_db;  // attenuation of noise in dB (negative number), 0: use default
    int crc;
    int enable;// v300 not use
};

struct ak_audio_aslc_attr 
{
    signed long limit;   // max amplitude of samples, 1~32767(Q15), 0: use default. use AK32Q15
    signed long  aslc_db;  //automatic sound volume control, aslc_db is for the voluume, unit db,  -200~70db
    int crc;
};


/** 
 * ak_ai_print_filter_info - print audio filter version & support functions
 * notes: filter such as: EQ, 3D, RESAMPLE, AGC and so on
 */
void ak_ai_print_filter_info(void);

/**
 * ak_ai_get_version - get audio in version
 * return: version string
 * notes:
 */
const char* ak_ai_get_version(void);

/**
 * ak_ai_open - open audio in device
 * @input[IN]: open audio input param
 * return: opened handle, NULL failed
 * notes: sample_bits set 16 bit
 */
void* ak_ai_open(const struct pcm_param *param);

/**
 * ak_ai_get_params - start ADC capture
 * @handle[IN]: audio in opened handle
 * return: 0 success, -1 failed
 * notes: call after set all kind of ADC attr
 */
int ak_ai_get_params(void *ai_handle, struct pcm_param *param);

/**
 * ak_ai_get_handle - get ai handle
 * @dev_id[IN]: audio in device id
 * @ai_handle[OUT]: audio in opened handle
 * return: 0 success, -1 failed
 * notes: 
 */
int ak_ai_get_handle(int dev_id, void **ai_handle);

/**
 * ak_ai_start_capture - start ADC capture
 * @handle[IN]: audio in opened handle
 * return: 0 success, -1 failed
 * notes: call after set all kind of ADC attr
 */
int ak_ai_start_capture(void *handle);

/**
 * ak_ai_stop_capture - stop ADC capture
 * @handle[IN]: audio in opened handle
 * return: 0 success, -1 failed
 * notes:
 */
int ak_ai_stop_capture(void *handle);

/**
 * ak_ai_get_frame - get audio frame
 * @handle[IN]: audio in opened handle
 * @frame[OUT]: audio frame info
 * @ms[IN]: <0 block mode; =0 non-block mode; >0 wait time
 * return: 0 success, -1 failed
 * notes:
 */
int ak_ai_get_frame(void *handle, struct frame *frame, long ms);

/**
 * ak_ai_release_frame - release one frame data
 * @handle[IN]: audio in opened handle
 * @frame[IN]: audio frame info
 * return: 0 success, -1 failed
 * notes: use "ak_ai_get_frame" get frame data,after used,release frame
 */
int ak_ai_release_frame(void *handle, struct frame *frame);

/**
 * ak_ai_set_capture_size - set audio capture size
 *		that read from AD driver each time
 * @handle[IN]: audio in opened handle
 * @capture_size[IN]: appointed frame interval, [512, 2048], unit: byte
 * return: 0 success, -1 failed
 * notes: 1. if you do not call, the default frame size is 2048
 *      2. set capture size before start capture.
 */
int ak_ai_set_capture_size(void *handle, unsigned int capture_size);

/**
 * ak_ai_set_frame_interval - set audio frame interval, unit: ms
 * @handle[IN]: audio in opened handle
 * @frame_interval[IN]: audio frame interval, [10, 125], unit: ms
 * return: 0 success, -1 failed
 * notes: You must set frame interval before call ak_ai_get_frame()
 */
int ak_ai_set_frame_interval(void *handle, int frame_interval);

/**
 * ak_ai_clear_frame_buffer - frame buffer clean
 * @handle[IN]: audio in opened handle
 * return: 0 success, -1 failed
 * notes: clean buffer after "ak_ai_open"
 */
int ak_ai_clear_frame_buffer(void *handle);

/**
 * @macro
 *  ak_ai_set_volume - set volume
 *
 */
#define ak_ai_set_volume(__handle, __vol) \
	((AK_SUCCESS == ak_ai_set_adc_volume (__handle, (__vol) >= 8 ? 8 : ((__vol) % 8)) \
			&& AK_SUCCESS == ak_ai_set_aslc_volume (__handle, (__vol) >= 8 ? (__vol) - 8 : 0)) ?\
					AK_SUCCESS : AK_FAILED)

/**
 * ak_ai_set_adc_volume - set volume
 * @handle[IN]: audio in opened handle
 * @volume[IN]: new volume value, [0, 8]: 0-mute, 8-max adc volume
 * return: 0 success, -1 failed
 * notes:
 */
int ak_ai_set_adc_volume(void *handle, int volume);

/**
 * ak_ai_set_aslc_volume - set volume
 * @handle[IN]: audio in opened handle
 * @volume[IN]: new volume value, [0, 6]: 0-not use aslc, 6-max aslc volume
 * return: 0 success, -1 failed
 * notes:
 */
int ak_ai_set_aslc_volume(void *handle, int volume);

/**
 * @macro
 *  ak_ai_get_volume - get volume
 *
 */
#define ak_ai_get_volume(__handle) \
	(ak_ai_get_adc_volume(__handle) + ak_ai_get_aslc_volume(__handle))

/**
 * ak_ai_get_adc_volume - get adc volume
 * @handle[IN]: audio in opened handle
 * return: success return volume value, [0, 8]: 0-mute, 8-max adc volume,
 			-1 failed
 * notes:
 */
int ak_ai_get_adc_volume(void *handle);

/**
 * ak_ai_get_aslc_volume - get aslc volume
 * @handle[IN]: audio in opened handle
 * return: success return volume value, [0, 6]: 0-not use aslc, 6-max aslc volume,
 			-1 failed
 * notes:
 */
int ak_ai_get_aslc_volume(void *handle);

/**  
 * ak_ai_set_resample - set audio resampling
 * @handle[IN]: audio in opened handle
 * @enable[IN]: 0 disable, 1 enable
 * return: 0 success, -1 failed
 */
int ak_ai_set_resample(void *handle, int enable);

/**
 * ak_ai_set_nr_agc - adc nr&agc switch
 * @handle[IN]: opened audio input handle
 * @enable[IN]: 0 disable nr&agc, 1 enable nr&agc.
 * return: 0 success, -1 failed
 * notes: nr&agc only support 8k sample rate
 */
int ak_ai_set_nr_agc(void *handle, int enable);

/**
 * ak_ai_set_nr - adc nr switch
 * @handle[IN]: opened audio input handle
 * @enable[IN]: 0 disable nr, 1 enable nr.
 * return: 0 success, -1 failed
 * notes: This function is use to enable nr without agc,
 		  if want to enable nr and agc,use ak_ai_set_nr_agc with enable;
 		  if want to disable nr and agc,use ak_ai_set_nr_agc with disable.
 */
int ak_ai_set_nr(void *handle, int enable);

/**
 * ak_ai_set_nr_max - enable max nr level
 * @handle[IN]: audio in opened handle
 * @enable[IN]: 0 disable nr max, 1 enable nr max.
 * return: 0 success, -1 failed
 * notes: call after set ak_ai_set_nr_agc
 */
int ak_ai_set_nr_max(void *handle, int enable);

/**
 * ak_ai_set_aec - AEC switch
 * @handle[IN]: opened audio input handle
 * @enable[IN]: 0 disable AEC, 1 enable AEC
 * return: 0 success, -1 failed
 * notes: when AEC enable, must set ai and ao
 */
int ak_ai_set_aec(void *handle, int enable);

/**
 * ak_ai_set_agc_attr -  set attribute
 * @handle[IN]: opened audio input handle
 * @agc_attr[IN]: agc attribute
 * return: 0 success, other failed
 * notes:
 */
int ak_ai_set_agc_attr(void *handle, struct ak_audio_agc_attr *agc_attr);							

/**
 * ak_ai_get_agc_attr -  get attribute
 * @handle[IN]: audio in opened handle
 * @agc_attr[OUT]: agc attribute
 * return: 0 success, other failed
 * notes:
 */
int ak_ai_get_agc_attr(void *handle, struct ak_audio_agc_attr *agc_attr);

/**
 * ak_ai_set_aec_attr -  set attribute
 * @handle[IN]: opened audio input handle
 * @aec_attr[IN]: aec attribute
 * return: 0 success, other failed
 * notes:
 */
int ak_ai_set_aec_attr(void *handle, struct ak_audio_aec_attr *aec_attr);

/**
 * ak_ai_get_aec_attr -  get attribute
 * @handle[IN]: audio in opened handle
 * @aec_attr[OUT]: aec attribute 
 * @aec_enable[OUT]: 0 disable AEC, 1 enable AEC
 * return: 0 success, other failed
 * notes:
 */
int ak_ai_get_aec_attr(void *handle, struct ak_audio_aec_attr *aec_attr);

/**
 * ak_ai_set_nr_attr -  set nr attribute
 * @handle[IN]: opened audio input handle
 * @nr_attr[IN]: nr attribute
 * return: 0 success, other failed
 * notes:
 */
int ak_ai_set_nr_attr(void *handle, struct ak_audio_nr_attr *nr_attr);

/**
 * ak_ai_get_nr_attr -  get nr attribute
 * @handle[IN]: audio in opened handle
 * @nr_attr[OUT]: nr attribute
 * return: 0 success, other failed
 * notes:
 */
int ak_ai_get_nr_attr(void *handle, struct ak_audio_nr_attr *nr_attr);

/**
 * ak_ai_enable_agc - adc agc switch
 * @handle[IN]: opened audio input handle
 * @agc_enable[IN]: 0 disable agc, 1 enable agc.
 * return: 0 success, -1 failed
 * notes: if want to set agc,must set nr first
 */
int ak_ai_enable_agc(void *handle, int agc_enable);

/**
 * ak_ai_enable_eq - enable eq
 * @handle[IN]: audio out opened handle 
 * @enable[IN]: 1 enable,0 disable
 * return: 0 success -1 failed
 * notes:
 */
int ak_ai_enable_eq(void *handle, int enable);

/**
 * ak_ai_set_eq_attr - set eq attribute
 * @handle[IN]: audio out opened handle 
 * @eq_attr[IN]: eq attribute
 * return: 0 success  -1 failed
 * notes:
 */
int ak_ai_set_eq_attr(void *handle, struct ak_audio_eq_attr *eq_attr);

/**
 * ak_ai_get_eq_attr - get eq attribute
 * @handle[IN]: audio in opened handle
 * @eq_attr[OUT]: eq attribute
 * @eq_enable[OUT]: 1 enable,0 disable
 * return: 0 success  -1 failed
 * notes:
 */
int ak_ai_get_eq_attr(void *handle, struct ak_audio_eq_attr *eq_attr);

/**
 * ak_ai_save_aec_dump_file - whether save aec dump file
 * @handle[IN]: audio in opened handle
 * @enable[IN]: 0 disable , 1 enable.
 * return: 0 success, -1 failed
 * notes: call after set all kind of ADC attr
 */
int ak_ai_save_aec_dump_file(void *handle, int enable);

/**
 * ak_ai_set_source - set audio input source, linein or mic
 * @handle[IN]: opened audio input handle
 * @src[IN]: appointed source, default AI_SOURCE_AUTO
 * return: 0 success, -1 failed
 * notes:
 */
int ak_ai_set_source(void *handle, enum ai_source src);

/**
 * ak_ai_get_source - get audio input source, linein or mic
 * @handle[IN]: opened audio input handle
 * return: success return appointed source, -1 failed
 */
enum ai_source ak_ai_get_source(void *handle);

/**
 * ak_ai_get_runtime_status - get ai run time status
 * @handle[IN]: audio in opened handle
 * @status[OUT]: get ai other status :nr&agc resample aec volume source
 * return: 0 success, -1 failed
 * notes: call after set all kind of ADC attr
 */
int ak_ai_get_runtime_status(void *handle, 
						struct ai_runtime_status *status);

/**
 * ak_ai_close - close audio input
 * @handle[IN]: opened audio input handle
 * return: 0 success, -1 failed
 * notes:
 */
int ak_ai_close(void *handle);

#endif
