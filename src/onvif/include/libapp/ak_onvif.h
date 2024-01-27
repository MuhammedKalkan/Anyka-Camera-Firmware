#ifndef _AK_ONVIF_H_
#define _AK_ONVIF_H_


/**
 * ak_onvif_get_version - get onvif version
 * return: version string
 */
const char* ak_onvif_get_version(void);

/**
 * ak_onvif_init - init onvif
 * @vi[IN]: opened vi handle
 * @ai[IN]: opened ai handle
 * return: void
 */
void ak_onvif_init(void *vi, void *ai);

/**
 * ak_onvif_destroy - destory onvif
 * return: void
 */
void ak_onvif_destroy(void);

/**
 * ak_onvif_set_day_night - set day night mode
 * @vi_handle[IN]: opened vi handle
 * @ir_val[IN]: 0 night, 1 day
 * return: -1 failed , 0 success
 */
int ak_onvif_set_day_night(void *vi_handle, int ir_val);

/**
 * ak_onvif_set_switch_ir - set ircut auto switch enable
 * @enable[IN]: enable or not
 * return: void
 */
void ak_onvif_set_switch_ir(int enable);

#endif
