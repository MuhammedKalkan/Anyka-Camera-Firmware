
/**
 * @brief
 *  N1 对接设备 IPC 路由器相关 API 接口定义。
 * @details
 *
 */

#include "n1_device.h"

#ifndef NK_N1_DEVICE_IPCR_H_
#define NK_N1_DEVICE_IPCR_H_
NK_CPP_EXTERN_BEGIN


/**
 * @brief
 *  N1 对接设备双向语音相关事件定义。
 */
typedef struct Nk_N1DeviceEventIPCR {

	/**
	 * @brief
	 *  设置/清除路由序列号。
	 * @details
	 *  设置当前连接的路由序列号，当 IPC 路由器配对时会出发此事件，用户需要保存该序列号便于查询，
	 *  在事件 @ref onGetRID 返回，\n
	 *  当 @ref rid 传入 Nil 时，表示清除该对应路由器序号连接。
	 *
	 * @param ctx [in,out]
	 *  事件上下文。
	 * @param rid [in]
	 *  路由序列号。
	 */
	NK_Void
	(*onSetRID)(NK_PVoid *ctx, NK_PChar rid);

	/**
	 * @brief
	 *  获取已配对的路由序列号。
	 * @details
	 *  如果当前设备已经配对一个 IPC 路由器，返回该路由器序列号，如果未进行路由器配对，此事件直接返回。
	 *
	 * @param ctx [in,out]
	 *  事件上下文。
	 * @param stack [out]
	 *  序列号结果返回值内存起始地址。
	 * @param stacklen [in]
	 *  序列号结果返回值内存大小。
	 */
	NK_Void
	(*onGetRID)(NK_PVoid *ctx, NK_PChar stack, NK_Size stacklen);

	/**
	 * @brief
	 *  已配对路由器心跳事件。
	 *
	 * @param ctx [in,out]
	 *  事件上下文。
	 *
	 */
	NK_Void
	(*onKeepAlive)(NK_PVoid *ctx);


} NK_N1DeviceEventIPCR;

/**
 * @brief
 *  配置 N1 设备生产相关事件。
 * @details
 *
 * @param Event [in]
 *  用户事件定义。
 *
 * @return
 *  配置成功返回 0，否则返回 -1。
 */
NK_API NK_Int
NK_N1Device_IPCR(NK_N1DeviceEventIPCR *Event);



NK_CPP_EXTERN_END
#endif /* NK_N1_DEVICE_IPCR_H_ */
