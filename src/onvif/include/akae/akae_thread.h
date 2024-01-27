
/**
 * 线程控制。
 */

#include <akae_object.h>


#if !defined(AKSPC_THREAD_H_)
#define AKSPC_THREAD_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * 线程句柄。
 */
typedef AK_int AK_Thread;


/**
 * 无效线程句柄定义。
 */
#define AK_THREAD_INVALID          (-1)

/**
 * 线程命名字符串最大长度。
 */
#define AK_THREAD_MAX_NICKNAME     (16)

/**
 * 可创建线程最大数。
 */
#define AK_THREAD_MAX_BACKLOG      (32)

/**
 * 可传入最大参数。
 */
#define AK_THREAD_MAX_ARGUMENTS    (32)

/**
 * 线程默认优先级。
 */
#define AK_THREAD_DEFAULT_PRIO     (0)

/**
 * 线程最高优先级，此值以下优先级依次递减。
 */
#define AK_THREAD_HIGHEST_PRIO     (1)

/**
 * 线程最低优先级，此值以下优先级依次递增。
 */
#define AK_THREAD_LOWEST_PRIO      (99)


#define AK_THREAD_STAT_IDLE        (AK_MK4CC('i', 'd', 'l', 'e'))
#define AK_THREAD_STAT_TERMINATED  (AK_MK4CC('t', 'e', 'r', 'm'))
#define AK_THREAD_STAT_RUNNING     (AK_MK4CC('r', 'u', 'n', 0))
#define AK_THREAD_STAT_PREPARE     (AK_MK4CC('p', 'r', 'e', 'p'))


/**
 * 获取当前平台最小线程栈长度。
 */
AK_API AK_size akae_thread_min_stacklen ();
#define AK_THREAD_STACK_MIN        (akae_thread_min_stacklen ())

/**
 * 获取当前平台最大线程栈长度。
 */
AK_API AK_size akae_thread_max_stacklen ();
#define AK_THREAD_STACK_MAX        (akae_thread_max_stacklen ())

/**
 * 获取当前平台默认线程栈长度。
 */
AK_API AK_size akae_thread_default_stacklen ();
#define AK_THREAD_STACK_DEF        (akae_thread_default_stacklen ())


/**
 * 线程实现定义。\n
 *
 * @param[IN]
 *  th
 *  线程句柄，由 @ref akae_thread_create() 创建线程成功后返回。\n
 *  内部可以通过句柄调用 @ref akae_thread_suspend()、@ref akae_thread_terminated() 等接口。
 *
 * @param[IN]
 *  argc
 *  线程参数个数，由 @ref akae_thread_create() 传入。
 *
 * @param[IN]
 *  argv
 *  线程参数数组，具体有效个数见 @ref argc，由 @ref akae_thread_create() 传入。
 *
 */
typedef AK_void (*AK_ThreadFunc)(AK_Thread th, AK_int argc, AK_voidptr argv[]);


/**
 * 创建一个线程。\n
 * 线程创建完成以后，便会调用 @ref Func 线程实现，\n
 * 在 @ref Func 线程实现内部，通过调用 @ref akae_thread_terminated() 判断线程是否需要继续执行。\n
 * 如果 @ref akae_thread_terminated() 返回 AK_true，则表示该线程需要退出，\n
 * 用户在 @ref Func 内部通过条件判断返回。
 *
 * @param[IN]
 *  nickname
 *  线程昵称，用于命名启动线程，如果传入 AK_null，模块会分配一个默认命名。
 *
 * @param[IN]
 *  stacklen
 *  线程使用栈大小。
 *
 * @param[IN]
 *  priority
 *  线程优先级，最高与最低优先级定义见 @ref AK_THREAD_HIGHEST_PRIO 和 @ref AK_THREAD_LOWEST_PRIO。
 *
 * @param[IN]
 *  Func
 *  线程实现，用户根据需要实现线程具体逻辑，内部会通过回调接口调用用户实现。
 *
 * @param[IN]
 *  argc
 *  传入参数个数，最大见 @ref AK_THREAD_MAX_ARGUMENTS。
 *
 * @param[IN]
 *  argv
 *  传入参数，只有 @ref argc 数量内有效。
 *
 * @return
 *  创建线程成功返回线程句柄，失败返回 @ref AK_THREAD_INVALID。
 *
 */
AK_API AK_Thread akae_thread_create (AK_chrcptr nickname,
		AK_size stacklen, AK_size priority,
		AK_ThreadFunc Func, AK_int argc, AK_voidptr argv[]);

/**
 * 简化 @ref akae_thread_create() 参数快速调用，\n
 * 默认使用线程实现 @ref __Func 对应接口命名作为线程名称。
 */
#define akae_thread_create_default(__Func) \
	akae_thread_create (#__Func, 0, AK_THREAD_DEFAULT_PRIO, __Func, 0, AK_null)

/**
 * 简化 @ref akae_thread_create() 参数快速调用，\n
 * 默认使用线程实现 @ref __Func 对应接口命名作为线程名称
 */
#define akae_thread_create_default2(__Func, __argc, __argv) \
	akae_thread_create (#__Func, 0, AK_THREAD_DEFAULT_PRIO, __Func, __argc, __argv)



/**
 * 释放线程，\n
 * 调用该接口以后，会向线程发送一个终止信令。\n
 * 用户在线程实现内部通过调用 @ref akae_thread_terminated() 返回 AK_true。然后退出。\n
 * 对于非分离线程（@ref akae_thread_create() 调用时 detachable 为 AK_false 创建），\n
 * 该接口会等待线程实现结束才返回，\n
 * 对于分离线程，该接口调用后会立即返回，不会等待线程结束。
 *
 *
 *
 */
AK_API AK_void akae_thread_release (AK_Thread th, AK_boolean wait);

/**
 * 获取线程是否终止标识，在 @ref AK_ThreadFunc 实现内调用，\n
 * 用于判断是否需要退出当前线程实现。
 */
AK_API AK_boolean akae_thread_terminated (AK_Thread th);


/**
 * 获取线程当前运行状态。
 *
 * @retval AK_THREAD_STAT_IDLE
 *  线程被挂起进入休眠。
 * @retval AK_THREAD_STAT_RUNNING
 *  线程正在运行。
 * @retval AK_THREAD_STAT_PREPARE
 *  线程正在预备，可能刚创建完还未完全启动。
 * @retval AK_THREAD_STAT_TERMINATED
 *  线程已经终止，或有可能传入一个无效的线程句柄
 *
 */
AK_API AK_int akae_thread_status (AK_Thread th);

/**
 * 线程挂起操作，释放 CPU 资源。\n
 * 之用于在线程实现内部调用，\n
 * 当外部请求释放线程时该接口会立即返回。
 *
 * @param[IN]
 *  s
 *  挂起的秒数。
 * @param[IN]
 *  ms
 *  挂起的毫秒数。
 * @param[IN]
 *  us
 *  挂起的微秒数。
 *
 */
AK_API AK_void akae_thread_suspend (AK_Thread th, AK_size s, AK_size ms, AK_size us);



/**
 * 设置线程昵称。\n
 * 如果设置一个 AK_null 或者空的字符串，\n
 * 接口会默认设置一个默认昵称，\n
 * 昵称最大长度为 @ref AK_THREAD_MAX_NICKNAME。\n
 * 超过最大长度的昵称设置时会以最大长度限制截断。
 *
 * @param[IN]
 *  Object
 *  对象句柄。
 *
 * @param[IN]
 *  nickname
 *  设置的线程昵称，如果传入 AK_null 或者空字符串，表示把线程昵称设置成默认值。
 *
 * @retval AK_ERR_INVAL_OBJECT
 *  设置错误，传入无效的对象句柄。
 * @retval AK_OK
 *  设置成功。
 *
 */
AK_API AK_int akae_thread_set_nickname (AK_Thread th, AK_chrcptr nickname);

/**
 * 获取线程昵称，见 @ref akae_thread_set_nickname()。
 * 通过栈内存 @ref stack 返回昵称内容。
 *
 */
AK_API AK_chrptr akae_thread_get_nickname (AK_Thread th, AK_chrptr stack, AK_size stacklen);

/**
 * 遍历所有线程并依次终止。\n
 * 该接口会阻塞等待所有线程退出，建议在用户进程逻辑确认退出前调用该接口，用于确保所有线程有效退出。\n
 * 在这个期间如果调用了 @ref akae_thread_create() 等会互斥，因此在调用该接口返回前，\n
 * 应用实现需要保证不能调用其他方法。
 *
 */
AK_API AK_void akae_thread_waitall ();

AK_C_HEADER_EXTERN_C_END
#endif ///< THREAD_H_
