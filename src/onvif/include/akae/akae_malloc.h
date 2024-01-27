
/**
 * 内存分配器。\n
 * 用户通过指定一块内存交由内存分配器接管，内存分配器内部通过对内存进行有效管理，\n
 * 由用户多次动态分配使用。\n
 * 注意：内存由分配器接管以后，禁止对内存进行外部读写操作，避免破坏管理异常。
 */

#include <akae_object.h>

#if !defined(AKAE_MALLOC_H_)
#define AKAE_MALLOC_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * 最小用户传入接管内存长度，使用内存分配其至少提供定义的内存给内存分配器接管。
 */
#define AK_MALLOC_MIN_GIVEN_MEM_LEN (1024)

/**
 * 内存分配器运行状态。
 */
typedef struct _AK_MallocatorStatus {

	/// 空闲的内存块数量。
	AK_size free_blocks;

	/// 已释放的内存块数量。
	AK_size allocated_blocks;

	/// 总容量。
	AK_size capacity;

	/// 使用量。
	AK_size usage;

	/// 用量峰值。
	AK_size usage_peak;

} AK_MallocatorStatus;


/**
 * 创建一个内存分配器。
 *
 * @param[IN]
 *  threadsafe
 *  线程安全设定，可以选择内存分配器是否启用线程安全，\n
 *  启用线程安全以后，对性能有一定的开销，如果分配器不使用在线程安全环境，可以不需要启用。
 *
 * @param[IN]
 *  givenMemAddr
 *  用户传入内存的起始地址，该内存由内存分配器接管。
 *
 * @param[IN]
 *  givenMemLen
 *  用户传入内存的长度，该值需要大于 @ref AK_MALLOC_MIN_GIVEN_MEM_LEN，\n
 *  内存分配器才能创建成功，否则会创建失败。
 *
 * @return
 *  创建成功返回内存分配器对象句柄，创建失败返回 AK_null。
 *
 */
AK_API AK_Object akae_malloc_create (AK_boolean threadsafe, AK_bytptr givenMemAddr, AK_size givenMemLen);

/**
 * 销毁一个内存分配器。\n
 *
 * @param[IN]
 *  Object
 *  分配器对象句柄，由 @ref akae_malloc_create() 创建。
 *  释放成功以后，该句柄指针将不再使用。
 *
 * @param[OUT]
 *  givenMemAddr
 *  用户传入内存的起始地址，在 @ref akae_malloc_create() 创建对象时传入，\n
 *  在释放时返还用户，传入 AK_null 则用户不需要获取该值。
 *
 * @param[OUT]
 *  givenMemLen
 *  用户传入内存的长度，在 @ref akae_malloc_create() 创建对象时传入，\n
 *  在释放时返还用户，传入 AK_null 则用户不需要获取该值。
 *
 * @retval AK_OK
 *  释放成功。
 * @retval AK_ERR_INVAL_OBJECT
 *  释放失败，传入无效的对象。
 * @retval AK_ERR_INTERNAL
 *  释放失败，出现一个系统层的异常。
 *
 */
AK_API AK_int akae_malloc_release (AK_Object Object, AK_bytptr *givenMemAddr, AK_size *givenMemSize);

/**
 * 由内存分配器分配一块内存。
 *
 * @param[IN]
 *  Object
 *  分配器对象句柄，由 @ref akae_malloc_create() 创建。
 *
 * @param[IN]
 *  len
 *  分配内存的长度，必须大于 0，否则返回分配失败。
 *
 * @return
 *  分配成功返回内存起始地址，分配失败返回 AK_null。
 */
AK_API AK_voidptr akae_malloc_traced_alloc (AK_Object Object, AK_size size, AK_chrcptr filename, AK_uint32 lineno);

/**
 * 带跟踪信息快速调用.
 */
#define akae_malloc_alloc(__Object, __size) akae_malloc_traced_alloc(__Object, __size, akae_basename (__FILE__), __LINE__)


/**
 * 带跟踪信息快速调用, 并且分配后把内存初始化为 0.
 */
#define akae_malloc_alloc_zero(__Object, __size) \
	akae_memzero (akae_malloc_traced_alloc(__Object, __size, akae_basename (__FILE__), __LINE__), (__size))





/**
 * 改变已分配的内存大小。\n
 * 用户传入已分配的内存地址 @ref preptr 以及重新分配的长度 @ref newlen，\n
 * 分配器会重新分配一块内存大小 @ref newlen 的内存给用户。\n
 * 注意：\n
 * 如果分配失败，@ref preptr 内存会继续保留，\n
 * 如果分配成功，会返回新的内存起始地址，@ref preptr 所在的内存会被分配器回收，\n
 * 用户不能继续使用。\n
 * 如果 @ref newlen 传入 0，则表示释放 @ref preptr 所在内存，与 @ref akae_malloc_free() 行为一致。
 * 释放以后，@ref preptr 同样无法继续使用。
 *
 */
AK_API AK_voidptr akae_malloc_realloc (AK_Object Object, AK_voidptr preptr, AK_size newlen, AK_chrcptr filename, AK_uint32 lineno);

/**
 * 带跟踪信息快速调用.
 */
#define akae_malloc_realloc(__Object, __preptr, __size) akae_malloc_realloc(__Object, __preptr, __size, akae_basename (__FILE__), __LINE__)


/**
 * 由内存分配器分配一块内存，并把字符串数据复制过去。
 * 内部会调用 @ref akae_malloc_alloc()。
 *
 */
AK_API AK_chrptr akae_malloc_traced_strdup (AK_Object Object, AK_chrcptr str, AK_chrcptr filename, AK_uint32 lineno);

/**
 * 带跟踪信息快速调用.
 */
#define akae_malloc_strdup(__Object, __str) akae_malloc_traced_strdup(__Object, __str, akae_basename (__FILE__), __LINE__)


/**
 * 复制一个指定长度的字符串，见 @ref akae_malloc_strdup()。
 *
 */
AK_API AK_chrptr akae_malloc_traced_strndup (AK_Object Object, AK_chrcptr str, AK_size n, AK_chrcptr filename, AK_uint32 lineno);

/**
 * 带跟踪信息快速调用.
 */
#define akae_malloc_strndup(__Object, __str, __n) akae_malloc_traced_strndup(__Object, __str, __n, akae_basename (__FILE__), __LINE__)


/**
 * 由内存分配器分配一块内存，并把一块内存数据复制过去。
 * 内部会调用 @ref akae_malloc_alloc()。
 *
 */
AK_API AK_voidptr akae_malloc_traced_memdup (AK_Object Object, AK_voidptr mem, AK_size len, AK_chrcptr filename, AK_uint32 lineno);

/**
 * 带跟踪信息快速调用.
 */
#define akae_malloc_memdup(__Object, __mem, __len) akae_malloc_traced_memdup(__Object, __mem, __len, akae_basename (__FILE__), __LINE__)



/**
 * 释放 @ref ptr 所指向的内存，\n
 * 该内存地址对应的内存区域必须由 @ref Object 对象分配，否则释放失败。\n
 * 如果 @ref ptr 传入 AK_null，该接口返回成功。\n
 * 建议调用该接口以后，操作 @ref ptr = AK_null，有效避免内存重复释放。
 *
 * @param[IN]
 *  Object
 *  内存分配器对象句柄。
 *
 * @param[IN]
 *  ptr
 *  目标释放的内存起始地址。
 *
 * @retval AK_OK
 *  成功释放 @ref ptr 对应内存。
 * @retval AK_ERR_INVAL_OBJECT
 *  释放失败，用户传入了非法的对象句柄。
 * @retval AK_ERR_INVAL_PARAM
 *  释放失败，用户传入了非法的内存指针，有可能该内存区域并非由 @ref Object 所分配。
 * @retval AK_ERR_NOT_PERM
 *  释放失败，有可能用户重复释放了该内存两次。
 */
AK_API AK_int akae_malloc_free (AK_Object Object, AK_voidptr ptr);

/**
 * 调用 @ref akae_malloc_free() 并把传入指针置空，
 */
#define akae_malloc_free_clear(__Object, __ptr) \
	({\
		AK_int res = akae_malloc_free (__Object, __ptr);\
		if (AK_OK == res) {\
			(__ptr) = AK_null;\
		}\
		res;\
	})


/**
 * 功能类似 @ref akae_malloc_free ()，但不做内存整理，\n
 * 用户可以根据实际情况，当使用连续的内存释放时，\n
 * 先连续调用 @ref akae_malloc_free_lazy () 接口后，\n
 * 最后调用 @ref akae_malloc_free ()，\n
 * 换来一定的性能节省。
 *
 */
AK_API AK_int akae_malloc_free_lazy (AK_Object Object, AK_voidptr ptr);


/**
 * 存储空间碎片整理。\n
 * 与 @ref akae_malloc_free_lazy() 方法搭配使用。
 * 用户调用 @ref akae_malloc_free() 接口时每次均会对内存管理空间做碎片化整理。\n
 * 这个情况下，如果对于用户频繁调用 @ref akae_malloc_free() 一个操作过程中释放太多数据块会形象性能，\n
 * 这部分性能主要消耗在频繁碎片整理的冗余操作上。\n
 * 针对这种连续多次频繁的释放数据的上层应用场合，可以通过 @ref akae_malloc_free_lazy() 调用有效降低，\n
 * 在多次调用 @ref akae_malloc_free_lazy() 以后会产生大量的内存碎片，\n
 * 这个时候可以通过手动调用该接口进行碎片整理。
 *
 */
AK_API AK_int akae_malloc_defragment (AK_Object Object);


/**
 * 获取分配内存的长度（单位：字节）。\n
 * 由于对齐优化逻辑作用，真实申请的内存长度有可能稍大于申请的值。
 *
 * @return
 *  获取成功返回已分配的内存的长度，获取失败返回 0。
 */
AK_API AK_size akae_malloc_memlen (AK_Object Object, AK_voidptr ptr);


/**
 * 获取分配器可分配的容量（单位：字节）。
 */
AK_API AK_size akae_malloc_capacity (AK_Object Object);


/**
 * 获取分配器的分配使用量（单位：字节）。
 */
AK_API AK_size akae_malloc_usage (AK_Object Object);

/**
 * 获取分配器的分配峰值（单位：字节），通过该接口可以大致评估该分配器是否将会出现过载。
 */
AK_API AK_size akae_malloc_usage_peak (AK_Object Object);

/**
 * 获取内存分配器运行状态。
 */
AK_API AK_int akae_malloc_status (AK_Object Object, AK_MallocatorStatus *Status);


/**
 * 基于分配器所分配的内存，安全内存拷贝操作。\n
 * 内部会调用 @ref akae_malloc_memlen() 先获取分配的内存长度。\n
 * 然后通过长度限制实现安全拷贝，避免内存越界。
 */
AK_API AK_voidptr akae_malloc_safe_memcpy (AK_Object Object, AK_voidptr dest, AK_voidptr src, AK_size n);

/**
 * 基于分配器所分配的内存，安全字符串拷贝操作。\n
 * 参考 @ref akae_malloc_safe_memcpy()。
 */
AK_API AK_chrptr akae_malloc_safe_strcpy (AK_Object Object, AK_chrptr dest, AK_chrptr src);


AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_MALLOC_H_
