/**
 * SHA1 哈希运算。
 */

#include <akae_object.h>


#if !defined(AKAE_SHA1SUM_H_)
#define AKAE_SHA1SUM_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * SHA1 哈希运算长度。
 */
#define AK_SHA1SUM_RESULT_LEN (20)


typedef struct _AK_Sha1Sum {

	AK_uint32 _magic;
    AK_uint32 state[5];
    AK_uint32 count[2];
    AK_byte buffer[64];
    AK_char base16[AK_SHA1SUM_RESULT_LEN * 2 + 1];

} AK_Sha1Sum;


/**
 * 初始化一个 SHA1 运算句柄。
 *
 * @retval AK_OK
 *  初始化成功。
 * @retval AK_ERR_INVAL_OPERATE
 *  初始化失败，传入异常参数。
 */
AK_API AK_int akae_sha1sum_init (AK_Sha1Sum *Sha1);


/**
 * 销毁句柄，销毁后句柄将无法使用。
 *
 */
AK_API AK_int akae_sha1sum_destroy (AK_Sha1Sum *Sha1);

/**
 * 哈希数据累计计算。
 *
 * @param[IN] byte
 *  运算的数据的内存起始地址。
 * @param[IN] len
 *  运算数据的长度。
 *
 * @retval AK_OK
 *  运算成功，可以通过 @ref akae_sha1sum_result() 获取当前结果。
 * @retval AK_ERR_INVAL_OBJECT
 *  运算失败，用户传入了无效句柄。
 * @retval AK_ERR_INVAL_PARAM
 *  运算失败，用户传入了无效参数。
 *
 */
AK_API AK_int akae_sha1sum_update (AK_Sha1Sum *Sha1, AK_voidcptr byte, AK_size len);

/**
 * 获取 SHA1 累计运算结果。
 *
 * @retval AK_OK
 *  获取结果成功，结果保存在 @ref stack 所在内存位置。
 * @retval AK_ERR_INVAL_OBJECT
 *  获取结果失败，用户传入了无效句柄。
 * @retval AK_ERR_INVAL_PARAM
 *  获取结果失败，用户传入了无效参数。
 * @retval AK_ERR_OUT_OF_RANGE
 *  获取结果失败，用户传入 @ref stacklen 长度不足。
 */
AK_API AK_int akae_sha1sum_result (AK_Sha1Sum *Sha1, AK_bytptr stack, AK_size stacklen);


AK_API AK_chrptr akae_sha1sum_result_base16 (AK_Sha1Sum *Sha1);


/**
 * 对定长数据 SHA1 运算的快速调用。
 */
#define AK_SHA1SUM_BYTES(__byte, __byte_len) \
	({\
		AK_Sha1Sum Sha1;\
		akae_sha1sum_init (&Sha1);\
		akae_sha1sum_update (&Sha1, __byte, __byte_len);\
		akae_sha1sum_result_base16 (&Sha1);\
	})

/**
 * 对字符串 SHA1 运算的快速调用。
 */
#define AK_SHA1SUM_STRING(__string) AK_SHA1SUM_BYTES(__string, akae_strlen (__string))



AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_SHA1SUM_H_

