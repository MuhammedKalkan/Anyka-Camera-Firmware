#ifndef _AK_COMMON_H_
#define _AK_COMMON_H_

#define AK_SUCCESS          0
#define AK_FAILED           -1
#define AK_FALSE			0
#define AK_TRUE          	1

#ifdef AK_RTOS
typedef long time_t;
#endif

#define COLOR_PRINT(COLOR_MODE, COLOR_BACK, COLOR_FRONT, BUFF, LEN_BUFF, ... )  \
{\
	snprintf(BUFF, LEN_BUFF, ##__VA_ARGS__);\
	printf("\033[%d;%d;%dm%s%s", COLOR_MODE, COLOR_BACK, COLOR_FRONT, BUFF, COLOR_END);\
}

#define COLOR_LEN                32
#define COLOR_BEGIN              "\033[%d;%d;%dm"
#define COLOR_END                "\033[0m"

#define COLOR_MODE_NORMAL        0                                                                  //缺省模式
#define COLOR_MODE_BOLD          1                                                                  //高亮
#define COLOR_MODE_UNDERLINED    4                                                                  //下划线
#define COLOR_MODE_BLINK         5                                                                  //闪烁
#define COLOR_MODE_NEGATIVE      7                                                                  //反色

#define COLOR_BACK_BLACK         40                                                                 //背景颜色定义
#define COLOR_BACK_RED           41
#define COLOR_BACK_GREEN         42
#define COLOR_BACK_YELLOW        43
#define COLOR_BACK_BLUE          44
#define COLOR_BACK_PURPLE        45
#define COLOR_BACK_CYAN          46
#define COLOR_BACK_WHITE         47

#define COLOR_FRONT_BLACK        30                                                                 //字体颜色定义
#define COLOR_FRONT_RED          31
#define COLOR_FRONT_GREEN        32
#define COLOR_FRONT_YELLOW       33
#define COLOR_FRONT_BLUE         34
#define COLOR_FRONT_PURPLE       35
#define COLOR_FRONT_CYAN         36
#define COLOR_FRONT_WHITE        37

/**
 * print functions
 * 1. error print, call this when sys-func fail or self-define error occur
 * 2. warning print, warning conditions
 * 3. notice print, normal but significant
 * 4. normal print, normal message or tips
 * 5. info print, use for cycle print like watch-dog feed print
 * 6. debug print, use for debug
 */
enum LOG_LEVEL {
	LOG_LEVEL_RESERVED = 0,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_NOTICE,
	LOG_LEVEL_NORMAL,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
};

struct ak_timeval {
	unsigned long sec;     /* seconds */
	unsigned long usec;    /* microseconds */
};

struct ak_date {
	int year; 		//4 number
	int month;		//0-11
	int day; 		//0-30
	int hour; 		//0-23
	int minute; 	//0-59
	int second; 	//0-59
	int timezone; 	//local time zone 0-23
};

/**
 * ak_print: print function we defined for debugging
 * @level[IN]: print level [0,5]
 * @fmt[IN]: format like printf()
 * @...[IN]: variable arguments list
 * return: we return 0 always.
 */
int ak_print(int level, const char *fmt, ...)__attribute__((format(printf,2,3)));

#define ak_print_error(fmt, arg...) \
	ak_print(LOG_LEVEL_ERROR, "\n" fmt, ##arg)
#define ak_print_warning(fmt, arg...) \
	ak_print(LOG_LEVEL_WARNING, "\n" fmt, ##arg)
#define ak_print_notice(fmt, arg...) \
	ak_print(LOG_LEVEL_NOTICE, fmt, ##arg)
#define ak_print_normal(fmt, arg...) \
	ak_print(LOG_LEVEL_NORMAL, fmt, ##arg)
#define ak_print_info(fmt, arg...) \
	ak_print(LOG_LEVEL_INFO, fmt, ##arg)
#define ak_print_debug(fmt, arg...) \
	ak_print(LOG_LEVEL_DEBUG, fmt, ##arg)

#define ak_print_error_ex(fmt, arg...) \
	ak_print(LOG_LEVEL_ERROR, "\n[%s:%d] " fmt, __func__, __LINE__, ##arg)
#define ak_print_warning_ex(fmt, arg...) \
	ak_print(LOG_LEVEL_WARNING, "\n[%s:%d] " fmt, __func__, __LINE__, ##arg)
#define ak_print_notice_ex(fmt, arg...) \
	ak_print(LOG_LEVEL_NOTICE, "[%s:%d] " fmt, __func__, __LINE__, ##arg)
#define ak_print_normal_ex(fmt, arg...) \
	ak_print(LOG_LEVEL_NORMAL, "[%s:%d] " fmt, __func__, __LINE__, ##arg)
#define ak_print_info_ex(fmt, arg...) \
	ak_print(LOG_LEVEL_INFO, "[%s:%d] " fmt, __func__, __LINE__, ##arg)
#define ak_print_debug_ex(fmt, arg...) \
	ak_print(LOG_LEVEL_DEBUG,  "[%s:%d] " fmt, __func__, __LINE__, ##arg)

/* magic test 宏，用于测试接口传入的参数是否本模块返回的 */
#define TEST_MAGIC(_handle, _magic)  (_handle->magic != _magic)

#define CHECK_MAGIC_RETURN_VAL(_handle, _magic, _val)	\
	do { \
		if (TEST_MAGIC(_handle, _magic)) {	\
			ak_print_error_ex("invalid handle: %p\n", _handle); \
			return _val; \
		}	\
	} while (0)

#define CHECK_MAGIC_RETURN(_handle, _magic)	\
	do { \
		if (TEST_MAGIC(_handle, _magic)) {	\
			ak_print_error_ex("invalid handle: %p\n", _handle);	\
			return;	\
		}	\
	} while (0)
	
#define ak_malloc(size)         malloc(size)
#define ak_calloc(nmemb, size)  calloc(nmemb, size)
#define ak_free(ptr)            free(ptr)

#ifdef AK_RTOS
enum DEV_BLKID {
    DEV_MTDBLOCK=0,
    DEV_MMCBLOCK,
};

/**
 * ak_mount_fs - mount fs
 * @dev_id[IN]: device id
 * @partition_id[IN]: partition id
 * @mount_point[IN]: mount point
 * return: 0 success, -1 failed
 */
int ak_mount_fs(int dev_id, int partition_id, const char *mount_point);

/**
 * ak_umount_fs - umount fs
 * @dev_id[IN]: device id
 * @partition_id[IN]: partition id
 * @mount_point[IN]: mount point
 * return: 0 success, -1 failed
 */
int ak_umount_fs(int dev_id, int partition_id, const char *mount_point);

/**
 * ak_mount_check_status - check fs mount status
 * @void:
 * return: 0 success, -1 failed
 */
int ak_mount_check_status(void);

#else
#include <signal.h>

#define SYS_CMD_RESULT_LEN  4096
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

/**
 * ak_backtrace - backtrace current code context when caught appointed signal
 * @signal_no[IN]: signal no.
 * @si[IN]:
 * @ptr[IN]:
 * @return: none
 * notes: handle signal: SIGINT, SIGTERM, SIGSEGV
 */
void ak_backtrace(unsigned int signal_no, siginfo_t *si, void *ptr);

#endif

/**
 * ak_print_set_level - set current print level
 * @level[IN]: current level
 * return: old level
 */
int ak_print_set_level(int level);

/**
 * ak_common_get_version - get common module version
 * return: version string
 */
const char* ak_common_get_version(void);

/**
 * ak_print_null: NULL function
 * return: void
 * note:use for the lib print callback function
 */
void ak_print_null(const char *fmt, ...);

/* use for register call */
void print_normal(const char *fmt, ...);

/**
 * ak_set_module_print: set module print flag.
 * @module_name[IN]: print level [0,5]
 * @enable[IN]: 0 disable, 1 enable
 * return: 0 success, -1 failed
 * notes: 1. module name max len is 20 bytes;
 *      2. if the module name is a new one, it'll add to list, and the memory
 *      won't free.
 *      3. if you don't set module print flag, the default action is print.
 */
int ak_set_module_print(const char *module_name, unsigned char enable);

/**
 * ak_print_set_syslog_level - set print log level
 * @level[IN]: level to set, value in enum LOG_LEVEL
 * return: always return 0
 */
int ak_print_set_syslog_level(int level);

/**
 * ak_check_file_exist - check appointed path dir/file exist
 * @file[IN]: absolutely path
 * return: 0 on exist, others not
 */
int ak_check_file_exist(const char *path);

/**
 * ak_is_regular_file - Check whether appointed file is a regular file
 * @file_path[IN]: absolutely file path
 * return: 1 regular file, 0 not regular file, -1 failed
 */
int ak_is_regular_file(const char *file_path);

/**
 * ak_is_dev_file - Check whether appointed file is a device file
 * @file_path[IN]: absolutely file path
 * return: 1 regular file, 0 not regular file, -1 failed
 */
int ak_is_dev_file(const char *file_path) ;

/**
 * ak_sleep_ms - sleep certain time
 * @ms[IN]: milli-seconds
 * return: none
 */
void ak_sleep_ms(const int ms);

/**
 * ak_get_ostime - get OS time since cpu boot
 * @tv[OUT]: time value since startup
 * return: void
 */
void ak_get_ostime(struct ak_timeval *tv);

/**
 * ak_get_localdate - get local date time
 * @date[OUT]: get date time
 * return: 0 success, -1 failed
 * notes:
 */
int ak_get_localdate(struct ak_date *date);

/**
 * ak_set_localdate - set local date time
 * @date[IN]: set date time
 * return: 0 success, -1 failed
 */
int ak_set_localdate(const struct ak_date *date);

/**
 * ak_diff_ms_time - diff value of ms time between cur_time and pre_time
 * @cur_time[IN]: current time
 * @pre_time[IN]: previous time
 * return: diff time, uint: ms
 */
long ak_diff_ms_time(const struct ak_timeval *cur_time,
					const struct ak_timeval *pre_time);

/**
 * ak_seconds_to_string - transfer total seconds to readable time string
 * @secs: passed total seconds from 1970-01-01 00:00:00
 * return: readable time string after transferred
 * notes: string format: yyyy-MM-dd HH:mm:ss, ex: 2016-04-06 10:06:06
 *      time_str min len 20 bytes.
 * IMPORTANT: use ONLY ONCE after call this function.
 */
char* ak_seconds_to_string(time_t secs);

/**
 * ak_date_to_seconds - transfer date time value to seconds
 * @seconds[IN]: seconds from 1970-01-01 00:00:00
 * @date[OUT]: date time value
 * return: 0 success; -1 failed
 * notes: seconds from 1970-01-01 00:00:00 +0000(UTC)
 */
long ak_seconds_to_date(long seconds, struct ak_date *date);

/**
 * ak_date_to_seconds - transfer date time value to seconds
 * @date[IN]: date time value
 * return: seconds after transferred; failed -1
 * notes: seconds from 1970-01-01 00:00:00 +0000(UTC)
 */
long ak_date_to_seconds(const struct ak_date *date);

/**
 * ak_date_to_string - transfer date time value to time string
 * @date[IN]: date time value
 * @str[OUT]: date time string after transfer OK
 * return: 0 success; -1 failed
 * notes: 1. string format: yyyyMMdd-HHmmss, ex: 20160406-100606
 *		2. MAKE SURE str has enough space outside
 */
int ak_date_to_string(const struct ak_date *date, char *str);

/**
 * ak_string_to_date - transfer date time string to date time value
 * @time_str[IN]: time string
 * @date[OUT]: date time value after transfer OK
 * return: 0 success; otherwise -1
 * notes: string format: yyyyMMdd-HHmmss, ex: 20160406-100606
 */
int ak_string_to_date(const char *time_str, struct ak_date *date);

/**
 * ak_print_date - print date time value
 * @date[IN]: date time value
 * return: 0 success; -1 failed
 * notes: format: yyyyMMdd-HHmmss, ex: 20160406-100606
 */
int ak_print_date(const struct ak_date *date);

static inline void add_ref(int *ref, int val)
{
    *ref += val;
}

static inline void del_ref(int *ref, int val)
{
    *ref -= val;
}

static inline int dec_ref_and_test(int *ref, int val)
{
	int temp;

    temp = *ref;
    temp -= val;
    *ref = temp;

    return temp;
}

static inline void set_bit(int *flag, unsigned char bit)
{
    *flag |= (1 << bit);
}

static inline int test_bit(int nr, volatile void *addr)
{
    return (1UL & (((const int *) addr)[nr >> 5] >> (nr & 31))) != 0UL;
}

static inline void clear_bit(int *flag, unsigned char bit)
{
    *flag &= ~(1 << bit);
}

#endif
