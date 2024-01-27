
/**
 * 模块基础数据结构。\n
 * 作为其类似模块的父类。\n
 * 通过对其子类模块数据结构强行转换可以获取子类模块名称。
 *
 * 通过多态可以调用 NK_Object 的内容。
 *
 */

#include "types.h"


#ifndef NK_OBJECT_H_
#define NK_OBJECT_H_
NK_CPP_EXTERN_BEGIN

/**
 * Object 模块句柄。
 */
typedef struct Nk_Object
{
#define THIS struct Nk_Object *const

	/**
	 * 获取此模块名称。
	 */
	NK_PChar
	(*name)(THIS);

	/**
	 * 调试打印此模块相关信息。
	 */
	NK_Void
	(*dump)(THIS);

	/**
	 * 线程安全标识声明。\n
	 * 标明此模块的操作接口是否线程安全。
	 */
	NK_Boolean
	(*threadsafe)(THIS);

	/**
	 * 释放当前模块。\n
	 * 调用此接口之后，模块句柄不再有效，\n
	 * 外部用于需及时将句柄置 Nil 避免产生不必要的错误。
	 */
	NK_Void
	(*free)(THIS);

#undef THIS
} NK_Object;


NK_CPP_EXTERN_END
#endif /* NK_OBJECT_H_*/
