
/**
 * 链表应用实现。
 */

#include <akae_object.h>


#if !defined(AK_STACK_H_)
#define AK_STACK_H_
AK_C_HEADER_EXTERN_C_BEGIN

/**
 * 创建一个栈结构对象。
 */
AK_API AK_Object akae_stack_create (AK_Object Malloc, AK_boolean threadsafe);
/**
 * 释放动态申请对象，对应 @ref akae_stack_create() 方法使用。
 */
AK_API AK_int akae_stack_release (AK_Object Object);

/**
 * 调用 @ref akae_stack_release() 并把传入句柄置于 AK_null。
 */
#define akae_stack_release_clear(__Object) \
	({\
		AK_int const res = akae_stack_release(__Object);\
		if (AK_OK == res) {\
			__Object = AK_null;\
		}\
		res;\
	})

/**
 * 按照栈结构在顶部存入一个数据。\n
 * 注意的是，传入的数据指针 @ref data 所指向的内存由上层用户管理，模块内部不会对其进行内存管理（包括回收、销毁等）。\n
 * 模块单纯作为一个容器维护一系列内存指针。
 *
 *
 * @return
 *  操作成功返回 AK_OK，否则返回失败。
 */
AK_API AK_int akae_stack_push (AK_Object Object, AK_voidptr data);


/**
 * 按照栈结构在顶部取出一个数据，（FILO）先进后出操作。\n
 *
 * @verbatim
 *
 *   akae_stack_push()
 *   +
 *   |     +-+-+-+-+-+-+-+-+-+-+
 *   +-----> N,N-1,...3,2,1,0  |
 *   +-----+                   |
 *   |     +-+-+-+-+-+-+-+-+-+-+
 *   v
 *   akae_stack_pop()
 *
 *
 * @endverbatim
 *
 * @return
 *  操作成功返回 AK_OK，否则返回失败。
 */
AK_API AK_int akae_stack_pop (AK_Object Object, AK_voidptr *data);


/**
 * 按照栈结构在底部取出一个数据，（FIFO）先进先出操作。\n
 *
 *
 * @verbatim
 *
 *   akae_stack_push()
 *   +
 *   |     +-+-+-+-+-+-+-+-+-+-+
 *   +-----> N,N-1,...3,2,1,0  |
 *         |                   +--+
 *         +-+-+-+-+-+-+-+-+-+-+  |
 *                                v
 *                       akae_stack_shift()
 *
 * @endverbatim
 *
 * @retval AK_ERR_NOT_EXIST
 *  操作失败，队列为空，不能有效获取数据。
 */
AK_API AK_int akae_stack_shift (AK_Object Object, AK_voidptr *data);

/**
 * 查看队列的数据内容，查看以后，不会对队列结构产生改变。\n
 * 区别与 @ref akae_stack_pop() 与 @ref akae_stack_shift() 接口。
 *
 */
AK_API AK_int akae_stack_peek (AK_Object Object, AK_int id, AK_voidptr *data);

/**
 * 获取栈结构的深度（大小）。
 *
 * @return
 *  操作成功返回栈结构的深度（个数），失败返回 0。\n
 *  注意，该接口返回 0 有二义性，表示栈是空，或者传入非法句柄。
 */
AK_API AK_size akae_stack_depth (AK_Object Object);




AK_C_HEADER_EXTERN_C_END
#endif ///< AK_STACK_H_
