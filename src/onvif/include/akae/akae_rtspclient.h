/**
 * RTSP 协议基础工具集定义。
 *
 */

#include <akae_rtsp.h>
#include <akae_malloc.h>
#include <akae_verbose.h>


#if !defined(AKAE_RTSPCLIENT_H_)
# define AKAE_RTSPCLIENT_H_
AK_C_HEADER_EXTERN_C_BEGIN

typedef struct _AK_RtspClientStatus {

	/// 连接标识，当前处于连接状态返回 AK_true。
	AK_boolean connected;

	/// 服务端媒体载体类型。
	struct {

		/// 服务端下载媒体载体类型。
		/// 该媒体通过客户端下载到本地进行播放。
		/// Up 前缀表示该媒体通过客户端上传到服务端交互。
		struct {

			/// 类型序号。
			AK_int type;

			/// 类型名称。
			AK_char name[8];

		} Video, Audio, UpVideo, UpAudio;

	} Payload;

	/// 内存用量信息。
	struct {
		/// 内存总量。
		AK_size total;
		/// 内存用量。
		AK_size usage;

	} Memory;

	/// 重连信息。
	struct {

		/// 重连总次数。
		AK_size total;
		/// 有效重连次数。
		AK_size success;

	} Reconnect;

	/// 接收到的数据包个数信息。
	struct {

		struct {

			AK_size video;
			AK_size audio;

		} Rtp;

		struct {

			AK_size rr;

		} Rtcp;

	} RecvPacket;


} AK_RtspClientStatus;

/**
 * 打印 @ref AK_RtspClientStatus 对象结构。
 */
#define AK_RTSP_CLIENT_STATUS_DUMP(__Status) \
	do {\
		AK_VerboseForm Form;\
		akae_verbose_form_init (&Form, "RTSP Clietn Status", 64, 4);\
		akae_verbose_form_put_kv (&Form, AK_true,     "Connected",       "%s", (__Status)->connected ? "Yes" : "No");\
		akae_verbose_form_put_kv (&Form, AK_true,     "Reconnect",       "%u/%u", (__Status)->Reconnect.success, (__Status)->Reconnect.total);\
		akae_verbose_form_put_text (&Form, AK_true,   "Payload Type");\
		akae_verbose_form_put_kv (&Form, AK_false,    "Video",       "%2d - %s", (__Status)->Payload.Video.type, (__Status)->Payload.Video.name);\
		akae_verbose_form_put_kv (&Form, AK_false,    "Audio",       "%2d - %s", (__Status)->Payload.Audio.type, (__Status)->Payload.Audio.name);\
		akae_verbose_form_put_kv (&Form, AK_false,    "Video Upstream",       "%2d - %s", (__Status)->Payload.UpVideo.type, (__Status)->Payload.UpVideo.name);\
		akae_verbose_form_put_kv (&Form, AK_true,     "Audio Upstream",       "%2d - %s", (__Status)->Payload.UpAudio.type, (__Status)->Payload.UpAudio.name);\
		akae_verbose_form_put_kv (&Form, AK_true,     "Memory",          "%u.%03u/%u.%03uKB - %d%%",\
				(__Status)->Memory.usage / 1000, (__Status)->Memory.usage % 1000,\
				(__Status)->Memory.total / 1000, (__Status)->Memory.total % 1000,\
				(__Status)->Memory.usage * 100 / (__Status)->Memory.total);\
		akae_verbose_form_put_kv (&Form, AK_false,    "Received RTP - Video",     "%u", (__Status)->RecvPacket.Rtp.video);\
		akae_verbose_form_put_kv (&Form, AK_false,    "Received RTP - Audio",     "%u", (__Status)->RecvPacket.Rtp.audio);\
		akae_verbose_form_finish (&Form);\
	} while (0)


/**
 * 创建一个 RTSP 客户端对象。\n
 * 创建时需要传入一个内存分配器，该内存分配器用于客户端接收数据时的缓冲。
 *
 * @post 创建的对象需要调用 @ref akae_rtsp_client_release() 释放。
 *
 * @param[IN] Malloc
 *  内部缓冲分配器。
 * @param[IN] mem
 *  指定使用的内存大小，传入 0，则表示尽可能使用 @ref Malloc 的内存空间。\n
 *  如用户设计上 @ref Mallo 内存分配器为当前模块独立提供，传入 0 即可。
 *
 * @return
 *  成功返回 RTSP 客户端对象句柄，失败返回 AK_null。
 */
AK_API AK_Object akae_rtsp_client_create (AK_Object Malloc, AK_size mem);

/**
 * 释放一个 RTSP 客户端对象。
 *
 * @pre 释放的对象由 @ref akae_rtsp_client_create() 创建。
 *
 * @param[IN] Object
 *  模块对象，由 @ref akae_rtsp_client_create() 创建。
 *
 * @retval AK_OK
 *  操作成功。
 * @retval AK_ERR_INVAL_OBJECT
 *  操作失败，传入非法对象。
 */
AK_API AK_int akae_rtsp_client_release (AK_Object Object);

/**
 * 使能显示调试协议报文输出。
 */
AK_API AK_void akae_rtsp_client_verbose (AK_Object Object, AK_boolean enabled);


/**
 * 链接 @var url 服务器地址。\n
 * 该接口第一次调用会向 @var url 服务器地址发起，\n
 * 连接成功以后可以通过 @ref akae_rtsp_client_get_stream() 方法获取数据。\n
 * 该接口调用成功以后，如果连接过程中出现断开内部会不断尝试重连保证连接可靠。\n
 * 如果连续调用两次该接口，会先断开前一次连接，对新的 @var url 服务器地址连接，\n
 * 对于新的地址连接结果通过接口返回。
 *
 * @post
 *  在 @ref akae_rtsp_client_create() 创建对象成功以后调用。
 *
 * @param[IN] Object
 *  模块对象，由 @ref akae_rtsp_client_create() 创建。
 * @param[IN] url
 *  服务器地址，对端地址，如：rtsp://192.168.1.100:8554/stream
 * @param[IN] timeo_ms
 *  连接超时，传入 -1 表示使用最长超时，0 表示最短超时，其余为超时时间（单位：毫秒）。
 *
 * @retval AK_OK
 *  操作成功。
 * @retval AK_ERR_INVAL_OBJECT
 *  操作失败，传入非法对象。
 * @retval AK_ERR_NOT_ACCESS
 *  操作成功，地址连接失败。
 *
 */
AK_API AK_int akae_rtsp_client_connect (AK_Object Object, AK_chrptr url, AK_ssize timeo_ms);


/**
 * 断开链接。
 */
AK_API AK_int akae_rtsp_client_disconnect (AK_Object Object);

/**
 * 获取流媒体数据。\n
 * 该接口需要调用足够快以避免由于模块内内存不足导致丢数据的问题。
 * 如果上层业务有占用本接口获取数据的风险，可以稍微增大 @ref akae_rtsp_client_create() 传入内存分配器的可用内存大小。
 *
 * @param[OUT] Rtp
 * @param[OUT] data
 * @param[IN] timeo
 *  等待超时时间（单位：微秒），小于 0 表示一直等待，等于 0 表示异步并返回，大于 0 表示等待的时间。
 *
 * @return
 *  操作成功，数据属性参数以及起始地址通过 @ref Rtp 和 @ref data 返回，并返回数据长度，\n
 *  如果到 @ref timeo 超时时间，缓冲中没有数据，会返回 0，\n
 *  操作失败返回对应错误码。
 *
 * @retval AK_ERR_INVAL_OBJECT
 *  操作错误，可能传入了一个错误的对象。
 * @retval AK_ERR_INVAL_PARAM
 *  操作错误，可能传入错误的参数。
 * @retval AK_ERR_INVAL_OPERATE
 *  操作错误，上一个调用周期未调用 @ref akae_rtsp_client_release_stream() 释放所使用的数据。
 */
AK_API AK_ssize akae_rtsp_client_wait_stream (AK_Object Object, AK_RtpHeadfield *Rtp, AK_bytptr *data, AK_size timeo);

/**
 * @ref akae_rtsp_client_wait_stream() 的快速调用。
 */
#define akae_rtsp_client_get_stream(__Object, __Rtp, __data) akae_rtsp_client_wait_stream(__Object, __Rtp, __data, 0)


/**
 * 释放流媒体数据。
 * 与 @ref akae_rtsp_client_wait_stream() 对应。
 *
 */
AK_API AK_int akae_rtsp_client_release_stream (AK_Object Object);

/**
 * 获取当前客户端状态信息。
 */
AK_API AK_int akae_rtsp_client_status (AK_Object Object, AK_RtspClientStatus *Status);


/**
 *
 * @param Object
 * @param time_ms
 * @param data
 * @param datalen
 *
 * @retval AK_ERR_INVAL_OBJECT
 *  操作失败，可能传入了一个错误的对象。
 * @retval AK_ERR_INVAL_PARAM
 *  操作失败，可能传入了无效的参数。
 * @retval AK_ERR_NOT_ACCESS
 *  操作失败，当前服务端连接不支持反向音频传输。
 * @retval AK_ERR_PEER_BROKEN
 *  操作失败，链接断开。
 *
 */
AK_API AK_int akae_rtsp_client_send_audio (AK_Object Object, AK_size time_ms, AK_voidptr data, AK_size datalen);


AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_RTSPCLIENT_H_
