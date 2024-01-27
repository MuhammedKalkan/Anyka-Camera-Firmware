
/**
 * 日志信息控制单体模块。
 */

#include <akae_typedef.h>
#include <akae_log.h>

#if !defined(AKSPC_STDLIB_H_)
#define AKSPC_STDLIB_H_
AK_C_HEADER_EXTERN_C_BEGIN



/**
 * 检测当前运行平台是否为小端模式。
 *
 * @return
 *  如果当前运行平台是小端模式返回 AK_true，\n
 *  大端模式则返回 AK_false。
 */
AK_API AK_boolean akae_little_endian(void);


/**
 * 获取当前程序编译日期时间。\n
 * 内部通过解析编译器 __DATE__ 以及 __TIME__ 获取。\n
 * 不需要获取某变量，可以对应参数传入 AK_null。
 *
 * @param[out]
 *  year
 *  程序编译年份（>1900)。
 * @param[out]
 *  month
 *  程序编译月份（1 - 12)。
 * @param[out]
 *  mday
 *  程序编译日期（1 - 31)。
 * @param[out]
 *  hour
 *  程序编译所在小时（1 - 23)。
 * @param[out]
 *  min
 *  程序编译所在分钟（1 - 59)。
 * @param[out]
 *  sec
 *  程序编译所在秒钟（1 - 59)。
 *
 */
AK_API AK_void akae_whenbuild(AK_int *year, AK_int *month,
		AK_int *mday, AK_int *hour, AK_int *min, AK_int *sec);


/**
 * 对比两段内存的数据是否匹配。\n
 * 调用该接口时，用户要保证内存长度的合法性。
 *
 * @param[in]
 *  s1
 *  第一段内存数据。
 * @param[in]
 *  s2
 *  第二段内存数据。
 * @param[in]
 *  n
 *  对比的内存长度（单位：字节）。
 *
 * @return
 *  如果 @ref n 传入 0，则返回 0。\n
 *  如果两段内存数据 @ref n 个字节匹配返回 0，\n
 *  如果 @ref n 个字节中 @ref s1 数据小于 @ref s2 数据，则返回小于 0，\n
 *  如果大于则返回大于 0。
 *
 */
AK_API AK_int akae_memcmp(AK_bytptr s1, AK_bytptr s2, AK_size n);



AK_API AK_voidptr akae_memchr(AK_voidcptr s, AK_char chr, AK_size n);


AK_API AK_voidptr akae_memset(AK_voidptr s, AK_char c, AK_size n);

/**
 * 内存数据清 0，多用于内存使用初始化。
 */
AK_API AK_voidptr akae_memzero(AK_voidptr s, AK_size n);

/**
 * 初始化数组为 0 的快速调用操作。
 */
#define AK_MEMZERO_ARRAY(__array) \
		akae_memzero(__array, sizeof(__array));

/**
 * 初始化对象（结构体，联合体）为 0 的快速调用操作。
 */
#define AK_MEMZERO_OBJECT(__object) \
		akae_memzero(&(__object), sizeof(__object));


/*
 * Find the first occurrence of the byte string needle in byte string haystack.
 */
AK_API AK_voidptr akae_memmem (AK_voidcptr haystack, AK_size haystacklen,
		AK_voidcptr needle, AK_size needlelen);

AK_API AK_size akae_strlen(AK_chrcptr s);

AK_API AK_chrptr akae_strstr(AK_chrcptr haystack, AK_chrptr needle);


AK_API AK_chrptr akae_strcpy(AK_chrptr dest, AK_chrcptr src);

AK_API AK_chrptr akae_strncpy(AK_chrptr dest, AK_chrcptr src, AK_size n);

/**
 * 字符串修葺。\n
 * 去掉字符串开头与结束位置连续的空格。\n
 *
 * @param[in]
 *  text
 *  输入字符串。
 * @param[out]
 *  stack
 *  栈区内存缓冲，存放输出结果字符串。\n
 *  长度有 @ref stacklen 指定。
 * @param[in]
 *  stacklen
 *  栈区内存长度。
 *
 * @return
 *  输出字符串。
 */
AK_API AK_chrptr akae_strtrim(AK_chrcptr text,
		AK_chrptr stack, AK_size stacklen);

/**
 * 字符串背景填充。\n
 * 根据用户填入内容，加上背景字符串，并按照指定长度输出。\n
 * 如 text 可以通过填充 - 字符输出 --text--。
 *
 * @param[in]
 *  text
 *  输入字符串。
 * @param[in]
 *  bgchr
 *  背景字符串，必须为可见字符。
 * @param[out]
 *  stack
 *  栈区内存缓冲，存放输出结果字符串。\n
 *  长度有 @ref stacklen 指定。
 * @param[in]
 *  stacklen
 *  栈区内存长度，输出字符串长度根据 @ref stacklen 决定。\n
 *  字符串长度为 @ref stacklen 长度减去 1。
 *
 * @return
 *  输出字符串。
 */
AK_API AK_chrptr akae_strbgchr(AK_chrcptr text, AK_char bgchr,
		AK_chrptr stack, AK_size stacklen);

/**
 * 字符串比较。\n
 * 两个字符串完全相同的时候返回 True， 否则返回 False。
 */
AK_API AK_boolean akae_streq(AK_chrcptr str1, AK_chrcptr str2);

/**
 * 忽略大小写进行比较，见 @ref akae_streq()。
 */
AK_API AK_boolean akae_strcaseeq(AK_chrcptr str1, AK_chrcptr str2);

/**
 * 字符串前缀比较。
 * 两个字符串前缀相同的时候返回 True，\n
 * 如 absolute 与前缀 abs 比较匹配。\n
 * 否则返回 False。
 */
AK_API AK_boolean akae_strprefixeq(AK_chrcptr str, AK_chrcptr prefix);

/**
 * 忽略大小写进行比较，见 @ref akae_strprefixeq()。
 */
AK_API AK_boolean akae_strprefixcaseeq(AK_chrcptr str, AK_chrcptr prefix);

/**
 * 字符串后缀比较。\n
 * 两个字符串后缀相同的时候返回 True， 否则返回 False。
 */
AK_API AK_boolean akae_strsuffixeq(AK_chrcptr str, AK_chrcptr suffix);

/**
 * 忽略大小写进行比较，见 @ref akae_strsuffixeq()。
 */
AK_API AK_boolean akae_strsuffixcaseeq(AK_chrcptr str, AK_chrcptr suffix);


/**
 * 字符串中各个字符转化成全大写。
 */
AK_API AK_chrptr akae_strupper(AK_chrcptr text,
		AK_chrptr stack, AK_size stacklen);

/**
 * 字符串中各个字符转化成全小写。
 */
AK_API AK_chrptr akae_strlower(AK_chrcptr text,
		AK_chrptr stack, AK_size stacklen);

/**
 * 内存拷贝。\n
 * 两块内存可以出现重叠，内部会自动识别并处理。
 *
 * @return
 *  返回目标内存首地址。
 */
AK_API AK_bytptr akae_memcpy(AK_voidptr dest, AK_voidcptr src, AK_size n);


/**
 * 监测内存数据，并向按十六进制终端打印输出。
 *
 * @param[in]
 *  mem
 * @param[in]
 *  len
 */
AK_API AK_void akae_hexdump(AK_voidptr mem, AK_size len);


/**
 * @brief
 *  获取进程启动时间戳（单位：纳秒）。
 */
AK_API AK_size64 akae_clock_nano(void);

/**
* @brief
*  获取进系统时间戳（单位：微秒）。
*/
AK_API AK_size64 akae_clock_macro(void);

/**
 * 计算语句运行的时常（单位：微秒）。
 */
#define AK_TIME(__process) \
	({\
		AK_size64 us = akae_clock_macro ();\
		__process;\
		akae_clock_macro () - us;\
	})

/**
* @brief
*  获取进系统时间戳（单位：毫秒）。
*/
AK_API AK_size64 akae_clock_milli(void);

/**
* @brief
*  获取进系统时间戳（单位：秒）。
*/
AK_API AK_size64 akae_clock(void);


/**
 * @brief
 *  输出去掉目录后的路径结尾。
 */
AK_API AK_chrptr akae_basename(AK_chrptr path);

AK_API AK_boolean akae_has_file(AK_chrptr path);

/**
 * @brief
 *  返回 Epoch, 1970-01-01 00:00:00 +0000 的秒数。
 */
AK_API AK_uint32 akae_utc();

AK_API AK_void akae_sleep(AK_size sec);

AK_API AK_void akae_usleep(AK_size usec);

AK_API AK_int akae_atoi(AK_chrptr str);

AK_API AK_int64 akae_atoi64(AK_chrptr str);





/**
 * 数据小端对齐修正。
 */
#define AK_ALIGN_LEFT_END(__v, __av) \
	((__av) <= 0 ? 0 : (\
			((__v > 0) ?\
					((__v) - ((__v) % (__av)))\
					:\
					 ((__v) - ((__v) % (__av)) - ((0 != ((__v) % (__av))) ? (__av) : 0))\
					)))

/**
 * 数据大端对齐修正。
 */
#define AK_ALIGN_RIGHT_END(__v, __av) \
		((__av) <= 0 ? 0 : (\
			((__v > 0) ?\
					((__v) - ((__v) % (__av)) + ((0 != ((__v) % (__av))) ? (__av) : 0))\
					:\
					 ((__v) - ((__v) % (__av)))\
					)))


/**
 * 16 位大小端转换。
 */
#define AK_SWAP16(__v)  ((((AK_uint16)(__v) & 0xff00) >> 8) | (((AK_uint16)(__v) & 0x00ff) << 8))
 // 长整型大小端互换

/**
 * 32 位大小端转换。
 */
#define AK_SWAP32(__v)  ((((AK_uint32)(__v) & 0xff000000) >> 24) |\
                            (((AK_uint32)(__v) & 0x00ff0000) >> 8) |\
                            (((AK_uint32)(__v) & 0x0000ff00) << 8) |\
                            (((AK_uint32)(__v) & 0x000000ff) << 24))


/**
 * 4 字节数据实现本地序到网络序的转换。
 */
#define AK_HTON32(__h) \
	(akae_little_endian() ? AK_SWAP32(__h) : __h)

/**
 * 3 字节数据实现本地序到网络序的转换。
 */
#define AK_HTON24(__h) (AK_HTON32(__h) >> 8)

/**
 * 2 字节数据实现本地序到网络序的转换。
 */
#define AK_HTON16(__h) \
	(akae_little_endian() ? AK_SWAP16(__h) : __h)


/**
 * 4 字节数据实现网络序到本地序的转换。
 */
#define AK_NTOH32(__n) AK_HTON32(__n)

/**
 * 3 字节数据实现网络序本地序转换。
 */
#define AK_NTOH24(__n) (AK_NTOH32(__n) >> 8)

/**
 * 2 字节数据实现网络序到本地序的转换。
 */
#define AK_NTOH16(__n) AK_HTON16(__n)


/// @brief 返回两数的最小值。
#define AK_MIN(__a, __b) ((__a) <= (__b) ? (__a) : (__b))

/// @brief 返回两数的最大值。
#define AK_MAX(__a, __b) ((__a) >= (__b) ? (__a) : (__b))

/**
 * @macro
 *  把变量 @ref __v 固定在最大最小范围内。
 */
#define AK_FIX_RANGE(__v, __min, __max) \
	(__min) <= (__max) ? AK_MAX(AK_MIN(__v, (__max)), (__min)) : AK_MAX(AK_MIN(__v, (__min)), (__max))



AK_C_HEADER_EXTERN_C_END
#endif ///< STDLIB_H_
