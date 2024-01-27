/**
 * 日志信息控制单体模块。
 */

#include <akae_typedef.h>

#if !defined(AKAE_VERBOSE_H_)
#define AKAE_VERBOSE_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * 终端输出表格句柄。
 * 存放表格输出过程中的上下文。
 */
typedef struct _AK_VerboseForm {

	AK_char title[32];

	/**
	 * 表格宽度。
	 */
	AK_size width, full_width;

	/**
	 * 左边距空格。
	 */
	AK_size padding;

	/**
	 * 结束行标识。
	 */
	AK_boolean end_ln;

} AK_VerboseForm;

/**
 * 开始绘制终端表格。
 * 初始化 @ref Form 句柄。
 */
AK_API AK_int akae_verbose_form_init (AK_VerboseForm *Form, AK_chrcptr title, AK_size width, AK_size padding);

/**
 * 终端表格追加一行明文文本。
 *
 * @param[in]			end_ln			行结束标识，为 True 时明文追加后会加上底部边框。
 * @param[in]			fmt				文本输出格式。
 *
 * @retval 0		输出成功。
 * @retval -1		输出失败。
 *
 */
AK_API AK_int akae_verbose_form_put_text (AK_VerboseForm *Form, AK_boolean end_ln, AK_chrptr fmt, ...);

/**
 * 终端表格追加一行“键 - 值”格式的文本。。
 *
 * @param[in]			end_ln			行结束标识，为 True 时明文追加后会加上底部边框。
 * @param[in]			key				键文本。
 * @param[in]			fmt				值文本输出格式。
 *
 * @retval 0		输出成功。
 * @retval -1		输出失败。
 *
 */
AK_API AK_int akae_verbose_form_put_kv (AK_VerboseForm *Form, AK_boolean end_ln, AK_chrcptr key, AK_chrcptr fmt, ...);

/**
 * 终端表格快速输出一行“键 - 32 位整型”格式文本操作宏。
 *
 */
#define akae_verbose_form_put_key_int32(__Form, __end_ln, __key, __int32) \
	akae_verbose_form_put_kv (__Form, __end_ln, __key, "%d - %08x", (AK_int)(__int32), (AK_UInt32)(__int32))

/**
 * 终端表格快速输出一行“键 - 字符串”格式文本操作宏。
 *
 */
#define akae_verbose_form_put_key_int64(__Form, __end_ln, __key, __int64) \
	akae_verbose_form_put_kv (__Form, __end_ln, __key, "%lld - %016x", (AK_int64)(__int64), (AK_UInt64)(__int64))

/**
 * 终端表格快速输出一行“键 - 字符串”格式文本操作宏。
 *
 */
#define akae_verbose_form_put_key_string(__Form, __end_ln, __key, __str) \
	akae_verbose_form_put_kv (__Form, __end_ln, __key, "%s", __str)


/**
 * 结束绘制。\n
 * 调用此接口会根据之前的输出情况追加一个底部边框，并复位句柄。
 *
 */
AK_API AK_int akae_verbose_form_finish (AK_VerboseForm *Form);



AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_VERBOSE_H_
