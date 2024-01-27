/**
 * 信号量。
 */

#include <akae_typedef.h>


#if !defined(AKAE_SEMAPHORE_H_)
#define AKAE_SEMAPHORE_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * 信号量句柄定义。
 */
#define AK_Semaphore AK_handle


/**
 * 创建信号量模块句柄。
 */
AK_API AK_Semaphore akae_sem_create (AK_size value);

/**
 * 销毁 Sem 模块句柄。
 */
AK_API AK_int akae_sem_release (AK_Semaphore handle);

/**
 * 调用 @ref akae_sem_release() 并把传入句柄置于 AK_null。
 */
#define akae_sem_release_clear(__handle) \
	({\
		AK_int const res = akae_sem_release(__handle);\
		if (AK_OK == res) {\
			__handle = AK_INVAL_HANDLE;\
		}\
		res;\
	})


/**
 * @brief
 *  超时阻塞等待信号量，与发送信号量 @ref akae_sem_post() 配对使用。\n
 *
 * @details
 *  通过一定时间内超时等待获取信号量，获取到信号量以后，\n
 *  模块内部信号量的值会减小 1。
 *
 * @param[IN] timeo_us
 *  等待超时时间（单位：微秒），小于 0 表示一直等待，等于 0 表示异步并返回，大于 0 表示等待的时间。
 *
 * @return
 *  成功返回0，否则返回-1。
 */
AK_API AK_int akae_sem_wait (AK_Semaphore handle, AK_ssize timeo_us);

/**
 * @brief
 *  非阻塞等待信号量，与发送信号量 @ref akae_sem_post() 配对使用。\n
 *
 * @details
 *  非阻塞的方式去获取一个信号量，获取成功后，\n
 *  模块内部信号量的值会减小 1，获取当前信号量的值可以通过 @ref NK_Sem::value() 方法。
 *
 * @return
 *  信号量大于0时立即返回，否则返回 -1。
 */
AK_API AK_int akae_sem_get (AK_Semaphore handle);

/**
 * @brief
 *  发送信号量。
 *
 * @details
 *  发送一个信号量，发送成功以后模块内部信号两的值会增加 1，\n
 * 获取当前信号量的值可以通过 @ref NK_Sem::value() 方法。
 *
 * @return
 *  成功返回 0，否则返回 -1，可能由于模块已经被销毁。
 */
AK_API AK_int akae_sem_post (AK_Semaphore handle);

/**
 * @brief
 *  获取当前信号量的值。
 *
 */
AK_API AK_size akae_sem_value (AK_Semaphore handle);



AK_C_HEADER_EXTERN_C_END
#endif /* AKAE_SEMAPHORE_H_ */

