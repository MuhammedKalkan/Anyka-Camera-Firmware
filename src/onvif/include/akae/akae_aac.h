/**
 * AAC 处理相关操作方法。
 */

#include <akae_typedef.h>
#include <akae_log.h>


#if !defined(AKAE_AAC_H_)
#define AKAE_AAC_H_
AK_C_HEADER_EXTERN_C_BEGIN

/**
 * AK_AacAdtsFixedHeader::samplingFreqIndex 对应枚举值。
 */
enum {

	AK_AAC_SAMPLE_FREQ_96000 = 0,
	AK_AAC_SAMPLE_FREQ_88200,
	AK_AAC_SAMPLE_FREQ_64000,
	AK_AAC_SAMPLE_FREQ_48000,
	AK_AAC_SAMPLE_FREQ_44100,
	AK_AAC_SAMPLE_FREQ_32000,
	AK_AAC_SAMPLE_FREQ_24000,
	AK_AAC_SAMPLE_FREQ_22050,
	AK_AAC_SAMPLE_FREQ_16000,
	AK_AAC_SAMPLE_FREQ_12000,
	AK_AAC_SAMPLE_FREQ_11025,
	AK_AAC_SAMPLE_FREQ_8000,

	AK_AAC_SAMPLE_FREQ_RESV = 0xf,

};

enum {

	AK_AAC_CH_DEF_IN_AOT_SPEC_CONFIG = 0, ///< Defined in AOT Specifc Config
	AK_AAC_CH1_FC,                   ///< 1 channel: front-center
	AK_AAC_CH2_FLR,                  ///< 2 channels: front-left, front-right
	AK_AAC_CH3_FCLR,                 ///< 3 channels: front-center, front-left, front-right
	AK_AAC_CH4_FCLR_BC,              ///< 4 channels: front-center, front-left, front-right, back-center
	AK_AAC_CH5_FCLR_BLR,             ///< 5 channels: front-center, front-left, front-right, back-left, back-right
	AK_AAC_CH6_FCLR_BCR,             ///< 6 channels: front-center, front-left, front-right, back-left, back-right, LFE-channel
	AK_AAC_CH8_FCLR_SLR_BCR,         ///< 8 channels: front-center, front-left, front-right, side-left, side-right, back-left, back-right, LFE-channel
	AK_AAC_CH_DEF_RESV = 0x0f,

};




/**
 * AAC 单帧 ADTS 长度。
 */
#define AK_AAC_ADTS_SZ  (7)


#pragma pack(push, 1)

typedef struct _AK_AacAdtsFixedHeader {

    AK_uint32 syncword;  //12 bit 同步字 '1111 1111 1111'，说明一个ADTS帧的开始
    AK_uint32 id;        //1 bit MPEG 标示符， 0 for MPEG-4，1 for MPEG-2
    AK_uint32 layer;     //2 bit 总是'00'
    AK_uint32 protectionAbsent;  //1 bit 1表示没有crc，0表示有crc
    AK_uint32 profile;           //2 bit 表示使用哪个级别的AAC
    AK_uint32 samplingFreqIndex; //4 bit 表示使用的采样频率
    AK_uint32 privateBit;        //1 bit
    AK_uint32 channelCfg; //3 bit 表示声道数
    AK_uint32 originalCopy;         //1 bit
    AK_uint32 home;                  //1 bit

} AK_AacAdtsFixedHeader;


typedef struct _AK_AacAdtsVariableHeader {

    /*下面的为改变的参数即每一帧都不同*/
    AK_uint32 copyrightIdentificationBit;   //1 bit
    AK_uint32 copyrightIdentificationStart; //1 bit
    AK_uint32 aacFrameLength;               //13 bit 一个ADTS帧的长度包括ADTS头和AAC原始流
    AK_uint32 adtsBufferFullness;           //11 bit 0x7FF 说明是码率可变的码流

    /* number_of_raw_data_blocks_in_frame
     * 表示ADTS帧中有number_of_raw_data_blocks_in_frame + 1个AAC原始帧
     * 所以说number_of_raw_data_blocks_in_frame == 0
     * 表示说ADTS帧中有一个AAC数据块并不是说没有。(一个AAC原始帧包含一段时间内1024个采样及相关数据)
     */
    AK_uint32 numberOfRawDataBlockInFrame; //2 bit

} AK_AacAdtsVariableHeader;


/**
 * 打印 @ref AK_AacAdtsFixedHeader 和 @ref AK_AacAdtsVariableHeader 对象结构。
 */
#define AK_AAC_ADTS_HEADER_DUMP(__FixedHdr, __VarHdr) \
	do {\
		AK_VerboseForm Form;\
		akae_verbose_form_init (&Form, "AAC ADTS Header", 64, 4);\
		if (AK_null != (__FixedHdr)) {\
			akae_verbose_form_put_text (&Form, AK_true,   "Fixed Header");\
			akae_verbose_form_put_kv (&Form, AK_false,     "                id",       "%u", (AK_uint32)(__FixedHdr)->id               );\
			akae_verbose_form_put_kv (&Form, AK_false,     "             layer",       "%u", (AK_uint32)(__FixedHdr)->layer            );\
			akae_verbose_form_put_kv (&Form, AK_false,     "  protectionAbsent",       "%u", (AK_uint32)(__FixedHdr)->protectionAbsent );\
			akae_verbose_form_put_kv (&Form, AK_false,     "           profile",       "%u", (AK_uint32)(__FixedHdr)->profile          );\
			akae_verbose_form_put_kv (&Form, AK_false,     " samplingFreqIndex",       "%u", (AK_uint32)(__FixedHdr)->samplingFreqIndex);\
			akae_verbose_form_put_kv (&Form, AK_false,     "        privateBit",       "%u", (AK_uint32)(__FixedHdr)->privateBit       );\
			akae_verbose_form_put_kv (&Form, AK_false,     "        channelCfg",       "%u", (AK_uint32)(__FixedHdr)->channelCfg       );\
			akae_verbose_form_put_kv (&Form, AK_false,     "      originalCopy",       "%u", (AK_uint32)(__FixedHdr)->originalCopy     );\
			akae_verbose_form_put_kv (&Form, AK_true,      "              home",       "%u", (AK_uint32)(__FixedHdr)->home             );\
		}\
		if (AK_null != (__VarHdr)) {\
			akae_verbose_form_put_text (&Form, AK_true,   "Variable Header");\
			akae_verbose_form_put_kv (&Form, AK_false,     " copyrightIdentificationStart",  "%u",      (AK_uint32)(__VarHdr)->copyrightIdentificationStart  );\
			akae_verbose_form_put_kv (&Form, AK_false,     "               aacFrameLength",  "%u",      (AK_uint32)(__VarHdr)->aacFrameLength                );\
			akae_verbose_form_put_kv (&Form, AK_false,     "           adtsBufferFullness",  "%u/0x%x", (AK_uint32)(__VarHdr)->adtsBufferFullness, (AK_uint32)(__VarHdr)->adtsBufferFullness );\
			akae_verbose_form_put_kv (&Form, AK_false,     "  numberOfRawDataBlockInFrame",  "%u(%u)",  (AK_uint32)(__VarHdr)->numberOfRawDataBlockInFrame, (AK_uint32)(__VarHdr)->numberOfRawDataBlockInFrame + 1 );\
		}\
		akae_verbose_form_finish (&Form);\
	} while (0)


#pragma pack(pop)


/**
 * 解析一个 AAC 帧数据，并分析出 ADTS。
 * 传入的数据必须是 AAC-ADTS 完整帧结构，因此传入的 @ref datalen 必须大于等于一帧的长度，否则解析到 ADTS 以后检查长度不匹配会返回错误。
 *
 * @param[IN] data
 *  AAC-ADTS 二进制帧数据的内存起始地址。
 * @param[IN] datalen
 *  AAC-ADTS 二进制帧数据的长度。
 * @param[OUT] FixHdr
 *  从帧数据中解析到的 ADTS 数据。
 *
 * @return
 *  解析成功在 @ref Adts 参数返回该帧的 ADTS 属性，从返回值返回该帧的长度（包含 ADTS 长度），\n
 *  上层调用者可以通过数据信息偏移 @ref AK_AAC_ADTS_SZ 获取去掉 ADTS 的净荷 AAC 数据内容。
 *
 * @retval AK_ERR_INVAL_PARAM
 *  传入非法参数。
 * @retval AK_ERR_INVAL_OBJECT
 *  传入数据错误，@ref data 传入的数据并非合法的 AAC-ADTS 数据。
 * @retval AK_ERR_OUT_OF_RANGE
 *  传入的帧长度错误，与帧内解析到的 ADTS 标定长度有差异。
 *
 */
AK_API AK_ssize akae_aac_disset_frame (AK_bytptr data, AK_size datalen, AK_AacAdtsFixedHeader *FixHdr, AK_AacAdtsVariableHeader *VarHdr);

/**
 * 通过采样率序号获取当前 AAC 的采样率，序号对应 AK_AacAdtsFixedHeader::samplingFreqIndex。
 *
 * @return
 *  获取成功返回对应的采样率（单位：Hz），采样率序号异常返回 0。
 */
AK_API AK_size akae_aac_get_sample_rate_by_index (AK_int id);

/**
 * 通过采样率获取 AAC 采样率序号，与 @ref akae_aac_get_sample_rate_by_index() 对应。
 *
 * @return
 *  通过采样率获取对应属性索引，如果采样率不在合法列表以内返回 @ref AK_AAC_SAMPLE_FREQ_RESERVED。
 */
AK_API AK_size akae_aac_get_sample_rate_index (AK_int rate);

/**
 *
 * @return
 *  获取成功返回对应的通道数，序号异常返回 0。
 */
AK_API AK_size akae_aac_get_channels_by_index (AK_int idx);


/**
 *
 * @return
 *  通过采样同道数返回对应属性索引，如果通道树不在合法列表以内返回 @ref AK_AAC_CH_DEF_IN_AOT_SPEC_CONFIG。
 */
AK_API AK_size akae_aac_get_channels_index (AK_int channels);




AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_AAC_H_
