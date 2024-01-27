#ifndef _AK_DRV_IR_H_
#define _AK_DRV_IR_H_

enum ak_ain_check_mode {
	AIN_MODE_DAY_MIN = 0x00,        //just use day min threshold to check
	AIN_MODE_DAY_MAX,               //just use day max threshold to check
	AIN_MODE_NIGTH_MIN,             //just use night min threshold to check
	AIN_MODE_NIGTH_MAX,             //just use night max threshold to check
	AIN_MODE_DAY_NIGTH,             //use dya and night mixed threshold to check
};

struct ak_ir_threshold {
	int min;        //min threshold
	int max;        //max threshold
	int feature;    //feature threshold
};

/**
 * ak_drv_ir_get_version - get drv ir version
 * return: version string
 */
const char* ak_drv_ir_get_version(void);

/**
 * ak_drv_ir_init - init ircut ,get ircut control mode
 * return: 0 - success; otherwise -1;
 */
int ak_drv_ir_init(void);

/**
 * ak_drv_ir_get_input_level - get value of light dependent resistors
 * return: 1 - day; 0-night; otherwise -1;
 */
int ak_drv_ir_get_input_level(void);

/**
 * ak_drv_ir_set_check_mode - set ain threshold check mode
 * @check_mode[IN]: check mode in 'enum ak_ain_check_mode'
 * return: 0 success, -1 failed
 */
int ak_drv_ir_set_check_mode(enum ak_ain_check_mode check_mode);

/**
 * ak_drv_ir_get_threshold - get ain threshold, including day and night.
 * @day_thr[OUT]: ain day mode threshold value
 * @night_thr[OUT]: ain nigth mode threshold value
 * return: 0 success, -1 failed
 */
int ak_drv_ir_get_threshold(struct ak_ir_threshold *day_thr,
                            struct ak_ir_threshold *night_thr);

/**
 * ak_drv_ir_set_threshold - set ain threshold, including day and night.
 * @day_thr[IN]: ain day mode threshold value
 * @night_thr[IN]: ain nigth mode threshold value
 * return: 0 success, -1 failed
 */
int ak_drv_ir_set_threshold(const struct ak_ir_threshold *day_thr,
                            const struct ak_ir_threshold *night_thr);

/**
 * ak_drv_ir_set_ircut - set ircut to switch
 * @status_level[IN]:  status level to control day or night, [0,1]
 * return: 0 success, -1 failed
 */
int ak_drv_ir_set_ircut(int status_level);

#endif
