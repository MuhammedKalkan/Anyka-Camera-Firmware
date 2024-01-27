
/**
 * @brief
 *  Tosser 2.0 模块。\n
 *  此模块实现通过 TCP 插件服务方式监听 TCP 传输请求，\n
 *  通过此模块可以快速实现 TCP 被动式请求的服务器，如 HTTP、RTSP 等。
 *
 *
 *  专业术语：
 *  Sink - 槽
 *  Sunk - 响应槽
 */


//#include <NkUtils/allocator.h>
//#include <NkEmbedded/socket.h>
//#include <NkEmbedded/thread.h>
#include <akae_typedef.h>
#include <akae_object.h>
#include <akae_thread.h>
#include <akae_socket.h>


#if !defined(AKAE_TOSSER_H_)
#define AKAE_TOSSER_H_
AK_C_HEADER_EXTERN_C_BEGIN

/**
 * 信号槽名称的最大长度。
 */
#define AK_TOS_SINK_NAME_MAX    (32)


/**
 * 探测报文事件，当模块接收到一个报文，并不清除属于那个监听器的时，
 * 会触发此事件判断。
 *
 * @param[IN]
 *  Tosser
 *  当前事件调用的 Tosser 模块句柄。
 *
 * @param[IN] usrdata
 *  模块句柄传入上下文，由 @ref NK_Tosser2_Create() 方法指定。
 *
 * @param[IN] bytes
 *  数据内存起始地址
 *
 * @param[IN] len
 *  数据 @ref bytes 有效长度
 *
 * @return
 * 当报文确定不属于此监听器时返回 -1，模块将不会在当前会话继续触发探测，
 * 当模块未能确定报文内容是否属于当前监听器时，返回 0，模块在下一个探测周期会继续探测，
 * 当模块确定当前报文数据当前监听器，返回 @ref msg_len，模块会继续触发 @ref onEventLoop 事件。
 *
 */
typedef AK_boolean (*AK_TosserSunkProbe)(AK_Object Tosser, AK_voidptr usrdata, AK_bytptr bytes, AK_size len, AK_size *probe_actlen);


/**
 * 事件循环事件，当会话连接到模块，并确认会话属于当前监听器时，
 * 会触发此事件。
 *
 * @param[IN]		Tosser		当前事件调用的 Tosser 模块句柄。
 * @param[IN]		usrdata			Tosser 模块句柄传入上下文，由 @ref NK_Tosser2_Create() 方法指定。
 * @param[IN]		Thread		当前线程控制器，可以调用 @ref NK_Thread::suspend() 和 NK_Thread::terminate() 进行线程相应调度。
 * @param[IN]		ConnTCP		会话 TCP 连接句柄。
 *
 *
 */
typedef AK_void (*AK_TosserSunkEventloop)(AK_Object Tosser, AK_size probe_actlen, AK_voidptr usrdata, AK_Thread th, AK_Socket conn);


typedef struct _AK_TosserSink {

	/**
	 * Sink 探测事件。
	 */
	AK_TosserSunkProbe  OnProbe;

	/**
	 * Sink 响应事件循环。
	 */
	AK_TosserSunkEventloop OnEventloop;


} AK_TosserSink;


/**
 * 创建 Tosser 句柄。
 *
 */
AK_API AK_Object akae_tosser_create (AK_Object Malloc, AK_uint16 port);

/**
 * 释放 Tosser 模块句柄。
 *
 * @param[IN]
 *  Object
 *  模块句柄。
 *
 * @return
 *  释放成功放回 AK_OK，失败返回对应的返回码，有可能句柄异常。
 *
 */
AK_API AK_int akae_tosser_release (AK_Object Object);


/**
 * 返回监听端口。
 *
 *
 * @return
 *  获取成功返回当前模块正在监听的端口号（1-65535），失败返回对应的错误码。
 */
AK_API AK_int akae_tosser_listen_port (AK_Object Object);

/**
 * @brief
 *  重置监听端口。
 * @details
 *  重置端口后，只能重原来新的端口上才能监听链接。
 *
 * @param AK_Object Object [in]
 *  this 指针。
 * @param port [in]
 *  监听端口号。
 *
 * @retval AK_OK
 *  重启成功，模块将使用新的端口进行监听。
 * @retval AK_ERR_INVAL_OBJECT
 *  操作失败，传入了错误的对象句柄。
 * @retval AK_ERR_INVAL_OPERATE
 *  操作失败，模块继续沿用就的端口进行监听，有可能是传入一个不可用的端口导致。
 *
 */
AK_API AK_int akae_tosser_restart (AK_Object Object, AK_uint16 port);

/**
 * 增加一个监听器。
 *
 */
AK_API AK_int akae_tosser_add_sink (AK_Object Object, AK_chrcptr name, AK_TosserSink *Sink, AK_voidptr usrdata);

/**
 * 移除一个监听器。
 */
AK_API AK_int akae_tosser_drop_sink (AK_Object Object, AK_chrcptr name);



AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_TOSSER_H_

