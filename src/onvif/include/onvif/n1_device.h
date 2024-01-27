
/**
 * N1 设备 API 接口。
 */

#include "n1_def.h"

#ifndef NK_N1_DEVICE_H_
#define NK_N1_DEVICE_H_
NK_CPP_EXTERN_BEGIN


#define NK_N1_DEV_EXCEP_CLASS_READ_FRAME_TIMEO  (0x10010) ///< 读帧超时异常。


/**
 * @brief
 *  N1 设备异常定义。
 */
typedef struct Nk_N1DeviceException {

	/// 异常的分类。
	NK_Size classify;

	/// classify 的值为 NK_N1_DEV_EXCEP_CLASS_READ_FRAME_TIMEO。
	struct {
		/// 超时的通道序号。
		NK_Int chid, streamid;
		/// 超时的时间。
		NK_Size seconds;

	} ReadFrameTimeout;

} NK_N1DeviceException;


/**
 * @brief N1 设备模块运行时上下文。
 */
typedef struct Nk_N1Device {

	/**
	 * 当前设备的序列号。\n
	 * 用户在初始化的时候传入，作为此设备在局域网中连接的唯一性。\n
	 * 若不设置此值，则内部会随机产生一个字符串作为当前设备的唯一标识。\n
	 * 版本 > 1.8.0，之后不再使用，\n
	 */
	NK_Char device_id[128];

	/**
	 * 云设备序列号。\n
	 * 该序列号是设备在互联网通讯上唯一身份码，在生产时统一分配并记录在设备当中。\n
	 * 版本 > 1.8.0，之后不再使用。
	 */
	NK_Char cloud_id[32];

	/**
	 * 模块用于监听网络事件的端口。\n
	 * 由于模块使用网络连接方式为 TCP，因此配置此端口时注意不能与模块以外的其他 TCP 绑定端口冲突。\n
	 * 建议使用 1024 以上的端口。
	 */
	NK_UInt16 port;

	/**
	 * 用户上下文。\n
	 * 用于模块触发事件时与用户外部共享数据。
	 */
	NK_PVoid user_ctx;


	struct {

		/**
		 * @brief
		 *  获取设备能力集事件。
		 * @details
		 *  模块初始化时会出发此方法获取设备能力集，当设备未实现此事件时，\n
		 *  模块内部会指定缺省值，详见 @ref NK_N1DeviceCapabilities 定义。
		 * @param ctx [in,out]
		 *  用户事件上下文，在方法 @ref NK_N1Device_Init() 调用时传入。
		 * @param Capabilities [out]
		 *  设备能力集。
		 *
		 */
		NK_Void
		(*onCapabilities)(NK_PVoid ctx, NK_N1DeviceCapabilities *Capabilities);


		/**
		 * 流媒体现场抓图事件。\n
		 * 客户端需要预览摄像机现场图片的时候会出发此接口向设备获取一张现场缩略图。\n
		 * 缩略图只支持 JPEG 文件格式。
		 *
		 * @param[in]		channel_id		抓图通道，从 0 开始。
		 * @param[in]		width			图片宽度。
		 * @param[in]		height			图片高度。
		 * @param[in]		pic				图片缓冲。
		 * @param[in,out]	size			缓冲/图片大小。
		 *
		 * @retval NK_N1_ERR_NONE					现场抓图成功。
		 * @retval NK_N1_ERR_DEVICE_BUSY			设备忙碌，客户端有可能会定时重试。
		 * @retval NK_N1_ERR_DEVICE_NOT_SUPPORT		设备不支持此抓图功能，客户段将不再对该设备进行抓图。
		 *
		 */
		NK_N1Error
		(*onLiveSnapshot)(NK_Int channel_id, NK_Size width, NK_Size height, NK_PByte pic, NK_Size *size);


		/**
		 * @brief
		 *  流媒体直播连接事件。
		 * @details
		 *  客户端连接时会触发此事件。\n
		 *  实现时根据传入的 Session::channel_id 和 Session::stream_id 对应设备情况，\n
		 *  初始化会话 @ref Session 数据结构，\n
		 *  并根据具体实现情况返回相对的数值引导模块做出响应的响应。\n
		 *  其中 @ref Session::user_session 用于保留用户会话，\n
		 *  实现时可以保留实施媒体的数据源相关信息。\n
		 *
		 * @param Session [in,out]
		 *  会话上下文，模块通过会话上下文与实现共享数据。
		 * @param ctx [in,out]
		 *  用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
		 *
		 * @retval NK_N1_ERR_NONE
		 *  连接成功，客户端继续获取数据媒体数据。
		 * @retval NK_N1_ERR_DEVICE_BUSY
		 *  设备忙碌，当用户媒体源资源请求失败时返回此值。
		 * @retval NK_N1_ERR_DEVICE_NOT_SUPPORT
		 *  设备不支持此连接请求，当客户端请求的通道和码流超出设备提供的范围返回此致。
		 * @retval NK_N1_ERR_NOT_AUTHORIZATED
		 *  用户校验失败，请求客户端对应用户不具备此媒体直播权限。
		 *
		 */
		NK_N1Error
		(*onLiveConnected)(NK_N1LiveSession *Session, NK_PVoid ctx);

		/**
		 * @brief
		 *  流媒体直播断开事件。
		 * @details
		 *  客户端断开连接时模块会触发此事件。\n
		 *
		 * @param Session [in,out]
		 *  会话上下文，模块通过会话上下文与实现共享数据。
		 * @param ctx [in,out]
		 *  用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
		 *
		 * @retval NK_N1_ERR_NONE
		 *  断开成功，客户端不再获取数据媒体数据。
		 *
		 */
		NK_N1Error
		(*onLiveDisconnected)(NK_N1LiveSession *Session, NK_PVoid ctx);

		/**
		 * 流媒体直播读取事件。\n
		 * 当流媒体被点播时，客户端获取媒体帧数据会触发该事件。
		 *
		 * @param[in,out]	Session			会话上下文，模块通过会话上下文与实现共享数据。
		 * @param[in,out]	ctx				用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
		 * @param[out]		payload_type	读取的数据载体类型。
		 * @param[out]		ts_ms			读取的数据时间戳（单位：毫秒）。
		 * @param[out]		data_r			读取的数据所在内存的地址。
		 *
		 * @retval 大于0								读取成功，读取的数据长度。
		 * @retval 等于0								读取失败，没有数据可读。
		 * @retval 小于0								读取失败，读取时发生错误，需要退出本会话。
		 *
		 */
		NK_SSize
		(*onLiveReadFrame)(NK_N1LiveSession *Session, NK_PVoid ctx,
				NK_N1DataPayload *payload_type, NK_UInt32 *ts_ms, NK_PByte *data_r, NK_N1VideoFrameType *frametype);

		/**
		 * 流媒体直播读取结束事件。\n
		 * 在触发事件 @ref onLiveReadFrame() 后库内部会引用数据的内存地址，\n
		 * 当使用完这部分数据以后会触发此事件，便于用户释放引用的数据资源。
		 *
		 *
		 * @param[in,out]	Session			会话上下文，模块通过会话上下文与实现共享数据。
		 * @param[in,out]	ctx				用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
		 * @param[in]		data_r			触发事件 @ref onLiveReadFrame() 时引用的数据内存地址。
		 * @param[in]		size			触发事件 @ref onLiveReadFrame() 时引用的数据大小。
		 *
		 * @retval NK_N1_ERR_NONE					操作成功。
		 */
		NK_N1Error
		(*onLiveAfterReadFrame)(NK_N1LiveSession *Session, NK_PVoid ctx,
				NK_PByte *data_r, NK_Size size);

		/**
		 * @brief
		 *  请求直播连接事件。
		 * @details
		 *  当对应通道码流（见 @ref chid @ref streamid）的第一个客户请求连接接入时，会出发此事件，通知用户该码流直播开始。\n
		 *
		 * @param ctx [in,out]
		 *  用户事件上下文，在方法 @ref NK_N1Device_Init() 调用时传入。
		 * @param chid [in]
		 *  码流连接对应多媒体的通道号，按逻辑顺序从 0 开始递增。
		 * @param streamid [in]
		 *  码流连接对应多媒体的通道下的码流号，按逻辑顺序从 0 开始递增。
		 *
		 */
		NK_Void
		(*onAttachStream)(NK_PVoid ctx, NK_Int chid, NK_Int streamid);

		/**
		 * @brief
		 *  断开直播连接事件。
		 * @details
		 *  当对应通道码流（见 @ref chid @ref streamid）的最后一个客户请求连接接入时，会出发此事件，通知用户该码流直播已经停止。\n。
		 *
		 * @param ctx [in,out]
		 *  用户事件上下文，在方法 @ref NK_N1Device_Init() 调用时传入。
		 * @param chid [in]
		 *  码流连接对应多媒体的通道号，按逻辑顺序从 0 开始递增。
		 * @param streamid [in]
		 *  码流连接对应多媒体的通道下的码流号，按逻辑顺序从 0 开始递增。
		 *
		 */
		NK_Void
		(*onDetachStream)(NK_PVoid ctx, NK_Int chid, NK_Int streamid);

		/**
		 * @brief
		 *  码流建议事件。
		 * @details
		 *  当模块对当前码流网络流畅性有建议行调整的时候会触发此事件。
		 *
		 * @param ctx [in,out]
		 *  用户事件上下文，在方法 @ref NK_N1Device_Init() 调用时传入。
		 * @param chid [in]
		 *  通道 ID 号，从 0 开始。
		 * @param streamid [in]
		 *  码流 ID 号，从 0 开始。
		 * @param kbps [in]
		 *  建议码流值，单位 kbps。
		 *
		 */
		NK_Void
		(*onRecommedStream)(NK_PVoid ctx, NK_Int chid, NK_Int streamid, NK_Int kbps, NK_Int fps);

		/**
		 * @brief
		 *  监听端口变更事件。
		 * @details
		 *  当外部通讯使端口变更时会出发此事件，用户不需要干预端口变更实现，\n
		 *  只需要把新的端口保存到配置，用于下次系统启动时初始化。
		 *
		 * @param[in,out] ctx
		 *  用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
		 * @param[in] port
		 *  变更的端口号。
		 *
		 */
		NK_Void
		(*onPortChanged)(NK_PVoid ctx, NK_UInt16 port);

		/**
		 * 局域网配置事件。
		 *
		 * @param[in,out]	ctx				用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
		 * @param[in]		set_or_get		设置/获取标识，当此值为 True 时候 Setup 为传入参数，反之为传出参数。
		 * @param[in,out]	Setup			设置数据结构。
		 *
		 * @retval NK_N1_ERR_NONE					配置或操作成功。
		 * @retval NK_N1_ERR_DEVICE_NOT_SUPPORT		设备不支持该配置或操作。
		 * @retval NK_N1_ERR_INVALID_PARAM			配置或操作传入了无效的参数。
		 * @retval NK_N1_ERR_NOT_AUTHORIZATED		用户校验失败，请求客户端对应用户不具备此设置权限。
		 */
		NK_N1Error
		(*onLanSetup)(NK_PVoid ctx, NK_Boolean set_or_get, NK_N1LanSetup *Setup);


		/**
		 * @brief
		 *  检测有线网络链接。
		 */
		NK_Boolean
		(*onDetectRJ45Connected)(NK_PVoid ctx);

		/**
		 * @brief
		 *  获取视频编码器参数。
		 * @details
		 *
		 *
		 * @param ctx [in,out]
		 *  用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
		 * @param chid [in]
		 *  通道 ID，从 0 开始。
		 * @param streamid [in]
		 *  码流 ID，从 0 开始。
		 * @param Encoder [out]
		 *  视频编码器数据结构。
		 *
		 * @return
		 *  获取成功返回 True，并在 @ref Encoder 返回具体参数，否则返回 False。
		 *
		 */
		NK_Boolean
		(*onGetVideoEncoder)(NK_PVoid ctx, NK_Int chid, NK_Int streamid, NK_N1VideoEncoder *Encoder);


		/**
		 * @brief
		 *  设置视频编码器参数。
		 * @details
		 *  详见 @ref onGetVideoEncoder()。
		 *
		 * @return
		 *  设置成功返回 True，否则返回 False。
		 */
		NK_Boolean
		(*onSetVideoEncoder)(NK_PVoid ctx, NK_Int chid, NK_Int streamid, NK_N1VideoEncoder *Encoder);

		/**
		 * @brief
		 *  获取红外滤光片工作模式事件。
		 * @details
		 *  根据当前红外滤光片工作模式返回对应结果。
		 *
		 * @param ctx [in,out]
		 *  用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
		 * @param chid [in]
		 *  视频输入通道 ID，从 0 开始。
		 * @param mode [out]
		 *  滤光片工作模式。
		 * @param writable [out]
		 *  如果滤光片不能设置此变量返回只读标识 NK_False。
		 *
		 */
		NK_Void
		(*onGetIRCutFilter)(NK_PVoid ctx, NK_Int chid, NK_N1IRCutFilterMode *mode, NK_Boolean *writable);

		/**
		 * @brief
		 *  设置红外滤光片工作模式事件。
		 * @details
		 *  根据指定红外滤光片工作模式设置。
		 *
		 * @param ctx [in,out]
		 *  用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
		 * @param chid [in]
		 *  视频输入通道 ID，从 0 开始。
		 * @param mode [in]
		 *  滤光片工作模式。
		 *
		 */
		NK_Void
		(*onSetIRCutFilter)(NK_PVoid ctx, NK_Int chid, NK_N1IRCutFilterMode mode);

		/**
		 * @brief
		 *  保留事件。
		 */
		NK_Void
		(*onGetTime)(NK_PVoid ctx);

		/**
		 * @brief
		 *  保留事件。
		 */
		NK_Void
		(*onSetTime)(NK_PVoid ctx);

		/**
		 * @brief
		 *  保留事件。
		 */
		NK_Void
		(*onGetEther)(NK_PVoid ctx, NK_Boolean wifi, NK_N1EthConfig *EthCfg);

		/**
		 * @brief
		 *  保留事件。
		 */
		NK_Void
		(*onSetEther)(NK_PVoid ctx, NK_Boolean wifi, NK_N1EthConfig *EthCfg);

		/**
		 * @brief
		 *  设备异常事件。
		 * @details
		 *
		 * @param ctx [in,out]
		 *  用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
		 * @param Excp [in]
		 *  异常情况数据结构。
		 */
		NK_Void
		(*onCatchException)(NK_PVoid ctx, NK_N1DeviceException *Excp);

		/**
		 * @brief
		 *  设备复位事件。
		 * @details
		 *  当外部需要设备进行复位时，会触发此事件。
		 *
		 * @param ctx [in,out]
		 *  用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
		 *
		 */
		NK_Void
		(*onReset)(NK_PVoid ctx);

		/**
		 * @brief
		 *  设备重启事件。
		 * @details
		 *  当外部需要设备进行重启时，会触发此事件。
		 *
		 * @param ctx [in,out]
		 *  用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
		 *
		 */
		NK_Void
		(*onReboot)(NK_PVoid ctx);

		/**
		 * @brief
		 *  获取第三方唯一码。
		 * @details
		 *  与发现设备相关，第三方传入该设备出厂时的设备唯一码，\n
		 *  以确保局域网发现设备时的设备唯一性。
		 *
		 * @param ctx [in,out]
		 *  用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
		 * @param stack [out]
		 *  返回 UID 内存起始地址。
		 * @param stacklen [in]
		 *  返回 UID 内存大小。
		 *
		 */
		NK_Void
		(*onGet3rdUID)(NK_PVoid ctx, NK_PChar stack, NK_Size stacklen);

		/**
		 * @brief
		 *  强制设置获取I帧数据接口。
		 * @details
		 *  通知设备编码I帧数据进行传输。\n
		 *  设置强制编码一帧I帧数据。
		 *
		 * @param ctx [in,out]
		 *  用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
		 * @param chid [in]
		 *  通道 ID，从 0 开始。
		 * @param streamid [in]
		 *  码流 ID，从 0 开始。
		 * @param
		 *  返回空。
		 *
		 */
		NK_Void
		(*onMakeIFrame)(NK_PVoid ctx, NK_Int chid, NK_Int streamid);
		/**
		 * @brief
		 *  保留扩展事件。
		 */
		NK_Void
		(*reserved[8])(NK_PVoid ctx);

	} Event;

/**
 * 向下兼容定义。
 */
#define EventSet Event

} NK_N1Device;


/**
 * @brief
 *  获取 N1 版本号。
 * @brief
 *  用于判断二进制库文件与头文件是否匹配，避免由于数据结构大小不匹配而产生潜在的问题。
 *
 * @param ver_maj [out]
 *  与 @ref NK_N1_VER_MAJ 匹配比较。
 * @param ver_min [out]
 *  与 @ref NK_N1_VER_MIN 匹配比较。
 * @param ver_rev [out]
 *  与 @ref NK_N1_VER_REV 匹配比较。
 * @param ver_num [out]
 *  与 @ref NK_N1_VER_NUM 匹配比较。
 *
 * @retval
 *  返回版本号对应码，譬如 1.1.0.5 版本 返回 01010005。
 *
 */
NK_API NK_Size
NK_N1Device_Version(NK_UInt32 *ver_maj, NK_UInt32 *ver_min, NK_UInt32 *ver_rev, NK_UInt32 *ver_num);

/**
 * @macro
 *  版本有效性校验。
 */
#define NK_N1_VALIDATE_VERSION() \
	((NK_N1Device_Version(NK_Nil, NK_Nil, NK_Nil, NK_Nil) == NK_N1_VER_CODE()) ? NK_True : NK_False)


/**
 * @brief
 *  模块初始化接口。
 * @details
 *  在模块使用前必须调用此接口对模块进行初始化，内部调用 @ref NK_N1Device_InitEx(license, Device, 0) 方法。
 *
 * @param license [in]
 *  用户凭证。
 * @param Device [in]
 *  设备参数。
 *
 * @return
 *  初始化成功返回 0，否则返回 -1。
 *
 * @see NK_N1Device_InitV2()
 *
 */
NK_API NK_Int
NK_N1Device_Init(NK_PChar license, NK_N1Device *Device);


/**
 * @brief
 *  模块初始化扩展接口。
 * @details
 *  使用此接口实现模块初始化，区别于 @ref NK_N1Device_InitEx() 接口，\n
 *  此接口通过参数 @ref memsz 初始化模块是否为媒体缓冲模式，\n
 *  当参数 @ref memsz 配置为 0 的时使用非缓冲模式，用户需要自己处理流媒体缓冲方法，\n
 *  当参数 @ref memsz 配置为有效的大小时，用户在媒体请求时通过 @ref NK_N1Device_SendH264() 等类似接口发送实时视频数据到模块内部缓冲，\n
 *  很大一定程度上可以降低用户开发难度。
 *
 * @param license [in]
 *  用户凭证文件路径。
 * @param memsz [in]
 *  媒体缓冲大小，为 0 时表示模块内不使用媒体缓冲，不为 0 时指定内部媒体缓冲的总大小，\n
 *  内部会根据传入的值从堆中申请相对应的堆内存用于媒体缓冲，大于 1M 时有效。
 * @param Device [in]
 *  设备参数。
 *
 * @return
 *  初始化成功返回 0，否则返回 -1。
 */
NK_API NK_Int
NK_N1Device_InitV2(NK_PChar license, NK_Size memsz, NK_N1Device *Device);

/**
 * @macro
 *  历史兼容接口。
 */
#define NK_N1Device_InitEx(__license, __Device, __opt) NK_N1Device_InitV2(__license, 0, __Device)

/**
 * @macro
 *  历史兼容接口。
 */
#define NK_N1Device_InitEx2(__license, __memsz, __Device, __opt) NK_N1Device_InitV2(__license, __memsz, __Device)

/**
 * 销毁 N1 设备运行环境。\n
 * 调用此接口前需要先调用 @ref NK_N1Device_Init 接口此接口才能成功返回。\n
 * 当接口成功返回 0 时，可以通过 @ref user_ctx_r 获取 @ref NK_N1Device_Init 传入的用户上下文。\n
 * 调用者可一个根据设计需求释放传入的用户上下文相关资源。
 *
 * @param[out]			Device				返回初始化时 @ref NK_N1Device_Init 传入的用户上下文。
 *
 * @return		成功返回 0，失败返回 -1。
 */
NK_API NK_Int
NK_N1Device_Destroy(NK_N1Device *Device);


/**
 * @brief
 *  向客户端发送通知信息。
 *
 * @param chid [in]
 *  媒体通道序列号，从 0 开始
 *
 * @return
 *  发送成功返回 True，失败返回 False，\n
 *  可能由于当前通知过多，或者类型不支持。
 */
NK_API NK_Boolean
NK_N1Device_Notify(NK_Int chid, NK_N1Notification *Notif);

/**
 * @brief
 *  SDK 运行时长（单位：纳秒）。
 * @details
 *  获取 SDK 运行时长，从 @ref NK_N1Device_Init() 调用后开始计数（单位：纳秒）。
 *
 * @return
 *  运行时长（单位：纳秒）。
 */
NK_API NK_Size64
NK_N1Device_UptimeNano();

#if 0

/**
 * @brief
 *  SDK 运行时长（单位：微秒）。
 */
static NK_Size64
NK_N1Device_UptimeMacro() {
	return (NK_N1Device_UptimeNano() + 500) / 1000;
}


/**
 * @brief
 *  SDK 运行时长（单位：毫秒）。
 */
static NK_Size64
NK_N1Device_UptimeMilli() {
	return (NK_N1Device_UptimeMacro() + 500) / 1000;
}

/**
 * @brief
 *  SDK 运行时长（单位：秒）。
 */
/*static NK_Size
NK_N1Device_Uptime() {
	return (NK_N1Device_UptimeMilli() + 500) / 1000;
}*/

#endif


/**
 * @brief
 *  获取 N1 设备序列号。
 *
 * @return
 *  返回 N1 设备序列号，如没有有效认证序列号则返回长度为 0 的字符串。
 */
NK_API NK_PChar
NK_N1Device_GetID();


/**
 * @brief
 *  获取 N1 设备序列号。
 * @details
 *  获取 N1 设备序列号同时获取该序列号的授权年月。
 *
 * @param authyear [out]
 *  授权年份。
 * @param authmonth [out]
 *  授权月份。
 *
 * @return
 *  返回 N1 设备序列号，如没有有效认证序列号则返回长度为 0 的字符串。
 */
NK_API NK_PChar
NK_N1Device_GetIDV2(NK_Size *authyear, NK_Size *authmonth);



/**
 * @brief
 *  向缓冲发送一帧 H.264 数据。
 * @details
 *  在媒体缓冲模式下，当设备有网络请求媒体时，会触发 @ref Live::onAttachStream 事件，\n
 *  用户需要调用此接口不断发送 H.264 视频编码实时数据（如果处于 H.264 的媒体点播系统）。
 *
 * @param sessionid [in]
 *  会话 ID，保留参数，此处一直为 0。
 * @param chid [in]
 *  直播通道 ID，从 0 开始。
 * @param streamid [in]
 *  直播码流 ID，从 0 开始。
 * @param ts_us [in]
 *  该帧相对时间戳，单位微秒（1/1000000 秒）。
 * @param keyFrame [in]
 *  关键帧标识，当前传入数据为关键帧时为 True，否则为 False。
 * @param data [in]
 *  帧数据所在内存的起始地址。
 * @param len [in]
 *  帧数据的数据长度。
 *
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
NK_API NK_Boolean
NK_N1Device_SendH264(NK_Int sessionid, NK_Int chid, NK_Int streamid, NK_UInt64 ts_us, NK_Boolean keyFrame, NK_PVoid data, NK_Size len, NK_N1VideoFrameType frametype);

/**
 * @macro
 *  直播发送方法，只用于直播视频，并由模块定义时间戳。
 */
#define NK_N1Device_SendNoTsH264(__chid, ___streamid, __keyFrame, __data, __len, frametype) \
	NK_N1Device_SendH264(0, __chid, ___streamid, NK_N1Device_UptimeMacro(), __keyFrame, __data, __len, frametype);

/**
 * @brief
 *  向缓冲发送一帧 HEVC 数据。
 * @details
 *  在媒体缓冲模式下，当模块请求媒体业务时，会触发 @ref Live::onAttachStream 事件，\n
 *  用户需要调用此接口不断发送 HEVC 视频编码实时数据（如果处于 HEVC 的媒体点播系统）。
 *
 * @param sessionid [in]
 *  会话 ID，保留参数，此处一直为 0。
 * @param chid [in]
 *  直播通道 ID，从 0 开始。
 * @param streamid [in]
 *  直播码流 ID，从 0 开始。
 * @param ts_us [in]
 *  该帧相对时间戳，单位微秒（1/1000000 秒）。
 * @param keyFrame [in]
 *  关键帧标识，当前传入数据为关键帧时为 True，否则为 False。
 * @param data [in]
 *  帧数据所在内存的起始地址。
 * @param len [in]
 *  帧数据的数据长度。
 *
 * @return
 *  发送成功返回 True，发送失败返回 False。
 *
 */
NK_API NK_Boolean
NK_N1Device_SendHEVC(NK_Int sessionid, NK_Int chid, NK_Int streamid, NK_UInt64 ts_us, NK_Boolean keyFrame, NK_PVoid data, NK_Size len, NK_N1VideoFrameType frametype);


/**
 * @macro HEVC 别名方法定义。
 */
#define NK_N1Device_SendH265 NK_N1Device_SendHEVC

/**
 * @macro
 *  直播发送方法，只用于直播视频，并由模块定义时间戳。
 */
#define NK_N1Device_SendNoTsHEVC(__chid, ___streamid, __keyFrame, __data, __len, __frametype) \
	NK_N1Device_SendHEVC(0, __chid, ___streamid, NK_N1Device_UptimeMacro(), __keyFrame, __data, __len, __frametype);
#define NK_N1Device_SendNoTsH265 NK_N1Device_SendNoTsHEVC

/**
 * @brief
 *  发送一个 G.711 音频包。
 * @details
 *  在媒体缓冲模式下，当模块请求媒体业务时，使用此方法发送一个 G.711 音频包，\n
 *  只支持 8k - 16bits 采样率。
 *
 * @param sessionid [in]
 *  会话 ID，保留参数，此处一直为 0。
 * @param chid [in]
 *  直播通道 ID，从 0 开始。
 * @param ts_us [in]
 *  该帧相对时间戳，单位微秒（1/1000000 秒）。
 * @param data [in]
 *  帧数据所在内存的起始地址。
 * @param len [in]
 *  帧数据的数据长度。
 * @param options [in]
 *  保留选项，一直为 0。
 *
 * @return
 *  发送成功返回 True，发送失败返回 False。
 *
 */
NK_API NK_Boolean
NK_N1Device_SendG711(NK_Int sessionid, NK_Int chid, NK_UInt64 ts_us, NK_PVoid data, NK_Size len, NK_UInt32 options);

/**
 * @macro
 *  直播发送方法，只用于直播视频，并由模块定义时间戳。
 */
#define NK_N1Device_SendNoTsG711(__chid, __data, __len, __options) \
	NK_N1Device_SendG711(0, __chid, NK_N1Device_UptimeMacro(), __data, __len, __options);


NK_CPP_EXTERN_END
#endif /* NK_N1_DEVICE_H_ */
