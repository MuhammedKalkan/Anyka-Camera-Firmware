
/**
 * @brief
 *  N1 对接海康协议和第三方协议公共接口功能的实现（用于实现协议搜索优先级策略）。
 * @details
 *  用户通过此模块实现有关海康协议和第三方协议公共接口的访问和设置，\n
 *  模块通过接口 @ref NK_N1Device_ProtocolComm() 加载事件，\n
 */

#include "n1_device.h"

#ifndef NK_N1_DEVICE_PROTOCOL_COMM_H_
#define NK_N1_DEVICE_PROTOCOL_COMM_H_
NK_CPP_EXTERN_BEGIN

typedef enum {
	NK_PROTOCOL_SEARCH_TYPE_ZW = 1,
	NK_PROTOCOL_SEARCH_TYPE_HIK,
	NK_PROTOCOL_SEARCH_TYPE_DH,
	NK_PROTOCOL_SEARCH_TYPE_XM,
	NK_PROTOCOL_SEARCH_TYPE_TST,
}NK_ePROTOCOL_SEARCH_TYPE;

typedef struct NK_Search_Device_Type{
	int type;
	struct timespec cpu_clock;   	//
	char mac[32];					//
}NK_stSearch_Device_Type,*NK_lpSearch_Device_Type;

/**
 * @brief
 *  事件定义。
 */
typedef struct Nk_N1DeviceEventProtocolComm {


	/**
	 * @brief
	 *  检测协议相关属性（时钟和mac等）。
	 * @details
	 *
	 * @return
	 *   0 不是相同设备发出，或者不是同一个时间发出; 1 同一个设备，同一个时间发出的搜索。 -1,错误
	 */
	NK_Int
	(*onCheck_type)(NK_PVoid ctx, NK_stSearch_Device_Type *device);

	/**
	 * @brief
	 *  设置协议公共接口相关属性(时钟和mac等)。
	 * @details
	 *
	 * @return
	 *  0设置成功，非零设置失败。
	 */

	NK_Int
	(*onSet_type)(NK_PVoid ctx, NK_stSearch_Device_Type *device);

} NK_N1DeviceEventProtocolComm;

/**
 * @brief
 *  配置海康协议和第三方协议交互相关事件。
 * @details
 *
 * @param Event [in]
 *  用户事件定义。
 *
 * @return
 *  配置成功返回 0，否则返回 -1。
 */
NK_API NK_Int
NK_N1Device_ProtocolComm(NK_N1DeviceEventProtocolComm *Event);



NK_CPP_EXTERN_END
#endif /* NK_N1_DEVICE_PROTOCOL_COMM_H_ */

