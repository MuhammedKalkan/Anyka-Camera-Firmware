
/**
 * N1 设备 TFCard 相关 API 接口。
 */

#include "n1_device.h"

#ifndef NK_N1_DEVICE_TFCARD_H_
#define NK_N1_DEVICE_TFCARD_H_
NK_CPP_EXTERN_BEGIN


/**
 * @brief
 *  TF 卡状态值。
 * @details
 *  描述 TF 卡目前所处状态。
 */
typedef enum Nk_N1TFCardStatus {

	NK_N1_TFC_STAT_NA = (0),
	NK_N1_TFC_STAT_WORK,
	NK_N1_TFC_STAT_EJECTED,
	NK_N1_TFC_STAT_FS_ERROR,
	NK_N1_TFC_STAT_FORMATTING,
	NK_N1_TFC_STAT_ZOMBIE,

} NK_N1TFCardStatus;


/**
 * @brief
 *  N1 设备 TFCard 事件定义。
 */
typedef struct Nk_N1DeviceEventTFCard {

	/**
	 * @brief
	 *  获取 TF 卡当前状态事件。
	 * @details
	 * @param ctx [in, out]
	 *  事件上下文。
	 * @return
	 *  返回 TF 卡当前所处状态。
	 */
	NK_N1TFCardStatus
	(*onStatus)(NK_PVoid ctx);

	/**
	 * @brief
	 *  检查 TF 卡文件事件。
	 * @details
	 *  当模块需要检查 TF 卡目录下 @ref path 对应是否存在时触发此事件，如果存在返回文件大小，如果不存在返回 -1。
	 *
	 * @param ctx [in, out]
	 *  事件上下文。
	 * @param path [in]
	 *  文件路径。
	 *
	 * @return
	 *  文件存在返回文件大小，否则返回 -1。
	 */
	NK_SSize
	(*onTouch)(NK_PVoid ctx, NK_PChar path);

	/**
	 * @brief
	 *  移除 TF 卡文件事件。
	 * @details
	 *  当模块需要移除 TF 卡目录下 @ref path 对应是否存在时触发此事件。
	 *
	 * @param ctx [in, out]
	 *  事件上下文。
	 * @param path [in]
	 *  文件路径。
	 *
	 */
	NK_Void
	(*onRemove)(NK_PVoid ctx, NK_PChar path);

	/**
	 * @brief
	 *  TF 卡写入数据事件。
	 * @details
	 *  当模块需要向 TF 某个文件路径下写入一个长度为 @ref len 的数据时触发此事件。
	 *
	 * @param ctx [in, out]
	 *  事件上下文。
	 * @param path [in]
	 *  文件路径。
	 * @param seek [in]
	 *  文件位置偏移量，-1 表示文件末尾。
	 * @param data [in]
	 *  数据起始地址。
	 * @param len [in]
	 *  数据长度。
	 *
	 * @return
	 *  返回实际写入长度。
	 */
	NK_SSize
	(*onWrite)(NK_PVoid ctx, NK_PChar path, NK_SSize seek, NK_PByte data, NK_Size len);

	/**
	 * @brief
	 *  TF 卡读取数据事件。
	 * @details
	 *  当模块需要从 TF 某个文件路径下读取一个长度为 @ref len 的数据时触发此事件。
	 *
	 * @param ctx [in, out]
	 *  事件上下文。
	 * @param ctx [in, out]
	 *  事件上下文。
	 * @param path [in]
	 *  文件路径。
	 * @param skip [in]
	 *  文件位置偏移量，-1 表示文件末尾。
	 * @param buf [out]
	 *  返回数据内存起始地址。
	 * @param len [in]
	 *  数据长度。
	 *
	 * @return
	 *  返回实际读取长度。
	 */
	NK_SSize
	(*onRead)(NK_PVoid ctx, NK_PChar path, NK_SSize skip, NK_PByte buf, NK_Size len);

} NK_N1DeviceEventTFCard;

/**
 * @brief
 *  配置 N1 设备 TFCard 相关事件。
 * @details
 *
 *
 * @param Event [in]
 *  用户事件定义。
 *
 * @return
 *  配置成功返回 0，否则返回 -1。
 */
NK_API NK_Int
NK_N1Device_TFCard(NK_N1DeviceEventTFCard *Event);



NK_CPP_EXTERN_END
#endif /* NK_N1_DEVICE_TFCARD_H_ */
