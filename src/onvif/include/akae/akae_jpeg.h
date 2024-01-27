/**
 * JPEG 处理相关操作方法。
 */

#include <akae_typedef.h>
#include <akae_log.h>


#if !defined(AKAE_JPEG_H__)
#define AKAE_JPEG_H__
AK_C_HEADER_EXTERN_C_BEGIN


#define AK_JPEG_SEG_MAKER   (0xFF)	//none	Start of Image

#define AK_JPEG_SEG_SOI     (0xD8)	//none	Start of Image
#define AK_JPEG_SEG_S0F(__n) \
	(0xC0 + (__n))	                ///< Start of Frame, variable size.

#define AK_JPEG_SEG_DHT     (AK_JPEG_SEG_S0F (4))	/// Define Huffman Tables, variable size
#define AK_JPEG_SEG_DQT     (0xDB)	///< Define Quantization Table(s), variable size
#define AK_JPEG_SEG_DRI     (0xDD)	//4 bytes	Define Restart Interval
#define AK_JPEG_SEG_SOS     (0xDA)	//variable size	Start Of Scan
//#define AK_JPEG_SEG_RSTn    (0xD )  ////n//(//n//#0..7)	none	Restart
#define AK_JPEG_SEG_APP(__n) \
	(0xE0 + (__n))					///< Application specific
#define AK_JPEG_SEG_COM     (0xFE)	//variable size	Comment
#define AK_JPEG_SEG_EOI     (0xD9)	//none	End Of Image


#pragma pack(push, 1)

/**
 * JPEG 文件分段段头数据结构。
 */
typedef struct _AK_JpegSegmentHeader {

	/// FF 标识，以 0xff 作为分段边界。
	AK_uint8 ff;

	/// 该段名称，如 @ref AK_JPEG_SEG_SOI
	AK_uint8 name;

	/// 该段数据长度，单位：字节。
	AK_uint16 length;

} AK_JpegSegmentHeader;


/**
 * JPEG SOF0 数据结构。
 */
typedef struct _AK_JpegSegmentSOF0 {

	AK_JpegSegmentHeader Header;

	/// This is in bits/sample, usually 8 (12 and 16 not supported by most software).
	AK_uint8   precision;

	/// This must be > 0
	AK_uint16  height;

	/// This must be > 0
	AK_uint16  width;

	/// Usually 1 = grey scaled, 3 = color YcbCr or YIQ 4 = color CMYK
	AK_byte    ncomponent;

	/// Read each component data of 3 bytes. It contains,
	/// (component Id(1byte)(1 = Y, 2 = Cb, 3 = Cr, 4 = I, 5 = Q),
	/// sampling factors (1byte) (bit 0-3 vertical., 4-7 horizontal.),
	/// quantization table number (1 byte)).
	/// 4 components maxmam.
	AK_byte    components[4][3];

} AK_JpegSegmentSOF0;

#pragma pack(pop)


typedef struct _AK_JpegParseFile {

	struct {

		/// 属性所在偏移位置。
		AK_bytptr offset;

		/// 属性占用内存的长度。
		AK_size len;

	} SOI, EOI, SOF[8], SOF2, DHT, DQT[2], SOS, RST[8], APP[8], COM, Scans;

	/// 图像宽度，单位：像素。
	AK_size width;

	/// 图像高度，单位：像素。
	AK_size height;

	/// Lumi Q Table
	AK_byte lqt[64];

	/// Chroma Q Table
	AK_byte cqt[64];

} AK_JpegParseFile;



/**
 *
 *
 */
AK_API AK_int akae_jpeg_parse_file (AK_bytptr data, AK_size datalen, AK_JpegParseFile *Parse);


AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_JPEG_H__
