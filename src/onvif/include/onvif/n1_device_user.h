
/**
 * @brief
 *  N1 对接用户管理相关事件及接口定义。
 * @details
 *  用户通过此模块实现对用户管理的相关事件响应，实现对应功能，\n
 *  模块通过接口 @ref NK_N1Device_UserManage() 加载事件，\n
 *  该接口必须在 @ref NK_N1Device_Init() 调用成功后才能调用，否则失败。
 *
 */

#include "n1_device.h"

#ifndef NK_N1_DEVICE_USER_H_
#define NK_N1_DEVICE_USER_H_
NK_CPP_EXTERN_BEGIN


/**
 * @macro
 *  用户最大名称长度定义。
 */
#define NK_N1_USER_MAX_NAME_SZ    (64)

/**
 * @macro
 *  用户最大密码长度定义。
 */
#define NK_N1_USER_MAX_PASS_SZ    (NK_N1_USER_MAX_NAME_SZ)


/**
 * @brief
 *  用户等级定义。
 */
typedef enum Nk_N1UserClassify {

	NK_N1_USR_CLASS_ADMIN = (0),//!< NK_N1_USR_CLASS_ADMIN
	NK_N1_USR_CLASS_VIEWER,     //!< NK_N1_USR_CLASS_VIEWER

} NK_N1UserClassify;


/**
 * @brief
 *  相关响应事件定义。
 */
typedef struct Nk_N1DeviceEventUserManage {

	NK_Void
	(*onAdd)(NK_PVoid ctx, const NK_PChar username, const NK_PChar passphrase, NK_N1UserClassify classify);

	NK_Void
	(*onRemove)(NK_PVoid ctx, const NK_PChar username);

	NK_Void
	(*onEdit)(NK_PVoid ctx, const NK_PChar username, const NK_PChar passphrase, NK_N1UserClassify classify);

} NK_N1DeviceEventUserManage;

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
NK_N1Device_UserManage(NK_N1DeviceEventUserManage *Event);


/**
 * @brief
 *  加入一个认证用户，\n
 *
 * @param username [in]
 *  用户的用户名称。
 * @param passphrase [in]
 *  用户的校验密码。
 * @param classify [in]
 *  用户等级。
 *
 * @return
 *  加入成功返回 0，失败返回 -1。
 */
NK_API NK_Int
NK_N1Device_AddUser(const NK_PChar username, const NK_PChar passphrase, NK_N1UserClassify classify);

/**
 * @macro
 *  编辑更新用户。
 */
#define NK_N1Device_EditUser NK_N1Device_AddUser


/**
 * 删除一个认证用户。
 *
 * @param[in]			username			用户的用户名称。
 *
 * @return		成功返回 0，失败返回 -1。
 */
NK_API NK_Void
NK_N1Device_RemoveUser(const NK_PChar username);

/**
 * 获取对应用户名称的信息，\n
 * 如果用户存在则发挥该用户的用户密码 @ref passphrase 和禁止权限标识 @ref forbiden_r。
 *
 * @param[in]			username			用户的用户名称。
 * @param[out]			passphrase			用户的校验密码。
 * @param[out]			forbidden_r			用户禁止权限标识。
 *
 * @return		用户存在返回 True，并从 @ref passphrase 和 @ref forbidden_r 获取相关信息，用户不存在则返回 False。
 */
NK_API NK_Boolean
NK_N1Device_HasUser(const NK_PChar username, NK_PChar passphrase, NK_N1UserClassify *classify);

/**
 * @brief
 *  根据序号索引用户。
 *
 * @param id [in]
 *  用户序号，根据新增用户的次序，从 0 开始。
 * @param username
 * @param passphrase
 * @param classify
 * @return
 */
NK_API NK_Boolean
NK_N1Device_IndexUser(NK_Int id, NK_PChar username, NK_PChar passphrase, NK_N1UserClassify *classify);


/**
 * 获取认证的用户数。
 *
 * @return		用户数量，如果用户数量为 0，则网络通讯不作任何认证。
 */
NK_API NK_Size
NK_N1Device_NumberOfUser();



NK_CPP_EXTERN_END
#endif /* NK_N1_DEVICE_USER_H_ */
