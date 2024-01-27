
/**
 * @brief
 *  N1 对接 Onvif 相关事件及接口定义。
 * @details
 *  用户通过此模块实现对 Onvif 的相关事件响应，实现对应功能。
 *
 */

#include "n1_device.h"

#ifndef NK_N1_DEVICE_ONVIF_H_
#define NK_N1_DEVICE_ONVIF_H_
NK_CPP_EXTERN_BEGIN


/**
 * @brief
 *  相关响应事件定义。
 */
typedef struct Nk_N1DeviceEventOnvif {

	/**
	 * @brief
	 *  设置 IP 自适应开关。
	 *
	 * @details
	 *  IP 自适应是根据 Onvif 发现设备局限性设计的一套地址适应策略，\n
	 *  当设备接收到 Onvif 发出的发现设备报文时，会根据报文的来源地址修改本地地址以便形成 UDP 单播的回复路径。\n
	 *  此事件根据外部用户控制逻辑触发 IP 自适应功能的总开关。
	 *
	 * @param ctx [in]
	 *  用户上下文。
	 * @param on [in]
	 *  开关标识，NK_True 表示打开，NK_False 表示关闭。
	 *
	 */
	NK_Void
	(*onSetAutoIPAdapt)(NK_PVoid ctx, NK_Boolean on);

	/**
	 * @brief
	 *  获取 IP 自适应开关。
	 *
	 * @details
	 *  见 @ref onSetAutoIPAdapt，此事件根据外部用户控制逻辑触发获取 IP 自适应功能的总开关状态。
	 *
	 * @param ctx [in]
	 *  用户上下文。
	 *
	 * @return
	 *  返回开关状态标识，NK_True 表示打开，NK_False 表示关闭。
	 */
	NK_Boolean
	(*onGetAutoIPAdapt)(NK_PVoid ctx);

	/**
	 * @brief
	 *  活动 Onvif IP 自适应功能。
	 *
	 * @details
	 *  IP 自适应功能活动事件，在 IP 自适应功能开启（见 @ref onSetAutoIPAdapt）的前提下，\n
	 *  功能会由于外部因素对该功能进行临时转成非活动，通过触发此事件实现用户操作。
	 *
	 * @param ctx [in]
	 *  用户上下文。
	 * @param actived [in]
	 *  活动标识，NK_True 表示活动，NK_False 表示非活动。
	 *
	 */
	NK_Void
	(*onActiveAutoIPAdapt)(NK_PVoid ctx, NK_Boolean actived);

} NK_N1DeviceEventOnvif;

/**
 * @brief
 *  配置相关扩展事件。
 * @details
 *
 * @param Event [in]
 *  用户事件定义。
 *
 * @return
 *  配置成功返回 0，否则返回 -1。
 */
NK_API NK_Int
NK_N1Device_Onvif(NK_N1DeviceEventOnvif *Event);



NK_CPP_EXTERN_END
#endif /* NK_N1_DEVICE_ONVIF_H_ */
