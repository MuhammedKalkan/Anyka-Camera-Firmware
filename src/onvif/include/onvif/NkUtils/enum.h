/**
 * N1 枚举数据定义。
 *
 */

#include <NkUtils/types.h>

#ifndef NK_ENUM_H_
#define NK_ENUM_H_
NK_CPP_EXTERN_BEGIN

/**
 * 未定义枚举变量。
 */
#define NK_ENUM_UNDEFINED ""

/**
 * 定义枚举值映射文本集的接口模板。
 * 通过此接口定义函数有效解决重复命名定义问题。
 * 宏定义在编译的时候会展开成相关的类型接口。
 */
#define DECLARE_NK_ENUM_MAP(__type)  NK_PChar NK_Enum_Map##__type(NK_##__type enm)

/**
 * 定义文本集的接口映射枚举值模板。
 */
#define DECLARE_NK_ENUM_UNMAP(__type)  NK_##__type NK_Enum_Unmap##__type(NK_PChar text)


/**
 * 调用对应类型的枚举接口。
 */
#define NK_ENUM_MAP(__type, __enm) NK_Enum_Map##__type(__enm)

/**
 * 调用对应类型的枚举接口。
 */
#define NK_ENUM_UNMAP(__type, __text) NK_Enum_Unmap##__type(__text)



NK_CPP_EXTERN_END
#endif /* NK_N1_ENUM_H_ */
