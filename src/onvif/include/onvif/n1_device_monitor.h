
/**
 * @brief
 *  N1 对接系统监视相关事件及接口定义。
 * @details
 *  用户通过此模块实现对系统监视 的相关事件响应，实现对应功能。
 *
 */

#include "n1_device.h"

#ifndef NK_N1_DEVICE_MONITOR_H_
#define NK_N1_DEVICE_MONITOR_H_
NK_CPP_EXTERN_BEGIN


/**
 * @brief
 *  相关响应事件定义。
 */
typedef struct Nk_N1DeviceEventMonitor {

	/**
	 * @brief
	 *  获取 CPU 占用率。
	 *
	 * @return
	 *  返回 CPU 占用率千分率的整数，范围 0 - 1000，如 50.5% 则返回 505。
	 *
	 */
	NK_Size
	(*onGetCPUUsage)(NK_PVoid ctx);

	/**
	 * @brief
	 *  获取内存占用率。
	 *
	 */
	NK_Size
	(*onGetMemoryUsage)(NK_PVoid ctx);


} NK_N1DeviceEventMonitor;

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
NK_N1Device_Monitor(NK_N1DeviceEventMonitor *Event);



NK_CPP_EXTERN_END
#endif /* NK_N1_DEVICE_MONITOR_H_ */
