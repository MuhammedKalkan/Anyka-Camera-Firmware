/**
 * MD5 哈希运算。
 */

#include <akae_object.h>


#if !defined(AKSPC_MD5SUM_H_)
#define AKSPC_MD5SUM_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * MD5 哈希运算长度。
 */
#define AK_MD5SUM_RESULT_LEN (16)



typedef struct _AK_Md5Sum {

	AK_uint32 _magic;     ///< read-only
	AK_uint32 A;           ///< read-only
	AK_uint32 B;           ///< read-only
	AK_uint32 C;           ///< read-only
	AK_uint32 D;           ///< read-only
	AK_uint32 total[2];    ///< read-only
	AK_uint32 buflen;      ///< read-only
	AK_char buffer[128];   ///< read-only
	AK_char base16[AK_MD5SUM_RESULT_LEN * 2 +1]; ///< ///< read-only

} AK_Md5Sum;



/**
 * 初始化一个 MD5 运算句柄。
 *
 *
 * @retval AK_OK
 *  初始化成功。
 * @retval AK_ERR_INVAL_OPERATE
 *  初始化失败，传入异常参数。
 */
AK_API AK_int akae_md5sum_init (AK_Md5Sum *Md5);


/**
 * 销毁句柄，销毁后句柄将无法使用。
 *
 */
AK_API AK_int akae_md5sum_destroy (AK_Md5Sum *Md5);

/**
 * 哈希数据累计计算。
 *
 * @param[IN] byte
 *  运算的数据的内存起始地址。
 * @param[IN] len
 *  运算数据的长度。
 *
 * @retval AK_OK
 *  运算成功，可以通过 @ref akae_md5sum_result() 获取当前结果。
 * @retval AK_ERR_INVAL_OBJECT
 *  运算失败，用户传入了无效句柄。
 * @retval AK_ERR_INVAL_PARAM
 *  运算失败，用户传入了无效参数。
 *
 */
AK_API AK_int akae_md5sum_update (AK_Md5Sum *Md5, AK_voidptr byte, AK_size len);

/**
 * 获取 MD5 累计运算结果。
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
AK_API AK_int akae_md5sum_result (AK_Md5Sum *Md5, AK_bytptr stack, AK_size stacklen);



AK_API AK_chrptr akae_md5sum_result_base16 (AK_Md5Sum *Md5);



/**
 * 对定长数据 MD5 运算的快速调用。
 */
#define AK_MD5SUM_BYTES(__byte, __byte_len) \
	({\
		AK_Md5Sum Md5;\
		akae_md5sum_init (&Md5);\
		akae_md5sum_update (&Md5, __byte, __byte_len);\
		akae_md5sum_result_base16 (&Md5);\
	})

/**
 * 对字符串 MD5 运算的快速调用。
 */
#define AK_MD5SUM_STRING(__string) AK_MD5SUM_BYTES(__string, akae_strlen (__string))



AK_C_HEADER_EXTERN_C_END
#endif ///< AKSPC_MD5SUM_H_

