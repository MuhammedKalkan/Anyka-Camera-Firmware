
/**
 * 日志信息控制单体模块。
 */

#include <akae_typedef.h>

#if !defined(AKSPC_LOG_H_)
#define AKSPC_LOG_H_
AK_C_HEADER_EXTERN_C_BEGIN

/**
 * 日志输出单行最大长度。
 */
#define AK_LOG_1LINE_MAX_LEN (128)
#define AK_LOG_TAG_MAX_LEN   (8)

/**
 * 日志输出等级。
 */
#define AK_LOG_CLASS_ALLON   (0)
#define AK_LOG_CLASS_DEBUG2  (AK_LOG_CLASS_ALLON + 1)
#define AK_LOG_CLASS_DEBUG   (AK_LOG_CLASS_ALLON + 2)
#define AK_LOG_CLASS_INFO    (AK_LOG_CLASS_ALLON + 3)
#define AK_LOG_CLASS_WARN    (AK_LOG_CLASS_ALLON + 4)
#define AK_LOG_CLASS_ERROR   (AK_LOG_CLASS_ALLON + 5)
#define AK_LOG_CLASS_ALERT   (AK_LOG_CLASS_ALLON + 6)
#define AK_LOG_CLASS_ALLOFF  (AK_LOG_CLASS_ALLON + 7)

/**
 * 日志输出。
 * 通过该接口，可以指定日志输出对应颜色标签。
 */
AK_API AK_void akae_log_verbose(AK_int clss, AK_chrcptr tag, AK_chrcptr fmt, ...);


#define akae_log_tag_debug2(__tag, __fmt...)   akae_log_verbose(AK_LOG_CLASS_DEBUG2, (__tag), ##__fmt)
#define akae_log_tag_debug(__tag, __fmt...)    akae_log_verbose(AK_LOG_CLASS_DEBUG,  (__tag), ##__fmt)
#define akae_log_tag_info(__tag, __fmt...)     akae_log_verbose(AK_LOG_CLASS_INFO,   (__tag), ##__fmt)
#define akae_log_tag_warning(__tag, __fmt...)  akae_log_verbose(AK_LOG_CLASS_WARN,   (__tag), ##__fmt)
#define akae_log_tag_error(__tag, __fmt...)    akae_log_verbose(AK_LOG_CLASS_ERROR,  (__tag), ##__fmt)
#define akae_log_tag_alert(__tag, __fmt...)    akae_log_verbose(AK_LOG_CLASS_ALERT,  (__tag), ##__fmt)

#define akae_log_debug2(__fmt...)   akae_log_verbose(AK_LOG_CLASS_DEBUG2, AK_null, ##__fmt)
#define akae_log_debug(__fmt...)    akae_log_verbose(AK_LOG_CLASS_DEBUG,  AK_null, ##__fmt)
#define akae_log_info(__fmt...)     akae_log_verbose(AK_LOG_CLASS_INFO,   AK_null, ##__fmt)
#define akae_log_warning(__fmt...)  akae_log_verbose(AK_LOG_CLASS_WARN,   AK_null, ##__fmt)
#define akae_log_error(__fmt...)    akae_log_verbose(AK_LOG_CLASS_ERROR,  AK_null, ##__fmt)
#define akae_log_alert(__fmt...)    akae_log_verbose(AK_LOG_CLASS_ALERT,  AK_null, ##__fmt)


/**
 * 代码运行时检查点。
 * 在代码中执行此命令会输出该代码位置的文件名称和行好信息，
 * 主要用于在调试中定位代码运行位置。
 */
#define AK_CHECK_POINT() \
	do {\
		AK_chrptr file = akae_basename(__FILE__);\
		akae_log_debug("\r\n--  @  %s:%d \r\n", file, __LINE__);\
	} while(0)

/**
 * 代码运行时断言。
 * 判断表达式逻辑不为真时，会终止程序运行，
 * 通常用于单元测试时对代码行为逻辑作严谨的判断，代码发布时关闭断言。
 */
#define AK_ASSERT(__exp) \
	do{\
		if(!(__exp)){\
			akae_log_alert("\"%s()\" Assertion Condition ( \"%s\" ) @ %d.", __PRETTY_FUNCTION__, #__exp, __LINE__);\
			exit(1);\
		}\
	}while(0)

/**
 * 条件为真断言。
 */
#define AK_ASSERT_TRUE(__condition) \
	AK_ASSERT(__condition)

/**
 * 条件为假断言。
 */
#define AK_ASSERT_FALSE(__condition) \
	AK_ASSERT(!(__condition))

/**
 * 条件逻辑测试。
 * 测试条件中的逻辑是否为真，当逻辑不为真时，根据 @ref __verbose 标识输出日志。
 */
#define AK_TEST(__condition, __verbose) \
	((__condition) ? AK_true : \
		((__verbose) ? \
			(akae_log_warning("\"%s:%d\" Condition ( \"%s\" ) Failed.", akae_basename (__FILE__), __LINE__, #__condition), AK_false) : AK_false))

/**
 * 条件预设期望。
 * 测试条件中的逻辑非真时会输出提示日志。
 */
#define AK_EXPECT(__condition) \
	AK_TEST(__condition, AK_true)

/**
 * 条件预设期望。
 * 在方法中通过判断一个预设条件，针对无返回值的方法当条件不为真时跳出方法，跳出过程中没有日志输出提示。
 */
#define AK_EXPECT_RETURN(__condition) \
	do{\
		if (!AK_TEST(__condition, AK_false)){\
			return;\
		}\
	}while(0)

/**
 * 条件预设期望。
 * 见 @ref AK_EXPECT_RETURN()，针对带返回值的方法。
 */
#define AK_EXPECT_RETURN_VAL(__condition, __ret) \
	do{\
		if (!AK_TEST(__condition, AK_false)){\
			return(__ret);\
		}\
	}while(0)

/**
 * 条件预设期望。
 * 当条见不满足时，会根据跳转到 @ref __location 代码位置。
 */
#define AK_EXPECT_JUMP(__condition, __location) \
	do{\
		if (!AK_TEST(__condition, AK_false)){\
			goto __location;\
		}\
	}while(0)

/**
 * 条件预设期望。
 * 当条见不满足时，会跳出当前循环。
 */
#define AK_EXPECT_BREAK(__condition) \
	if (!AK_TEST(__condition, AK_false)){\
		break;\
	}\

/**
 * 条件预设期望。
 * 当条见不满足时，会跳过当前循环。
 */
#define AK_EXPECT_CONTINUE(__condition) \
	if (!AK_TEST(__condition, AK_false)){\
		continue;\
	}\


/**
 * 条件预设期望。
 * @ref AK_EXPECT_RETURN()，跳出过程中带有日志输出提示。
 */
#define AK_EXPECT_VERBOSE_RETURN(__condition) \
	do{\
		if (!AK_TEST(__condition, AK_true)){\
			return;\
		}\
	}while(0)

/**
 * 条件预设期望。
 * @ref AK_EXPECT_RETURN()，跳出过程中带有日志输出提示，针对带返回值的方法。
 */
#define AK_EXPECT_VERBOSE_RETURN_VAL(__condition, __ret) \
	do{\
		if (!AK_TEST(__condition, AK_true)){\
			return(__ret);\
		}\
	}while(0)

#define AK_EXPECT_VERBOSE_JUMP(__condition, __location) \
	do{\
		if (!AK_TEST(__condition, AK_true)){\
			goto __location;\
		}\
	}while(0)


#define AK_EXPECT_VERBOSE_BREAK(__condition) \
	if (!AK_TEST(__condition, AK_true)){\
		break;\
	}\


#define AK_EXPECT_VERBOSE_CONTINUE(__condition) \
	if (!AK_TEST(__condition, AK_true)){\
		continue;\
	}\



AK_C_HEADER_EXTERN_C_END
#endif ///< LOG_H_
