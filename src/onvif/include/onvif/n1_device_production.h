
/**
 * N1 设备生产对接相关 API 接口。
 */

#include "n1_device.h"

#if !defined(NK_N1_DEVICE_PRODUCTION_H_)
#define NK_N1_DEVICE_PRODUCTION_H_
NK_CPP_EXTERN_BEGIN


/**
 * @brief
 *  N1 提示音语言定义。
 */
typedef enum Nk_N1SoundPrompt {

	NK_N1_SOUND_PROMPT_NA = (0),
	NK_N1_SOUND_PROMPT_ENGLISH = (1000),
	NK_N1_SOUND_PROMPT_CHINESE_MAINLAND = (2000),

} NK_N1SoundPrompt;


/**
 * @brief
 *  N1 设备生产对接事件定义。
 */
typedef struct Nk_N1DeviceEventProduction {


	/**
	 * @brief
	 *  检测 IO 的输入状态。
	 * @details
	 *  对于输入接口（如按键、外部报警输入等），通过返回值返回 IO 状态进行测试。\n
	 *  IO 名称一览表。\n
	 *  +--------------+--------------+--------------+--------------------------------------------+\n
	 *  | ioName       | id           | Get / Put    | Description                                |\n
	 *  +--------------+--------------+--------------+--------------------------------------------+\n
	 *  | RstKey       | 0            | Get          | 复位按钮，返回 True 表示按下，返回 False 表示弹起。 |\n
	 *  +--------------+--------------+--------------+--------------------------------------------+\n
	 *
	 * @param ctx [in,out]
	 *  用户上下文，在调用接口 @ref NK_N1Device_InitEx() 时传入。
	 * @param name [in]
	 *  IO 名称，此名称需要用户与模块适配。
	 * @param id [in]
	 *  IO 口序号。
	 *
	 * @return
	 *  对于按键类 IO 接口，True 表示按钮按下，False 表示按钮弹起，\n
	 *  对于电平类 IO 接口，True 表示高电平，False 表示低电平。
	 *
	 */
	NK_Boolean
	(*onGetIO)(NK_PVoid ctx, const NK_PChar name, NK_Int id);


	/**
	 * @brief
	 *  设置 IO 的输出状态。
	 * @details
	 *  详见 @ref onGetIO() 事件。
	 *
	 * @param ctx [in,out]
	 *  用户上下文，在调用接口 @ref NK_N1Device_InitEx() 时传入。
	 * @param name [in]
	 *  IO 名称，此名称需要用户与模块适配。
	 * @param id [in]
	 *  IO 口序号。
	 * @param stat [in]
	 *  IO 输出状态，针对不同的。
	 *
	 */
	NK_Void
	(*onPutIO)(NK_PVoid ctx, const NK_PChar name, NK_Int id, NK_Boolean stat);

	/**
	 * @brief
	 *  写入出厂设备唯一码。\n
	 *  用于生产时出厂初始化配置。
	 *
	 * @param[in,out] ctx
	 *  用户上下文，在调用接口 @ref NK_N1Device_InitEx() 时传入。
	 * @param[in] id
	 *  设备唯一码字串。
	 * @param[in] len
	 *  设备唯一码字串长度。
	 *
	 *
	 */
	NK_Void
	(*onWriteUID)(NK_PVoid ctx, const NK_PChar uid, NK_Size len);

	/**
	 * @brief
	 *  获取 @ref onWriteUID() 事件写入的出厂设备唯一码。\n
	 * @details
	 *  调用 @ref NK_N1Device_GetID() 接口时内部会触发此事件，\n
	 *  因此此事件实现中不能调用 @ref NK_N1Device_GetID() 方法。
	 *
	 * @param ctx [in,out]
	 *  用户上下文，在调用接口 @ref NK_N1Device_InitEx() 时传入。
	 * @param stack [out]
	 *  设备唯一码字串返回内存起始地址。
	 * @param stacklen [in]
	 *  传入栈区长度。
	 *
	 */
	NK_Void
	(*onReadUID)(NK_PVoid ctx, NK_PChar stack, NK_Size stacklen);


	/**
	 * @brief
	 *  获取 EEPROM 芯片数据。
	 * @details
	 *  调用 @ref NK_N1Device_GetID() 接口时内部会触发此事件，\n
	 *  因此此事件实现中不能调用 @ref NK_N1Device_GetID() 方法。
	 *
	 * @param ctx [in,out]
	 *  用户上下文，在调用接口 @ref NK_N1Device_InitEx() 时传入。
	 * @param datasz [in]
	 *  需要 EEPROM 数据大小，目前之支持 256。
	 * @param data [out]
	 *  EEPROM 数据内容，数据长度必须与 @ref datasz 匹配。
	 * @param mask0 [out]
	 *  数据掩码，保留。
	 * @param mask1 [out]
	 *  数据掩码，保留。。
	 *
	 */
	NK_Void
	(*onE2PROM)(NK_PVoid ctx, NK_Size datasz, NK_PByte data, NK_UInt32 *mask0, NK_UInt32 *mask1);


	/**
	 * @brief
	 *  获取设备提示音事件。
	 * @brief
	 *  设备出厂设置获取当前设备提示音语言。
	 *
	 * @param ctx [in,out]
	 *  用户上下文，在调用接口 @ref NK_N1Device_InitEx() 时传入。
	 * @param lang [out]
	 *  当前提示音语言。
	 */
	NK_Void
	(*onGetSoundPrompt)(NK_PVoid ctx, NK_N1SoundPrompt *lang);


	/**
	 * @brief
	 *  设置设备提示音事件。
	 * @brief
	 *  详见 @ref onGetPrompt。
	 */
	NK_Void
	(*onSetSoundPrompt)(NK_PVoid ctx, NK_N1SoundPrompt lang);


} NK_N1DeviceEventProduction;

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
NK_N1Device_Production(NK_N1DeviceEventProduction *Event);



NK_CPP_EXTERN_END
#endif /* NK_N1_DEVICE_PRODUCTION_H_ */
