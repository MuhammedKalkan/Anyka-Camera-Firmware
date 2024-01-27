
/**
 * Basic Data Type Definations.s
 *
 */


#include <stdio.h>
#include <stdlib.h>

/**
 * 兼容 WIN32 就版本部分编译器缺少部分预编译宏的问题。
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
#define AK_ALIAS_TYPE(__type, __alias)

/**
 * C 头文件接口声明，在 C/C++ 混合编程链接文件标识兼容性，\n
 * 在使用 C 编译器时有效，使用 C++ 编译器时无效。
 */
#if defined(__cplusplus)
#  define AK_C_HEADER_EXTERN_C_BEGIN       extern "C" {
#  define AK_C_HEADER_EXTERN_C_END         }
#else
#  define AK_C_HEADER_EXTERN_C_BEGIN
#  define AK_C_HEADER_EXTERN_C_END
#endif

/**
 * Windows Export Symbol.
 */
#if defined(_WIN32)
#  define AK_API __declspec(dllexport)
#else
#  define AK_API extern
#endif


#if !defined(AKAE_TYPEDEF_H_)
# define AKAE_TYPEDEF_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * Multi-Platform Common Basic Type Definations.
 */
typedef  char           AK_int8;
typedef  unsigned char  AK_uint8;
typedef  char           AK_char;
typedef  AK_char*       AK_chrptr;
typedef  const AK_char* AK_chrcptr;
typedef  AK_uint8       AK_byte;
typedef  AK_byte*       AK_bytptr;
typedef  int            AK_int;
typedef  void           AK_void;
typedef  void*          AK_voidptr;
typedef  const void*    AK_voidcptr;
typedef  AK_int         AK_boolean;
typedef  float          AK_float;
typedef  double         AK_dbfloat;


/**
 * @brief
 *  Type Definition.\n
 */
#if defined(_WIN32)
#  if _MSC_VER  <= 1200 ///< VC6
#    include <WINNT.H>
typedef		SHORT       AK_int16;
typedef		INT32       AK_int32;
typedef		INT64       AK_int64;
typedef		USHORT      AK_uint16;
typedef		UINT32      AK_uint32;
typedef		UINT64      AK_uint64;
typedef     INT_PTR     AK_intaddr;
#  else
#  include <stdint.h>
typedef		int16_t		AK_int16;
typedef		int32_t		AK_int32;
typedef		int64_t		AK_int64;
typedef		uint16_t	AK_uint16;
typedef		uint32_t	AK_uint32;
typedef		uint64_t	AK_uint64;
typedef     intptr_t    AK_intaddr;
typedef     unitptr_t   AK_unitptr;
#  endif
#elif defined(_ECOS)
#  include <cyg_type.h>

typedef     cyg_halint16        AK_int16;
typedef     cyg_halint32        AK_int32;
typedef     cyg_halint64        AK_int64;
typedef     cyg_uint16          AK_uint16;
typedef     cyg_uint32          AK_uint32;
typedef     cyg_uint64          AK_uint64;
typedef     cyg_haladdress      AK_intaddr;


#else

# include <stdint.h>
typedef		int16_t		AK_int16;
typedef		int32_t		AK_int32;
typedef		int64_t		AK_int64;
typedef		uint16_t	AK_uint16;
typedef		uint32_t	AK_uint32;
typedef		uint64_t	AK_uint64;
typedef     intptr_t    AK_intaddr;
typedef     uintptr_t   AK_uintaddr;

#endif

/**
 * Some Type Redefinition.
 */
typedef		AK_int16    AK_ssize16;
typedef		AK_uint16   AK_size16;
typedef		AK_int32    AK_ssize;
typedef		AK_uint32   AK_size;
typedef		AK_int64    AK_ssize64;
typedef		AK_uint64   AK_size64;


/**
 * @brief
 *  Boolean True or False Defination.\n
 */
#define      AK_false    (0)
#define      AK_true     (!AK_false)
#if !defined(AK_NOARGS)
#  define    AK_NOARGS   AK_void
#endif

/**
 * @brief
 *  Nil Pointer Defination.\n
 */
#if defined(NULL)
#  define AK_null      (NULL)
#else
#  define   AK_null      ((AK_voidptr)0)
#endif

/**
 * 句柄定义。
 */
#define  AK_handle                    AK_intaddr

/**
 * 非法句柄数值。
 */
#define AK_INVAL_HANDLE               (0)

/**
 * 判断句柄是否合法，针对创建句柄的接口函数结果判断。
 */
#define  AK_VALID_HANDLE(__h)         ((__h) > AK_INVAL_HANDLE)


/**
 * 计算组 32 位组合码，一般用于生成哈希值或者幻数。
 */
#define   AK_MK4CC(__byte0, __byte1, __byte2, __byte3)  \
	((__byte0) << 24 | (__byte1)<<16 | (__byte2)<<8 | (__byte3))

/**
 * 计算组 16 位组合码，一般用于生成哈希值或者幻数。
 */
#define   AK_MK2CC(__byte0, __byte1)  \
	((__byte0)<<8 | (__byte1))





/**
 * 返回值定义。
 */
#define   AK_OK                       (0)
#define   AK_ERR_UNKNOWN              (-1)
#define   AK_ERR_EXCEPTION            (-AK_MK2CC('E', 'X'))
#define   AK_ERR_INTERNAL             (-AK_MK2CC('I', 'N'))
#define   AK_ERR_INVAL_PARAM          (-AK_MK2CC('I', 'P'))
#define   AK_ERR_INVAL_OPERATE        (-AK_MK2CC('I', 'O'))
#define   AK_ERR_INVAL_OBJECT         (-AK_MK2CC('I', 'o'))
#define   AK_ERR_INVAL_HANDLE         (AK_ERR_INVAL_OBJECT)
#define   AK_ERR_PEER_BROKEN          (-AK_MK2CC('P', 'B'))
#define   AK_ERR_TIME_OUT             (-AK_MK2CC('T', 'O'))
#define   AK_ERR_OUT_OF_RANGE         (-AK_MK2CC('O', 'R'))
#define   AK_ERR_OUT_OF_MEM           (-AK_MK2CC('O', 'M'))
#define   AK_ERR_OUT_OF_BACKLOG       (-AK_MK2CC('O', 'B'))
#define   AK_ERR_BUSY                 (-AK_MK2CC('B', 'S'))
#define   AK_ERR_IO                   (-AK_MK2CC('I', 'O'))
#define   AK_ERR_NOT_ACCESS           (-AK_MK2CC('N', 'A'))
#define   AK_ERR_NOT_EXIST            (-AK_MK2CC('N', 'X'))
#define   AK_ERR_NOT_SUPPORT          (-AK_MK2CC('N', 'S'))
#define   AK_ERR_NOT_PERM             (-AK_MK2CC('N', 'P'))
#define   AK_ERR_DEAD_LOCK            (-AK_MK2CC('D', 'L'))

#define   AK_ERR_STRING(__errno) \
	((AK_OK == (__errno)) ? "OK" :\
		((AK_ERR_EXCEPTION       == (__errno)) ? "Exception" :\
		((AK_ERR_INTERNAL        == (__errno)) ? "Internal Error" :\
		((AK_ERR_INVAL_PARAM     == (__errno)) ? "Invalid Parameter" :\
		((AK_ERR_INVAL_OPERATE   == (__errno)) ? "Invalid Operation" :\
		((AK_ERR_INVAL_HANDLE    == (__errno)) ? "Invalid Handle" :\
		((AK_ERR_INVAL_OBJECT    == (__errno)) ? "Invalid Object" :\
		((AK_ERR_PEER_BROKEN     == (__errno)) ? "Peer Broken" :\
		((AK_ERR_TIME_OUT        == (__errno)) ? "Timeout" :\
		((AK_ERR_OUT_OF_RANGE    == (__errno)) ? "Out of Range" :\
		((AK_ERR_OUT_OF_MEM      == (__errno)) ? "Out of Memory" :\
		((AK_ERR_OUT_OF_BACKLOG  == (__errno)) ? "Out of Backlog" :\
		((AK_ERR_BUSY            == (__errno)) ? "Resource Busy" :\
		((AK_ERR_IO              == (__errno)) ? "I/O Error" :\
		((AK_ERR_NOT_ACCESS      == (__errno)) ? "Not Accessable" :\
		((AK_ERR_NOT_EXIST       == (__errno)) ? "Not Exist" :\
		((AK_ERR_NOT_SUPPORT     == (__errno)) ? "Not Support" :\
		((AK_ERR_NOT_PERM        == (__errno)) ? "Not Permission" :\
		((AK_ERR_DEAD_LOCK       == (__errno)) ? "Dead Lock" :\
		"Unknown"\
	)))))))))))))))))))




AK_C_HEADER_EXTERN_C_END
#endif /* TYPEDEF_H_ */
