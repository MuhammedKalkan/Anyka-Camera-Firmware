/**
 * RFC 文档位置：https://tools.ietf.org/html/rfc6184
 *
 */


#include <akae_typedef.h>
#include <akae_socket.h>

#if !defined (AKAE_RTP_H_)
#define AKAE_RTP_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * 单个 RTP 包的 MTU 大小。
 */
#define AK_RTP_MTU_SZ          (1258)

/**
 * RTP 净荷数据的类型。
 */
#define AK_RTP_PT_UNDEF        (-1)
#define AK_RTP_PT_PCMU         (0)
#define AK_RTP_PT_RESERVED1    (1)
#define AK_RTP_PT_RESERVED2    (2)
#define AK_RTP_PT_GSM          (3)
#define AK_RTP_PT_G723         (4)
#define AK_RTP_PT_DVI4_8000    (5)
#define AK_RTP_PT_DVI4_16000   (6)
#define AK_RTP_PT_LPC          (7)
#define AK_RTP_PT_PCMA         (8)
#define AK_RTP_PT_G722         (9)
#define AK_RTP_PT_L161         (10) ///< 44.1khz ,2 channels  //10
#define AK_RTP_PT_L162         (11) ///< 44.1khz, 1 channel
#define AK_RTP_PT_QCELP        (12)
#define AK_RTP_PT_CN           (13)
#define AK_RTP_PT_MPA          (14)
#define AK_RTP_PT_G728         (15)
#define AK_RTP_PT_DVI4_11025   (16)
#define AK_RTP_PT_DVI4_22050   (17)
#define AK_RTP_PT_G729         (18)
#define AK_RTP_PT_CEIB         (25)
#define AK_RTP_PT_JPEG         (26)
#define AK_RTP_PT_H261         (31)
#define AK_RTP_PT_MPV          (32)
#define AK_RTP_PT_MP2T         (33)
#define AK_RTP_PT_H263         (34)
#define AK_RTP_PT_H264         (96)
#define AK_RTP_PT_H265         (96 + 0x10000)  ///< Distinguish to H.264
#define AK_RTP_PT_AAC          (97)

#define AK_RTP_PT_HEVC         (AK_RTP_PT_H265)



/**
 * 通过 PAYLOAD 序号获取名称。
 *
 * @return
 *  获取成功返回 PAYLOAD 的名称，失败返回 AK_null。
 */
AK_API AK_chrptr akae_rtp_payload_name (AK_int payload_type);


/**
 * 通过 PAYLOAD 名称获取序号。
 *
 * @return
 *  获取成功返回 PAYLOAD 的序号，失败返回 @ref AK_RTP_PT_UNDEFINE。
 */
AK_API AK_int akae_rtp_payload_type (AK_chrptr name);



#define AK_RTP_PSLICE          (1)
#define AK_RTP_ISLICE          (5)
#define AK_RTP_SEI             (6)
#define AK_RTP_SPS             (7)
#define AK_RTP_PPS             (8)

#define AK_RTP_STAP_A          (24) //single-time aggregation packet
#define AK_RTP_STAP_B          (25)
#define AK_RTP_MTAP16          (26) //multi-time aggregation packet
#define AK_RTP_MTAP24          (27)
#define	AK_RTP_FU_A            (28) //fragmentation unit
#define AK_RTP_FU_B            (29) //fragmentation unit

#define AK_RTP_H265_TAIL_N         (1)
#define AK_RTP_H265_TAIL_R         (2)
#define AK_RTP_H265_IDR_W_RADL     (19)
#define AK_RTP_H265_IDR_N_LP       (20)
#define AK_RTP_H265_VPS_NUT        (32)
#define AK_RTP_H265_SPS_NUT	       (33)
#define AK_RTP_H265_PPS_NUT	       (34)
#define AK_RTP_H265_AUD_NUT	       (35)
#define AK_RTP_H265_EOS_NUT	       (36)
#define AK_RTP_H265_EOB_NUT	       (37)
#define AK_RTP_H265_FD_NUT         (38)
#define AK_RTP_H265_PREFIX_SEI_NUT (39)
#define AK_RTP_H265_SUFFIX_SEI_NUT (40)
#define AK_RTP_H265_NUT_END        (47)

#define AK_RTP_H265_AP		48
#define AK_RTP_H265_FU		49






#pragma pack(push, 1)
typedef struct _AK_RtpHeadfield {

// 0                   1                   2                   3
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |V=2|P|X| CC|M|     PT      |       sequence number             |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                           timestamp                           |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |           synchronization source (SSRC) identifier            |
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// |            contributing source (CSRC) identifiers             |
// |                             ....                              |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    AK_byte      csrc_cnt     : 4;
    AK_byte      extension    : 1;
    AK_byte      padding      : 1;
    AK_byte      version      : 2;
    AK_byte      payload_type : 7;
    AK_byte      marker       : 1;
    AK_uint16    seqno;
    AK_uint32    timestamp;
    AK_uint32    ssrc;

    /// 这里定义忽略 CSRC。

} AK_RtpHeadfield;

typedef struct _AK_RtpOverRtspInterleaved {

	AK_char   dollar;
	AK_uint8  channel;
	AK_size16 length;

} AK_RtpOverRtspInterleaved;


typedef union _AK_NalUnitH264 {

	struct {

		AK_byte type               : 5;
		AK_byte nal_ref_idc        : 2;
		AK_byte forbidden_zero     : 1; // must be 0
	};

	AK_byte byte;

} AK_NalUnitH264;

/**
 * FU indicator
 */
#define AK_RtpFragUnitH264 AK_NalUnitH264
#define AK_RtpFragUnitH265 AK_NalUnitH265

/**
 * FU header
 */
typedef union _AK_RtpFragUnitHeader {

	struct {
		AK_byte type:6;
		AK_byte stop_bit:1;
		AK_byte start_bit:1;
	};

	AK_byte bytes;

} AK_RtpFragUnitHeader;

typedef union _AK_NalUnitH265 {

	struct {
		AK_uint16 tid : 3;
		AK_uint16 layer_id : 6;
		AK_uint16 type : 6;
		AK_uint16 forbidden_zero_bit : 1; // must be 0
	};

	AK_uint16 bytes;

} AK_NalUnitH265;


typedef struct _AK_RtpJpegHeader {

        AK_uint32  tspec : 8;       ///< type-specific field
        AK_uint32  off   : 24;      ///< fragment byte offset
        AK_uint8   type;            ///< id of jpeg decoder params
        AK_uint8   q;               ///< quantization factor (or table id)
        AK_uint8   width;           ///< frame width in 8 pixel blocks
        AK_uint8   height;          ///< frame height in 8 pixel blocks

} AK_RtpJpegHeader;


typedef struct _AK_RtpJpegHeaderQTable {

        AK_uint8  mbz;
        AK_uint8  precision;
        AK_uint16 length;

} AK_RtpJpegHeaderQTable;


//typedef union _AK_FUForH265 {
//
//	struct {
//		AK_char type:6;
//		AK_char stop_bit:1;
//		AK_char start_bit:1;
//	};
//
//	AK_char padding;
//
//} AK_FUForH265;


#pragma pack(pop)

/**
 * RTP 会话定义，\n
 * 对应 RTSP 每次 SETUP 都会产生一个 RTP 会话。
 */
typedef struct _AK_RtpSession {

	/// 分配器。
	AK_Object Malloc;

	/// 连接套接字 Socket 句柄。
	AK_Socket sock;

	/// Interleaved 通道号，当使用 TCP 传输时有效，使用 UDP 传输时该值为 -1。
	AK_int interleaved;

	/// 媒体采样时钟。
	AK_size clock_hz;

	/// RTP 对应 SSRC，初始化时创建，只读。
	AK_uint32 ssrc;

	/// 序列序号，初始化时为 0。
	AK_size seqno;

	/// 基准时间戳（单位：毫秒）。
	AK_uint32 baseTimestamp;

	/// 最新时间戳（单位：毫秒）。
	AK_uint32 lastestTimestamp;

	/// 数据计数。
	AK_uint32 packetNumber, octetBytes;

	/// 媒体码流相关数据对象定义，
	/// 对象体内包括用于流媒体码流的缓冲管道、临时变量等，
	/// 媒体获取的时，会先把媒体接收缓冲到数据结构内的管道中。
	/// 视频片管道，对于 H.264 与 H.265 视频数据在未接收到 marker 时无法清晰视频单帧大小。
	/// 因此在接收 H.264 与 H.265 时需要先缓冲到片管道，待完整接收到一帧以后再读取缓冲到媒体管道。
	struct {

		AK_size size; ///< 累计接收大小。
		AK_Object Buffer; ///< 针对 H.264 和 H.265 的二级片缓存。

	} RecvSlice;

} AK_RtpSession;


/**
 * 初始化基于 RTP 会话参数。
 *
 * @param[IN] sock
 *  套接字句柄，传入套接字必须是有效链接套接字，即有可达对端（已经成功调用 connect），否则返回失败。
 * @param[IN[ interleaved
 *  对于带 interleaved 的 TCP 连接信道。
 * @param[OUT] Session
 *  RTP 会话上下文，初始化成功后通过该变量返回。
 *
 * @return
 */
AK_API AK_int akae_rtp_session_init (AK_Object Malloc, AK_uint32 ssrc, AK_Socket sock, AK_int interleaved, AK_size clock_hz, AK_RtpSession *Session);

/**
 * 销毁 RTP 会话，与 @ref akae_rtp_session_init 相对。
 *
 */
AK_API AK_void akae_rtp_session_destroy (AK_RtpSession *Session);


/**
 * @brief
 *  发送一个 Slice 的 H.264 数据。
 * @details
 *  调用该接口时，接口内部不会进行 H.264 Slide 特征检测，\n
 *  直接把 @ref slice 所在内存位置的数据进行 RTP 打包并发送，\n
 *  用户须要保证 @ref slice 对应数据为完整的 H.264 Slice 数据（可以解码）。\n
 *  使用该接口可以进一步提高发送性能，适用于分片编码支持的编码器。
 *
 * @param[IN] Session
 *  会话句柄，通过 @ref akae_rtp_session_init() 初始化。
 * @param[IN] time_ms
 *  数据时间戳（单位：毫秒）。
 * @param[IN] data
 *  H.264 Slice 数据所在内存起始位置。
 * @param[IN] datalen
 *  H.264 Slice 数据长度。
 *
 * @return
 *  发送成功返回 AK_True，失败返回 AK_False。
 */
AK_API AK_boolean akae_rtp_session_send_h264_1slice (AK_RtpSession *Session, AK_uint32 time_ms, AK_bytptr data, AK_size datalen);


/**
 * @brief
 *  发送一个 Slice 的 H.265 片数据。
 */
AK_API AK_boolean akae_rtp_session_send_h265_1slice (AK_RtpSession *Session, AK_uint32 time_ms, AK_bytptr data, AK_size datalen);

AK_API AK_boolean akae_rtp_session_send_audio (AK_RtpSession *Session, AK_int payload_type, AK_uint32 time_ms, AK_bytptr data, AK_size datalen);

/**
 * 接收一个 RTP 数据包并缓存处理并缓冲。
 * 接收成功以后会缓冲到内部管道，@var RtpSession 内部的缓冲中。
 *
 * @param[IN] RtpSession
 *  当前接收 RTP 会话。
 *
 * @param[IN] rtp_len
 *  需要接收的 RTP 数据包长度。
 *
 * @return
 *  接收成功返回 AK_true，接收失败返回 AK_false。
 */
AK_API AK_boolean akae_rtp_session_recv_and_buffer (AK_RtpSession *RtpSession, AK_size rtp_len, AK_Object Buffer);






AK_C_HEADER_EXTERN_C_END
#endif /* AKAE_RTP_H_ */
