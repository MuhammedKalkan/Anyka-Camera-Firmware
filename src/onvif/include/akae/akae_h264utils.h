/**
 * H.264 处理相关操作方法。
 */

#include <akae_typedef.h>
#include <akae_log.h>


#if !defined(AKAE_H264UTILS_H_)
#define AKAE_H264UTILS_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * 获取 NALU 开始码长度。结果为 3（短开始码）或者 4（长开始码）。
 * 如果获取失败返回长度 0，即内存数据异常或者找不到 H。264 的开始码。
 *
 */
AK_API AK_size akae_h264_size_start_code (AK_bytptr nalu, AK_size len);


/**
 * 去掉 NALU 数据的开始码，通过 @ref ns_nalu 与 @ref ns_len 返回无开始码的 NALU 数据所在内存位置。
 *
 * @retval AK_OK
 *  操作成功。
 * @retval AK_ERR_INVAL_PARAM
 *  操作失败，可能传入了错误参数。
 */
AK_API AK_int akae_h264_nalu_remove_start_code (AK_bytptr nalu, AK_size len, AK_bytptr *ns_nalu, AK_size *ns_len);


/**
 * 在内存 @ref mem 中搜索 H.264 NALU 的开始码，从而定位 NALU 的位置。
 * 检索到开始码的内存起始位置，以及开始码的长度。
 *
 * @param[IN] mem
 *  所搜索的内存区域起始地址。
 * @param[IN] len
 *  所检索的内存区域长度。
 * @param[OUT] s
 *  返回检索到的开始码起始位置。
 * @param[OUT] slen
 *  返回检索到的开始码的长度。
 *
 * @retval AK_OK
 *  成功找到一个 NALU 开始码。
 * @retval AK_ERR_INVAL_PARAM
 *  查找失败，可能传入了错误参数。
 * @retval AK_ERR_NOT_EXIST
 *  查找失败，找不到开始码。
 *
 */
AK_API AK_int akae_h264_seize_start_code (AK_voidptr mem, AK_size len, AK_bytptr *s, AK_size *slen);



/**
 * 在内存 @ref mem 中搜索 H.264 NALU 数据位置。
 * 检索到以后结果通过 @ref nalu 与 @ref nalu_len 返回。
 *
 * @param[IN] mem
 *  所搜索的内存区域起始地址。
 * @param[IN] len
 *  所检索的内存区域长度。
 * @param[OUT] nalu
 *  返回 NALU 数据的内存起始地址。
 * @param[OUT] nalu_len
 *  返回 NALU 数据的长度。
 * @param[OUT] nalu_slen
 *  返回 NALU 开始码的长度。
 *
 * @retval AK_OK
 *  成功找到一个 NALU 开始码。
 * @retval AK_ERR_INVAL_PARAM
 *  查找失败，可能传入了错误参数。
 * @retval AK_ERR_NOT_EXIST
 *  查找失败，找不到开始码。
 */
AK_API AK_int akae_h264_seize_nalu (AK_voidptr mem, AK_size len, AK_bytptr *nalu, AK_size *nalu_len, AK_size *nalu_slen);


AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_H264UTILS_H_
