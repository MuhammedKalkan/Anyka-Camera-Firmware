/**
 * RTP 队列模块，用于处理 RTSP 服务中的数据分发。
 * 内部会使用 akae_qworm 模块。
 *
 */

#include <akae_malloc.h>
#include <akae_rtp.h>
#include <akae_aac.h>


#if !defined(AKAE_RTSPQUEUE_H_)
# define AKAE_RTSPQUEUE_H_
AK_C_HEADER_EXTERN_C_BEGIN

/**
 * RTP 读列读用户句柄。
 */
#define AK_RtpQueueReader AK_handle


typedef struct _AK_RtpQueueStatus {

	/// 队列写入数据的大小。
	AK_uint32 bytes;

	/// 队列写入数据的速率，单位 Bytes/s。
	AK_uint32 byterate;

	/// 队列内存用量。
	AK_uint32 memory;

} AK_RtpQueueStatus;


/**
 * 创建一个 RTSP 队列。
 *
 * @param[IN] payload_type
 *  净荷数据的类型。
 */
AK_API AK_Object akae_rtp_queue_create (AK_Object Malloc, AK_int payload_type);

/**
 * 释放一个 RTSP 队列
 *
 */
AK_API AK_int akae_rtp_queue_release (AK_Object Object);


/**
 * 获取缓冲设定的载荷类型，该类型从 @ref akae_rtp_queue_create() 传入。
 */
AK_API AK_int akae_rtp_queue_payload_type (AK_Object Object);



/**
 * @ref akae_rtp_queue_release() 快速调用。
 */
#define akae_rtp_queue_release_clear(__Object) \
	({\
		AK_int const res = akae_rtp_queue_release (__Object);\
		if (AK_OK == res) {\
			__Object = AK_null;\
		}\
		res;\
	})

/**
 * 获取 H.264 缓冲队列的 SPS 包数据。
 *
 * @param[OUT] stack
 *  获取 SPS 的内存起始地址。
 *
 * @param[IN] stacklen
 *  获取 SPS 的内存最大可用长度。
 *
 * @return
 *  获取成功返回 SPS 的数据长度，SPS 数据存放在 @ref stack 所在内存地址，失败返回对应的错误码。\n
 *  注意：如果截至用户调用该接口时仍未缓冲到 SPS 数据，该接口返回 0。
 *
 * @retval AK_ERR_NOT_SUPPORT
 *  获取失败，该队列不支持获取 H.264 的 SPS 数据。
 * @retval AK_ERR_OUT_OF_MEM
 *  获取失败，@ref stacklen 传入内存长度不足返回 SPS 数据。
 *
 */
AK_API AK_ssize akae_rtp_queue_get_h264_sps (AK_Object Object, AK_bytptr stack, AK_size stacklen);

/**
 * 获取 H.264 缓冲队列的 PPS 包数据。
 * 参考 @ref akae_rtp_queue_get_h264_sps()。
 *
 */
AK_API AK_ssize akae_rtp_queue_get_h264_pps (AK_Object Object, AK_bytptr stack, AK_size stacklen);

/**
 * 获取 H.265 缓冲队列的 SPS、PPS 包数据。
 */
#define akae_rtp_queue_get_h265_sps akae_rtp_queue_get_h264_sps
#define akae_rtp_queue_get_h265_pps akae_rtp_queue_get_h264_pps


/**
 * 获取 H.265 缓冲队列的 VPS 包数据。
 * 参考 @ref akae_rtp_queue_get_h264_sps()。
 *
 */
AK_API AK_ssize akae_rtp_queue_get_h265_vps (AK_Object Object, AK_bytptr stack, AK_size stacklen);

/**
 * 获取 AAC 缓冲队列的 ADTS 数据。
 *
 */
AK_API AK_ssize akae_rtp_queue_get_aac_adts (AK_Object Object, AK_AacAdtsFixedHeader *Header, AK_bytptr stack, AK_size stacklen);


/**
 * 获取队列当前状态。
 */
AK_API AK_int akae_rtp_queue_status (AK_Object Object, AK_RtpQueueStatus *Status);


/**
 * 向 RTP 队列追加净荷数据。
 */
AK_API AK_int akae_rtp_queue_add_payload (AK_Object Object, AK_uint32 time_ms, AK_bytptr data, AK_size datalen);

/**
 * 获取一个读序列用户。
 *
 * @return
 *  返回一个用户句柄，失败返回 AK_INVAL_HANDLE。
 */
AK_API AK_RtpQueueReader akae_rtp_queue_get_reader (AK_Object Object);


/**
 * 用户句柄使用完以后回收到平台。
 */
AK_API AK_int akae_rtp_queue_put_reader (AK_Object Object, AK_RtpQueueReader reader);


/**
 * 从队列中获取分块的 RTP 包。
 *
 * @verbatim
 *
 *  从接口返回的 @ref data 指向一片连续的内存内存空间，内存空间开始是一个 RTP 的头域结构。
 *  紧挨着 RTP 头域结构以后是一片 RTP 的净荷数据内容，这块连续内存的长度在获取成功后通过返回值返回。
 *
 *  +----------------+ <-- @ref data
 *  | RTP Headfield  |
 *  |                |
 *  +----------------+
 *  |                |
 *  |    RTP Data    |
 *  |                |
 *  +----+-----------+
 *
 * @endverbatim
 *
 */
AK_API AK_ssize akae_rtp_queue_get_rtp (AK_RtpQueueReader reader, AK_boolean peek, AK_uint32 *time_ms, AK_bytptr *data);






AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_RTSPQUEUE_H_
