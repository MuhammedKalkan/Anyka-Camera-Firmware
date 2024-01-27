/**
 * NkUtils 内存分配器。
 * 通过此模块可以处理动态内存分配以及管理。
 * 此模块不支持线程安全。
 *
 */

#include "object.h"

#ifndef NK_ALLOCATOR_H_
#define NK_ALLOCATOR_H_
NK_CPP_EXTERN_BEGIN

/**
 * @brief
 * 内存分配器模块句柄。
 *
 * 通过内存分配器，可以实现对一块相对较大的内存进行动态分配。
 *
 */
typedef struct Nk_Allocator
{
#define THIS struct Nk_Allocator *const Allocator

	/**
	 * 模块基础接口。
	 */
	NK_Object Object;

	/**
	 * 申请内存块。
	 *
	 * @param[in]		size		申请内存块的大小。
	 *
	 * @return	成功时返回申请到的内存块的起始地址，否者返回 NK_Nil。
	 *
	 */
	NK_PVoid
	(*alloc)(THIS, NK_Size size);


	/**
	 * 重新分配内存.\n
	 * 资源满足的情况下会在原有内存上
	 *
	 * @param[in]		ptr			上一次分配的内存位置。
	 * @param[in]		size		重新申请的内存大小。
	 *
	 * @return	成功时返回重新申请的内存起始地址，否则返回 NK_Nil，失败后原来申请的内存仍然可以接续使用。
	 */
	NK_PVoid
	(*realloc)(THIS, NK_PVoid ptr, NK_Size size);

	/**
	 * 申请一块内存空间并复制字符串。\n
	 * 模块会根据传入字符串的长度通过 @ref NK_Allocator::alloc 方法申请合适的内存空间，\n
	 * 然后把字符串数据拷贝到内存空间中，\n
	 * 克隆后得到的内存空间必须通过 @ref free方法回收。
	 *
	 * @param[in]		str			复制的字符串。
	 *
	 * @return	申请成功时返回申请的内存块起始地址，失败返回 NK_Nil。
	 *
	 */
	NK_PChar
	(*strdup)(THIS, NK_PChar str);

	/**
	 * 与 @ref strdup 相似，最大复制 n 个字符。
	 *
	 * @param[in]		str			复制的字符串。
	 * @param[in]		n			复制字节的最大长度。
	 *
	 * @return	申请成功时返回申请的内存块起始地址，失败返回 NK_Nil。
	 */
	NK_PChar
	(*strndup)(THIS, NK_PChar str, NK_Size n);

	/**
	 * 复制内存块。\n
	 * 复制以后，会产生一块新的内存块，内用与原内存块相同。\n
	 * 产生的内存块须要通过 @ref free 方法回收。\n
	 * 调用此接口时必须保证 @ref mem 所在内存块可用长度必须大于等于 @ref size，否则会产生潜在的错误。
	 *
	 * @param[in]		mem			复制的内存块其实空间。
	 * @param[in]		size		复制字节的最大长度。
	 *
	 * @return	申请成功时返回申请的内存块起始地址，失败返回 NK_Nil。
	 */
	NK_PVoid
	(*memdup)(THIS, NK_PVoid mem, NK_Size size);


	/**
	 * 回收内存空间。
	 *
	 * @param[in]		ptr			由 @ref alloc 或者 @ref realloc 分配的内存块地址。
	 *
	 * @return	无。
	 *
	 */
	NK_Void
	(*freep)(THIS, NK_PVoid ptr);

	/**
	 * 获取所分配的内存块长度。
	 *
	 * @param[in]		ptr			由 @ref alloc 或者 @ref realloc 分配的内存块地址。
	 *
	 * @return
	 * 如果使用 OS 内存分配器，此接口一直返回 0。\n
	 * 如果使用用户自定义内存分配器，\n
	 * 此接口调用成功时会返回所分配的内存块长度，内存块地址非法时返回 -1。
	 *
	 */
	NK_SSize
	(*length)(THIS, NK_PVoid ptr);

	/**
	 * 返回内存分配器内部缓冲总容量。
	 *
	 * @return
	 * 当使用用户指派的内存缓冲时，\n
	 * 此接口获取内存分配器内部缓冲总量。\n
	 * 当使用 OS 的内存分配器时，此接口总是返回 -1。
	 *
	 */
	NK_SSize
	(*capacity)(THIS);

	/**
	 * 返回内存分配器内部缓冲已用量。
	 *
	 * @return
	 * 当使用用户指派的内存缓冲时，\n
	 * 此接口获取内存分配器内部缓冲总量。\n
	 * 当使用 OS 的内存分配器时，此接口总是返回 -1。
	 */
	NK_SSize
	(*used)(THIS);

#undef THIS
} NK_Allocator;

/**
 * 创建模块句柄。\n
 * 传入一块固定大小的内存区域，由内存分配器接管。\n
 * 内存分配器接管后，用户可以根据须要在内存区域内动态分配内存块。\n
 * 由于算法调度开销，传入内存块大小不能小于 1K 字节，否则会创建失败，\n
 * 建议使用时内存区域大于 4K 字节。
 *
 * @param[in]		mem			用于分配的内存区域起始位置。
 * @param[in]		len			用于分配的内存区域大小，必须大于 1K。
 *
 * @return		创建成功返回模块句柄，失败返回 Nil。
 */
extern NK_Allocator *
NK_Alloc_Create(NK_PByte mem, NK_Size len);

/**
 * 销毁模块句柄。\n
 *
 * @param[in]		Allocator_r	模块句柄引用。
 * @param[out]		mem_r		释放成功时，从此变量获取创建时传入的内存区域起始位置。
 *
 * @return		返回创建时传入的内存块长度，@ref mem_r 获取创建时传入的内存区域起始位置。
 */
extern NK_SSize
NK_Alloc_Free(NK_Allocator **Allocator_r, NK_PByte *mem_r);


NK_CPP_EXTERN_END
#endif /* NK_ALLOCATOR_H_ */

