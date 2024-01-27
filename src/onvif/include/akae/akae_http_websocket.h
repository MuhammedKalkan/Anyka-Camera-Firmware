
/**
 * WebSocket 功能接口定义。
 * 详见 https://tools.ietf.org/html/rfc6455。
 *
 */

#include <akae_typedef.h>
#include <akae_socket.h>
#include <akae_httpserver.h>

#if !defined(AKAE_HTTP_WEBSOCKET_H_)
#define AKAE_HTTP_WEBSOCKET_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * @macro
 *  WebSocket 帧数据类型定义。
 */
#define AK_HTTP_WSOCK_OPT_CONTINUATION (0x00) ///< Denotes a continuation frame.
#define AK_HTTP_WSOCK_OPT_TEXT         (0x01) ///< Denotes a text frame.
#define AK_HTTP_WSOCK_OPT_BINARY       (0x02) ///< Denotes a binary frame.
#define AK_HTTP_WSOCK_OPT_CONN_CLOSE   (0x08) ///< Denotes a connection close.
#define AK_HTTP_WSOCK_OPT_PING         (0x09) ///< Denotes a ping.
#define AK_HTTP_WSOCK_OPT_PONG         (0x0a) ///< Denotes a pong.

/**
 * @macro
 *  掩码键值长度，单位：字节。
 */
#define AK_HTTP_WSOCK_MASK_KEY_LEN          (4)

/**
 * @macro
 *  最小的帧头长度，单位：字节。
 */
#define AK_HTTP_WS_MIN_FRAME_HEAD_SZ   (2)

/**
 * @macro
 *  最大的帧头长度。
 */
#define AK_HTTP_WS_MAX_FRAME_HEAD_SZ   (AK_HTTP_WS_MIN_FRAME_HEAD_SZ + 8 + AK_HTTP_WSOCK_MASK_KEY_LEN)

/**
 * @brief
 *  WebSocket 帧头域数据结构，\n
 *  对应 WebSocket 版本 13。
 */
typedef struct _AK_HttpWebSocketFrame {

	AK_size size;              ///< 头域长度，单位：字节。

	/// FIN 标致位。
	AK_boolean fin;

	///< 掩码标识。
	AK_boolean mask;

	///< 数据类型。
	AK_uint32 optype;

	///< 额外长度描述大小，可能是 0，2，8。
	AK_size extraLenBytes;

	///< 净荷数据长度。
	AK_size64 payloadlen;

	/// 掩码键数组。
	AK_byte maskingKey[AK_HTTP_WSOCK_MASK_KEY_LEN];

} AK_HttpWebSocketFrame;


//AK_API AK_Socket
//AK_HTTPWS_Handshake (AK_Object Object, AK_PChar url, AK_PChar username, AK_PChar passphrase, AK_int us);

/**
 * @brief
 *  WebSocket 握手回应。
 * @brief
 *  在 CGI 实现内调用，针对 WebSocket 版本 13 及相关版本，\n
 *  如果当前 CGI 会话是 WebSocket 会话，\n
 *  内部会调用 @ref Conn 套接字发送接口回复握手确认信息。
 *
 * @param[IN] wsock
 *  Websocket 连接 TCP 句柄。
 * @param Session [in]
 *  CGI 会话。
 *
 * @return
 *  握手回应成功返回 True，否则返回 False。
 */
AK_API AK_boolean akae_http_websocket_handshake_answer (AK_HttpServerSession *Session);


/**
 * 在超时时间内, 等待对端数据帧，并获取帧参数。
 *
 * @param[IN] wsock
 *  Websocket 连接 TCP 句柄。
 *
 * @param[IN] timeo
 *  等待超时时间，单位：微秒，\n
 *  内部调用 @ref akae_socket_can_read() 接口.
 *  为 0 表示不等待超时, 为 -1 表示一直等待，直到错误返回。
 *
 * @param[OUT] Headfield
 *  等待成功后获取到帧头域信息, 紧接着通过 @ref akae_http_websocket_recv_payload() 接收净荷数据.
 *
 * @retval AK_OK
 *  成功等待到一帧数据.
 * @retval AK_ERR_TIME_OUT
 *  等待超时, 对端没有数据.
 * @retval AK_ERR_PEER_BROKEN
 *  等待失败, 对端断开.
 * @retval AK_ERR_EXCEPTION
 *  等待失败, 通信过程中对端发送报文异常.
 *
 */
AK_API AK_int akae_http_websocket_wait_frame (AK_Socket wsock, AK_int timeo, AK_HttpWebSocketFrame *Headfield);

/**
 * 服务器等待数组.
 */
AK_API AK_int akae_http_websocket_server_wait_frame (AK_Socket wsock, AK_int timeo, AK_HttpWebSocketFrame *Headfield);


AK_API AK_int akae_http_websocket_client_wait_frame (AK_Socket wsock, AK_int timeo, AK_HttpWebSocketFrame *Headfield);


/**
 * @brief
 *  接收数据帧。
 * @details
 *  此方法用于接收帧的净荷数据，并保存到 @ref stack 内存地址上，\n
 *  调用此接口会立即返回不会阻塞，因此在调用之前必须先调用 @ref AK_HTTPWS_PredictFrame() 或者 @ref AK_HTTPWS_WaitFrame() 确保有数据可以接收，\n
 *  并根据 @ref AK_HTTPWS_PredictFrame() 或者 @ref AK_HTTPWS_WaitFrame() 的调用结果确定分配存放内存的空间，配置 @ref stacklen 大小。\n
 *  如果接收的帧净荷数据大小大于 @ref stacklen，此方法会返回 0，不进行读取。
 *
 * @param[IN] wsock
 *  Websocket 连接 TCP 句柄。
 * @param[IN] Headfield
 *  帧头域信息, 由 @ref akae_http_websocket_wait_frame() 获取.
 * @param stack [out]
 *  帧的净荷数据内存位置的起始地址。
 * @param stacklen [in]
 *  帧的经和数据内存大小，单位：字节。
 *
 * @return
 *  读取成功返回净荷数据的大小，有可能为 0，错误返回 -1。
 *
 */
AK_API AK_ssize akae_http_websocket_recv_payload (AK_Socket wsock, AK_HttpWebSocketFrame *Headfield, AK_bytptr stack, AK_size64 stacklen);



/**
 * @macro
 *  净荷数据加掩码宏操作。
 */
#define AK_HTTP_WSOCK_MASK_PAYLOAD(__mask, __payload, __payloadlen) \
	do {\
		AK_int __i = 0;\
		for (__i = 0; __i < __payloadlen; ++__i) {\
			*((AK_bytptr)(__payload) + __i) = *((AK_bytptr)(__payload) + __i) ^ *((AK_bytptr)(__mask) + __i % 4);\
		}\
	} while(0)

/**
 * @macro
 *  净荷数据去掩码宏操作。
 */
#define AK_HTTP_WSOCK_UMASK_PAYLOAD(__mask, __payload, __payloadlen) AK_HTTP_WSOCK_MASK_PAYLOAD(__mask, __payload, __payloadlen)

/**
 * @brief
 *  发送帧数据。
 * @details
 *  向对方发送一帧数据。\n
 *  注意：该接口只是无差异发送帧的方法，但具体在 WebSocket 协议中，向服务端发送和向客户段发送 @ref mask 是不一样的。\n
 *  详细参考 @ref akae_http_websocket_server_send_frame() 与 @ref akae_http_websocket_client_send_frame()。
 *
 * @param[IN] wsock
 *  Websocket 连接 TCP 句柄。
 * @param optype [in]
 *  净荷数据内行。
 * @param mask [in]
 *  掩码标识，标识发送数据是否使用数据掩码。
 * @param data [in]
 *  净荷数据的内存起始地址。
 * @param len [in]
 *  净荷数据长度。
 *
 * @retval AK_OK
 *  发送成功。
 * @retval AK_ERR_INVAL_HANDLE
 *  发送失败，可能是据听失败。
 * @retval AK_ERR_INVAL_PARAM
 *  发送失败，可能是参数错误。
 */
AK_API AK_int akae_http_websocket_send_frame (AK_Socket wsock, AK_uint32 optype, AK_boolean mask, AK_voidptr payload, AK_size64 payloadlen);

/**
 * 服务端向客户段发送一帧数据。
 */
#define akae_http_websocket_server_send_frame(__wsock, __optype, __payload, __payloadlen) \
	akae_http_websocket_send_frame (__wsock, __optype, AK_false, (AK_voidptr)(__payload), __payloadlen)

/**
 * 客户段向服务端发送一帧数据。
 */
#define akae_http_websocket_client_send_frame(__wsock, __optype, __payload, __payloadlen) \
	akae_http_websocket_send_frame (__wsock, __optype, AK_true, (AK_voidptr)(__payload), __payloadlen)


/**
 * @macro
 *  服务器发送 Ping 控制数据操作宏。
 */
#define akae_http_websocket_server_ping(__wsock) \
	akae_http_websocket_server_send_frame (__wsock, AK_HTTP_WSOCK_OPT_PING, AK_null, 0)

/**
 * @macro
 *  服务器发送 Ping 控制数据并带有用户数据操作宏。
 */
#define akae_http_websocket_server_ping2(__wsock, __payload, __payloadlen) \
	((__payloadlen) <= 125 ? \
			akae_http_websocket_server_send_frame (__wsock, AK_HTTP_WSOCK_OPT_PING, (AK_voidptr)(__payload), __payloadlen) : AK_ERR_OUT_OF_RANGE)


/**
 * @macro
 *  客户端发送 Ping 控制数据操作宏。
 */
#define akae_http_websocket_client_ping(__wsock) \
		akae_http_websocket_client_send_frame (__wsock, AK_HTTP_WSOCK_OPT_PING, AK_null, 0)

/**
 * @macro
 *  客户端发送 Ping 控制数据并带有用户数据操作宏。
 */
#define akae_http_websocket_client_ping2(__wsock, __payload, __payloadlen) \
	((__payloadlen) <= 125 ? \
			akae_http_websocket_client_send_frame (__wsock, AK_HTTP_WSOCK_OPT_PING, (AK_voidptr)(__payload), __payloadlen) : AK_ERR_OUT_OF_RANGE)


/**
 * @macro
 *  服务器发送 Pong 控制数据操作宏。
 */
#define akae_http_websocket_server_pong(__wsock) \
	akae_http_websocket_server_send_frame (__wsock, AK_HTTP_WSOCK_OPT_PONG, AK_null, 0)

/**
 * @macro
 *  服务器发送 Pong 控制数据并带有用户数据操作宏。
 */
#define akae_http_websocket_server_pong2(__wsock, __payload, __payloadlen) \
	((__payloadlen) <= 125 ? \
			akae_http_websocket_server_send_frame (__wsock, AK_HTTP_WSOCK_OPT_PONG, (AK_voidptr)(__payload), __payloadlen) : AK_ERR_OUT_OF_RANGE)

/**
 * @macro
 *  服务器发送 Pong 控制数据操作宏。
 */
#define akae_http_websocket_client_pong(__wsock) \
	akae_http_websocket_client_send_frame (__wsock, AK_HTTP_WSOCK_OPT_PONG, AK_null, 0)

/**
 * @macro
 *  服务器发送 Pong 控制数据并带有用户数据操作宏。
 */
#define akae_http_websocket_client_pong2(__wsock, __payload, __payloadlen) \
	((__payloadlen) <= 125 ? \
			akae_http_websocket_client_send_frame (__wsock, AK_HTTP_WSOCK_OPT_PONG, (AK_voidptr)(__payload), __payloadlen) : AK_ERR_OUT_OF_RANGE)


AK_API AK_int akae_http_websocket_server_ping_pong (AK_Socket wsock, AK_int nonce, AK_int us);

AK_API AK_int akae_http_websocket_client_ping_pong (AK_Socket wsock, AK_int nonce, AK_int us);


/**
 * @macro
 *  发送 Close 控制数据操作宏。
 */
#define akae_http_websocket_close(__wsock) \
	akae_http_websocket_send_frame(__wsock, AK_HTTP_WSOCK_OPT_CONN_CLOSE, AK_false, AK_null, 0)

/**
 * @macro
 *  发送文本数据操作宏。
 */
#define akae_http_websocket_client_send_text(__wsock, __text) \
	akae_http_websocket_send_frame(__wsock, AK_HTTP_WSOCK_OPT_TEXT, AK_true, (AK_bytptr)(__text), akae_strlen(__text))

/**
 * @macro
 *  发送文本数据操作宏。
 */
#define akae_http_websocket_server_send_text(__wsock, __text) \
	akae_http_websocket_send_frame(__wsock, AK_HTTP_WSOCK_OPT_TEXT, AK_false, (AK_bytptr)(__text), akae_strlen(__text))


/**
 * @macro
 *  发送二进制数据操作宏。
 */
#define akae_http_websocket_client_send_binary(__wsock, __data, __datalen) \
	akae_http_websocket_send_frame(__wsock, AK_HTTP_WSOCK_OPT_BINARY, AK_true, (AK_bytptr)(__data), (__datalen))

/**
 * @macro
 *  发送二进制数据操作宏。
 */
#define akae_http_websocket_server_send_binary(__wsock, __data, __datalen) \
	akae_http_websocket_send_frame(__wsock, AK_HTTP_WSOCK_OPT_BINARY, AK_false, (AK_bytptr)(__data), (__datalen))


AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_HTTP_WEBSOCKET_H_
