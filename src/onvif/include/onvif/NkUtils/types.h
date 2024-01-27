
/**
 * Basic Data Type Definations.s
 *
 */

#if defined(_WIN32)
#  if _MSC_VER  <= 1200 ///< VC6
#    define __FUNCTION__ ""
#  endif
#  define _INTPTR (1) ///< Default 32b Pointer Size.
#  define __PRETTY_FUNCTION__			__FUNCTION__
#  define __func__						__FUNCTION__
#  define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>

/**
 * C++ Mixed Programming Symbol.
 */
#if defined(__cplusplus)
#  define NK_CPP_EXTERN_BEGIN       extern "C" {
#  define NK_CPP_EXTERN_END         }
#else
#  define NK_CPP_EXTERN_BEGIN
#  define NK_CPP_EXTERN_END
#endif

/**
 * Windows Export Symbol.
 */
#if defined(_WIN32)
#  define NK_API __declspec(dllexport)
#else
#  define NK_API extern
#endif


#if !defined(NK_UTILS_TYPES_H_)
# define NK_UTILS_TYPES_H_
NK_CPP_EXTERN_BEGIN

/**
 * @brief
 *  Type Defination.\n
 */
#if defined(_WIN32)
#  if _MSC_VER  <= 1200 ///< VC6
#    include <WINNT.H>
typedef     CHAR        NK_Int8;
typedef		SHORT       NK_Int16;
typedef		INT32       NK_Int32;
typedef		INT64       NK_Int64;
typedef		BYTE        NK_UInt8;
typedef		USHORT      NK_UInt16;
typedef		UINT32      NK_UInt32;
typedef		UINT64      NK_UInt64;
typedef		CHAR        NK_Char;
typedef		PCHAR       NK_PChar;
typedef		BYTE        NK_Byte;
typedef		PBYTE       NK_PByte;
typedef		INT         NK_Integer;
typedef		INT         NK_Int;
typedef     INT_PTR     NK_PtrInt;
typedef		SSIZE_T     NK_SSize;
typedef		SIZE_T      NK_Size;
typedef		INT64       NK_SSize64;
typedef		UINT64      NK_Size64;
typedef		VOID        NK_Void;
typedef		PVOID       NK_PVoid;
typedef		NK_Integer	NK_Boolean;
#  else
#  include <stdint.h>
typedef     int8_t		NK_Int8;
typedef		int16_t		NK_Int16;
typedef		int32_t		NK_Int32;
typedef		int64_t		NK_Int64;
typedef		uint8_t		NK_UInt8;
typedef		uint16_t	NK_UInt16;
typedef		uint32_t	NK_UInt32;
typedef		uint64_t	NK_UInt64;
typedef		char		NK_Char;
typedef		NK_Char*	NK_PChar;
typedef		NK_UInt8	NK_Byte;
typedef		NK_Byte*	NK_PByte;
typedef		int			NK_Integer;
typedef		int			NK_Int;
typedef     uintptr_t   NK_PtrInt;
typedef		NK_Int32	NK_SSize;
typedef		NK_UInt32	NK_Size;
typedef		NK_Int64	NK_SSize64;
typedef		NK_UInt64	NK_Size64;
typedef		void		NK_Void;
typedef		NK_Void*	NK_PVoid;
typedef		NK_Integer	NK_Boolean;
#  endif
#else
#  include <stdint.h>
typedef		int8_t		NK_Int8;
typedef		int16_t		NK_Int16;
typedef		int32_t		NK_Int32;
typedef		int64_t		NK_Int64;
typedef		uint8_t		NK_UInt8;
typedef		uint16_t	NK_UInt16;
typedef		uint32_t	NK_UInt32;
typedef		uint64_t	NK_UInt64;
typedef		char		NK_Char;
typedef		NK_Char*	NK_PChar;
typedef		NK_UInt8	NK_Byte;
typedef		NK_Byte*	NK_PByte;
typedef		int			NK_Integer;
typedef		int			NK_Int;
typedef     uintptr_t   NK_PtrInt;
typedef		NK_Int32	NK_SSize;
typedef		NK_UInt32	NK_Size;
typedef		NK_Int64	NK_SSize64;
typedef		NK_UInt64	NK_Size64;
typedef		void		NK_Void;
typedef		NK_Void*	NK_PVoid;
typedef		NK_Integer	NK_Boolean;
#endif


#define		NK_INT8		NK_Int8
#define		NK_UINT8	NK_UInt8
#define		NK_INT16	NK_Int16
#define		NK_UINT16	NK_UInt16
#define		NK_INT32	NK_Int32
#define		NK_UINT32	NK_UInt32
#define		NK_INT64	NK_Int64
#define		NK_UINT64	NK_UInt64
#define		NK_CHAR		NK_Char
#define		NK_PCHAR	NK_PChar
#define		NK_BYTE		NK_Byte
#define		NK_PBYTE	NK_PByte
#define		NK_INTEGER	NK_Integer
#define		NK_INT		NK_Int
#define     NK_PTRINT   NK_PtrInt
#define		NK_SSIZE	NK_SSize
#define		NK_SIZE		NK_Size
#define		NK_SSIZE64	NK_SSize64
#define		NK_SIZE64	NK_Size64
#define		NK_VOID		NK_Void
#define		NK_PVOID	NK_PVoid
#define		NK_BOOLEAN	NK_Boolean

/**
 * @brief
 *  Boolean True or False Defination.\n
 */
#define		NK_False	(0)
#define 	NK_True		(!NK_False)
#define		NK_FALSE	(NK_False)
#define		NK_TRUE		(NK_True)

/**
 * @brief
 *  Nil Pointer Defination.\n
 */
#define		NK_Nil		(NULL)//(NK_PVoid)(0)
#define		NK_NIL		(NK_Nil)

/**
 * @brief
 *  Single Float Defination.\n
 */
typedef		float		NK_Float;
#define		NK_FLOAT	NK_Float

/**
 * @brief
 *  Double Float Defination.\n
 */
typedef		double		NK_DFloat;
#define		NK_DFLOAT	NK_DFloat

#define inline __inline

/**
 * @brief
 *  UTC Time Defination, The Seconds Timestamp of 1970/01/01 0-:00:00.
 */
#define NK_UTC1970		NK_UInt32

/**
 * @brief
 *  Get the Element Offset of Structure.
 */
#define NK_STRUCT_ELEMENT_WHERE(__struct, __element) \
	(NK_Size)(((NK_PByte)((__struct *)(NK_Nil))->__element) - (NK_PByte)(NK_Nil))


/**
 * @brief
 *  Enum Mapping.
 */
typedef struct Nk_EnumStrMap {

	/**
	 * Enum ID Value.
	 */
	NK_Int id;

	/**
	 * Enum String Mapping.
	 */
	const NK_Char* str;

} NK_EnumStrMap;

/**
 * @macro
 *  Mapper Name.
 */
#define NK_ENUM_MAPPER(__enum_type) __enum_type##EnumStrMapper


/**
 * @brief
 *  Timezone GMT Seconds Differance.\n
 */
#define NK_TZ_GMT_OFF(__hour, __min) ((__hour) * 3600 + (__min) * 60)

/**
 * Time Zone Defination.\n
 * The Value is the Seconds Difference to GMT Timezone.\n
 * Such as Eastern GMT+01:00, Value is +3600.\n
 * This Value Add What is UTC1970 is the Seconds of Local Timestamp to 1970/01/01 00:00:00.\n
 */
typedef enum Nk_TimeZone {

	NK_TZ_GMT_W1200	= -NK_TZ_GMT_OFF(12, 0),//!< NK_TZ_GMT_W1200
	NK_TZ_GMT_W1100	= -NK_TZ_GMT_OFF(11, 0),//!< NK_TZ_GMT_W1100
	NK_TZ_GMT_W1000	= -NK_TZ_GMT_OFF(10, 0),//!< NK_TZ_GMT_W1000
	NK_TZ_GMT_W0900	= -NK_TZ_GMT_OFF(9, 0), //!< NK_TZ_GMT_W0900
	NK_TZ_GMT_W0800	= -NK_TZ_GMT_OFF(8, 0), //!< NK_TZ_GMT_W0800
	NK_TZ_GMT_W0700	= -NK_TZ_GMT_OFF(7, 0), //!< NK_TZ_GMT_W0700
	NK_TZ_GMT_W0600	= -NK_TZ_GMT_OFF(6, 0), //!< NK_TZ_GMT_W0600
	NK_TZ_GMT_W0500	= -NK_TZ_GMT_OFF(5, 0), //!< NK_TZ_GMT_W0500
	NK_TZ_GMT_W0430	= -NK_TZ_GMT_OFF(4, 30),//!< NK_TZ_GMT_W0430
	NK_TZ_GMT_W0400	= -NK_TZ_GMT_OFF(4, 0), //!< NK_TZ_GMT_W0400
	NK_TZ_GMT_W0330	= -NK_TZ_GMT_OFF(3, 30),//!< NK_TZ_GMT_W0330
	NK_TZ_GMT_W0300	= -NK_TZ_GMT_OFF(3, 0), //!< NK_TZ_GMT_W0300
	NK_TZ_GMT_W0200	= -NK_TZ_GMT_OFF(2, 0), //!< NK_TZ_GMT_W0200
	NK_TZ_GMT_W0100	= -NK_TZ_GMT_OFF(1, 0), //!< NK_TZ_GMT_W0100
	NK_TZ_GMT		= NK_TZ_GMT_OFF(0, 0),       //!< NK_TZ_GMT
	NK_TZ_GMT_E0100	= NK_TZ_GMT_OFF(1, 0),  //!< NK_TZ_GMT_E0100
	NK_TZ_GMT_E0200	= NK_TZ_GMT_OFF(2, 0),  //!< NK_TZ_GMT_E0200
	NK_TZ_GMT_E0300	= NK_TZ_GMT_OFF(3, 0),  //!< NK_TZ_GMT_E0300
	NK_TZ_GMT_E0330	= NK_TZ_GMT_OFF(3, 30), //!< NK_TZ_GMT_E0330
	NK_TZ_GMT_E0400	= NK_TZ_GMT_OFF(4, 0),  //!< NK_TZ_GMT_E0400
	NK_TZ_GMT_E0430	= NK_TZ_GMT_OFF(4, 30), //!< NK_TZ_GMT_E0430
	NK_TZ_GMT_E0500	= NK_TZ_GMT_OFF(5, 0),  //!< NK_TZ_GMT_E0500
	NK_TZ_GMT_E0530	= NK_TZ_GMT_OFF(5, 30), //!< NK_TZ_GMT_E0530
	NK_TZ_GMT_E0545	= NK_TZ_GMT_OFF(5, 45), //!< NK_TZ_GMT_E0545
	NK_TZ_GMT_E0600	= NK_TZ_GMT_OFF(6, 0),  //!< NK_TZ_GMT_E0600
	NK_TZ_GMT_E0630	= NK_TZ_GMT_OFF(6, 30), //!< NK_TZ_GMT_E0630
	NK_TZ_GMT_E0700	= NK_TZ_GMT_OFF(7, 0),  //!< NK_TZ_GMT_E0700
	NK_TZ_GMT_E0800	= NK_TZ_GMT_OFF(8, 0),  //!< NK_TZ_GMT_E0800
	NK_TZ_GMT_E0900	= NK_TZ_GMT_OFF(9, 0),  //!< NK_TZ_GMT_E0900
	NK_TZ_GMT_E0930	= NK_TZ_GMT_OFF(9, 30), //!< NK_TZ_GMT_E0930
	NK_TZ_GMT_E1000	= NK_TZ_GMT_OFF(10, 0), //!< NK_TZ_GMT_E1000
	NK_TZ_GMT_E1100	= NK_TZ_GMT_OFF(11, 0), //!< NK_TZ_GMT_E1100
	NK_TZ_GMT_E1200	= NK_TZ_GMT_OFF(12, 0), //!< NK_TZ_GMT_E1200
	NK_TZ_GMT_E1300	= NK_TZ_GMT_OFF(13, 0), //!< NK_TZ_GMT_E1300

} NK_TimeZone;

#if 0
/**
 * @brief
 *  Timezone Enum.\n
 */
static NK_EnumStrMap
NK_ENUM_MAPPER(NK_TimeZone)[] = {

		{	NK_TZ_GMT_W1200,	"GMT-12:00",	},
		{	NK_TZ_GMT_W1100,	"GMT-11:00",	},
		{	NK_TZ_GMT_W1000,	"GMT-10:00",	},
		{	NK_TZ_GMT_W0900,	"GMT-09:00",	},
		{	NK_TZ_GMT_W0800,	"GMT-08:00",	},
		{	NK_TZ_GMT_W0700,	"GMT-07:00",	},
		{	NK_TZ_GMT_W0600,	"GMT-06:00",	},
		{	NK_TZ_GMT_W0500,	"GMT-05:00",	},
		{	NK_TZ_GMT_W0430,	"GMT-04:30",	},
		{	NK_TZ_GMT_W0400,	"GMT-04:00",	},
		{	NK_TZ_GMT_W0330,	"GMT-03:30",	},
		{	NK_TZ_GMT_W0300,	"GMT-03:00",	},
		{	NK_TZ_GMT_W0200,	"GMT-02:00",	},
		{	NK_TZ_GMT_W0100,	"GMT-01:00",	},
		{	NK_TZ_GMT,			"GMT",			},
		{	NK_TZ_GMT_E0100,	"GMT+01:00",	},
		{	NK_TZ_GMT_E0200,	"GMT+02:00",	},
		{	NK_TZ_GMT_E0300,	"GMT+03:00",	},
		{	NK_TZ_GMT_E0330,	"GMT+03:30",	},
		{	NK_TZ_GMT_E0400,	"GMT+04:00",	},
		{	NK_TZ_GMT_E0430,	"GMT+04:30",	},
		{	NK_TZ_GMT_E0500,	"GMT+05:00",	},
		{	NK_TZ_GMT_E0530,	"GMT+05:30",	},
		{	NK_TZ_GMT_E0545,	"GMT+05:45",	},
		{	NK_TZ_GMT_E0600,	"GMT+06:00",	},
		{	NK_TZ_GMT_E0630,	"GMT+06:30",	},
		{	NK_TZ_GMT_E0700,	"GMT+07:00",	},
		{	NK_TZ_GMT_E0800,	"GMT+08:00",	},
		{	NK_TZ_GMT_E0900,	"GMT+09:00",	},
		{	NK_TZ_GMT_E0930,	"GMT+09:30",	},
		{	NK_TZ_GMT_E1000,	"GMT+10:00",	},
		{	NK_TZ_GMT_E1100,	"GMT+11:00",	},
		{	NK_TZ_GMT_E1200,	"GMT+12:00",	},
		{	NK_TZ_GMT_E1300,	"GMT+13:00",	},
};
#endif

/**
 * Text Encoding Defination.\n
 */
typedef enum Nk_TextEncoding
{
	NK_TEXT_ENC_UNDEF			= (-1),//!< NK_TEXT_ENC_UNDEF
	NK_TEXT_ENC_ASCII,         //!< NK_TEXT_ENC_ASCII
	NK_TEXT_ENC_GB2312,        //!< NK_TEXT_ENC_GB2312
	NK_TEXT_ENC_GBK,           //!< NK_TEXT_ENC_GBK
	NK_TEXT_ENC_UTF8,          //!< NK_TEXT_ENC_UTF8

} NK_TextEncoding;


NK_CPP_EXTERN_END
#endif /* NK_UTILS_TYPES_H_ */
