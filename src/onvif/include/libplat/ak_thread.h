#ifndef _AK_THREAD_H_
#define _AK_THREAD_H_



#ifdef AK_RTOS
typedef long        ak_pthread_t;
typedef long        ak_mutex_t;
typedef long        ak_sem_t;
typedef long        ak_mutexattr_t;
typedef long        ak_cond_t;

typedef struct timespec timespec;

#define ANYKA_THREAD_NORMAL_STACK_SIZE 	(200*1024)
#define ANYKA_THREAD_MIN_STACK_SIZE 	(100*1024)

#else
#include <semaphore.h>
#include <pthread.h>

#define ANYKA_THREAD_NORMAL_STACK_SIZE 	(200*1024)
#define ANYKA_THREAD_MIN_STACK_SIZE 	(100*1024)


/* Mutex types.  */
/*
enum
{
  PTHREAD_MUTEX_TIMED_NP,
  PTHREAD_MUTEX_RECURSIVE_NP,
  PTHREAD_MUTEX_ERRORCHECK_NP,
  PTHREAD_MUTEX_ADAPTIVE_NP
#ifdef __USE_UNIX98
  ,
  PTHREAD_MUTEX_NORMAL = PTHREAD_MUTEX_TIMED_NP,
  PTHREAD_MUTEX_RECURSIVE = PTHREAD_MUTEX_RECURSIVE_NP,
  PTHREAD_MUTEX_ERRORCHECK = PTHREAD_MUTEX_ERRORCHECK_NP,
  PTHREAD_MUTEX_DEFAULT = PTHREAD_MUTEX_NORMAL
#endif
#ifdef __USE_GNU
  	// For compatibility.
  , PTHREAD_MUTEX_FAST_NP = PTHREAD_MUTEX_TIMED_NP
#endif
};
*/

#define AK_THREAD_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE

/**
typedef union
{
	char __size[__SIZEOF_SEM_T];
	long int __align;
} sem_t;
*/
typedef sem_t				ak_sem_t;

/**
typedef unsigned long int pthread_t;
*/
typedef pthread_t 			ak_pthread_t;

/**
typedef union
{
	char __size[__SIZEOF_PTHREAD_ATTR_T];
	long int __align;} pthread_attr_t;
*/
typedef pthread_attr_t		ak_pthread_attr_t;

/**
typedef union
{
	struct __pthread_mutex_s
	{
		int __lock;
		unsigned int __count;
		int __owner;    //KIND must stay at this position in the structure to maintain binary compatibility.
		int __kind;
		unsigned int __nusers;
		__extension__ union
		{
			int __spins;
			__pthread_slist_t __list;
		};
	} __data;
	char __size[__SIZEOF_PTHREAD_MUTEX_T];
	long int __align;
} pthread_mutex_t;
*/
typedef pthread_mutex_t 	ak_mutex_t;

/**
typedef union
{
	char __size[__SIZEOF_PTHREAD_MUTEXATTR_T];
	long int __align;
} pthread_mutexattr_t;
*/
typedef pthread_mutexattr_t	ak_mutexattr_t;

/**
typedef union
{
  struct
  {
    int __lock;
    unsigned int __futex;
    __extension__ unsigned long long int __total_seq;
    __extension__ unsigned long long int __wakeup_seq;
    __extension__ unsigned long long int __woken_seq;
    void *__mutex;
    unsigned int __nwaiters;
    unsigned int __broadcast_seq;
  } __data;
  char __size[__SIZEOF_PTHREAD_COND_T];
  __extension__ long long int __align;
} pthread_cond_t;
*/
typedef pthread_cond_t 		ak_cond_t;

#endif	//end of AK_RTOS

typedef void* (* thread_func)(void *param);

/**
 * ak_thread_get_version - get thread module version
 * return: version string
 */
const char* ak_thread_get_version(void);

/**
 * ak_thread_create - create a posix thread.
 * @thread_id[OUT]: thread id
 * @func[IN]: thread function
 * @arg[IN]: argument for thread
 * @stack_size[IN]: thread stack size
 * @priority[IN]: thread priority, default -1
 * return: 0 success; -1 failed
 */
int ak_thread_create(ak_pthread_t *id, thread_func func, void *arg,
		int stack_size, int priority);

/**
 * ak_thread_join - join the thread
 * @id[IN]: thread id
 * return: 0 success; otherwise error number
 */
int ak_thread_join(ak_pthread_t id);

/**
 * ak_thread_detach - detach the thread
 * @id[IN]: thread id
 * return: 0 success; otherwise error number
 */
int ak_thread_detach(ak_pthread_t id);

/**
 * ak_thread_cancel - cancel the thread
 * @id[IN]: thread id
 * return: 0 success; otherwise error number
 */
int ak_thread_cancel(ak_pthread_t id);

/**
 * ak_thread_exit - thread exit
 * return: do not return
 */
void ak_thread_exit(void);

/**
 * ak_thread_get_tid - get thread id
 * return: thread id belongs to call function
 */
long int ak_thread_get_tid(void);

/**
 * ak_thread_mutex_init - init mutex
 * @mutex[OUT]: mutex pointer
 * @attr[IN]: mutexattribute pointer
 * return: 0 success; otherwise error number
 */
int ak_thread_mutex_init(ak_mutex_t *mutex, ak_mutexattr_t *attr);

/**
 * ak_thread_mutex_lock - lock mutex
 * @mutex[IN]: mutex pointer
 * return: 0 success; otherwise error number
 */
int ak_thread_mutex_lock(ak_mutex_t *mutex);

/**
 * ak_thread_mutex_unlock - unlock mutex
 * @mutex[IN]: mutex pointer
 * return: 0 success; otherwise error number
 */
int ak_thread_mutex_unlock(ak_mutex_t *mutex);

/**
 * ak_thread_mutex_destroy - destroy mutex
 * @mutex[IN]: mutex pointer
 * return: 0 success; otherwise error number
 */
int ak_thread_mutex_destroy(ak_mutex_t *mutex);

/**
 * ak_thread_sem_init - init semaphore
 * @sem[IN]: semaphore pointer
 * @value[IN]: semaphore initial value
 * return: 0 success; -1 failed
 */
int ak_thread_sem_init(ak_sem_t *sem, unsigned int value);

/**
 * ak_thread_sem_post - post semaphore
 * @sem[IN]: semaphore pointer
 * return: 0 success; -1 failed
 */
int ak_thread_sem_post(ak_sem_t *sem);

/**
 * ak_thread_sem_wait - wait semaphore
 * @sem[IN]: semaphore pointer
 * return: 0 success; -1 failed
 */
int ak_thread_sem_wait(ak_sem_t *sem);

/**
 * ak_thread_sem_destroy - destroy semaphore
 * @sem[IN]: semaphore pointer
 * return: 0 success; -1 failed
 */
int ak_thread_sem_destroy(ak_sem_t *sem);

/**
 * ak_thread_sem_trywait - try to wait semaphore
 * @sem[IN]: semaphore pointer
 * return: 0 success; -1 failed
 */
int ak_thread_sem_trywait(ak_sem_t *sem);

/**
 * ak_thread_sem_timedwait - wait semaphore timeout
 * @sem[IN]: semaphore pointer
 * @abs_timeout[IN]: specific timeout
 * return: 0 success; -1 failed
 */
int ak_thread_sem_timedwait(ak_sem_t *sem, const struct timespec *abs_timeout);

/**
 * ak_thread_set_name - set thread name
 * @name[IN]: pointer to 'thread name' no longer than 15B
 * return: 0 success; -1 failed
 */
int ak_thread_set_name(const char *name);

/**
 * ak_thread_cond_init - init condition
 * @cond[OUT]: condition pointer
 * return: 0 success; otherwise error number
 */
int ak_thread_cond_init(ak_cond_t *cond);

/**
 * ak_thread_cond_destroy - destroy condition
 * @cond[IN]: condition pointer
 * return: 0 success; otherwise error number
 */
int ak_thread_cond_destroy(ak_cond_t *cond);

/**
 * ak_thread_cond_wait - wait condition
 * @cond[IN]: condition pointer
 * @mutex[IN]: mutext pointer
 * return: 0 success; otherwise error number
 */
int ak_thread_cond_wait(ak_cond_t *cond, ak_mutex_t *mutex);

/**
 * ak_thread_cond_timedwait - wait condition timeout
 * @cond[IN]: condition pointer
 * @mutex[IN]: mutext pointer
 * @abstime[IN]: timeout value
 * return: 0 success; otherwise error number
 */
int ak_thread_cond_timedwait(ak_cond_t *cond,
		ak_mutex_t *mutex, const struct timespec *abstime);

/**
 * ak_thread_cond_signal - post signal on condition
 * @cond[IN]: condition pointer
 * return: 0 success; otherwise error number
 */
int ak_thread_cond_signal(ak_cond_t *cond);

/**
 * ak_thread_cond_broadcast - post signal by broadcast on condition
 * @cond[IN]: condition pointer
 * return: 0 success; otherwise error number
 */
int ak_thread_cond_broadcast(ak_cond_t *cond);

/**
 * ak_thread_mutexattr_init - init mutexattribute
 * @attr[IN]: mutexattribute pointer
 * return: 0 success; otherwise error number
 */
int ak_thread_mutexattr_init(ak_mutexattr_t *attr);

/**
 * ak_thread_mutexattr_destroy - destroy mutexattribute
 * @attr[IN]: mutexattribute pointer
 * return: 0 success; otherwise error number
 */
int ak_thread_mutexattr_destroy(ak_mutexattr_t *attr);

/**
 * ak_thread_mutexattr_settype - set mutexattribute type
 * @attr[IN]: mutexattribute pointer
 * @kind[IN]: mutexattribute type
 * return: 0 success; otherwise error number
 */
int ak_thread_mutexattr_settype(ak_mutexattr_t *attr, int kind);

/**
 * ak_thread_mutexattr_gettype - get mutexattribute type
 * @attr[IN]: mutexattribute pointer
 * @kind[OUT]: mutexattribute type
 * return: 0 success; otherwise error number
 */
int ak_thread_mutexattr_gettype(const ak_mutexattr_t *attr, int *kind);
#endif	//end of _AK_THREAD_H_
