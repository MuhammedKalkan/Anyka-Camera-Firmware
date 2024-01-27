#ifndef _AK_DRV_IRLED_H_
#define _AK_DRV_IRLED_H_

struct ak_drv_irled_hw_param {
	int irled_working_level;
};

/**
 * ak_drv_irled_get_version - get drv irled version
 * return: version string
 */
const char* ak_drv_irled_get_version(void);

/**
 * ak_drv_irled_init - set irled module initialize
 * return: 0 - success; otherwise -1;
 */
int ak_drv_irled_init(struct ak_drv_irled_hw_param *param);

/**
 * ak_drv_irled_get_working_stat - get irled module working status
 * return: 1 - irled is working; 0 - irled is NOT working; -1 - get fail;
 */
int ak_drv_irled_get_working_stat(void);

/**
 * ak_drv_irled_set_working_stat - set irled module working status
 * @working[IN]: set irled working or not. 0 - NOT working; 1 - working;
 * return: 1 - irled is working; 0 - irled is NOT working; -1 - get fail;
 */
int ak_drv_irled_set_working_stat(int working);
#endif
