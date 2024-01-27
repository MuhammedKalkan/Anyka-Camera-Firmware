
/**
 * 线程互斥锁。
 */

#include <akae_object.h>


#if !defined(AKSPC_RWLOCK_H_)
#define AKSPC_RWLOCK_H_
AK_C_HEADER_EXTERN_C_BEGIN

/**
 * 读写锁句柄。
 */
#define AK_Rwlock AK_handle


AK_API AK_Rwlock akae_rwlock_create ();

AK_API AK_int akae_rwlock_release (AK_Rwlock handle);

/**
 * 调用 @ref akae_rwlock_release() 并把句柄无效化。
 */
#define akae_rwlock_release_clear(__handle) \
	({\
		AK_int res = akae_rwlock_release (__handle);\
		if (AK_OK == res) {\
			(__handle) = AK_INVAL_HANDLE;\
		}\
		res;\
	})

AK_API AK_int akae_rwlock_writelock (AK_Rwlock handle);

AK_API AK_int akae_rwlock_try_writelock (AK_Rwlock handle);

AK_API AK_int akae_rwlock_timed_writelock (AK_Rwlock handle, AK_ssize ms);

AK_API AK_int akae_rwlock_readlock (AK_Rwlock handle);

AK_API AK_int akae_rwlock_try_readlock (AK_Rwlock handle);

AK_API AK_int akae_rwlock_timed_readlock (AK_Rwlock handle, AK_ssize ms);

AK_API AK_int akae_rwlock_unlock (AK_Rwlock handle);

/**
 * 进入读锁处理。
 */
#define AK_RWLOCK_RLOCK_PROC(__handle, __proc_program...) \
	do {\
		if (AK_VALID_HANDLE (__handle) ? (AK_OK == akae_rwlock_readlock(__handle)) : AK_true) {\
			__proc_program;\
			if (AK_VALID_HANDLE (__handle)) {\
				akae_rwlock_unlock (__handle);\
			}\
		} else {\
			akae_log_alert ("Read Lock @%s() Error.", __func__);\
		}\
	} while (0)

/**
 * 进入写锁处理。
 */
#define AK_RWLOCK_WLOCK_PROC(__handle, __proc_program...) \
	do {\
		if (AK_VALID_HANDLE (__handle) ? (AK_OK == akae_rwlock_writelock(__handle)) : AK_true) {\
			__proc_program;\
			if (AK_VALID_HANDLE (__handle)) {\
				akae_rwlock_unlock (__handle);\
			}\
		} else {\
			akae_log_alert ("Write Lock @%s() Error.", __func__);\
		}\
	} while (0)


AK_C_HEADER_EXTERN_C_END
#endif ///< RWLOCK_H_
