
/**
 * @brief
 *  N1 对接空中配对相关事件及接口定义。
 * @details
 *  用户通过此模块实现对空中配对的相关事件响应，实现对应功能，\n
 *  模块通过接口 @ref NK_N1Device_AirPair() 加载事件，\n
 *  该接口必须在 @ref NK_N1Device_Init() 调用成功后才能调用，否则失败。
 *
 */

#include "n1_device.h"

#ifndef NK_N1_DEVICE_AIRPAIR_H_
#define NK_N1_DEVICE_AIRPAIR_H_
NK_CPP_EXTERN_BEGIN


/**
 * @brief
 *  相关响应事件定义。
 */
typedef struct Nk_N1DeviceEventAirPair {

	/**
	 * 扫描无线热点事件。\n
	 * 当模块须要检测附近的 WiFi NVR 的时候触发此事件。\n
	 * 用户通过实现此接口，告知模块附近的 WiFi 热点信息。
	 *
	 * @param[in,out]	ctx				用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
	 * @param[out]		HotSpots		热点数据结构栈区缓冲。
	 * @param[in,out]	n_hotSpots		传入热点数据结构栈区的缓冲，传出实际扫描到热点的数量。
	 *
	 * @retval NK_N1_ERR_NONE					扫描热点成功，结果保存在 @ref HotSpots 数组中。
	 *
	 */
	NK_N1Error
	(*onScanWiFiHotSpot)(NK_PVoid ctx, NK_WiFiHotSpot *HotSpots, NK_Size *n_hotSpots);


	/**
	 * 连接无线热点事件。\n
	 * 对于支持无线连接的设备，当模块希望用户连接某个 NVR 设备的时候会触发此事件，\n
	 * 触发此事件时，同样会发起一个配置 @ref NK_N1LanSetup::WiFiNVR 的事件，\n
	 * 与 @ref onLanSetup 事件不同的时候，这里发起配置 @ref NK_N1LanSetup::WiFiNVR 的事件时，\n
	 * 用户不需要保存 @ref NK_N1LanSetup::WiFiNVR 的配置数据，\n
	 * 而 @ref onLanSetup 事件则须要用户保存 @ref NK_N1LanSetup::WiFiNVR 对应数据结构。\n
	 *
	 * @param[in,out]	ctx				用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
	 * @param[in,out]	Setup			局域网配置数据结构。
	 *
	 * @retval NK_N1_ERR_NONE					连接热点成功。
	 */
	NK_N1Error
	(*onConnectWiFiHotSpot)(NK_PVoid ctx, NK_N1LanSetup *Setup);

} NK_N1DeviceEventAirPair;

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
NK_N1Device_AirPair(NK_N1DeviceEventAirPair *Event);


/**
 * @brief
 *  WiFi NVR 配对控制。
 * @details
 *  如果设备具备 Wi-Fi 连接的能力，可以通过调用此接口使设备域 Wi-Fi NVR 进行配对连接。\n
 *  参数 @ref enabled 决定此方式时开启 Wi-Fi NVR 配对还是关闭。\n
 *  当开启 Wi-Fi NVR 配对时，\n
 *  会在后台开启一个线程，维持与 Wi-Fi NVR 无线连接，直至关闭配对位置。\n
 *  在与 Wi-Fi NVR 维持无线连接的时候，\n
 *  会触发 NK_N1DeviceEventAirPair::onScanHotSpots 和 NK_N1DeviceEventAirPair::onConnectWiFiHotSpot 事件，\n
 *  用户须对以上接口实现才能使 Wi-Fi NVR 配对功能正常工作。
 *
 * @param enabled [in]
 *  开启 / 关闭 Wi-Fi NVR 配对标识。
 *
 * @return
 *  成功返回 0，失败返回 -1。
 */
NK_API NK_Int
NK_N1Device_PairWiFiNVR(NK_Boolean enabled);



NK_CPP_EXTERN_END
#endif /* NK_N1_DEVICE_AIRPAIR_H_ */
