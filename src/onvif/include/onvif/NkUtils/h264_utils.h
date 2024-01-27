/**
 * H.264 处理相关操作方法。
 */

#include <NkUtils/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#if !defined(NK_UTILS_H264_UTILS_H_)
#define NK_UTILS_H264_UTILS_H_
NK_CPP_EXTERN_BEGIN

/**
 * 获取 H.264 NALU 的开始码长度。
 *
 * @param[in]			nalu			NALU 数据。
 * @param[in]			nalu_len		NALU 数据长度。
 * @param[out]			start_code_len	NALU 开始码长度，一般为 3 或 4。
 *
 * @return		获取成功返回 0，获取失败返回 -1。
 *
 */
NK_API NK_Int
NK_H264Utils_StartCodeSize(NK_PByte nalu, NK_Size nalu_len, NK_Size *start_code_len);

/**
 * 去掉 H.264 NALU 的开始码。
 *
 * @param[in]			nalu			NALU 数据。
 * @param[in]			nalu_len		NALU 数据长度。
 * @param[out]			nalu_raw		NALU 去掉开始码以后的地址偏移。
 * @param[out]			nalu_raw_len	NALU 去掉开始码以后的长度。
 *
 * @return		操作成功返回 0，失败返回 -1，由可能 NALU 非法。
 */
NK_API NK_Int
NK_H264Utils_StripStartCode(NK_PByte nalu, NK_Size nalu_len, NK_PByte *nalu_raw, NK_Size *nalu_raw_len);

/**
 * 在内存中搜索 H.264 NALU 的开始码，从而定位 NALU 的位置。
 *
 * @param[in]			mem				内存数据区域。
 * @param[in]			len				内存数据区域长度。
 * @param[out]			start_code		NALU 开始码位置。
 * @param[out]			start_code_len	NALU 开始码的长度。
 *
 * @return		成功返回 0，失败返回 -1，可能内存区域内不存在 NALU 数据。
 */
NK_API NK_Int
NK_H264Utils_SeekStartCode(NK_PVoid mem, NK_Size len, NK_PByte *start_code, NK_Size *start_code_len);


/**
 * 从内存中定位一个 NALU 数据。
 *
 * @param[in]			mem				内存数据区域。
 * @param[in]			len				内存数据区域长度。
 * @param[out]			nalu			NALU 数据。
 * @param[out]			start_code_len	NALU 开始码的长度。
 * @param[out]			nalu_len		NALU 数据的长度。
 *
 * @return		成功返回 0，失败返回 -1。
 */
NK_API NK_Int
NK_H264Utils_ExtractNALU(NK_PVoid mem, NK_Size len, NK_PByte *nalu, NK_Size *nalu_start_code_len, NK_Size *nalu_len);


NK_CPP_EXTERN_END
#endif /* NK_UTILS_H264_UTILS_H_ */
