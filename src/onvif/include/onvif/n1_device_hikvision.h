
/**
 * @brief
 *  N1 对接海康威视相关事件及接口定义。
 * @details
 *  用户通过此模块实现对空中配对的相关事件响应，实现对应功能，\n
 *  模块通过接口 @ref NK_N1Device_Hikvision() 加载事件，\n
 *  该接口必须在 @ref NK_N1Device_Init() 调用成功后才能调用，否则失败。
 */

#include "n1_device.h"

#ifndef NK_N1_DEVICE_HIKVISION_H_
#define NK_N1_DEVICE_HIKVISION_H_
NK_CPP_EXTERN_BEGIN

/**
 * @brief
 *  事件定义。
 */
typedef struct Nk_N1DeviceEventHikvision {

	/**
	 * @brief
	 *  获取监听端口事件。
	 * @details
	 *
	 * @return
	 *  返回监听端口号。
	 */
	NK_UInt16
	(*onGetPort)(NK_PVoid ctx);

	/**
	 * @brief
	 *  设置监听端口事件。
	 * @details
	 *  当模块监听端口发生变更时会出发此事件，\n
	 *  用户根据变更的端口保存到配置中。
	 *
	 * @param ctx [in,out]
	 * @param port [in]
	 *  设置端口号事件，用户接收到此时间
	 *
	 */
	NK_Void
	(*onSetPort)(NK_PVoid ctx, NK_UInt16 port);


} NK_N1DeviceEventHikvision;

/**
 * @brief
 *  配置海康威视协议相关事件。
 * @details
 *
 * @param Event [in]
 *  用户事件定义。
 *
 * @return
 *  配置成功返回 0，否则返回 -1。
 */
NK_API NK_Int
NK_N1Device_Hikvision(NK_N1DeviceEventHikvision *Event);



NK_CPP_EXTERN_END
#endif /* NK_N1_DEVICE_HIKVISION_H_ */
