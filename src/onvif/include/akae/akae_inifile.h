

#include <akae_object.h>

#if !defined(AKSPC_INIFILE_H_)
#define AKSPC_INIFILE_H_
AK_C_HEADER_EXTERN_C_BEGIN

/**
 * 最长段区文本长度定义。
 */
#define AK_INIFL_MAX_SECTION_LEN     (64)

/**
 * 最长键文本长度定义。
 */
#define AK_INIFL_MAX_KEY_LEN         (128)

/**
 * 最长值文本长度定义。
 */
#define AK_INIFL_MAX_VALUE_LEN       (128)


/**
 * 创建一个 ini 模块对象。\n
 * 成功创建以后，可以通过读写对其进行操作。
 *
 * @param[IN] Malloc
 *  内存分配器，模块内部会使用它来进行一些资源分配。
 *
 * @param[IN] threadsafe
 *  线程安全标致，置位表示启用线程安全，内部会接口会启用互斥读写，\n
 *  启用线程安全以后，模块需要额外的性能开销，用户使用是如果确保该模块在线程内调用可以不需要启用线程安全。
 *
 * @return
 *  创建成功返回模块句柄，解析失败返回 AK_null。
 *
 */
AK_API AK_Object akae_inifile_create (AK_Object Malloc,
		AK_boolean threadsafe);

/**
 * 解析 ini 文本同时创建一个 ini 模块句柄。\n
 * 内部会调用 @ref akae_inifile_create() 方法。
 *
 */
AK_API AK_Object akae_inifile_parse (AK_Object Malloc,
		AK_boolean threadsafe,
		AK_chrptr initext);

/**
 * 释放一个 ini 模块对象。
 */
AK_API AK_int akae_inifile_release (AK_Object Object);


/**
 * 从对象中读取字符串。\n
 * 当未找到指定的区块与键索时，会返回缺省值，以保证用户调用有序性。
 *
 * @param[IN] Object
 *  ini 模块对象。
 *
 * @param[IN] section
 *  ini 段区，通过段区和键索引数值，传入 AK_null 表示索引无段区键值。
 *
 * @param[IN] key
 *  ini 键，通过段区和键索引数值。
 *
 * @param[IN] def
 *  ini 缺省数值，当在用户指定的段区和键不存在时，会返回其缺省值。
 *
 * @return
 *  返回指定段区和键上的数值。
 *
 */
AK_API AK_chrptr akae_inifile_read_string (AK_Object Object,
		AK_chrptr section,
		AK_chrptr key,
		AK_chrptr def);

/**
 * 从对象中读取一个整型数据。\n
 * 内部调用 @ref akae_inifile_read_string() 方法。
 */
AK_API AK_int akae_inifile_read_integer (AK_Object Object,
		AK_chrptr section,
		AK_chrptr key,
		AK_int def);

/**
 * 向对象写入字符串。
 *
 * @param[IN] Object
 *  ini 模块对象。
 *
 * @param[IN] section
 *  ini 段区，通过段区和键索引数值，传入 AK_null 表示索引无段区键值。
 *
 * @param[IN] key
 *  ini 键，通过段区和键索引数值。
 *
 * @param[IN] val
 *  写入数值。。
 *
 */
AK_API AK_int akae_inifile_write_string (AK_Object Object,
		AK_chrptr section,
		AK_chrptr key,
		AK_chrptr val);


/**
 * 向对象中写入整型数据。\n
 * 内部调用 @ref akae_inifile_write_string() 方法。
 */
AK_API AK_int akae_inifile_write_integer (AK_Object Object,
		AK_chrptr section,
		AK_chrptr key,
		AK_int val);

/**
 * 移除个段。
 */
AK_API AK_int akae_inifile_remove_section (AK_Object Object, AK_chrptr section);


/**
 * 移除指定位置数据。
 * 注意：尽管指定位置上数据不存在，该方法仍然返回成功。
 */
AK_API AK_int akae_inifile_remove_key (AK_Object Object,
		AK_chrptr section,
		AK_chrptr key);


/**
 * 对 ini 对象进行序列化，并把结果返回到内存中。
 *
 * @param[IN] pretty
 *  美化标识，置位时表示在序列化过程中填充一些空格进行对齐美化，\n
 *  因此美化会增加返回结果的内存用量。
 *
 * @param[OUT] stack
 *  序列化数据返回结果的内存起始地址。
 *
 * @param[IN] stacklen
 *  序列化数据返回结果的内存最大长度。
 *
 * @return
 *  成功返回序列化结果长度，失败返回对应错误码。
 *
 * @retval AK_ERR_INVAL_OPERATE
 *  操作失败，用户传入了非法对象。
 * @retval AK_ERR_INVAL_PARAM
 *  操作失败，用户传入非法参数，可能 @ref stack 或 @ref stacklen 异常。
 *
 *
 */
AK_API AK_ssize akae_inifile_stringify (AK_Object Object,
		AK_boolean pretty,
		AK_chrptr stack,
		AK_size stacklen);



AK_C_HEADER_EXTERN_C_END
#endif ///< AKSPC_INIFILE_H_
