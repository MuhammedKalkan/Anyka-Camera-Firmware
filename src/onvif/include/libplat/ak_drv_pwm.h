#ifndef _AK_DRV_PWM_H_
#define _AK_DRV_PWM_H_

/** 
 * ak_drv_pwm_open: open PWM driver 
 * return: PWM handle , or NULL; 
 */
void* ak_drv_pwm_open(int device_no);

/** 
 * ak_drv_pwm_set: set pwm driver 
 * return: int  ; 
 */
int ak_drv_pwm_set(void *handle, unsigned int clock, unsigned int duty_num, unsigned int duty_den);

/** 
 * ak_drv_pwm_close: close pwm handle . 
 * @handle[IN]:  pwm handle
 * return: 0 - success; otherwise -1; 
 */	
int ak_drv_pwm_close(void *handle);

#endif
