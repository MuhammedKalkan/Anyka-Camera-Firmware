/**
 * JSON 操作模块。
 */

#include <akae_object.h>

#if !defined(AKSPC_JSON_H_)
#define AKSPC_JSON_H_
AK_C_HEADER_EXTERN_C_BEGIN

/**
 * JSON 路径最大文本长度。
 */
#define AK_JSON_PATH_MAX_LEN     (128)

/**
 * JSON 对象类型定义。
 */
#define AK_JSON_TYPE_UNDEF       (0) ///< 未定义类型。
#define AK_JSON_TYPE_NULL        (1) ///< 空类型。
#define AK_JSON_TYPE_BOOLEAN     (2) ///< 布尔类型。
#define AK_JSON_TYPE_NUMBER      (3) ///< 数类型
#define AK_JSON_TYPE_STRING      (4) ///< 字符串类型。
#define AK_JSON_TYPE_ARRAY       (5) ///< 数组类型。
#define AK_JSON_TYPE_OBJECT      (6) ///< 对象结构类型。

/**
 * 解析 JSON 文本到数据结构。
 *
 * @param[IN] Mallctr
 *  内存分配器，创建 JSON 对象的时候需要使用内存分配器分配内存。
 *
 * @param[IN] text
 *  文本。
 *
 * @return
 *  解析成功会创建一个 JSON 对象，失败返回 AK_null，\n
 *  有可能参数异常，或者 @ref text 文本内容解析失败。
 *  解析成功。
 *
 */
AK_API AK_Object akae_json_parse (AK_Object Mallctr, AK_chrcptr text);


/**
 * 复制 JSON 数据对象，\n
 * 复制以后得到一个与原来数据结构内容一样的对象句柄，\n
 * 两者可以对立使用，互不引用。
 *
 * @param[IN] Mallctr
 *  模块内存分配器。
 *
 * @param[IN] Object
 *  JSON 数据对象。
 *
 * @return
 *  复制成功以后，返回新创建的 JSON 对象，\n
 *  失败返回 AK_null，有可能是参数错误。
 */
AK_API AK_Object akae_json_duplicate (AK_Object Mallctr, AK_Object Object);

/**
 * 释放 JSON 数据对象。
 *
 * @param[IN] Object
 *  JSON 数据对象。
 *
 * @retval AK_OK
 *  释放成功。
 * @retval AK_ERR_INVAL_OBJECT
 *  释放失败，对象无效。
 */
AK_API AK_int akae_json_release (AK_Object Object);

/**
 * 获取 JSON 数据对象所使用的内存分配器。\n
 * 分配器在创建 JSON 数据对象时传入，\n
 * 该方法主要用于创建多个对象时，满足分配器同源管理。
 *
 * @param[IN] Object
 *  JSON 数据对象。
 *
 * @return
 *  返回内存分配器句柄，失败返回 AK_null，可能是参数错误。
 */
AK_API AK_Object akae_json_mallocator (AK_Object Object);


/**
 * 获取 JSON 数据对象类型。
 *
 * @param[IN] Object
 *  对象句柄。
 *
 * @return
 *  获取成功返回数据类型，\n
 *  获取失败返回 AK_JSON_TYPE_UNDEF，可能是参数错误。
 *
 */
AK_API AK_int akae_json_typeof (AK_Object Object);


/**
 * 获取 JSON 数据对象键值。
 *
 * @param[IN] Object
 *  对象句柄。
 *
 * @return
 *  获取成功返回键值字符串，如果字符串是 "" 则该节点是根节点，没有对应键值，\n
 *  获取失败返回 AK_null，可能参数错误。
 *
 */
AK_API AK_chrptr akae_json_keyof (AK_Object Object);


/**
 * 获取 JSON 布尔数据对象上数值。
 *
 * @param[IN] Object
 *  对象句柄。
 *
 * @param[IN] def
 *  缺省值，当数据结构类型不匹配或者读取错误时会返回此值。
 *
 * @return
 *  返回数据对象的布尔值，当参数异常，或者数据对象并非布尔型的时候，\n
 *  会返回缺省值 @ref def。
 *
 */
AK_API AK_boolean akae_json_get_boolean (AK_Object Object, AK_boolean def);

/**
 * 获取 JSON 数值数据对象上数值。
 */
AK_API AK_dbfloat akae_json_get_number (AK_Object Object, AK_dbfloat def);

/**
 * 获取 JSON 字符串数据对象上数值。
 */
AK_API AK_chrptr akae_json_get_string(AK_Object Object, AK_chrptr def);

/**
 * 获取 JSON 数据对象上数值。
 */
AK_API AK_size akae_json_sizeof (AK_Object Object);


AK_API AK_Object akae_json_parent (AK_Object Object);

AK_API AK_Object akae_json_next_object (AK_Object Object);

/**
 * 获取数组中的元素对象。
 */
AK_API AK_Object akae_json_indexof (AK_Object Array, AK_int which);

/**
 * 获取对象中的元素对象。
 */
AK_API AK_Object akae_json_indexof_key (AK_Object Object, AK_chrcptr key);

/**
* 不区分大小写获取对象中的元素对象。
*/
AK_API AK_Object akae_json_indexof_casekey (AK_Object Object, AK_chrcptr key);


/**
 * 创建空类型 JSON 对象。
 */
AK_API AK_Object akae_json_create_null (AK_Object Mallctr);

/**
 * 创建布尔类型 JSON 对象。
 */
AK_API AK_Object akae_json_create_boolean (AK_Object Mallctr, AK_boolean b);


/**
 * 创建数值类型 JSON 对象。
 */
AK_API AK_Object akae_json_create_number (AK_Object Mallctr, AK_dbfloat number);


/**
 * 创建字符串类型 JSON 对象。
 */
AK_API AK_Object akae_json_create_string (AK_Object Mallctr, AK_chrcptr str);


/**
 * 创建数组类型 JSON 对象。
 */
AK_API AK_Object akae_json_create_array (AK_Object Mallctr);


/**
 * 创建结构类型 JSON 对象。
 */
AK_API AK_Object akae_json_create_object (AK_Object Mallctr);


/**
 * 创建整型数组 JSON 对象。
 */
AK_API AK_Object akae_json_create_integer_array (AK_Object Mallctr, AK_int *numbers, AK_size len);


/**
 * 创建浮点数组 JSON 对象。
 */
AK_API AK_Object akae_json_create_float_array(AK_Object Mallctr, AK_float *numbers, AK_size len);


/**
 * 创建双精度浮点数组 JSON 对象。
 */
AK_API AK_Object akae_json_create_dfloat_array(AK_Object Mallctr, AK_dbfloat *numbers, AK_size len);


/**
 * 创建字符串浮点数组 JSON 对象。
 */
AK_API AK_Object akae_json_create_string_array (AK_Object Mallctr, AK_chrcptr *array, AK_size len);


/**
 * 追加一个对象到数组。
 */
AK_API AK_Object akae_json_append_object_to_array (AK_Object Array, AK_Object Object);


/**
 * 往 JSON 对象中加入一个新的子对象成员。\n
 * 注意：如果 JSON 对象中本身存在键值为 @ref key 的值，调用此接口不会将其覆盖，\n
 * 而是新增一个键值一样的子对象，这样可以兼容某些协议的数据包要求。\n
 * 但本模块只支持操作第一个添加的键值对应的数据。\n
 *
 * @param[IN] Object
 *  JSON 数据结构对象。
 *
 * @param[IN] key
 *  JSON 数据结构对应的键值。
 *
 * @param[IN] AddObject
 *  加入的 JSON 对象。
 *
 * @return
 *  创建成功返回创建的对象句柄，\n
 *  注意：创建成功以后，@ref AddObject 对象内存管理将会被 @ref Object 所接管，\n
 *  用户不需要额外释放 @ref AddObject 对象。\n
 *  失败返回 AK_null，这个时候由于创建失败，因此 @ref Object 无法接管 @ref AddObject 对象内存管理，\n
 *  因此用户需要额外手动释放 @ref AddObject。
 *
 */
AK_API AK_Object akae_json_add_object_to_object (AK_Object Object, AK_chrcptr key, AK_Object AddObject);

/**
 * 新建一个空类型数据到对象中。
 */
AK_API AK_Object akae_json_add_null_to_object (AK_Object Object, AK_chrptr key);


/**
 * 新建一个值为 @ref b 的布尔类型数据到对象中。
 */
AK_API AK_Object akae_json_add_boolean_to_object (AK_Object Object, AK_chrptr key, AK_boolean b);


/**
 * 新建一个值为 @ref n 的数据类型数据到对象中。
 */
AK_API AK_Object akae_json_add_number_to_object (AK_Object Object, AK_chrptr key, AK_dbfloat n);


/**
 * 新建一个值为 @ref s 的字符串类型数据到对象中。
 */
AK_API AK_Object akae_json_add_string_to_object (AK_Object Object, AK_chrptr key, AK_chrptr s);

/**
 * 根据索引分离出一个对象，\n
 * 分离出来的对象，不在受原对象内存管理，用户使用完以后需要手动将其释放。\n
 *
 * @param[IN] Object
 *  JSON 对象。
 *
 * @param[IN] which
 *  索引位置，从 0 开始，可以通过传入负数倒序索引，例如 -1 从倒数第一个元素开始索引。
 *
 * @return
 *  返会索引到的对象句柄，失败返回 AK_null。
 */
AK_API AK_Object akae_json_detach_object_by_index (AK_Object Object, AK_int which);

/**
 * 根据键值分离出一个对象，\n
 * 分离出来的对象，不在受原对象内存管理，用户使用完以后需要手动将其释放。\n
 * 参考 @ref akae_json_detach_object_by_index()。
 */
AK_API AK_Object akae_json_detach_object_by_key (AK_Object Object, AK_chrcptr key);

/**
 * 根据索引删除一个对象。
 */
AK_API AK_int akae_json_remove_object_by_index (AK_Object Object, AK_int which);


/**
 * 根据键值删除一个对象，
 */
AK_API AK_int akae_json_remove_object_by_key (AK_Object Object, AK_chrcptr key);


/**
 * 根据索引替换一个对象。
 */
AK_API AK_Object akae_json_replace_object_by_index (AK_Object Array, AK_size which, AK_Object Replace);


/**
 * 根据键值替换一个对象，
 */
AK_API AK_Object akae_json_replace_object_by_key (AK_Object Object, AK_chrcptr key, AK_Object Replace);

/**
 * 替换对象。
 */
AK_API AK_Object akae_json_replace_object (AK_Object Object, AK_Object Replace);


/**
 * 强制转换对象类型。
 */
AK_API AK_int akae_json_convert_type (AK_Object Object, AK_int type);



/**
 *
 * 通过 JSON 路径查询对象元素。\n
 * 当 @ref Item 为 Nil 时表示读取该路径上的对象并返回，\n
 * 反之则更新该路径上的对象。\n
 * 路径表达式符合 JSONPath 标准的子集，详见 http://goessner.net/articles/JsonPath/index.html。\n
 * 目前支持通过 “.” 和 “[]” 表达式索引，如：\n
 * \n
 * store.book[0].title \n
 * $.store[0].color \n
 * $.store.persion[2][0][0] \n
 * $.store.persion[(@.length-2)][(@.length-1)][0] \n
 * $.store['bicycle'].color \n
 * 其它复杂表达式暂不支持。
 *
 * @param[IN] Object
 *  JSON 数据对象。
 *
 * @param[IN] jsonpath
 *  JSON 描述路径，最大长度为 @ref AK_JS_PATH_MAX_LEN 个字节。\n
 *
 * @return
 *  获取成功返回对象句柄，失败返回 AK_null，可能是参数错误。
 *
 */
AK_API AK_Object akae_json_path_get_object (AK_Object Object, AK_chrcptr jsonpath);


/**
 * 在路径上获取一个布尔数据操作，如果该路径上不存在返回缺省值 @ref __def。
 */
#define akae_json_path_get_boolean(__Object, __jsonpath, __def) \
	akae_json_get_boolean (akae_json_path_get_object((__Object), (__jsonpath)), (__def))

/**
 * 在路径上获取一个数值数据操作，如果该路径上不存在返回缺省值 @ref __def。
 */
#define akae_json_path_get_number(__Object, __jsonpath, __def) \
	akae_json_get_number (akae_json_path_get_object((__Object), (__jsonpath)), (__def))

/**
 * 在路径上获取一个字符串数据操作，如果该路径上不存在返回缺省值 @ref __def。
 */
#define akae_json_path_get_string(__Object, __jsonpath, __def) \
	akae_json_get_string (akae_json_path_get_object((__Object), (__jsonpath)), (__def))


/**
 * 在对应 JSON 路径上设置一个 JSON 对象。\n
 * JSON 路径规则见 @ref akae_json_path_get_object()。\n
 *
 * @param[IN] Object
 *  JSON 数据对象。
 *
 * @param[IN] jsonpath
 *  JSON 描述路径，最大长度为 @ref AK_JSON_PATH_MAX_LEN 个字节。\n
 *
 * @param[IN] SetObject
 *  当为 AK_null 时表示读取该路径上的对象并返回，\n
 *  反之则更新该路径上的对象并返回。\n
 *
 * @return
 *  应用成功返回新应用的 JSON 数据对象（@ref SetObject），\n
 *  这个时候 @ref SetObject 内存分配会被 @ref Object 接管，\n
 *  用户不需要额外对 @ref SetObject 进行释放。\n
 *  如果设置失败返回 AK_null，这个时候，用户需要手动对 @ref SetObject 进行释放。
 *
 */
AK_API AK_Object akae_json_path_set_object (AK_Object Object, AK_chrcptr jsonpath, AK_Object SetObject);


/**
 * 通过 JSON 路径在 JSON 对象上加入一个布尔类型的数据。
 */
#define akae_json_path_set_boolean(__Object, __jsonpath, __b) \
	do {\
		AK_Object __NewObject = akae_json_create_boolean (akae_json_mallocator(__Object), __b);\
		if (AK_null != __NewObject) {\
			if (!akae_json_path_set_object ((__Object), (__jsonpath), __NewObject)) {\
				akae_json_release (__NewObject);\
			}\
		}\
	} while (0)

/**
 * 通过 JSON 路径在 JSON 对象上加入一个数值类型的数据。
 */
#define akae_json_path_set_number(__Object, __jsonpath, __n) \
	do {\
		AK_Object __NewObject = akae_json_create_number (akae_json_mallocator(__Object), __n);\
		if (AK_null != __NewObject) {\
			if (!akae_json_path_set_object ((__Object), (__jsonpath), __NewObject)) {\
				akae_json_release (__NewObject);\
			}\
		}\
	} while (0)

/**
 * 通过 JSON 路径在 JSON 对象上加入一个字符串类型的数据。
 */
#define akae_json_path_set_string(__Object, __jsonpath, __s) \
	do {\
		AK_Object __NewObject = akae_json_create_string (akae_json_mallocator(__Object), __s);\
		if (AK_null != __NewObject) {\
			if (!akae_json_path_set_object ((__Object), (__jsonpath), __NewObject)) {\
				akae_json_release (__NewObject);\
			}\
		}\
	} while (0)



/**
 * 在 JSON 路径上移除一个 JSON 对象。
 */
AK_API AK_int akae_json_path_remove_object (AK_Object Object, AK_chrptr jsonpath);

/**
 * @brief
 *  查询元素集合，通过 JSON 路径查询一系列相关元素集合。
 *  查询成功后会返回一个由 @ref Obj 内部内存分配器创建的 JSON 数组，
 *  在用户对 JSON 数组使用后需要手动调用 @ref akae_json_Free() 方法将其释放。
 *
 * @param[IN] Obj
 *  JSON 数据对象。
 *
 * @param[IN] jpath
 *  JSON 集合描述路径。
 *
 * @retval JSON 数组
 *  查询到的数据在 JSON 集合内，可通过 @ref akae_json_ArrayOf() 对集合内容进行遍历，\n
 *  如果查询失败，数据集合为空。
 *
 * @retval Nil
 *  查询失败，可能参数错误。
 */
AK_API AK_Object akae_json_path_query (AK_Object Object, AK_chrptr jsonpath);






/**
 * 压缩 JSON 文本，\n
 * 去掉文本中 JSON 有效数据结以外的空格、换行和缩进等符号，\n
 * 达到节省文本消耗内存大小的目的。
 *
 * @param[IN,OUT] jsonp
 *  JSON 文本数据。
 *
 * @retval AK_OK
 *  操作成功。
 * @retval AK_ERR_INVAL_PARAM
 *  操作失败，参数非法。
 */
AK_API AK_int akae_json_minify_text (AK_chrptr jsonp);


/**
 * 转换 JSON 数据对象成文本。\n
 * 数据指针会保留在模块内部，待模块释放时自动释放，用户不需要额外释放。
 *
 * @param[IN] Object
 *  JSON 数据对象。
 *
 * @param[IN] pretty
 *  美化标识，为 True 输出更适合阅读，但占据较大的内存空间。
 *
 * @return
 *  转换成功返回字符串的起始地址，转换失败返回 AK_null，可能是参数错误。
 *
 */
AK_API AK_chrptr akae_json_stringify (AK_Object Object, AK_boolean pretty);

/**
 * 参考 @ref akae_json_stringify()，从栈区获取结果。
 */
AK_API AK_ssize akae_json_stringify2 (AK_Object Object, AK_boolean pretty, AK_chrptr stack, AK_size stacklen);

/**
 * @brief
 *  转换成 Javascript 变量定义文本。
 *
 *
 */
AK_API AK_ssize
akae_json_ToJSVar(AK_Object Object, AK_chrptr key, AK_chrptr stack, AK_size stacklen);


/**
 * @macro
 *  JSON 数据对象遍历操作。
 */
#define akae_json_foreach(__Object, __Child) \
	for ((__Child) = akae_json_indexof((__Object), 0);\
		AK_null != (__Child);\
		(__Child) = akae_json_next_object((__Child)))

/**
 * 清空 JSON 数据对象操作。
 */
#define akae_json_empty_object(__Object) \
	do {\
		if (AK_null != (__Object)) {\
			if (AK_JSON_TYPE_ARRAY == akae_json_typeof (__Object)\
					|| AK_JSON_TYPE_OBJECT == akae_json_typeof (__Object)) {\
				while (akae_json_sizeof(__Object) > 0) {\
					akae_json_remove_object_by_index ((__Object), 0);\
				}\
			}\
		}\
	} while (0)

/**
 * 创建差异补丁。\n
 * 补丁同样是一个 JSON 数据对象，由 @ref Alloctr 创建，\n
 * 用户在使用该对象之后按照 JSON 数据对象释放方法 @ref akae_json_Free() 释放该对象，否则会造成内存泄漏。\n
 * 详见 https://tools.ietf.org/html/rfc7396。
 *
 * @param[IN] Mallctr
 *  创建差异补丁的内存分配器。
 *
 * @param[IN] Object1
 *  对比差异源对象。
 *
 * @param[IN] Object2
 *  对比差异目标对象。
 *
 * @return
 *  对比成功创建一个对比差异对象，对比失败返回 AK_null，可能是参数错误。
 *
 */
AK_API AK_Object akae_json_create_patch (AK_Object Mallctr, AK_Object Object1, AK_Object Object2);

/**
 * @brief
 *  合并补丁到数据对象。\n
 *  合并之后，@ref Patch 对象不会被释放，\n
 *  用户使用 @ref akae_json_Free() 方法释放该对象，否则会造成内存泄漏。\n
 *  详见 https://tools.ietf.org/html/rfc7396。
 *
 * @param[IN] Object
 *  目标对象，此方法会直接根据补丁内容修改补丁
 *
 * @param[IN] Patch
 *  通过 @ref akae_json_create_patch() 方法创建的补丁对象。
 *
 * @return
 */
AK_API AK_Object akae_json_merge_patch (AK_Object Object, AK_Object Patch);


AK_C_HEADER_EXTERN_C_END
#endif /* JSON_H_ */
