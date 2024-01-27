/**
 * 线程互斥锁对象接口定义。
 */

#include <akae_typedef.h>


#if !defined(AKAE_MUTEX_H_)
#define AKAE_MUTEX_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * 互斥锁对象定义。
 */
#define AK_Mutex AK_handle


/**
 * 动态申请一个数据对象。
 */
AK_API AK_Mutex akae_mutex_create (AK_NOARGS);

/**
 * 通过 @ref recursive 标识是否启用嵌套锁。
 */
AK_API AK_Mutex akae_mutex_create2 (AK_boolean recursive);

/**
 * 释放动态申请对象，对应 @ref akae_mutex_create() 或者 @ref akae_mutex_create2() 方法使用。
 */
AK_API AK_int akae_mutex_release (AK_Mutex handle);

/**
 * 释放同时置空句柄。
 */
#define akae_mutex_release_clear(__handle) \
	({\
		AK_int const res = akae_mutex_release (__handle);\
		if (res == AK_OK) {\
			__handle = AK_INVAL_HANDLE;\
		}\
		res;\
	})


/**
 * 同步获取锁。\n
 * 如果分别在两个线程调用该接口，先调用的会获得锁，\n
 * 后调用者需要等待，先调用者解锁，从而实现互斥操作。\n
 * 如果在同一个线程中连续两次调用，\n
 * 接口会返回死锁 @ref AK_ERR_DEAD_LOCK 错误。
 *
 */
AK_API AK_int akae_mutex_lock (AK_Mutex handle);

/**
 * 异步获取锁。\n
 * 如果锁未被获得过，调用该接口可以获得锁，与 @ref akae_mutex_lock() 相同。\n
 * 当锁已经被其他线程获得了。使用该接口上锁会返回一个 @ref AK_ERR_BUSY 错误。
 *
 */
AK_API AK_int akae_mutex_try_lock (AK_Mutex handle);


/**
 * 超时内尝试获取锁。
 */
AK_API AK_int akae_mutex_timed_lock (AK_Mutex handle, AK_ssize us);


/**
 * 释放锁。\n
 * 与 @ref akae_mutex_lock() 或 @ref akae_mutex_try_lock() 配对使用。
 */
AK_API AK_int akae_mutex_unlock (AK_Mutex handle);




/**
 * 进入互斥锁处理快速调用。
 */
#define AK_MUTEX_LOCK_PROC(__handle, __proc_exp...) \
	do {\
		if (0 == (__handle)) { /* 不需要进入互斥锁临界。*/\
			__proc_exp;\
		} else if (AK_OK == akae_mutex_lock(__handle)) {/* 进入互斥锁临界后操作。*/\
			__proc_exp;\
			akae_mutex_unlock (__handle);\
		} else {\
			akae_log_error ("Mutex Lock @%s() Error.", __func__);/* 进入互斥锁临界异常需要反馈输出。*/\
		}\
	} while (0)



/**
 * 全局临界区互斥进出标识。
 */
AK_API AK_void akae_mutex_global_critical (AK_boolean enter);

/**
 * 
 * 进入临界区。
 *
 * @post akae_mutex_leave_critical()
 */
#define akae_mutex_enter_critical() \
	do {\
		/* akae_log_debug2 ("Enter Critical."); AK_CHECK_POINT (); */\
		akae_mutex_global_critical(AK_true);\
	} while (0)

/**
 * @pre akae_mutex_enter_critical()
 * 离开临界区，在临界区操作实现完毕以后调用。
 */
#define akae_mutex_leave_critical() \
	do {\
		akae_mutex_global_critical(AK_false);\
		/* akae_log_debug2 ("Leave Critical."); AK_CHECK_POINT (); */\
	} while (0)


/**
 * 进入临界区处理快速调用。
 */
#define AK_CRITICAL_PROC(__proc_exp...) \
	do {\
		akae_mutex_enter_critical (); /* 进入临界区。 */\
		do {\
			__proc_exp;\
		} while (0);\
		akae_mutex_leave_critical (); /* 离开临界区。 */ \
	} while (0)




AK_C_HEADER_EXTERN_C_END
#endif ///< MUTEX_H_
