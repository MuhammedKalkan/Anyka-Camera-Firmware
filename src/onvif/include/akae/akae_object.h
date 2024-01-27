
/**
 * 对象定义模块。
 * 该模块定义基本对象类型，用于对象继承、多态实现。
 */

#include <akae_stdlib.h>

#if !defined(AKSPC_OBJECT_H_)
#define AKSPC_OBJECT_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * 对象结构。
 * 对象初始化接口对应 create 以及 release 方法。
 */
typedef struct  _AK_Object {
#define AK_This struct _AK_Object *const

	/// 对象名称。
	AK_chrcptr   (*name) (AK_This, AK_chrptr stack, AK_size stacklen);

	/// 对象打印输出。
	AK_void     (*dump) (AK_This);

	/// 对象线程安全标识。
	AK_boolean  (*threadsafe) (AK_This);

	/// 释放对象实现。
	AK_void     (*release) (AK_This);

#undef AK_This
} *AK_Object;



/**
 * 静态对象结构。
 * 静态对象初始化接口对应 init 以及 destroy 方法。
 */
typedef struct  _AK_StaticObject {

	/// 用户不可见。
	AK_byte _opacity[sizeof(struct _AK_Object) + 256];

} AK_StaticObject;


AK_C_HEADER_EXTERN_C_END
#endif ///< OBJECT_H_
