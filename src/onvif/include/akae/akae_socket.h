
/**
 * 套接字。
 */

#include <akae_object.h>


#if !defined(AKSPC_SOCKET_H_)
#define AKSPC_SOCKET_H_
AK_C_HEADER_EXTERN_C_BEGIN



/**
 * 套接字可用句柄数。
 */
#define AK_SOCK_MAX_BACKLOG  (256)

/**
 * TCP 状态定义。
 */
#define AK_SOCK_TCP_STAT_ESTABLISHED   AK_MK2CC('e', 's')
#define AK_SOCK_TCP_STAT_SYN_SENT      AK_MK2CC('s', 's')
#define AK_SOCK_TCP_STAT_SYN_RECV      AK_MK2CC('s', 'r')
#define AK_SOCK_TCP_STAT_FIN_WAIT1     AK_MK2CC('f', '1')
#define AK_SOCK_TCP_STAT_FIN_WAIT2     AK_MK2CC('f', '2')
#define AK_SOCK_TCP_STAT_TIME_WAIT     AK_MK2CC('t', 'w')
#define AK_SOCK_TCP_STAT_CLOSE         AK_MK2CC('c', 'l')
#define AK_SOCK_TCP_STAT_CLOSE_WAIT    AK_MK2CC('c', 'w')
#define AK_SOCK_TCP_STAT_LAST_ACK      AK_MK2CC('l', 'a')
#define AK_SOCK_TCP_STAT_LISTEN        AK_MK2CC('l', 't')
#define AK_SOCK_TCP_STAT_CLOSING       AK_MK2CC('c', 'n')
#define AK_SOCK_TCP_STAT_UNKNOWN       (-1)


/**
 * 套接字句柄。
 */
#define AK_Socket AK_handle


/**
 * 初始化套接字。\n
 * 初始化需要传入一个静态对象内存数据。\n
 * 初始化成功会返回一个 @ref AK_Object 对象，失败返回 AK_null。
 *
 * @param[OUT]
 *  Object
 *  栈区中对象内存缓冲，用于初始化使用。
 *
 * @param[IN]
 *  更具用户需要，可选协议字段包括 tcp、udp、icmp。
 *
 * @return
 *  初始化成功返回套接字句柄，否则返回 AK_null。
 *
 */
AK_API AK_Socket akae_socket_create (AK_chrcptr protocol);

/**
 * 创建 TCP 套接字快速调用。
 */
#define akae_socket_create_tcp() akae_socket_create("tcp")

/**
 * 创建 UDP 套接字快速调用。
 */
#define akae_socket_create_udp() akae_socket_create("udp")

/**
 * 创建 ICMP 套接字快速调用。
 */
#define akae_socket_create_icmp() akae_socket_create("icmp")


/**
 * 销毁套接字。\n
 * 与 @ref akae_socket_create() 配对使用。\n
 * 内部会调用 @ref akae_socket_close() 关闭套接字。\n
 * 用户可以直接调用当前接口同时关闭套接字并销毁句柄，\n
 * 也可以先手动调用 @ref akae_socket_close() 关闭套接字，再调用当前接口销毁句柄。
 *
 * @param[IN]
 *  Object
 *  套接字句柄。
 *
 * @retval AK_ERR_INVAL_OBJECT
 *  销毁失败，套接字句柄无效。
 * @retval AK_OK
 *  销毁成功。
 */
AK_API AK_int akae_socket_release (AK_Socket handle);
#define akae_socket_release_clear(__h) \
	({\
		AK_int const res = akae_socket_release (__h);\
		if (AK_OK == res) {\
			__h = AK_INVAL_HANDLE;\
		}\
		res;\
	})


/**
 * 在一定时间内，判断套接字是否可读，如果对端有数据传到本地，\n
 * 套子字有数据可读则返回 AK_true，否则返回 AK_false。\n
 * 可以通过传入 @ref wait_us 参数限定判断时间，如果传入 AK_null 则表示持续判断，除非套接字被关闭。\n
 * 传入数值为 0 的数据指针，则表示立即返回。\n
 * 传入指定的微秒数（us）控制等待的时间。\n
 *
 * @param[IN]
 *  Object
 *  套接字对象句柄。
 *
 * @param[IN]
 *  wait_us
 *  等待的时间，单位微秒。\n
 *  传入 -1 表示一直等待，传入 0 表示判断后不等待并立即返回，\n
 *  传入大于 0 的数值表示指定等待对应的微秒数。
 *
 * @return
 *  可读返回 AK_true，否则返回 AK_false。
 */
AK_API AK_boolean akae_socket_can_read (AK_Socket handle, AK_int wait_us);

/**
 * 判断套接字是否可写，见 @ref akae_socket_can_read()。
 */
AK_API AK_boolean akae_socket_can_write (AK_Socket handle, AK_int wait_us);

/**
 * 设置本地地址与端口。\n
 * 该接口在句柄生存期内只能调用一次，设置了本地地址与端口以后不能变更。
 *
 * @param[IN]
 *  handle
 *  套接字句柄。
 *
 * @param[IN]
 *  addr
 *  设置本地地址，设置成任意地址可通过传入 AK_null。
 *
 * @param[IN]
 *  port
 *  设置本地端口。
 *
 * @retval AK_OK
 *  设置成功。
 * @retval AK_ERR_INVAL_HANDLE
 *  设置失败，传入无效句柄。
 * @retval AK_ERR_OUT_OF_RANGE
 *  设置失败，端口取值返回非法。
 * @retval AK_ERR_INVAL_PARAM
 *  设置失败，传入无效地址参数。
 * @retval AK_ERR_INVAL_OPERATE
 *  该接口在句柄生存期内只能调用一次，重复调用会返回非法操作。
 *
 */
AK_API AK_int akae_socket_bind (AK_Socket handle, AK_chrptr addr, AK_int port);


AK_API AK_int akae_socket_local_addr (AK_Socket handle, AK_chrptr addr, AK_int *port);

/**
 * 监听端口数据。
 */
AK_API AK_int akae_socket_listen (AK_Socket handle, AK_size backlog);

AK_API AK_int akae_socket_connect (AK_Socket handle, AK_chrptr addr, AK_int port);

AK_API AK_int akae_socket_timed_connect (AK_Socket handle, AK_chrptr addr, AK_int port, AK_int timeo_us);

AK_API AK_int akae_socket_peer_addr (AK_Socket handle, AK_chrptr addr, AK_int *port);



/**
 *
 * @param[IN]
 *  hSockAK_ERR_OUT_OF_BACKLOG
 *  套接字句柄，由 @ref akae_socket_create() 创建。
 *
 * @param[OUT]
 *  addr
 *  接收到连接的对端地址，如果用户不需要获取该参数可以传入 AK_null。
 *
 * @param[OUT]
 *  port
 *  接收到连接的对端端口，如果用户不需要获取该参数可以传入 AK_null。
 *
 * @return
 *  成功接受到链接返回一个链接套接字，该套接字又客户接管，使用完以后需要通过 @ref akae_socket_release() 关闭，\n
 *  错误返回对应的错误码。
 *
 *
 * @retval AK_ERR_INVAL_HANDLE
 *  传入无效的套接字句柄，\n
 *  或者在该接口调用过程中，外部调用了 @ref akae_socket_release() 方法释放了句柄。
 *
 * @retval AK_ERR_OUT_OF_BACKLOG
 *  没有可分配的套接句柄，句柄最大数 @ref AK_SOCK_MAX_BACKLOG，\n
 *  如果已分配套接字数超过该数目将无法继续创建，用户应该检查是否出现句柄泄漏的问题。
 *
 */
AK_API AK_Socket akae_socket_accept (AK_Socket handle, AK_chrptr addr, AK_int *port);

/**
 * 参考 @ref akae_socket_accept()，支持超时控制。
 *
 * @param[IN]
 *  timeo
 *  如果 timeo = -1，该方法与 @ref akae_socket_accept() 相同。
 *  如果 timeo = 0，则不进行超时等待，立即返回。
 *  如果 time > 0，则在超市 @ref timeo 微秒内等待超时接收链接。
 *
 * @retval AK_ERR_OUT_OF_TIMEO
 *  超时时间内未接收到连接返回。
 *
 */
AK_API AK_Socket akae_socket_accept2 (AK_Socket handle, AK_ssize timeo, AK_chrptr addr, AK_int *port);

AK_API AK_int akae_socket_get_ttl (AK_Socket handle, AK_size *ttl);

AK_API AK_int akae_socket_set_ttl (AK_Socket handle, AK_size ttl);

AK_API AK_int akae_socket_set_send_timeout (AK_Socket handle, AK_ssize us);

AK_API AK_int akae_socket_set_recv_timeout (AK_Socket handle, AK_ssize us);

AK_API AK_int akae_socket_get_send_timeout (AK_Socket handle, AK_ssize *us);

AK_API AK_int akae_socket_get_recv_timeout (AK_Socket handle, AK_ssize *us);


AK_API AK_ssize akae_socket_send_buffer (AK_Socket handle, AK_bytptr buffer, AK_size len);

AK_API AK_ssize akae_socket_send_buffer_all (AK_Socket handle, AK_bytptr buffer, AK_size len);

AK_API AK_ssize akae_socket_recv_buffer (AK_Socket handle, AK_bytptr buffer, AK_size len);

AK_API AK_ssize akae_socket_recv_buffer_all (AK_Socket handle, AK_bytptr buffer, AK_size len);

AK_API AK_ssize akae_socket_peek_buffer (AK_Socket handle, AK_bytptr buffer, AK_size len);

AK_API AK_ssize akae_socket_flush_buffer (AK_Socket handle, AK_size len);

/**
 *
 * @param handle
 * @param buffer
 * @param len
 * @return
 */
AK_API AK_ssize akae_socket_recv_buffer_all (AK_Socket handle, AK_bytptr buffer, AK_size len);

AK_API AK_ssize akae_socket_send_buffer_to (AK_Socket handle,
		AK_bytptr buffer, AK_size len, AK_chrptr toIP, AK_uint16 toPort);

AK_API AK_ssize akae_socket_recv_buffer_from (AK_Socket handle,
		AK_bytptr buffer, AK_size len, AK_chrptr fromIP, AK_uint16 *fromPort);

/**
 * 设置 TCP 是否使用 NODELAY 模式。
 *
 * @return
 *  操作成功返回 AK_OK，否则返回对应错误码。
 */
AK_API AK_int akae_socket_tcp_nodelay (AK_Socket handle, AK_boolean enabled);


AK_C_HEADER_EXTERN_C_END
#endif ///< SOCKET_H_
