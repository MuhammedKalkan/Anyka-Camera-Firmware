/**
 * RTSP 服务器功能接口定义。
 *
 */

#include <akae_rtsp.h>
#include <akae_httputils.h>
#include <akae_verbose.h>
#include <akae_tosser.h>
#include <akae_malloc.h>
#include <akae_rtpqueue.h>


#if !defined(AKAE_RTSPSERVER_H_)
# define AKAE_RTSPSERVER_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * RTSP 服务器最大链接 URL 地址。
 */
#define AK_RTSP_SERVER_MAX_URL       (16)

/**
 * RTSP 接收媒体最小缓冲包数。
 */
#define AK_RTSP_SERVER_MIN_BUFCNT    (8)

/**
 * RTSP 接收媒体最大缓冲包数。
 */
#define AK_RTSP_SERVER_DEF_BUFCNT    (64)

/**
 * RTSP 接收媒体最大缓冲包数。
 */
#define AK_RTSP_SERVER_MAX_BUFCNT    (128)


/**
 * 创建一个 RTSP 服务器。
 * 注意，创建服务器成功以后服务器未真正开始进行监听，\n
 * 当调用 @ref akae_rtsp_server_start() 以后才开始工作。
 *
 * @param[IN] Malloc
 *  对象内部使用的内存分配器。
 *
 * @return
 *  创建成功返回对象句柄。
 *
 */
AK_API AK_Object akae_rtsp_server_create (AK_Object Malloc);

/**
 * 释放一个 RTSP 服务器。
 *
 * @param[IN] Object
 *  服务器对象，由 @ref akae_rtsp_server_create() 创建。
 *
 * @retval AK_OK
 *  释放成功。
 * @retval AK_ERR_INVAL_OBJECT
 *  操作失败，可能传入了异常的参数。
 *
 */
AK_API AK_int akae_rtsp_server_release (AK_Object Object);

/**
 * 使能显示调试协议报文输出。
 */
AK_API AK_void akae_rtsp_server_verbose (AK_Object Object, AK_boolean enabled);


AK_API AK_void akae_rtsp_server_verbose_http (AK_Object Object, AK_boolean enabled);


/**
 * 启用 RTSP 服务器，该接口操作成功以后，外部可以开始访问。
 *
 * @param[IN] Object
 *  服务器对象，由 akae_rtsp_server_create() 获取。
 * @param[IN] port
 *  服务器监听的 TCP 端口号。
 *
 * @retval AK_OK
 *  操作成功。
 */
AK_API AK_int akae_rtsp_server_start (AK_Object Object, AK_uint16 port);

/**
 * 停用 RTSP 服务器，与 @ref akae_rtsp_server_start() 相对。
 *
 * @retval AK_OK
 *  操作成功。
 */
AK_API AK_int akae_rtsp_server_finish (AK_Object Object);


/**
 * RTSP 收发服务用户回调接口定义，该接口由客户定义实现在 RTSP 服务会话中进行码流数据处理。
 *
 * @param[IN] Rtsp
 *  RTSP 会话句柄，由模块内下发，传入 @ref akae_rtsp_server_send_h264_1slice() 等接口，\n
 *  作为会话发送句柄。
 *
 * @param[IN] userdata
 *  会话用户数据，通过 @ref akae_rtsp_server_add_url() 传入。
 *
 * @param[IN] th
 *  会话线程句柄，在回调实现过程中，通过 @ref akae_thread_terminated(th) 判断当前会话是否需要继续发送。
 *
 */
typedef AK_void (*AK_RtspRecvSendRtpFunc)(AK_voidptr Rtsp, AK_voidptr userdata, AK_Thread th);



/**
 * 创建一个 URL 监听地址，创建成功以后，会返回该监听 registry，用于该会话操作。
 *
 * @param[IN] Object
 *  服务器对象，由 @ref akae_rtsp_server_create() 创建。
 *
 * @param[IN] url
 *  监听地址，操作成功以后，该地址开始监听请求。
 *
 * @param[IN] Func
 *  RTSP 发送 RTP 数据用户线程，当有数据请求时会触发该用户回调，\n
 *  回调中可以利用 @ref userdata 传输用户上下文。
 *
 * @param[IN] userdata
 *  用户上下文，该指针数据将会在 @ref Func 触发时传给用户。
 *
 * @return
 *  操作成功返回大于 0 的一个 registry，用于接口操作，失败返回对应错误码。
 *
 * @retval AK_ERR_INVAL_OBJECT
 *  操作失败，传入一个无效的对象。
 * @retval AK_ERR_INVAL_PARAM
 *  操作失败，可能传入一个非法参数。
 *
 */
AK_API AK_int akae_rtsp_server_register_url (AK_Object Object, AK_chrptr url, AK_RtspRecvSendRtpFunc Func, AK_voidptr userdata);


/**
 * 设置接收缓冲的数量。\n
 * 可以通过传入 @var bufcnt 限制接收媒体缓冲的包数，\n
 * @var bufcnt 最大最小值分别由 @ref AK_RTSP_SERVER_MAX_BUFCNT 和 @ref AK_RTSP_SERVER_MIN_BUFCNT 定义。\n
 * 调用 @ref akae_rtsp_server_add_url() 以后若不调用当前接口内部缺省设置为 @ref AK_RTSP_SERVER_MIN_BUFCNT。
 *
 */
AK_API AK_int akae_rtsp_server_set_url_receiver(AK_Object Object, const AK_int registry, AK_size bufcnt);

/**
 * 设置 URL 可重入属性，避免由于并发访问造成流媒体缓冲触发混乱。
 */
AK_API AK_int akae_rtsp_server_set_url_reentrant(AK_Object Object, const AK_int registry, AK_boolean enabled);


/**
 * 放弃一个 URL 地址监听，与 @ref akae_rtsp_server_add_url() 相对。
 *
 * @param[IN] Object
 *  服务器对象，由 @ref akae_rtsp_server_create() 创建。
 *
 * @param[IN] registry
 *  监听地址序号，由 @ref akae_rtsp_server_add_url() 创建。
 *
 * @retval AK_OK
 *  操作成功。
 * @retval AK_ERR_INVAL_OBJECT
 *  操作失败，传入一个无效的对象。
 * @retval AK_ERR_INVAL_PARAM
 *  操作失败，可能传入一个非法参数。
 */
AK_API AK_int akae_rtsp_server_unregister_url(AK_Object Object, const AK_int registry);

/**
 * 声明服务视频载体。
 *
 * @param[IN] Object
 *  服务器对象，由 @ref akae_rtsp_server_create() 创建。
 * @param[IN] registry
 *  监听地址序号，由 @ref akae_rtsp_server_add_url() 创建。
 * @param[IN] payload
 *  视频载体类型，如 @ref AK_RTP_PT_H264。
 * @param[IN] clock_hz
 *  视频计数时钟基数，对于视频来说是采集时钟频率，对于音频来说对应采样率。
 *
 * @retval AK_OK
 *  操作成功。
 * @retval AK_ERR_INVAL_OBJECT
 *  操作失败，传入一个无效的对象。
 */
AK_API AK_int akae_rtsp_server_describe_video(AK_Object Object, const AK_int registry, AK_uint32 payload,
		AK_size clock_hz, AK_Object RtpQueue);


/**
 * 声明反向视频服务载体。
 *
 */
AK_API AK_int akae_rtsp_server_describe_upstream_video (AK_Object Object, AK_int const registry, AK_uint32 payload, AK_size clock_hz);


/**
 * 针对具体媒体 @ref akae_rtsp_server_describe_video() 描述快速调用。
 */
#define akae_rtsp_server_describe_h264(__Object, __registry, __RtpQueue)     akae_rtsp_server_describe_video(__Object, __registry, AK_RTP_PT_H264, 90000, __RtpQueue)
#define akae_rtsp_server_describe_h265(__Object, __registry, __RtpQueue)     akae_rtsp_server_describe_video(__Object, __registry, AK_RTP_PT_H265, 90000, __RtpQueue)
#define akae_rtsp_server_describe_jpeg(__Object, __registry, __RtpQueue)     akae_rtsp_server_describe_video(__Object, __registry, AK_RTP_PT_JPEG, 90000, __RtpQueue)

/**
 * 声明服务音频载体。
 *
 */
AK_API AK_int akae_rtsp_server_describe_audio(AK_Object Object, const AK_int registry, AK_uint32 payload,
		AK_size channel, AK_size sample_rate, AK_Object RtpQueue);


#define akae_rtsp_server_describe_g711a(__Object, __registry, __RtpQueue)    akae_rtsp_server_describe_audio(__Object, __registry, AK_RTP_PT_PCMA, 1, 8000, __RtpQueue)
#define akae_rtsp_server_describe_g711u(__Object, __registry, __RtpQueue)    akae_rtsp_server_describe_audio(__Object, __registry, AK_RTP_PT_PCMU, 1, 8000, __RtpQueue)
#define akae_rtsp_server_describe_aac(__Object, __registry, __RtpQueue)      akae_rtsp_server_describe_audio(__Object, __registry, AK_RTP_PT_AAC, 1, 8000, __RtpQueue)



/**
 * 声明上载服务音频载体。
 *
 */
AK_API AK_int akae_rtsp_server_describe_upstream_audio(AK_Object Object, const AK_int registry, AK_uint32 payload,
			AK_size channel, AK_size sample_rate);

#define akae_rtsp_server_describe_upstream_g711a(__Object, __registry) akae_rtsp_server_describe_upstream_audio(__Object, __registry, AK_RTP_PT_PCMA, 1, 8000)
#define akae_rtsp_server_describe_upstream_g711u(__Object, __registry) akae_rtsp_server_describe_upstream_audio(__Object, __registry, AK_RTP_PT_PCMU, 1, 8000)
#define akae_rtsp_server_describe_upstream_aac(__Object, __registry)   akae_rtsp_server_describe_upstream_audio(__Object, __registry, AK_RTP_PT_AAC, 1, 16000)


/**
 * RTSP 服务器事件类型定义。
 */
enum {

	/// 未定义事件。
	AK_RTSP_EVT_UNDEF = (-1),

	/// RTSP 连接事件，当客户端连接上对应 URL 请求时会触发该事件。
	AK_RTSP_EVT_URL_CONNECT,

	/// RTSP 断开事件，当客户端连断开对应 URL 请求时会触发该事件。
	AK_RTSP_EVT_URL_DISCONNECT,

	/// URL 激活事件，区别于 @ref AK_RTSP_EVT_URL_CONNECTED 事件，\n
	/// 第一个 @ref AK_RTSP_EVT_URL_CONNECTED 触发时会触发该事件。
	AK_RTSP_EVT_URL_ACTIVE,

	/// URL 关闭事件，区别于 @ref AK_RTSP_EVT_URL_DISCONNECTED 事件，\n
	/// 最后一个 @ref AK_RTSP_EVT_URL_DISCONNECTED 触发时会触发该事件，\n
	/// 即服务器对应 URL 服务从激活到非激活状态切换。
	AK_RTSP_EVT_URL_DEACTIVE,

	/// 枚举个数标识。
	AK_RTSP_EVT_BUTT,

};


/**
 * RTSP 服务器事件参数定义。
 */
typedef struct _AK_RtspServerEventPara {

	///
	/// URL 地址。
	/// 事件
	/// @ref AK_RTSP_EVT_URL_CONNECTED
	/// @ref AK_RTSP_EVT_URL_DISCONNECTED
	/// @ref AK_RTSP_EVT_URL_ACTIVE
	/// @ref AK_RTSP_EVT_URL_DEACTIVE
	/// 有效。
	///
	AK_chrptr path;

	///
	/// URL 连接数。
	/// 事件
	/// @ref AK_RTSP_EVT_URL_CONNECTED
	/// @ref AK_RTSP_EVT_URL_DISCONNECTED
	/// 有效。
	///
	AK_int connected;

} AK_RtspServerEventPara;


/**
 * RTSP 服务器客户端请求/去请求事件回调定义。
 */
typedef AK_void (*AK_RtspServerOnEvent)(AK_voidptr Rtsp, AK_int evt, AK_voidptr userdata, AK_RtspServerEventPara *Para);


/**
 * 注册一个事件监听回调，如 @ref AK_RTSP_EVT_ACTIVE_URL。
 * 服务器在该事件触发时会触发该用户回调，用户可以根据上层业务需要在事件回调中实现记录。\n
 * 该接口重复调用，会覆盖上一次设定的回调。
 *
 * @param[IN] Rtsp
 *  注册的 RTSP 会话句柄。
 * @param[IN] evt
 *  事件类型，如 @ref AK_RTSP_EVT_ACTIVE_URL。
 * @param[IN] Func
 *  用户回调实现，如果传入空相当于调用 @ref akae_rtsp_server_drop_event_listener()。\n
 *  AK_RtspServerOnEvent 实现。
 * @param[IN] userdata
 *  事件触发用户上下文，通过 @ref EventFunc 回传给客户。
 *
 */
AK_API AK_int akae_rtsp_server_add_event_listener (AK_Object Object, AK_int const registry, AK_int evt, AK_RtspServerOnEvent Func, AK_voidptr userdata);

/**
 * 撤销事件监听。
 *
 * @param[IN] Rtsp
 *  注册的 RTSP 会话句柄。
 * @param[IN] evt
 *  事件类型。
 */
AK_API AK_int akae_rtsp_server_drop_event_listener (AK_Object Object, AK_int const registry, AK_int evt);


/**
 * 获取流媒体数据。\n
 * 该接口只能在 @ref AK_RtspRecvSendRtpFunc 回调中调用。\n
 * 该接口需要调用足够快以避免由于模块内内存不足导致丢数据的问题。\n
 * 可以稍微通过 @ref akae_rtsp_server_set_url_receiver() 接口增大缓存大小，最大不超过 @ref AK_RTSP_SERVER_MAX_BUFCNT。
 *
 * @verbatim
 *
 * akae_rtsp_server_get_stream() 与 akae_rtsp_server_release_stream() 工作流程。
 *
 *                                                                     +-----------------+
 *   Client Send                                             +---------> User Process    |
 *   Media-1                                                 |         | for Application |
 *                                                           |         +-----------------+
 *      +            Media Data Pipe         Server          |                  |
 *      |      +--------------------------+  Receive  +------+-------+          | After Processing
 *      +----->+  |  |  |  |  |  |  |  |  |  Media-*  |              |          |
 *             |  |  |  |  |  |  |  |  |  +-----------> get_stream() |  Loop    |
 *      +----->+  |  |  |  |  |  |  |  |  |           |              |          |
 *      |      +--------------------------+           +------+-------+          |
 *      +                                                    |                  |
 *                                                           |         +--------v---------+
 *   Client Send                                             |         | release_stream() |
 *   Media-2                                                 +---------+                  |
 *                                                                     +------------------+
 *
 * @endverbatim
 *
 * @param[IN] Rtsp
 *  RTSP 会话上下文，通过在 @ref AK_RtspRecvSendRtpFunc 回调中传入。
 * @param[OUT] Rtp
 *  获取数据的 RTP 数据信息。
 * @param[OUT] data
 *  获取数据所在内存的起始地址。
 *
 * @return
 *  操作成功，数据属性参数以及起始地址通过 @ref Rtp 和 @ref data 返回，并返回数据长度，
 *  操作失败返回对应错误码。
 *
 * @retval AK_ERR_INVAL_OBJECT
 *  操作错误，可能传入了一个错误的对象。
 * @retval AK_ERR_INVAL_PARAM
 *  操作错误，可能传入错误的参数。
 * @retval AK_ERR_NOT_EXIST
 *  操作错误，缓冲没有数据。
 * @retval AK_ERR_INVAL_OPERATE
 *  操作错误，上一个调用周期未调用 @ref akae_rtsp_server_release_stream() 释放所使用的数据。
 */
AK_API AK_ssize akae_rtsp_server_get_stream (AK_voidptr Rtsp, AK_RtpHeadfield *Rtp, AK_bytptr *data);

/**
 * 释放流媒体数据。\n
 * 该接口只能在 @ref AK_RtspRecvSendRtpFunc 回调中调用。\n
 * 与 @ref akae_rtsp_server_get_stream() 对应，\n
 * 当调用完 @ref akae_rtsp_server_get_stream() 以后未调用当前接口释放数据，\n
 * 将无法继续调用 @ref akae_rtsp_server_get_stream() 获取新的数据。
 *
 */
AK_API AK_int akae_rtsp_server_release_stream (AK_voidptr Rtsp);


AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_RTSPSERVER_H_
