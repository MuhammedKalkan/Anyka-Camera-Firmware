
#include <akae_typedef.h>


#if !defined (AKAE_BASE64_H_)
#define AKAE_BASE64_H_
AK_C_HEADER_EXTERN_C_BEGIN

/**
 * BASE64 编码
 *
 * @return
 *  编码成功返回编码结果的长度, 并将结果存放与 @ref stack 所在内存位置, 编码失败返回对应的返回值.
 *
 * @retval 0
 *  编码失败, 传入 0 长度的数据编码时候将返回长度为 0, 表示没有数据进行编码.
 * @retval AK_ERR_INVAL_PARAM
 *  编码失败, 可能传入了无效参数.
 * @retval AK_ERR_OUT_OF_RANGE
 *  编码失败, 输出结果内存空间不足.
 *
 */
AK_API AK_ssize akae_base64_encode (AK_voidptr data, AK_size datalen, AK_chrptr stack, AK_size stacklen);

/**
 * BASE64 解码
 *
 *
 * @return
 *  解码成功返回编码结果的长度, 并将结果存放与 @ref stack 所在内存位置, 编码失败返回对应的返回值.
 * @retval 0
 *  解码失败, 传入 0 长度的数据编码时候将返回长度为 0, 表示没有数据进行编码.
 * @retval AK_ERR_INVAL_PARAM
 *  解码失败, 可能传入了无效参数.
 * @retval AK_ERR_INVAL_OPERATE
 *  解码失败, 传入长度与不符合可解码长度.
 * @retval AK_ERR_OUT_OF_RANGE
 *  解码失败, 输出结果内存空间不足.
 *
 */
AK_API AK_ssize akae_base64_decode (AK_chrptr base64, AK_voidptr stack, AK_size stacklen);

AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_BASE64_H_
