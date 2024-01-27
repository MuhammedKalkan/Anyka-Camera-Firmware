#ifndef _AK_AED_H_
#define _AK_AED_H_

typedef void (*ak_aed_cb)(void *data, int len);

struct ak_aed_param {
	int        threshold;  //alarm threshold
	long 	   interval;   //detection time interval
	ak_aed_cb  aed_cb;     //aed callback
};

/**
 * ak_aed_set_interval - set detection interval time
 * @interval[IN]: detection interval time in micro secord
 * return: 0 success, -1 failed
 */
int ak_aed_set_interval(long interval);

/**
 * ak_aed_get_result - get aed result
 * trigger_ts[OUT]: store trigger time, ms
 * return: 1 -> event trigger; otherwise -> no event
 */
int ak_aed_get_result(unsigned long long *trigger_ts);

/** 
 * ak_aed_set_param - set aed param
 * @param[IN]: aed param, more detail definition of 
 * 				struct ak_aed_param 
 * return: 0 - success; otherwise -1; 
 */
int ak_aed_set_param(struct ak_aed_param *param);

/** 
 * ak_aed_enable - start or stop aed. 
 * @enable[IN]:  [0,1],  0 -> stop aed; 1 -> start aed 
 * return: 0 - success; otherwise -1; 
 */
int ak_aed_enable(int enable);

/*
 * ak_aed_init - audio event detection init
 * @ai_handle[IN]: opened ai handle
 * return: AK_SUCCESS on success, AK_FAILED on failed
 */
int ak_aed_init(void *ai_handle);

/*
 * ak_aed_exit - exit audio event detection
 * return: AK_SUCCESS on success, AK_FAILED on failed
 */
int ak_aed_exit(void);

/*
 * ak_aed_get_version - get version
 * return: pointer to version string
 */
const char *ak_aed_get_version(void);

#endif
