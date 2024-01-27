#ifndef _AK_ALARM_H_
#define _AK_ALARM_H_

/* system detect type, current implement: video  alarm */
enum  sys_detect_type {    
    SYS_DETECT_MOVE_ALARM = 0,		//move alarm detection
    SYS_DETECT_VOICE_ALARM,			//voice alarm detection
    SYS_DETECT_OTHER_ALARM,			//other alarm detection            
	SYS_DETECT_TYPE_NUM
};

/*callback function which send alarm info to ak*/
typedef void (* AK_ALARM_CB)(int type,int level,int start_time, int time_len);

/*callback function which check alarm info to send or to discard*/
typedef int (* AK_ALARM_FILTER_CB)(void);

/**
 * ak_alarm_get_version - get alarm version
 * return: version string
 */
const char* ak_alarm_get_version(void);

/**
 * ak_alarm_get_time - get md time of  alarm  
 * return:    md time of  alarm 
 */
int ak_alarm_get_time(void);

/**
 * ak_alarm_set_interval_time - set report_interval && detect_interval for ak alarm 
 * @report_interval[IN]: 		second 
 * @detect_interval[IN]: 		msecond
 * return:   successful return 0 ;  fail return -1 
 */
int ak_alarm_set_interval_time(int report_interval, int detect_interval);

/**
 * ak_alarm_set_ratios - set one type of ak alarm sensitivity
 * @type[IN]: alarm type to set 
 * @cur_level[IN]: current level
 * @total_level[IN]: total level of sensitivity
 * @ratios[IN]:  sensitivity value
 * return:  0 success;  -1 failed
 */
int ak_alarm_set_ratios(enum sys_detect_type type, int cur_level,
						int total_level, int *ratios);

/**
 * ak_alarm_init - init one type of ak alarm 
 * @type[IN]: 		alarm type to init 
 * @handle[IN]: 		vi or ai handle
 * @alarm_func[IN]: 	send alarm info to ak
 * @filter_func[IN]: 	check alarm info to send or to discard
 * return:   successful return 0 ;  fail return -1 
 */
int ak_alarm_init(enum sys_detect_type type, void *handle,
				AK_ALARM_CB alarm_func, AK_ALARM_FILTER_CB filter_func);

/**
 * ak_alarm_start - start one type of ak alarm 
 * @type[IN]: 	alarm type to start 
 * @level[IN]: 	alarm sensitivity
 * return:   successful return 0 ;  fail return -1 
 */
int ak_alarm_start(enum sys_detect_type type, int level);

/**
 * ak_alarm_stop - stop one type of ak alarm 
 * @type[IN]: 	alarm type to stop  
 * return:   successful return 0 ;  fail return -1 
 */
int ak_alarm_stop(enum sys_detect_type type);

#endif
