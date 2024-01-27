
/**
 * @brief
 *  N1 对接设备固件升级相关事件及接口定义。
 * @details
 *  用户通过此模块实现对固件升级的相关事件响应，实现对应功能，\n
 *  模块通过接口 @ref NK_N1Device_Upgrade() 加载事件，\n
 *  该接口必须在 @ref NK_N1Device_Init() 调用成功后才能调用，否则失败。
 *
 */

#include "n1_device.h"

#ifndef NK_N1_DEVICE_UPGRADE_H_
#define NK_N1_DEVICE_UPGRADE_H_
NK_CPP_EXTERN_BEGIN


/**
 * @brief
 *  N1 对接设备固件升级相关事件定义。
 */
typedef struct Nk_N1DeviceEventUpgrade {

	/**
	 * @brief
	 *  获取用户内存。
	 *
	 * @param[in,out] ctx
	 *  用户上下文，在调用接口 @ref NK_N1Device_InitEx() 时传入。
	 * @param[in] len
	 *  所需要获取的用户内存长度。
	 * @param[out] mem
	 *  返回用户内存的起始地址。
	 *
	 */
	NK_Void
	(*onGetUserMem)(NK_PVoid ctx, NK_Size len, NK_PVoid *mem);

	/**
	 * @brief
	 *  释放用户内存。
	 *
	 * @param ctx
	 *
	 * @param[in] len
	 *  申请的用户内存长度。
	 *
	 * @param[out] mem
	 *  用户内存的起始地址。
	 *
	 *
	 * @return
	 */
	NK_Void
	(*onPutUserMem)(NK_PVoid ctx, NK_Size len, NK_PVoid mem);


	/**
	 * @brief
	 *  固件镜像升级时导入固件文件事件。\n
	 *  当此事件实现返回 NK_N1_OK 后会触发 @ref onFirmwareUpgradeFile 事件，
	 *
	 * @param[in,out] ctx
	 *  用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
	 *
	 * @param[in] file
	 *  固件文件内存缓冲起始地址。
	 *
	 * @param[in] size
	 *  固件文件所占内存大小。
	 *
	 * @retval NK_N1_OK
	 *  固件没有异常，返回改值后系统将会触发 @ref onFirmwareUpgradeFile。
	 *
	 * @retval NK_N1_ERR_UPGRADE_NOT_SUPPORT
	 *  返回该值表明当前设备不支持固件升级。
	 *
	 * @retval NK_N1_ERR_FIRMWARE_FILE_ERROR
	 *  返回改制表明固件文件错误，当对固件文件进行校验发现失败后返回值。
	 *
	 * @retval NK_N1_ERR_FIRMWARE_VER_TOO_OLD
	 *  固件版本太旧不予升级。
	 *
	 */
	NK_N1Error
	(*onQualifyFile)(NK_PVoid ctx, NK_PByte file, NK_Size size);

	/**
	 * @brief
	 *  固件镜像升级固件文件事件。
	 *
	 * @param[in,out] ctx
	 *  用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
	 *
	 * @param[in] file
	 *  固件文件内存缓冲起始地址。
	 *
	 * @param[in] size
	 *  固件文件所占内存大小。
	 *
	 * @param[out] rate
	 *  固件升级进度，事件实现内部不断根据升级进度更新此值。
	 *
	 * @retval NK_N1_OK
	 *  升级完成。
	 *
	 * @retval NK_N1_ERR_UPGRADE_INTERRUPT
	 *  升级过程错误并中断。
	 */
	NK_N1Error
	(*onUpgradeFile)(NK_PVoid ctx, NK_PByte file, NK_Size size, NK_Size *rate);

	/**
	 * @brief
	 *  固件镜像升级 ROM 固件文件事件。\n
	 *  当用户实现此事件后，内部会根据 ROM 文件特点内部实现 @ref onQualifyFile 进行合法性校验然后触发此事件，\n
	 *  实现了此事件后，事件 @ref onQualifyFile 和 @ref onUpgradeFile 将不会被触发。\n
	 *
	 * @param[in,out] ctx
	 *  用户上下文，在调用接口 @ref NK_N1Device_Init 时传入。
	 * @param[in] version
	 *  固件版本码，通过 @ref NK_N1_VER_DEC() 方法分解成各个版本码。
	 * @param[in] file
	 *  固件文件内存缓冲起始地址。
	 * @param[in] size
	 *  固件文件所占内存大小。
	 * @param[out] rate
	 *  固件升级进度，事件实现内部不断根据升级进度更新此值。
	 *
	 * @retval NK_N1_OK
	 *  升级完成。
	 *
	 * @retval NK_N1_ERR_UPGRADE_INTERRUPT
	 *  升级过程错误并中断。
	 */
	NK_N1Error
	(*onUpgradeROM)(NK_PVoid ctx, NK_Size version, NK_PByte file, NK_Size size, NK_Size *rate);


	/**
	 * 固件镜像升级结束事件。
	 * 事件 @ref onFirmwareUpgradeFile 或 @ref onFirmwareUpgradeFileROM 触发后触发此事件。
	 *
	 * @param[in,out]	ctx
	 *
	 */
	NK_N1Error
	(*onUpgradeEnd)(NK_PVoid ctx);

} NK_N1DeviceEventUpgrade;


typedef struct Nk_N1DeviceUpgrade {
	/**
	 * 设备端口号。
	 *
	 * @param[in]	port,
	 * 此端口与N1设备端口号相同。
	 */
	NK_Int devport;
	/**
	 * 单独获取进度接口， 区分onUpgradeROM 获取进度。
	 * 通过 @ref NK_N1UpgradeRate_GetEvent 注册。
	 *
	 * @param[in,out]	Rate
	 *
	 * @param[in,out]	ctx
	 */
	NK_Int
	(*onGetUpgradeRate)(NK_PVoid ctx, NK_Int *Rate);
} NK_N1DeviceUpgrade;

/**
 * @brief
 *  配置 N1 固件升级相关事件。
 * @details
 *
 * @param Event [in]
 *  用户事件定义。
 *
 * @return
 *  配置成功返回 0，否则返回 -1。
 */
NK_API NK_Int
NK_N1Device_Upgrade(NK_N1DeviceEventUpgrade *Event);

/**
 * @brief
 *  配置 N1 单独获取进度值的接口
 * @details
 * 此接口在 设备注销@ref NK_N1Device_Destroy 后或没有初始化N1设备 @ref NK_N1Device_InitV2 有效。
 *
 * @param startup [in]
 *  是否启用。
 *
 * @param DeviceUpgrade [in]
 *  用户事件定义。
 *
 * @ret 配置成功返回 0，否则返回 -1。
 */
NK_API NK_Int
NK_N1UpgradeRate_GetEvent(NK_Boolean startup, NK_N1DeviceUpgrade *DeviceUpgrade);


NK_CPP_EXTERN_END
#endif /* NK_N1_DEVICE_UPGRADE_H_ */
