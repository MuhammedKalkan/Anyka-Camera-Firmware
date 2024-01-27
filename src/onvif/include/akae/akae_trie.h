
#include <akae_typedef.h>
#include <akae_object.h>

#if !defined(AKAE_TRIE_H_)
# define AKAE_TRIE_H_
AK_C_HEADER_EXTERN_C_BEGIN

/**
 * @macro
 *  最大键值长度。
 */
#define AK_STRLST_MAX_KEY_LEN (256)



/**
 * 创建字典树模块。创建句柄后，所有 @ref key 索引方式均大小写敏感，\n
 * 如果需要使用大小写不敏感方式见 @ref akae_trie_create2() 方法。
 *
 * @param[IN] Malloc
 *  用于列表模块的内存分配器。
 *
 * @return
 *  创建成功返回模块句柄，失败返回 AK_null。
 */
AK_API AK_Object akae_trie_create (AK_Object Malloc);

/**
 * 详见 @ref akae_trie_create(), 可通过参数 @ref case_sensitive 确定是否大小写敏感.
 */
AK_API AK_Object akae_trie_create2 (AK_Object Malloc, AK_boolean case_sensitive);

/**
 * 是放字典树模块句柄。\n
 * 字典书模块所使用的内存通过创建时传入的内存分配其回收.
 *
 */
AK_API AK_int akae_trie_release (AK_Object Object);


/**
 * 获取当前模块大小写敏感标识。\n
 * 注意该接口传入异常句柄同样返回 AK_false。因此返回结果准确性要在参数正确既定的情况下使用.
 *
 */
AK_API AK_boolean akae_trie_case_sensitive (AK_Object Object);


/**
 * 获取列表长度。\n
 * 注意该接口传入参数异常句柄返回 0。因此返回结果准确性要在参数正确既定的情况下使用.
 *
 * @return
 *  返回列表的长度（元素个数）。
 */
AK_API AK_size akae_trie_length (AK_Object Object);

/**
 * @brief
 *  向容器中插入以 @ref key 词条为索引 data 指向的内存区域，长度为 @ref data_len 的一组数据。\n
 *  容器会根据传入长度，在容器内部复制一分副本数据。\n
 *
 * @param overwrite [in]
 *  覆盖标识，设置为 False 时，如果容器中已经存在该关键字，并且不覆盖并追加一条关键字一样的数据。
 * @param key [in]
 *  词条字符串，长度必须大于 0，大小写敏感。
 * @param data [in]
 *  词条对应的数据。
 * @param datalen [in]
 *  数据长度，模块会根据长度在内部保存相等长度的内存块。
 *
 * @return
 *  插入成功返回 0，否则返回 -1。
 *
 */
AK_API AK_int akae_trie_insert (AK_Object Object, AK_chrcptr key, AK_bytptr data, AK_size datalen);

/**
 * 从字典树中移除 @ref key 键值对应数据。
 *
 * @param[IN] key
 *  词条字符串，长度必须大于 0，大小是否敏感根据 @ref akae_trie_create2() 初始化时传入指定。
 *
 * @retval AK_OK
 *  移除成功，
 *  移除成功返回 0，不存在该关键字返回 -1。
 */
AK_API AK_int akae_trie_remove (AK_Object Object, AK_chrcptr key);


/**
 * 通过键值索引数据。\n
 * 索引结果通过参数 @ref data 与 @ref datalen 返回，\n
 * 注：这里 @ref data 以及 @ref datalen 传入 AK_null，不会返回错误，但是用无无法从参数处获取索引结果。\n
 * 索引的结果为引用过程，因此对内存块改写会导致源数据破坏，用户尽可能在使用该数据时避免进行对其移除操作。
 *
 * @param[IN] key
 *  索引键值。
 *
 * @param[OUT] data
 *  返回结果的内存起始地址，索引到数据成功后，该地址会指向数据的起始地址。
 *
 * @param[OUT] datalen
 *  数据长度，索引到数据成功后，该数据返回数据的长度。
 *
 * @retval AK_OK
 *  索引成功。
 * @retval AK_ERR_INVAL_OBJECT
 *  索引失败，传入了无效的对象句柄。
 * @retval AK_ERR_INVAL_PARAM
 *  索引失败，肯呢个传入了错误的 @ref key 参数。
 * @retval AK_ERR_NOT_EXIST
 *  索引失败，键值对应数据不存在。
 *
 *
 */
AK_API AK_int akae_trie_indexof (AK_Object Object, AK_chrcptr key, AK_bytptr *data, AK_size *datalen);


/**
 * 通过键值索引数据。\n
 * 索引结果通过参数 @ref data 与 @ref datalen 返回，\n
 * 注：这里 @ref data 以及 @ref datalen 传入 AK_null，不会返回错误，但是用无无法从参数处获取索引结果。\n
 * 索引的结果为引用过程，因此对内存块改写会导致源数据破坏，用户尽可能在使用该数据时避免进行对其移除操作。
 *
 * @param[IN] pos
 *  字典树的值.
 *
 * @param[OUT] data
 *  返回结果的内存起始地址，索引到数据成功后，该地址会指向数据的起始地址。
 *
 * @param[OUT] datalen
 *  数据长度，索引到数据成功后，该数据返回数据的长度。
 *
 * @retval AK_OK
 *  索引成功。
 * @retval AK_ERR_INVAL_OBJECT
 *  索引失败，传入了无效的对象句柄。
 * @retval AK_ERR_INVAL_PARAM
 *  索引失败，肯呢个传入了错误的 @ref key 参数。
 * @retval AK_ERR_NOT_EXIST
 *  索引失败，键值对应数据不存在。
 *
 *
 */
AK_API AK_int akae_trie_locate (AK_Object Object, AK_int pos, AK_chrptr *key, AK_bytptr *data, AK_size *datalen);

/**
 * 判断字典树中是否存在某个键值。
 *
 * @param[IN] Object
 *  对象句柄
 *
 * @param[IN] key
 *  索引键值
 *
 * @retval AK_true
 *  存在键值对应数据。
 *
 * @ref AK_false
 *  键值数据不存在，该接口传入参数异常同样返回 AK_false。因此返回结果准确性要在参数正确既定的情况下使用。
 */
AK_API AK_boolean akae_trie_has_key (AK_Object Object, AK_chrcptr key);


/**
 * 从字典数中读取一个读取并移除数据.\n
 * 功能与 @ref akae_trie_indexof() 方法相似。\n
 * 数据读取以后, 将会从字典树中移除, 区别于 @ref akae_trie_indexof() 方法.
 *
 * @param[IN] Object
 *  对象句柄
 *
 * @param[IN] key
 *  索引键值
 *
 * @param[OUT] stack
 *  栈区内存空间地址，用于存放返回的数据。
 *
 * @param[IN] stacklen
 *  传入栈区内存空间。
 *
 * @retval AK_OK
 *  操作成功.
 * @retval AK_ERR_INVAL_OBJECT
 *  操作错误, 传入无效的对象句柄.
 * @retval AK_ERR_INVAL_PARAM
 *  操作错误, 传入无效参数.
 * @retval AK_ERR_OUT_OF_MEM
 *  操作错误, 传入栈区内存不足返回结果.
 *
 */
AK_ssize akae_trie_get_off (AK_Object Object, AK_chrcptr key, AK_bytptr stack, AK_size stacklen);


AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_TRIE_H_
