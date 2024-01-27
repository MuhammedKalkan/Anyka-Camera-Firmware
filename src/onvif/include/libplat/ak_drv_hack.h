#ifndef _AK_DRV_PTZ_H_
#define _AK_DRV_PTZ_H_


/**
 * ak_drv_ptz_get_version - get drv ptz version
 * return: version string
 */
const char* ak_drv_hack_get_version(void);

/**
 * ak_drv_ptz_open - init ptz, open motor device
 * return: 0 success; -1 failed
 */
int ak_drv_hack_open(void);

/**
 * ak_drv_ptz_close - close motor, release motor resource
 * return: 0 success, -1 failed
 */
int ak_drv_hack_close(void);


int ak_drv_hack_test(void);

#endif
