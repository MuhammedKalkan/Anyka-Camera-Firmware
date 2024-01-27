/*
 * N1 协议数据结构定义。
 *
 */

#include <stdio.h>
#include <NkUtils/types.h>
#include <NkUtils/log.h>
#include <NkUtils/assert.h>
#include <NkUtils/macro.h>

#ifndef NK_N1_DEF_H_
#define NK_N1_DEF_H_
NK_CPP_EXTERN_BEGIN


/**
 * @macro
 *  定义最大通道数。
 */
#define NK_N1_DEV_MAX_MEDIA_CH           (64)

/**
 * @macro
 *  定义最大码流数。
 */
#define NK_N1_DEV_MAX_MEDIA_CH_STREAM    (4)

/**
 * @macro
 *  定义最大码流点播数。
 */
#define NK_N1_DEV_MAX_MEDIA_STREAM_ONCMD (16)


/**
 * @macro
 *  32 位整型标识码转换。
 */
#define NK_N1_UINT32_SYM(__n1, __n2, __n3, __n4) (((__n1)<<24)|((__n2)<<16)|((__n3)<<8)|((__n4)<<0))


#define NK_N1_H264_PROFILE_BASELINE (1)
#define NK_N1_H264_PROFILE_MAIN (2)
#define NK_N1_H264_PROFILE_HIGH (3)
/**
 * 属性选项字符串最大长度。
 */
#define NK_N1_PROP_STR_MAX_LEN (128)

/**
 * 属性选项最大个数。
 */
#define NK_N1_PROP_OPT_MAX_ENT (32)

/**
 * @brief 属性类型定义。
 */
typedef enum Nk_N1PropType
{

	NK_N1_PROP_TYPE_UNDEF = (-1),

	/**
	 * 布尔属性。
	 */
	NK_N1_PROP_TYPE_BOOL = 0,
	/**
	 * 整型属性。
	 */
	NK_N1_PROP_TYPE_INT,
	/**
	 * 长整型属性。
	 */
	NK_N1_PROP_TYPE_INT64,
	/**
	 * 枚举行属性。
	 */
	NK_N1_PROP_TYPE_ENUM,
	/**
	 * 浮点型属性。
	 */
	NK_N1_PROP_TYPE_FLOAT,
	/**
	 * 文本型属性。
	 */
	NK_N1_PROP_TYPE_STRING,
	/**
	 * 物理地址属性。
	 */
	NK_N1_PROP_TYPE_HWADDR,
	/**
	 * IPv4 地址属性。
	 */
	NK_N1_PROP_TYPE_IPV4,

} NK_N1PropType;


/**
 * 布尔属性数据结构。
 */
typedef struct Nk_N1PropBoolean
{
	/**
	 * 属性只读标识。
	 */
	NK_Boolean read_only;

	/**
	 * 属性类型。
	 */
	NK_N1PropType type;

	NK_Boolean val;

} NK_N1PropBoolean;

/**
 * 检测属性数据结构是否合法。
 */
static inline NK_PChar
NK_N1_PROP_BOOL_CHECK(NK_N1PropBoolean *(__Prop))
{
	if (NK_True != (__Prop)->val && NK_False != (__Prop)->val) {
		return "Value Error.";
	}
	return NK_Nil;
}

/**
 * 终端打印 NK_N1PropBoolean 数据结构。
 * 主要用于调试。
 */
static inline NK_Void
NK_N1_PROP_BOOL_DUMP(NK_N1PropBoolean *(__Prop), const NK_PChar val_name)
{
	NK_TermTable Table;
	NK_PChar err = NK_Nil;

	NK_TermTbl_BeginDraw(&Table, "", 32, 4);
	err = NK_N1_PROP_BOOL_CHECK((__Prop));
	if (err) {
		NK_TermTbl_PutText(&Table, NK_True, "Error: %s", err);
	}
	NK_TermTbl_PutKeyValue(&Table, NK_True, "Value", "%s", (__Prop)->val ? "True" : "False");
	NK_TermTbl_EndDraw(&Table);
}

/**
 * @brief 整型属性数据结构。
 */
typedef struct Nk_N1PropInteger
{
	/**
	 * 属性只读标识。
	 */
	NK_Boolean read_only;

	/**
	 * 属性类型。
	 */
	NK_N1PropType type;

	/**
	 * 属性数值。
	 */
	NK_Int32 val;

	/**
	 * 默认数值值。\n
	 * 某些场合下，当属性数值发生错误时，默认可能作为调整参考依据。
	 */
	NK_Int32 def;

	/**
	 * 属性的最大值最小值。\n
	 * 当最大值和最小值均为 0 的时候表示属性数值不受最大最小值约束。\n
	 * 否则数值和默认数值必须处于最大最小值中间，属性才合法。
	 */
	NK_Int min, max;

	/**
	 * 属性选项数据结构。\n
	 * 当 Option 不为 Nil 时，@ref min 和 @ref max 数值无效。\n
	 * 数值的取值范围以选项为参考。
	 */
	struct {
		NK_Size entries;
		NK_Int32 opt[NK_N1_PROP_OPT_MAX_ENT];
	} _Option, *Option;

} NK_N1PropInteger;

/**
 * 检测属性数据结构是否合法。
 */
static inline NK_PChar
NK_N1_PROP_INT_CHECK(NK_N1PropInteger *(__Prop))
{
	NK_Int i = 0;

	if (!(__Prop)->Option) {
		/// 检查变量和默认值是否在范围内。
		if (!(0 == (__Prop)->min && 0 == (__Prop)->max)) {
			if ((__Prop)->min >= (__Prop)->max) {
				return "Range Error.";
			}
			if ((__Prop)->val > (__Prop)->max || (__Prop)->val < (__Prop)->min) {
				return "Value NOT in Range.";
			}
//			if ((__Prop)->def > (__Prop)->max || (__Prop)->def < (__Prop)->min) {
//				return "Default Value NOT in Range.";
//			}
		}
	} else {
		if ((__Prop)->Option->entries > sizeof((__Prop)->Option->opt) / sizeof((__Prop)->Option->opt[0])
				|| !(__Prop)->Option->entries) {

			return "Options Entires Error.";
		}
		for (i = 0; i < (NK_Int)(__Prop)->Option->entries; ++i) {
			if ((__Prop)->Option->opt[i] == (__Prop)->val) {
				break;
			}
		}
		if (i == (__Prop)->Option->entries) {
			return "Value NOT in Options.";
		}
		for (i = 0; i < (NK_Int)(__Prop)->Option->entries; ++i) {
			if ((__Prop)->Option->opt[i] == (__Prop)->def) {
				break;
			}
		}
		if (i == (NK_Int)(__Prop)->Option->entries) {
			return "Default Value NOT in Options.";
		}
	}
	return NK_Nil;
}

/**
 * 终端打印 NK_N1PropInteger 数据结构。
 * 主要用于调试。
 */
#define NK_N1_PROP_INT_DUMP(__Prop, val_name) \
	do {\
		NK_Int i = 0;\
		NK_TermTable Table;\
		NK_PChar err = NK_Nil;\
		NK_TermTbl_BeginDraw(&Table, !(val_name) ? "N1 Property Integer" : (val_name), 32, 4);\
		err = NK_N1_PROP_INT_CHECK((__Prop));\
		if (err) {\
			NK_TermTbl_PutText(&Table, NK_True, "Error: %s", err);\
		}\
		NK_TermTbl_PutKeyValue(&Table, NK_True, "Value", "%d", (__Prop)->val);\
		if (NK_Nil != (__Prop)->Option) {\
			NK_TermTbl_PutKeyValue(&Table, NK_True, "Default", "%d", (__Prop)->def);\
			NK_TermTbl_PutText(&Table, NK_True, "%-28s", "Options");\
			for (i = 0; i < (NK_Int)(__Prop)->Option->entries; ++i) {\
				NK_TermTbl_PutText(&Table, NK_False, "%28d", (__Prop)->Option->opt[i]);\
			}\
		} else {\
			NK_TermTbl_PutKeyValue(&Table, NK_True, "Minimum", "%d", (__Prop)->min);\
			NK_TermTbl_PutKeyValue(&Table, NK_True, "Maximum", "%d", (__Prop)->max);\
			NK_TermTbl_PutKeyValue(&Table, NK_True, "Default", "%d", (__Prop)->def);\
		}\
		NK_TermTbl_EndDraw(&Table);\
	} while (0)

/**
 * @brief 长整型属性数据结构。
 */
typedef struct Nk_N1PropInt64
{
	/**
	 * 属性只读标识。
	 */
	NK_Boolean read_only;

	/**
	 * 属性类型。
	 */
	NK_N1PropType type;

	NK_Int64 val, min, max, def;

	struct {
		NK_Size entries;
		NK_Int64 opt[NK_N1_PROP_OPT_MAX_ENT];
	} _Option, *Option;

} NK_N1PropInt64;

/**
 * 数据结构检查。
 */
static inline NK_PChar
NK_N1_PROP_INT64_CHECK(NK_N1PropInt64 *(__Prop))
{
        NK_INT i;
	/// 判断取值大小是否在相应的取值域内
        if ((__Prop)->Option == NK_Nil){
          if (!(0 == (__Prop)->min && 0 == (__Prop)->max)) {
           if ((__Prop)->min > (__Prop)->max){
              return "Range Error.";
           }
           if ((__Prop)->val > (__Prop)->max || (__Prop)->val < (__Prop)->min){
              return "Value Not In Range.";
           }
//           if ((__Prop)->def > (__Prop)->max || (__Prop)->def < (__Prop)->min){
//             return "Default Value Not In  Range.";
//          }
          }
        }else{
           if ((__Prop)->Option->entries > (sizeof((__Prop)->Option->opt)/sizeof((__Prop)->Option->opt[0]))){
              return "Range error!";
           }
           for (i = 0; i < (NK_Int)(__Prop)->Option->entries; i++){
              if ((__Prop)->Option->opt[i] == (__Prop)->val){
                 break;
              }
           }
           if (i == (__Prop)->Option->entries){
              return "Value Not In Options.";
           }

           for (i = 0; i < (NK_Int)(__Prop)->Option->entries; i++){
              if ((__Prop)->Option->opt[i] == (__Prop)->def){
                 break;
              }
           }
           if (i == (__Prop)->Option->entries){
              return "Default Value Not In Options.";
           }
        }

	return NK_Nil;
}

/**
 * 终端打印 NK_N1PropInteger 数据结构。
 * 主要用于调试。
 */
static inline NK_Void
NK_N1_PROP_INT64_DUMP(NK_N1PropInt64 *(__Prop), const NK_PChar val_name)
{
//	NK_Int i = 0;
//	NK_TermTable Table;
//	NK_PChar err = NK_Nil;
//
//	NK_TermTbl_BeginDraw(&Table, !(val_name) ? "N1 Property Int64" : (val_name), 32, 4);
//
//	err = NK_N1_PROP_INT64_CHECK((__Prop));
//	if (err) {
//		NK_TermTbl_PutText(&Table, NK_True, "Error: %s", err);
//	}
//	NK_TermTbl_PutKeyValue(&Table, NK_True, "Value", "%lld", (__Prop)->val);
//	if (NK_Nil != (__Prop)->Option) {
//		NK_TermTbl_PutKeyValue(&Table, NK_True, "Default", "%lld", (__Prop)->def);
//		NK_TermTbl_PutText(&Table, NK_True, "%-28s", "Options");
//		for (i = 0; i < (NK_Int)(__Prop)->Option->entries; ++i) {
//			NK_TermTbl_PutText(&Table, NK_False, "%28lld", (__Prop)->Option->opt[i]);
//		}
//	} else {
//		NK_TermTbl_PutKeyValue(&Table, NK_True, "Minimum", "%lld", (__Prop)->min);
//		NK_TermTbl_PutKeyValue(&Table, NK_True, "Maximum", "%lld", (__Prop)->max);
//		NK_TermTbl_PutKeyValue(&Table, NK_True, "Default", "%lld", (__Prop)->def);
//	}
//	NK_TermTbl_EndDraw(&Table);
}

/**
 * @brief 枚举属性数据结构。
 *
 * 枚举属性类似与整型属性。\n
 * 区别于整型属性，枚举属性没有最大最小的限制，但必须具备选项。\n
 * 每个枚举选项数值后必须有一个文本与之对应。
 */
typedef struct Nk_N1PropEnum
{
	/**
	 * 属性只读标识。
	 */
	NK_Boolean read_only;

	/**
	 * 属性类型。
	 */
	NK_N1PropType type;

	/**
	 * 属性数值和默认数值。
	 */
	NK_UInt32 val, def;

	struct {
		/**
		 * 有效选项的个数。
		 */
		NK_Size entries;
		/**
		 * 选项数值。
		 */
		NK_UInt32 opt[NK_N1_PROP_OPT_MAX_ENT];
		/**
		 * 选项数值对应的文本。
		 */
		NK_PChar str[NK_N1_PROP_OPT_MAX_ENT];

	} _Option, *Option;

} NK_N1PropEnum;

/**
 * 数据结构检查。
 */
static inline NK_PChar
NK_N1_PROP_ENUM_CHECK(NK_N1PropEnum *(__Prop))
{
	NK_Int i = 0;;
	/// 枚举结构数据取值条件判断
	if (!(__Prop)->Option) {
		return "Option NULL";
	}else{
		if (!((__Prop)->Option->entries > 0
				&& (__Prop)->Option->entries < (sizeof((__Prop)->Option->opt)/sizeof((__Prop)->Option->opt[0])))) {
			return "Option Range Error.";
		}
		/// 遍历所有枚举选项，每个枚举值不应该存在空字符串对应。
		for (i = 0; i < (NK_Int)(__Prop)->Option->entries; ++i) {
			if (!(__Prop)->Option->str[i]) {
				return "Option Text Error.";
			}
		}
		/// 遍历所有枚举选项，确保数值在枚举选项以内。
		for (i = 0; i < (NK_Int)(__Prop)->Option->entries; i++) {
			if ((__Prop)->Option->opt[i] == (__Prop)->val) {
				break;
			}
		}
		if (i == (__Prop)->Option->entries) {
			return "Value Not In Options.";
		}
	}
	return NK_Nil;
}
/**
 * 终端打印 NK_N1PropEnum 数据结构。
 * 主要用于调试。
 */
static inline NK_Void
NK_N1_PROP_ENUM_DUMP(NK_N1PropEnum *(__Prop), const NK_PChar val_name)
{
//	NK_Int i = 0;
//	NK_TermTable Table;
//	NK_PChar err = NK_Nil;
//
//	NK_TermTbl_BeginDraw(&Table, !(val_name) ? "N1 Property Enum" : (val_name), 32, 4);
//
//	err = NK_N1_PROP_ENUM_CHECK((__Prop));
//	if (err) {
//		NK_TermTbl_PutText(&Table, NK_True, "Error: %s", err);
//	}
//	NK_TermTbl_PutKeyValue(&Table, NK_True, "Value", "%u", (__Prop)->val);
//	if (NK_Nil != (__Prop)->Option) {
//		NK_TermTbl_PutKeyValue(&Table, NK_True, "Default", "%u", (__Prop)->def);
//		NK_TermTbl_PutText(&Table, NK_True, "%-28s", "Options");
//		for (i = 0; i < (NK_Int)(__Prop)->Option->entries; ++i) {
//			NK_TermTbl_PutText(&Table, NK_False, "%u: %s",
//					(__Prop)->Option->opt[i], (__Prop)->Option->str[i]);
//		}
//	}
//	NK_TermTbl_EndDraw(&Table);
}

/**
 * @brief 浮点属性数据结构。
 */
typedef struct Nk_N1PropFloat
{
	/**
	 * 属性只读标识。
	 */
	NK_Boolean read_only;

	/**
	 * 属性类型。
	 */
	NK_N1PropType type;

	NK_DFloat val, min, max, def;

	struct {
		NK_Size entries;
		NK_DFloat opt[NK_N1_PROP_OPT_MAX_ENT];
	} _Option, *Option;

} NK_N1PropFloat;

/**
 * 数据结构检查。
 */
static inline NK_PChar
NK_N1_PROP_FLOAT_CHECK(NK_N1PropFloat *(__Prop))
{
        NK_INT i;
	/// 浮点数据结构取值判断
        if ((__Prop)->Option == NK_Nil) {
          if (!(0 == (__Prop)->min && 0 == (__Prop)->max)) {
           if ((__Prop)->min > (__Prop)->max) {
              return "Range error.";
           }
           if ((__Prop)->val > (__Prop)->max || (__Prop)->val < (__Prop)->min) {
              return "Value Not In Range.";
           }
//           if ((__Prop)->def > (__Prop)->max || (__Prop)->def < (__Prop)->min) {
//              return "DefValue Not In Range.";
//          }
          }
        }else{
           if ((__Prop)->Option->entries > (sizeof((__Prop)->Option->opt)/sizeof((__Prop)->Option->opt[0]))) {
              return  "Range Error.";
           }
           for (i = 0; i < (NK_Int)(__Prop)->Option->entries; i++) {
              if ((__Prop)->Option->opt[i] == (__Prop)->val) {
                 break;
              }
           }
           if (i == (__Prop)->Option->entries) {
              return "Value Not In Options.";
           }

           for (i = 0; i < (NK_Int)(__Prop)->Option->entries; i++) {
              if ((__Prop)->Option->opt[i] == (__Prop)->def) {
                 break;
              }
           }
           if (i == (__Prop)->Option->entries) {
              return "Default Value Not In Options.";
           }
        }
	return NK_Nil;
}

/**
 * 终端打印 NK_N1PropEnum 数据结构。
 * 主要用于调试。
 */
static inline
NK_Void NK_N1_PROP_FLOAT_DUMP(NK_N1PropFloat *(__Prop), const NK_PChar val_name)
{
//	NK_Int i = 0;
//	NK_TermTable Table;
//	NK_PChar err = NK_Nil;
//
//	NK_TermTbl_BeginDraw(&Table, !(val_name) ? "N1 Property Float" : (val_name), 32, 4);
//
//	err = NK_N1_PROP_FLOAT_CHECK((__Prop));
//	if (err) {
//		NK_TermTbl_PutText(&Table, NK_True, "Error: %s", err);
//	}
//	NK_TermTbl_PutKeyValue(&Table, NK_True, "Value", "%.5f", (__Prop)->val);
//	if (NK_Nil != (__Prop)->Option) {
//		NK_TermTbl_PutKeyValue(&Table, NK_True, "Default", "%.5f", (__Prop)->def);
//		NK_TermTbl_PutText(&Table, NK_True, "%-28s", "Options");
//		for (i = 0; i < (NK_Int)(__Prop)->Option->entries; ++i) {
//			NK_TermTbl_PutText(&Table, NK_False, "%22.5f", (__Prop)->Option->opt[i]);
//		}
//	} else {
//		NK_TermTbl_PutKeyValue(&Table, NK_True, "Minimum", "%.5f", (__Prop)->min);
//		NK_TermTbl_PutKeyValue(&Table, NK_True, "Maximum", "%.5f", (__Prop)->max);
//		NK_TermTbl_PutKeyValue(&Table, NK_True, "Default", "%.5f", (__Prop)->def);
//	}
//	NK_TermTbl_EndDraw(&Table);
}

/**
 * 文本属性数据结构。
 */
typedef struct Nk_N1PropString
{
	/**
	 * 属性只读标识。
	 */
	NK_Boolean read_only;

	/**
	 * 属性类型。
	 */
	NK_N1PropType type;

	NK_Char val[NK_N1_PROP_STR_MAX_LEN + 1], def[NK_N1_PROP_STR_MAX_LEN + 1];

	/**
	 * 文本的最大长度，区别于 @ref NK_N1_PROP_STR_MAX_LEN。
	 * 若文本长度在此长度以内才合法。
	 */
	NK_Size max_len;

	/**
	 * 参考 NK_N1PropInteger::Option。
	 */
	struct {
		NK_Size entries;
		NK_PChar opt[NK_N1_PROP_OPT_MAX_ENT];
	} _Option, *Option;

} NK_N1PropString;

/**
 * 设置字符串属性。
 */
#define NK_N1_PROP_STR_SET(__Prop, __str) \
	do{\
		NK_Int i = 0;\
		if (NK_Nil != (__str)) {\
			if (NK_Nil != (__Prop)->Option) {\
				/** 须要符合选项中数据。*/\
				for (i = 0; i < (__Prop)->Option->entries; ++i) {\
					if (NK_Nil != (__Prop)->Option->opt[i] && NK_STRCMP((__Prop)->Option->opt[i], (__str))) {\
						(__Prop)->max_len = 0;\
						snprintf((__Prop)->val, sizeof((__Prop)->val), "%s", (__str));\
					}\
				}\
			} else {\
				snprintf((__Prop)->val, 0 == (__Prop)->max_len ? sizeof((__Prop)->val) : (__Prop)->max_len, "%s", (__str));\
			}\
		}\
	} while(0)

/**
 * 数据结构检查。
 */
static inline NK_PChar
NK_N1_PROP_STR_CHECK(NK_N1PropString *(__Prop))
{
        NK_INT i;
	/// 检查字符串的取值条件和范围
        if ((__Prop)->max_len < 0) {
           return "String Length Value Error.";
        }
        if ((__Prop)->max_len < strlen((__Prop)->val)) {
           return "Value String Length Error.";
        }
        if ((__Prop)->max_len < strlen((__Prop)->def)) {
           return "Default String Length Error.";
        }

        if ((__Prop)->Option->entries > (sizeof((__Prop)->Option->opt)/sizeof((__Prop)->Option->opt[0]))) {
           return "Range Error.";
        }

        for (i = 0; i < (NK_Int)(__Prop)->Option->entries; i++) {
           if (strlen((__Prop)->Option->opt[i]) <= (__Prop)->max_len) {
              if (strcmp((__Prop)->Option->opt[i], (__Prop)->val) == 0) {
                 break;
              }
           }
           else{
              return "Value String Length Error.";
           }
        }

        if (i == (__Prop)->Option->entries) {
           return "Value Not In Options.";
        }

        for (i = 0;i < (NK_Int)(__Prop)->Option->entries; i++) {
           if (strlen((__Prop)->Option->opt[i]) <= (__Prop)->max_len) {
              if (strcmp((__Prop)->Option->opt[i], (__Prop)->def) == 0) {
                 break;
              }
           }
           else{
              return "Defaule Value String Length Error.";
           }
        }
        if (i == (__Prop)->Option->entries) {
           if (i == (__Prop)->Option->entries) {
              return "Default Value Not In Options.";
           }
        }

	return NK_Nil;
}

/**
 * 终端打印 NK_N1PropEnum 数据结构。
 * 主要用于调试。
 */
static inline
NK_Void NK_N1_PROP_STRING_DUMP(NK_N1PropString *(__Prop), const NK_PChar val_name)
{
//	NK_Int i;
//	NK_TermTable Table;
//	NK_PChar err = NK_Nil;
//	NK_TermTbl_BeginDraw(&Table, !(val_name) ? "N1 Property String" : (val_name), 32, 4);
//	/// 判断字符串数据类型的取值和范围
//	err = NK_N1_PROP_STR_CHECK((__Prop));
//	if (err) {
//		NK_TermTbl_PutText(&Table, NK_True, "Error: %s", err);
//	}
//	NK_TermTbl_PutKeyValue(&Table, NK_True, "Value", "%s", (__Prop)->val);
//	if ((__Prop)->Option != NK_Nil) {
//		NK_TermTbl_PutKeyValue(&Table, NK_True, "Default", "%s", (__Prop)->def);
//		NK_TermTbl_PutText(&Table, NK_True, "%s", "Options");
//		for (i = 0; i < (NK_Int)(__Prop)->Option->entries; i++) {
//			NK_TermTbl_PutText(&Table, NK_False, "%s", (__Prop)->Option->opt[i]);
//		}
//	}
//	NK_TermTbl_EndDraw(&Table);
}

/**
 * @brief 设备物理地址属性数据结构。
 */
typedef struct Nk_N1PropHwAddr
{
	/**
	 * 属性只读标识。
	 */
	NK_Boolean read_only;

	/**
	 * 属性类型。
	 */
	NK_N1PropType type;

	NK_UInt8 val[6];

} NK_N1PropHwAddr;

/**
 * 设置 HW 地址属性的地址。
 */
#define NK_N1_PROP_HWADDR_SET(__Prop, __hw0, __hw1, __hw2, __hw3, __hw4, __hw5) \
	do{\
		(__Prop)->val[0] = (NK_UInt32)(__hw0);\
		(__Prop)->val[1] = (NK_UInt32)(__hw1);\
		(__Prop)->val[2] = (NK_UInt32)(__hw2);\
		(__Prop)->val[3] = (NK_UInt32)(__hw3);\
		(__Prop)->val[4] = (NK_UInt32)(__hw4);\
		(__Prop)->val[5] = (NK_UInt32)(__hw5);\
	} while(0)

/**
 * 文本转换成 HW 地址属性。
 */
#define NK_N1_PROP_HWADDR_ATON(__Prop, __hw_text) \
	do{\
		NK_Size len = strlen(__hw_text);\
		NK_Char *chr, ch;\
		NK_Int i = 0, ii = 0;\
		for (i = 0; i < 6; ++i) {\
			chr = __hw_text + i * 3;\
			if (chr < (__hw_text) + len) {\
				for (ii = 0; ii < 2; ++ii) {\
					if ((chr[ii] >= '0' && chr[ii] <= '9') \
							|| (chr[ii] >= 'a' && chr[ii] <= 'f') \
							|| (chr[ii] >= 'A' && chr[ii] <= 'F')) {\
						if (chr[ii] >= 'a' && chr[ii] <= 'f') ch = chr[ii] - 'a' + 10;\
						else if (chr[ii] >= 'A' && chr[ii] <= 'F') ch = chr[ii] - 'A' + 10;\
						else if (chr[ii] >= '0' && chr[ii] <= '9') ch = chr[ii] - '0';\
						else ch = 0;\
						\
						if (0 == ii) {\
							(__Prop)->val[i] = (ch << 4);\
						} else {\
							(__Prop)->val[i] |= ch;\
						}\
					}\
				}\
			}\
		}\
	} while(0);

/**
 * HW 地址属性转换成文本。
 */
#define NK_N1_PROP_HWADDR_NTOA(__Prop, __text, __size) \
	snprintf(__text, (__size), "%02x:%02x:%02x:%02x:%02x:%02x",\
		(NK_UInt32)((__Prop)->val[0]),\
		(NK_UInt32)((__Prop)->val[1]),\
		(NK_UInt32)((__Prop)->val[2]),\
		(NK_UInt32)((__Prop)->val[3]),\
		(NK_UInt32)((__Prop)->val[4]),\
		(NK_UInt32)((__Prop)->val[5]))

#define NK_N1_PROP_HWADDR_STR NK_N1_PROP_HWADDR_NTOA

/**
 * 数据结构检查。
 */
static inline NK_PChar
NK_N1_PROP_HWADDR_CHECK(NK_N1PropHwAddr *(__Prop))
{
	if ((0 == (__Prop)->val[0] && 0 == (__Prop)->val[1]
			&& 0 == (__Prop)->val[2] && 0 == (__Prop)->val[3]
			&& 0 == (__Prop)->val[4] && 0 == (__Prop)->val[5])
			|| (0xff == (__Prop)->val[0] && 0xff == (__Prop)->val[1]
					&& 0xff == (__Prop)->val[2] && 0xff == (__Prop)->val[3]
					&& 0xff == (__Prop)->val[4] && 0xff == (__Prop)->val[5])) {
		return "Ivalid Address.";
	}
	return NK_Nil;
}

/**
 * 终端打印 NK_N1PropHwAddr 数据结构。
 * 主要用于调试。
 */
static inline NK_Void
NK_N1_PROP_HWADDR_DUMP(NK_N1PropHwAddr *(__Prop), const NK_PChar val_name)
{
//	NK_TermTable Table;
//	NK_PChar err = NK_Nil;
//	NK_Char text[32];
//
//	NK_TermTbl_BeginDraw(&Table, !(val_name) ? "N1 Property Hardware Address" : (val_name), 64, 4);
//	err = NK_N1_PROP_HWADDR_CHECK((__Prop));
//	if (err) {
//		NK_TermTbl_PutText(&Table, NK_True, "Error: %s", err);
//	}
//	NK_N1_PROP_HWADDR_NTOA((__Prop), text, sizeof(text));
//	NK_TermTbl_PutKeyValue(&Table, NK_True, "Value", "%s", text);
//	NK_TermTbl_EndDraw(&Table);
}


/**
 * @brief IPv4 地址属性数据结构。
 */
typedef struct Nk_N1PropIPv4
{
	/**
	 * 属性只读标识。
	 */
	NK_Boolean read_only;

	/**
	 * 属性类型。
	 */
	NK_N1PropType type;

	NK_UInt8 val[4];

} NK_N1PropIPv4;

/**
 * 设置 IPv4 属性的地址。
 */
#define NK_N1_PROP_IPV4_SET(__Prop, __ip0, __ip1, __ip2, __ip3) \
	do{\
		(__Prop)->val[0] = (NK_UInt32)(__ip0);\
		(__Prop)->val[1] = (NK_UInt32)(__ip1);\
		(__Prop)->val[2] = (NK_UInt32)(__ip2);\
		(__Prop)->val[3] = (NK_UInt32)(__ip3);\
	} while(0)

/**
 * 文本转换成 IPv4 属性设置。
 */
#define NK_N1_PROP_IPV4_ATON(__Prop, __ipv4_text) \
	do{\
		NK_Char *ip0, *ip1, *ip2, *ip3;\
		ip0 = __ipv4_text;\
		if (NK_Nil != ip0) {\
			ip1 = strchr(ip0, '.');\
			if (NK_Nil != ip1++) {\
				ip2 = strchr(ip1, '.');\
				if (NK_Nil != ip2++) {\
					ip3 = strchr(ip2, '.');\
					if (NK_Nil != ip3++) {\
						(__Prop)->val[0] = atoi(ip0);\
						(__Prop)->val[1] = atoi(ip1);\
						(__Prop)->val[2] = atoi(ip2);\
						(__Prop)->val[3] = atoi(ip3);\
					}\
				}\
			}\
		}\
	} while(0);


/**
 * IPv4 属性转换成文本。
 */
#define NK_N1_PROP_IPV4_NTOA(__Prop, __text, __size) \
	snprintf(__text, (__size), "%d.%d.%d.%d",\
		(NK_Int)((__Prop)->val[0]),\
		(NK_Int)((__Prop)->val[1]),\
		(NK_Int)((__Prop)->val[2]),\
		(NK_Int)((__Prop)->val[3]))

#define NK_N1_PROP_IPV4_STR NK_N1_PROP_IPV4_NTOA

/**
 * 数据结构检查。
 */
static inline NK_PChar
NK_N1_PROP_IPV4_CHECK(NK_N1PropIPv4 *(__Prop))
{
	if (0 == (__Prop)->val[0] && 0 == (__Prop)->val[1]
			&& 0 == (__Prop)->val[2] && 0 == (__Prop)->val[3]) {
		return "Ivalid Address.";
	}
	return NK_Nil;
}

/**
 * 终端打印 NK_N1PropIPv4 数据结构。
 * 主要用于调试。
 */
static inline NK_Void
NK_N1_PROP_IPV4_DUMP(NK_N1PropIPv4 *(__Prop), const NK_PChar val_name)
{
//	NK_TermTable Table;
//	NK_PChar err = NK_Nil;
//
//	NK_TermTbl_BeginDraw(&Table, !(val_name) ? "N1 Property IPv4" : (val_name), 32, 4);
//	err = NK_N1_PROP_IPV4_CHECK((__Prop));
//	if (err) {
//		NK_TermTbl_PutText(&Table, NK_True, "Error: %s", err);
//	}
//	NK_TermTbl_PutKeyValue(&Table, NK_True, "Value", "%d.%d.%d.%d",
//			(__Prop)->val[0], (__Prop)->val[1], (__Prop)->val[2], (__Prop)->val[3]);
//	NK_TermTbl_EndDraw(&Table);
}

/**
 * 属性集合。
 */
typedef struct Nk_N1Property
{
	union {

		struct {

			/**
			 * 属性只读标识。
			 */
			NK_Boolean read_only;

			/**
			 * 属性类型。
			 */
			NK_N1PropType type;
		};

		NK_N1PropBoolean Boolean;
		NK_N1PropInteger Integer;
		NK_N1PropInt64 Integer64;
		NK_N1PropEnum Enum;
		NK_N1PropFloat Float;
		NK_N1PropString String;
		NK_N1PropHwAddr HwAddr;
		NK_N1PropIPv4 IPv4;
	};

} NK_N1Property;

/**
 * 数据结构检查。
 */
#define NK_N1_PROP_CHECK(__Prop) \
	((NK_N1_PROP_TYPE_BOOL == (__Prop)->type) \
	 ? NK_N1_PROP_BOOL_CHECK(&(__Prop)->Boolean) \
			:(NK_N1_PROP_TYPE_INT == (__Prop)->type) \
			 ?  NK_N1_PROP_INT_CHECK(&(__Prop)->Integer) \
					 :(NK_N1_PROP_TYPE_INT64 == (__Prop)->type) \
					  ? NK_N1_PROP_INT64_CHECK(&(__Prop)->Integer64) \
							  :(NK_N1_PROP_TYPE_ENUM == (__Prop)->type) \
							   ? NK_N1_PROP_ENUM_CHECK(&(__Prop)->Enum) \
									   :(NK_N1_PROP_TYPE_FLOAT == (__Prop)->type) \
										? NK_N1_PROP_FLOAT_CHECK(&(__Prop)->Float) \
												:(NK_N1_PROP_TYPE_STRING == (__Prop)->type) \
												 ? NK_N1_PROP_STRING_CHECK(&(__Prop)->String) \
														 :(NK_N1_PROP_TYPE_HWADDR == (__Prop)->type) \
														  ? NK_N1_PROP_HWADDR_CHECK(&(__Prop)->HwAddr) \
																  :(NK_N1_PROP_TYPE_IPV4 == (__Prop)->type) \
																   ? NK_N1_PROP_IPV4_CHECK(&(__Prop)->IPv4) \
																		   : "Invalid Property.")

/**
 * 终端打印 NK_N1PropSet 数据结构。
 * 主要用于调试。
 */
#define NK_N1_PROP_DUMP(__Prop, __val_name) \
	do{\
		if (NK_N1_PROP_TYPE_BOOL == (__Prop)->type) {\
			NK_N1_PROP_BOOL_DUMP(&(__Prop)->Boolean, __val_name);\
		} else if (NK_N1_PROP_TYPE_INT == (__Prop)->type) {\
			NK_N1_PROP_INT_DUMP(&(__Prop)->Integer, __val_name);\
		} else if (NK_N1_PROP_TYPE_INT64 == (__Prop)->type) {\
			NK_N1_PROP_INT64_DUMP(&(__Prop)->Integer64, __val_name);\
		} else if (NK_N1_PROP_TYPE_ENUM == (__Prop)->type) {\
			NK_N1_PROP_ENUM_DUMP(&(__Prop)->Enum, __val_name);\
		} else if (NK_N1_PROP_TYPE_FLOAT == (__Prop)->type) {\
			NK_N1_PROP_FLOAT_DUMP(&(__Prop)->Float, __val_name);\
		} else if (NK_N1_PROP_TYPE_STRING == (__Prop)->type) {\
			NK_N1_PROP_STRING_DUMP(&(__Prop)->String, __val_name);\
		} else if (NK_N1_PROP_TYPE_HWADDR == (__Prop)->type) {\
			NK_N1_PROP_HWADDR_DUMP(&(__Prop)->HwAddr, __val_name);\
		} else if (NK_N1_PROP_TYPE_IPV4 == (__Prop)->type) {\
			NK_N1_PROP_IPV4_DUMP(&(__Prop)->IPv4, __val_name);\
		}\
	} while (0)

/**
 * 属性追加一个选项。
 */
#define NK_N1_PROP_ADD_OPT(__Prop, __val) \
	do{\
		if (!(__Prop)->Option){\
			(__Prop)->Option = &((__Prop)->_Option);\
			(__Prop)->Option->entries = 0;\
			(__Prop)->Option->opt[(__Prop)->Option->entries++] = (__val);\
		}\
		if ((__Prop)->Option->entries >= NK_N1_PROP_OPT_MAX_ENT){\
		/* 选项配额已满。 */\
			break;\
		}\
		NK_Size size;\
		for(size = 0; size < (__Prop)->Option->entries; size++)\
		{\
			/* 选项不存在才添加 */\
			if(__val == (__Prop)->Option->opt[size])\
			{\
				break;\
			}else{\
				if(size == (__Prop)->Option->entries - 1)\
				{\
					(__Prop)->Option->opt[(__Prop)->Option->entries++] = (__val);\
				}\
            }\
		}\
	} while (0)

/**
 * 属性追加一个枚举选项。
 */
#define NK_N1_PROP_ADD_ENUM(__Prop, __type, __opt) \
	do{\
		NK_Size opt_entries = NK_Nil != (__Prop)->Option ? (__Prop)->Option->entries : 0;\
		NK_N1_PROP_ADD_OPT(__Prop, __opt);\
		if (opt_entries + 1 == (__Prop)->Option->entries)\
			(__Prop)->Option->str[(__Prop)->Option->entries - 1] = NK_ENUM_MAP(__type, __opt);\
	} while(0)


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


/**
 * 图像尺寸。
 */
#define NK_N1_IMG_SZ(__width, __height) ((NK_ALIGN_BIG_END(__width, 2) * 10000) + NK_ALIGN_BIG_END(__height, 2))
#define NK_N1_IMG_SZ_UNDEF      NK_N1_IMG_SZ(0, 0)
#define NK_N1_IMG_SZ_160X90     NK_N1_IMG_SZ(160, 90)
#define NK_N1_IMG_SZ_160X120    NK_N1_IMG_SZ(160, 120)
#define NK_N1_IMG_SZ_172X144    NK_N1_IMG_SZ(172, 144)
#define NK_N1_IMG_SZ_320X180    NK_N1_IMG_SZ(320, 180)
#define NK_N1_IMG_SZ_320X240    NK_N1_IMG_SZ(320, 240)
#define NK_N1_IMG_SZ_352X240    NK_N1_IMG_SZ(352, 240)
#define NK_N1_IMG_SZ_352X288    NK_N1_IMG_SZ(352, 288)
#define NK_N1_IMG_SZ_360X240    NK_N1_IMG_SZ(360, 240)
#define NK_N1_IMG_SZ_360X288    NK_N1_IMG_SZ(360, 288)
#define NK_N1_IMG_SZ_480X270    NK_N1_IMG_SZ(480, 270)
#define NK_N1_IMG_SZ_480X360    NK_N1_IMG_SZ(480, 360)
#define NK_N1_IMG_SZ_480X480    NK_N1_IMG_SZ(480, 480)
#define NK_N1_IMG_SZ_528X384    NK_N1_IMG_SZ(528, 384)
#define NK_N1_IMG_SZ_640X360    NK_N1_IMG_SZ(640, 360)
#define NK_N1_IMG_SZ_640X480    NK_N1_IMG_SZ(640, 480)
#define NK_N1_IMG_SZ_704X240    NK_N1_IMG_SZ(704, 240)
#define NK_N1_IMG_SZ_704X288    NK_N1_IMG_SZ(704, 288)
#define NK_N1_IMG_SZ_704X480    NK_N1_IMG_SZ(704, 480)
#define NK_N1_IMG_SZ_704X576    NK_N1_IMG_SZ(704, 576)
#define NK_N1_IMG_SZ_720X240    NK_N1_IMG_SZ(720, 240)
#define NK_N1_IMG_SZ_720X288    NK_N1_IMG_SZ(720, 288)
#define NK_N1_IMG_SZ_720X480    NK_N1_IMG_SZ(720, 480)
#define NK_N1_IMG_SZ_720X576    NK_N1_IMG_SZ(720, 576)
#define NK_N1_IMG_SZ_720X720    NK_N1_IMG_SZ(720, 720)
#define NK_N1_IMG_SZ_800X600    NK_N1_IMG_SZ(800, 600)
#define NK_N1_IMG_SZ_800X800    NK_N1_IMG_SZ(800, 800)
#define NK_N1_IMG_SZ_960X480    NK_N1_IMG_SZ(960, 480)
#define NK_N1_IMG_SZ_960X576    NK_N1_IMG_SZ(960, 576)
#define NK_N1_IMG_SZ_960X960    NK_N1_IMG_SZ(960, 960)
#define NK_N1_IMG_SZ_1280X720   NK_N1_IMG_SZ(1280, 720)
#define NK_N1_IMG_SZ_1280X960   NK_N1_IMG_SZ(1280, 960)
#define NK_N1_IMG_SZ_1280X1024  NK_N1_IMG_SZ(1280, 1024)
#define NK_N1_IMG_SZ_1056X1056  NK_N1_IMG_SZ(1280, 1024)
#define NK_N1_IMG_SZ_1600X900   NK_N1_IMG_SZ(1600, 900)
#define NK_N1_IMG_SZ_1600X1200  NK_N1_IMG_SZ(1600, 1200)
#define NK_N1_IMG_SZ_1920X1080  NK_N1_IMG_SZ(1920, 1080)
#define NK_N1_IMG_SZ_2048X1512  NK_N1_IMG_SZ(2048, 1512)
#define NK_N1_IMG_SZ_2048X1520  NK_N1_IMG_SZ(2048, 1520)
#define NK_N1_IMG_SZ_2048X1536  NK_N1_IMG_SZ(2048, 1536)
#define NK_N1_IMG_SZ_2304X1296  NK_N1_IMG_SZ(2304, 1296)
#define NK_N1_IMG_SZ_2304X1728  NK_N1_IMG_SZ(2304, 1728)
#define NK_N1_IMG_SZ_2560X1440  NK_N1_IMG_SZ(2560, 1440)
#define NK_N1_IMG_SZ_2592X1520  NK_N1_IMG_SZ(2592, 1520)
#define NK_N1_IMG_SZ_2592X1944  NK_N1_IMG_SZ(2592, 1944)
#define NK_N1_IMG_SZ_2688X1512  NK_N1_IMG_SZ(2688, 1512)
typedef NK_Size NK_N1ImageSize;


/**
 * 编码码率控制模式。
 */
typedef enum Nk_N1BitRateCtrlMode
{
	NK_N1_BR_CTRL_MODE_UNDEF = (-1),
	/**
	 * 恒定码率控制。
	 */
	NK_N1_BR_CTRL_MODE_CBR,

	/**
	 * 可变码率控制。
	 */
	NK_N1_BR_CTRL_MODE_VBR,

} NK_N1BitRateCtrlMode;

/**
 * 音频输入模式。
 */
typedef enum Nk_N1AudioInputMode
{
        NK_N1_AUDIO_INPUT_MODE_UNDEF = (-1),
        /**
         * 音频输入方式。
         */
        NK_N1_AUDIO_INPUT_MODE_AUTO,

        NK_N1_AUDIO_INPUT_MODE_LINE,

	    NK_N1_AUDIO_INPUT_MODE_MIC

} NK_N1AudioInputMode;

/**
 * 获取 NK_N1BitRateCtrlMode 枚举值对应的文本信息。
 */
extern DECLARE_NK_ENUM_MAP(N1BitRateCtrlMode);

/**
 * 获取 NK_N1BitRateCtrlMode 文本信息对应的枚举值。
 */
extern DECLARE_NK_ENUM_UNMAP(N1BitRateCtrlMode);

/**
 * 获取 NK_N1AudioInputMode 枚举值对应的文本信息。
 */
extern DECLARE_NK_ENUM_MAP(N1AudioInputMode);

/**
 * 获取 NK_N1AudioInputMode 文本信息对应的枚举值。
 */
extern DECLARE_NK_ENUM_UNMAP(N1AudioInputMode);

typedef enum Nk_N1VideoEncCodec
{
	NK_N1_VENC_CODEC_UNDEF = (-1),
	NK_N1_VENC_CODEC_MPEG,
	NK_N1_VENC_CODEC_H264,
	NK_N1_VENC_CODEC_HEVC,
	NK_N1_VENC_CODEC_H264_PLUS,
	NK_N1_VENC_CODEC_HEVC_PLUS,
} NK_N1VideoEncCodec;

/**
 * 获取 Nk_N1VideoEncCodec 枚举值对应的文本信息。
 */
extern DECLARE_NK_ENUM_MAP(N1VideoEncCodec);

/**
 * 获取 Nk_N1VideoEncCodec 文本信息对应的枚举值。
 */
extern DECLARE_NK_ENUM_UNMAP(N1VideoEncCodec);

typedef enum Nk_N1AudioEncCodec
{
	NK_N1_AUDIO_CODEC_UNDEF = (-1),
	NK_N1_AUDIO_CODEC_G711A,
	NK_N1_AUDIO_CODEC_G711U,
	NK_N1_AUDIO_CODEC_AAC,

} NK_N1AudioEncCodec;

/**
 * 获取 Nk_N1AudioEncCodec 枚举值对应的文本信息。
 */
extern DECLARE_NK_ENUM_MAP(N1AudioEncCodec);

/**
 * 获取 Nk_N1AudioEncCodec 文本信息对应的枚举值。
 */
extern DECLARE_NK_ENUM_UNMAP(N1AudioEncCodec);

typedef enum Nk_N1PTZCommand {

	NK_N1_PTZ_CMD_UNDEF			= (-1),

	NK_N1_PTZ_CMD_STOP			= (0),
	NK_N1_PTZ_CMD_CAMERA_PWRON  =(1),
	NK_N1_PTZ_CMD_LIGHT_PWRON       =(2),
	NK_N1_PTZ_CMD_WIPER_PWRON     = (3),
	NK_N1_PTZ_CMD_FAN_PWRON	= (4),
	NK_N1_PTZ_CMD_HEATER_PWRON	= (5),
	NK_N1_PTZ_CMD_AUX_PWRON1	= (6),
	NK_N1_PTZ_CMD_AUX_PWRON2	= (7),

	NK_N1_PTZ_CMD_TILT_UP			= (100),
	NK_N1_PTZ_CMD_TILT_DOWN,

	NK_N1_PTZ_CMD_PAN_LEFT		= (200),
	NK_N1_PTZ_CMD_PAN_RIGHT,
	NK_N1_PTZ_CMD_PAN_AUTO,
	NK_N1_PTZ_CMD_PAN_STOP_ALL,

	NK_N1_PTZ_CMD_ZOOM_IN			= (300),
	NK_N1_PTZ_CMD_ZOOM_OUT,

	NK_N1_PTZ_CMD_FOCUS_IN		= (400),
	NK_N1_PTZ_CMD_FOCUS_OUT,

	NK_N1_PTZ_CMD_IRIS_OPEN		= (500),
	NK_N1_PTZ_CMD_IRIS_CLOSE,
	NK_N1_PTZ_CMD_IRIS_ENLARGE,
	NK_N1_PTZ_CMD_IRIS_SHRINK,

	NK_N1_PTZ_CMD_SET_PRESET		= (1000),
	NK_N1_PTZ_CMD_GOTO_PRESET,
	NK_N1_PTZ_CMD_CLEAR_PRESET,
	NK_N1_PTZ_CMD_FILL_PRE_SEQ,		//将预置点加入巡航序列
	NK_N1_PTZ_CMD_SET_SEQ_DWELL,		//设置巡航点停顿时间
	NK_N1_PTZ_CMD_RUN_SEQ,			//开始巡航
	NK_N1_PTZ_CMD_STOP_SEQ,			//停止巡航
	NK_N1_PTZ_CMD_CLE_PRE_SEQ,		//将预置点从巡航速度中删除
} NK_N1PTZCommand;

/**
 * 获取 NK_N1PTZCommand 枚举值对应的文本信息。
 */
extern DECLARE_NK_ENUM_MAP(N1PTZCommand);

/**
 * 获取 NK_N1PTZCommand 文本信息对应的枚举值。
 */
extern DECLARE_NK_ENUM_UNMAP(N1PTZCommand);


/**
 * IRCut 滤光片工作类型。
 */

//typedef NK_UInt32 NK_N1IRCutFilterMode;
//#define NK_N1_IRCUT_MODE_AUTO      NK_N1_UINT32_SYM('I', 'R', 'A', 0)
//#define NK_N1_IRCUT_MODE_DAYLIGHT  NK_N1_UINT32_SYM('I', 'R', 'D', 0)
//#define NK_N1_IRCUT_MODE_NIGHT     NK_N1_UINT32_SYM('I', 'R', 'N', 0)

typedef NK_N1PropEnum NK_N1IRCutFilterMode;

typedef enum Nk_N1IRCutMode
{
	 NK_N1_IRCUT_MODE_UNDEFINED   =   -1,
	 NK_N1_IRCUT_MODE_AUTO   =   NK_N1_UINT32_SYM('I', 'R', 'A', 0),
	 NK_N1_IRCUT_MODE_DAYLIGHT =  NK_N1_UINT32_SYM('I', 'R', 'D', 0),
	 NK_N1_IRCUT_MODE_NIGHT  =   NK_N1_UINT32_SYM('I', 'R', 'N', 0),
	 NK_N1_IRCUT_MODE_LIGHT  =   NK_N1_UINT32_SYM('I', 'R', 'L', 0),
	 NK_N1_IRCUT_MODE_SMART  =   NK_N1_UINT32_SYM('I', 'R', 'S', 0)
} NK_N1IRCutMode;
/**
 * 获取 NK_N1IRCutMode 枚举值对应的文本信息。
 */
extern DECLARE_NK_ENUM_MAP(N1IRCutMode);

/**
 * 获取 NK_N1IRCutMode 文本信息对应的枚举值。
 */
extern DECLARE_NK_ENUM_UNMAP(N1IRCutMode);


typedef enum Nk_N1Result
{
	/**
	 * 未定义的错误。
	 */
	NK_N1_ERR_UNDEF					= (-1),

	/**
	 * 没有错误，操作成功。
	 */
	NK_N1_OK						= (0),
	NK_N1_ERR_NONE					= (NK_N1_OK),

	/**
	 * 参数错误相关。
	 */
	NK_N1_ERR_INVALID_PARAM			= (101),	///< 参数错误。
	NK_N1_ERR_INVALID_CHANNEL_STREAM_ID,		///< 通道/码流号 ID 错误。

	/**
	 * 设备内部错误相关。
	 */
	NK_N1_ERR_DEVICE_BUSY			= (201),	///<设备忙碌，有可能该功能正在被占用。
	NK_N1_ERR_DEVICE_NOT_SUPPORT,				///< 设备不支持。
	NK_N1_ERR_DEVICE_OUT_OF_MEMORY,
	NK_N1_ERR_DEVICE_OUT_OF_USER,

	/**
	 * 操作设备错误相关。
	 */
	NK_N1_ERR_INVALID_OPERATE		= (301),	///< 无效的设备操作。
	NK_N1_ERR_OPERATE_TIMEOUT,					///< 设备操作超时。

	/**
	 * 传输数据包错误。
	 */
	NK_N1_ERR_INVALID_DATAGRAM		= (401),	///< 无效的传输报文。


	/**
	 * 设备升级。
	 */
	NK_N1_ERR_UPGRADE_FAILED		= (600),
	NK_N1_ERR_UPGRADE_NOT_SUPPORT,
	NK_N1_ERR_FIRMWARE_FILE_ERROR,
	NK_N1_ERR_UPGRADE_INTERRUPT,
	NK_N1_ERR_FIRMWARE_STORE_ERROR,
	NK_N1_ERR_FIRMWARE_VER_TOO_OLD,

	/**
	 * 用户校验失败。
	 */
	NK_N1_ERR_NOT_AUTHORIZATED		= (1001),


	/**
	 * 双向语音。
	 */
	NK_N1_ERR_2WAYTALK              = (1200),
	NK_N1_ERR_2WAYTALK_INVALID_ID,
	NK_N1_ERR_2WAYTALK_SEND_FAILED,
	NK_N1_ERR_2WAYTALK_BUSY,
	NK_N1_ERR_2WAYTALK_INVALID_PACK,


} NK_N1Error, NK_N1Result;

/**
 * 兼容版本 < 1.4.0
 */
#define NK_N1Ret NK_N1Error


/**
 * Wi-Fi 工作模式。
 */
typedef enum Nk_N1EthWiFiMode {

	/**
	 * 不支持 Wi-Fi。
	 */
	NK_N1_ETH_WIFI_MODE_NA = (-1),

	/**
	 * 站点模式。
	 */
	NK_N1_ETH_WIFI_MODE_STA = (0),

	/**
	 * 热点模式。
	 */
	NK_N1_ETH_WIFI_MODE_AP,

	/**
	 * 中继模式。
	 */
	NK_N1_ETH_WIFI_MODE_REP,

} NK_N1EthWiFiMode;

#if 0
/**
 * Nk_N1EthWiFiMode 枚举对应字段。
 */
static NK_EnumStrMap
NK_ENUM_MAPPER(NK_N1EthWiFiMode)[] = {

		{	NK_N1_ETH_WIFI_MODE_NA,		"None",	},
		{	NK_N1_ETH_WIFI_MODE_STA,	"Station",	},
		{	NK_N1_ETH_WIFI_MODE_AP,		"Access Point",	},
		{	NK_N1_ETH_WIFI_MODE_REP,	"Repeater",	},

};

/**
 * Wi-Fi 连通信道。
 */
typedef enum Nk_N1EthWiFiAccessChannel {

	NK_N1_ETH_WIFI_ACCESS_CH_AUTO = (0),
	NK_N1_ETH_WIFI_ACCESS_CH_1,
	NK_N1_ETH_WIFI_ACCESS_CH_2,
	NK_N1_ETH_WIFI_ACCESS_CH_3,
	NK_N1_ETH_WIFI_ACCESS_CH_4,
	NK_N1_ETH_WIFI_ACCESS_CH_5,
	NK_N1_ETH_WIFI_ACCESS_CH_6,
	NK_N1_ETH_WIFI_ACCESS_CH_7,
	NK_N1_ETH_WIFI_ACCESS_CH_8,
	NK_N1_ETH_WIFI_ACCESS_CH_9,
	NK_N1_ETH_WIFI_ACCESS_CH_10,
	NK_N1_ETH_WIFI_ACCESS_CH_11,
	NK_N1_ETH_WIFI_ACCESS_CH_12,
	NK_N1_ETH_WIFI_ACCESS_CH_13,
	NK_N1_ETH_WIFI_ACCESS_CH_14,

} NK_N1EthWiFiAccessChannel;

/**
 * NK_N1EthWiFiAccessChannel 枚举对应字段。
 */
static NK_EnumStrMap
NK_ENUM_MAPPER(NK_N1EthWiFiAccessChannel)[] = {

		{	NK_N1_ETH_WIFI_ACCESS_CH_AUTO,	"Auto",	},
		{	NK_N1_ETH_WIFI_ACCESS_CH_1,		"1",	},
		{	NK_N1_ETH_WIFI_ACCESS_CH_2,		"2",	},
		{	NK_N1_ETH_WIFI_ACCESS_CH_3,		"3",	},
		{	NK_N1_ETH_WIFI_ACCESS_CH_4,		"4",	},
		{	NK_N1_ETH_WIFI_ACCESS_CH_5,		"5",	},
		{	NK_N1_ETH_WIFI_ACCESS_CH_6,		"6",	},
		{	NK_N1_ETH_WIFI_ACCESS_CH_7,		"7",	},
		{	NK_N1_ETH_WIFI_ACCESS_CH_8,		"8",	},
		{	NK_N1_ETH_WIFI_ACCESS_CH_9,		"9",	},
		{	NK_N1_ETH_WIFI_ACCESS_CH_10,	"10",	},
		{	NK_N1_ETH_WIFI_ACCESS_CH_11,	"11",	},
		{	NK_N1_ETH_WIFI_ACCESS_CH_12,	"12",	},
		{	NK_N1_ETH_WIFI_ACCESS_CH_13,	"13",	},
		{	NK_N1_ETH_WIFI_ACCESS_CH_14,	"14",	},

};
#endif
typedef enum Nk_N1ImageSceneMode {
	NK_N1_IMG_SCENE_MODE_AUTO = (0),
	NK_N1_IMG_SCENE_MODE_INDOOR,
	NK_N1_IMG_SCENE_MODE_OUTDOOR,
} NK_N1ImageSceneMode;

typedef enum Nk_N1ImageExposureMode {
	/// 自动曝光模式。
	NK_N1_IMG_EXPO_MODE_AUTO = (0),
	/// 强光曝光模式。
	NK_N1_IMG_EXPO_MODE_BRIGHT,
	/// 低照曝光模式。
	NK_N1_IMG_EXPO_MODE_DARK,
} NK_N1ImageExposureMode;

typedef enum Nk_N1ImageAutoWBMode {
	NK_N1_IMG_AWD_MODE_AUTO = (0),
	NK_N1_IMG_AWD_MODE_INDOOR,
	NK_N1_IMG_AWD_MODE_OUTDOOR,
} NK_N1ImageAutoWBMode;

typedef enum Nk_N1ImageBacklightCompensation {
	NK_N1_IMG_BL_COMP_AUTO = (0),
	NK_N1_IMG_BL_COMP_ALWAYS_ON,
	NK_N1_IMG_BL_COMP_ALWAYS_OFF,
} NK_N1ImageBacklightCompensation;

typedef enum Nk_N1LowlightMode{
	NK_N1_IMG_LL_MODE_OFF = (0),
	NK_N1_IMG_LL_MODE_ONLY_NIGHT,
	NK_N1_IMG_LL_MODE_DAY_AND_NIGHT,
	NK_N1_IMG_LL_MODE_AUTO,
} NK_N1LowlightMode;


typedef NK_UInt32 NK_N1MediaPackageType;

#define NK_N1_MEDIA_PT_UNDEF    (NK_N1_UINT32_SYM(0, 0, 0, 0))
#define NK_N1_MEDIA_PT_G711A    (NK_N1_UINT32_SYM('7', '1', '1', 'A'))
#define NK_N1_MEDIA_PT_G711U    (NK_N1_UINT32_SYM('7', '1', '1', 'U'))
#define NK_N1_MEDIA_PT_SPEEX    (NK_N1_UINT32_SYM('S', 'P', 'X', 0))
#define NK_N1_MEDIA_PT_AAC      (NK_N1_UINT32_SYM('A', 'A', 'C', 0))
#define NK_N1_MEDIA_PT_JPEG     (NK_N1_UINT32_SYM('J', 'P', 'E', 'G'))
#define NK_N1_MEDIA_PT_H264     (NK_N1_UINT32_SYM('H', '2', '6', '4'))
#define NK_N1_MEDIA_PT_H265     (NK_N1_UINT32_SYM('H', '2', '6', '5'))
#define NK_N1_MEDIA_PT_HEVC     (NK_N1_MEDIA_PT_H265)

/**
 * 媒体包数据定义。
 */
typedef struct Nk_N1MediaPackage {

	/// 媒体包类型。
	NK_N1MediaPackageType type;

	/// 报文的时间戳，单位 us。
	NK_Size64 timestamp;

	/// 类型属性。
	union {

		struct {

			/// 视频大小。
			NK_Size width, height;

			/// 帧率
			NK_Size framePS;

		} Video;

		struct {

			NK_Size sampleRate; ///< 8000, 11025, 16000, 22050, 24000...
			NK_Size sampleBitWidth; ///< 采样位宽，取值 8/16/24/32。
			NK_Size samplePerPackage;
			NK_Size track;

		} Audio;
	};
} NK_N1MediaPackage;


/**
 * N1 直播数据载体的类型。
 */
typedef enum Nk_N1DataPayload
{
	NK_N1_DATA_PT_UNDEF				= (-1),

	/**
	 * G.711 A-Law 音频静荷数据。
	 */
	NK_N1_DATA_PT_G711A				= (8),

	/**
	 * fisheye校准。
	 */
	NK_N1_DATA_PT_FIXPARAM				= (103),
	/**
	 * AAC 音频静荷数据。
	 */
	NK_N1_DATA_PT_AAC				= (104),
	/**
	 * JPEG Image.
	 */
	NK_N1_DATA_PT_JPEG			 	= (26),

	/**
	 * H.264 Nal-Unit 视频静荷数据。
	 */
	NK_N1_DATA_PT_H264_NALUS		= (96),

	/**
	 * HEVC Nal-Unit 视频静荷数据。
	 */
	NK_N1_DATA_PT_HEVC_NALUS		= (97),

	NK_N1_DATA_PT_CUSTOM			= (100),

	NK_N1_DATA_PT_CUSTOM2			= (102),

} NK_N1DataPayload;

/**
 * N1 直播视频数据帧的类型。
 */
typedef enum Nk_N1VideoFrameType
{
	NK_N1_DATA_FRAME_UNUSE = 0,
    NK_N1_DATA_FRAME_BASE_IDRSLICE = 1,                              //the Idr frame at Base layer
    NK_N1_DATA_FRAME_BASE_PSLICE_REFTOIDR,                           //the P frame at Base layer,the P frame at Base layer, referenced by other frames at Base layer and reference to Idr frame
    NK_N1_DATA_FRAME_BASE_PSLICE_REFBYBASE,                          //the P frame at Base layer, referenced by other frames at Base layer
    NK_N1_DATA_FRAME_BASE_PSLICE_REFBYENHANCE,                       //the P frame at Base layer, referenced by other frames at Enhance layer
    NK_N1_DATA_FRAME_ENHANCE_PSLICE_REFBYENHANCE,                    //the P frame at Enhance layer, referenced by other frames at Enhance layer
    NK_N1_DATA_FRAME_ENHANCE_PSLICE_NOTFORREF,                       //the P frame at Enhance layer ,not referenced
    NK_N1_DATA_FRAME_ENHANCE_PSLICE_BUTT
} NK_N1VideoFrameType;

/**
 * 直播会话上下文数据结构，\n
 * 在调用 @ref NkN1Utils_InitLiveSession 方法时会对此数据结构进行初始化，\n
 * 在初始化会话的时候会对此上下文进行初始化。\n
 *
 */
typedef struct Nk_N1LiveSession
{
	/**
	 * 实时媒体通道 ID，从 0 开始。
	 */
	NK_UInt32 channel_id;

	/**
	 * 实时媒体通道 ID 下码流 ID，从 0 开始。
	 */
	NK_UInt32 stream_id;

	/**
	 * 会话唯一 ID，由模块内部产生。
	 */
	NK_UInt32 session_id;

	/**
	 * 数据帧序列计数器。
	 * 会根据接口调用不断递增。
	 */
	NK_UInt32 sequence;

	struct
	{
		NK_N1DataPayload payload_type; ///< 媒体类型，见 @ref NK_N1_LIVE_SESS_PT_*。
		NK_Size width, heigth; ///< 视频宽高。

	} Video;

	struct
	{
		NK_N1DataPayload payload_type; ///< 媒体类型，见 @ref NK_N1_LIVE_SESS_PT_*。
		NK_UInt32 sample_rate; ///< 采样率，每秒音频采样的脉冲数量。
		NK_UInt32 sample_bitwidth; ///< 采样位宽，值可以是8、16、24、32。
		NK_Boolean stereo; ///< 单声道/立体生标识，为 False 代表单声道，反之为立体声。

	} Audio;

	/**
	 * 用户会话句柄。\n
	 * 调用者可以通过此句柄保留当前会话的上下文信息。\n
	 *
	 */
	NK_PVoid user_session;

	/**
	 * 模块内保留数据区。
	 */
	NK_Byte reserved[1024 * 2];

	/// @brief 当前会话的带宽，只读。
	NK_Size bandwidth;

	/// @brief 流量统计，只读。
	NK_Size bytesum;

	/// @brief 流量统计时间，只读。
	NK_Size timesum;

} NK_N1LiveSession;


/**
 * @brief
 *  N1 设备能力集。
 *
 * @details
 *
 */
typedef struct Nk_N1DeviceCapabilities {


	/// 设备硬件码，用于在固件升级时作为设备的唯一性，\n
	/// 缺省为 0。
	NK_UInt32 hwCode;

	/// 设备版本号，建议使用三段式格式 x.x.x，\n
	/// 缺省为 SDK 版本号。
	NK_Char swVersion[32];

	/// 设备名称，发现设备时作为名称返回，\n
	/// 缺省为 HD IPCAM。
	NK_Char name[64];

	/// 设备支持有线网络 RJ45 接口，\n
	/// 缺省为 True。
	NK_Boolean supportRJ45;

	/// 设备支持无线站点模式，对于支持 WiFi 站点模式的无线设备，置位此标识，\n
	/// 缺省为 False。
	NK_Boolean supportWiFiStation;

	/// 设备支持无线热点模式，对于支持 WiFi 热点模式的无线设备，置位此标识。\n
	/// 缺省为 False。
	NK_Boolean supportWiFiAP;

	/// 设备支持无线站点与热点模式同时工作，置位此标识。\n
	/// 当此标致置位后，自动忽略 @ref supportWiFiStation 和 @ref supportWiFiAP，\n
	/// 缺省为 False。
	NK_Boolean supportWiFiRepeater;

	/// 最大通道号，最大 128 路，\n
	/// 缺省为 1。
	NK_Size maxMediaChannel;

	/// 通道能力描述。
	struct {
		/// 每个通道最大码流，\n
		/// 缺省为 2。
		NK_Size maxStream;

		struct {

			/// 每个码流的最大访问数，\n
			/// 缺省为 4。
			NK_Size maxOnCommand;

		} Stream[NK_N1_DEV_MAX_MEDIA_CH_STREAM];

	} MediaChannel[NK_N1_DEV_MAX_MEDIA_CH];

	/// 最大 TF 卡支持，\n
	/// 缺省为 1。
	NK_Size maxTFCard;

	/// 最大硬盘支持，\n
	/// 缺省为 0。
	NK_Size maxHardDiskDriver;

} NK_N1DeviceCapabilities;



/**
 * N1 数据帧数据结构结构。\n
 * 考虑到在缓存帧数据的时候。
 *
 *
 */
typedef struct Nk_N1DataFrame
{
	NK_Size n_vectors; ///< 有效 Vectors 的个数。

	struct
	{
		NK_PVoid raw; ///<
		NK_Size len;

	} Vectors[1024];

} NK_N1DataFrame;


///
/// @brief
///  N1 视频编码器数据结构定义。
///
typedef struct Nk_N1VideoEncoder {

	/// 编码器名称，根据此值区分具体数据结构。
	NK_N1PropEnum Codec;

	union {
		struct {

			/// 编码器配置
			NK_N1PropEnum H264Profile;

			/// 编码分辨率。
			NK_N1PropEnum Resolution;

			/// 码率控制模式。
			NK_N1PropEnum BitRateCtrlMode;

			/// 编码码率（单位：kbps，千位每秒）。
			NK_N1PropInteger BitRate;

			/// 编码帧率（单位：fps，帧每秒）。
			NK_N1PropInteger FrameRate;

			/// 关键帧间隔（单位：frames，帧）。
			NK_N1PropInteger KeyFrameInterval;

		} H264, H265, HEVC;
	};

} NK_N1VideoEncoder;

/**
 * @brief
 *  网卡配置信息。
 */
typedef struct Nk_N1EthConfig {

	/**
	 *
	 * +-------------+------------+------------+------------+------------+------------+
	 * |             | ESSID      | PSK        | EnableDHCP | HwAddr     | { IPv4 }   |
	 * +-------------+------------+------------+------------+------------+------------+
	 * | NetWired    | n          | n          | y          | y          | y          |
	 * +-------------+------------+------------+------------+------------+------------+
	 * | NetWiFi     | y          | y          | y          | y          | y          |
	 * +-------------+------------+------------+------------+------------+------------+
	 *
	 */

	/**
	 * 连接无线热点 / NVR 所对应的 ESSID。
	 */
	NK_N1PropString ESSID;

	/**
	 * 当 classify 为 NK_N1_LAN_SETUP_WIFIAP 时，\n
	 * 表示热点的接入密码；\n
	 * 当 classify 为 NK_N1_LAN_SETUP_WIFISTA 或者 NK_N1_LAN_SETUP_WIFINVR 时，\n
	 * 表示连接无线热点 / NVR 所对应的 ESSID 的接入密码。
	 */
	NK_N1PropString PSK;

	/**
	 * 当 classify 为 NK_N1_LAN_SETUP_WIFIAP 时，\n
	 * 表示本地是否开启 DHCP 服务；\n
	 * 当 classify 为 NK_N1_LAN_SETUP_WIFISTA 时，\n
	 * 表示是否使用无线热点的 DHCP 服务获取地址；\n
	 * 当 classify 为 NK_N1_LAN_SETUP_WIFINVR 时，\n
	 * 此值一直为 False。
	 */
	NK_N1PropBoolean EnableDHCP;

	/**
	 * 物理网卡地址。
	 */
	NK_N1PropHwAddr HwAddr;

	/**
	 * IP 地址配置。
	 */
	NK_N1PropIPv4 IPAddress, Netmask, Gateway, DomainNameServer;

	/**
	 * Wi-Fi 工作模式，对应 @ref Nk_N1EthWiFiMode 类型。
	 */
	NK_N1PropEnum WiFiMode;

	/**
	 * Wi-Fi 连接相关参数。\n
	 * 每个对象包含一个数据（如 @ref _Station）和数据指针（如 @ref Station），当数据指针指向数据时表示有效，\n
	 * 数据结构中分表描述 Wi-Fi 连接所需要的 essid 和密码，分辨对应 @ref essid 和 @ref passphrase 参数，\n
	 * 在中继使用时，客户端会设置两个站点配置，分别为 @ref Station 和 @ref StationAlternative，具体使用哪个连接似乎设备自身策略，\n
	 * 对于站点模式来说，@ref Station::essid 和 @ref Station::passphrase 分别代表连接到的热点名称和校验密码，\n
	 * 对于热点模式，@ref AccessPoint::essid 和 @ref AccessPoint::passphrase 分别代表热点本身被广播名称和校验密码。\n
	 */
	struct {

		/**
		 * 连接网络热点的名称。
		 */
		NK_Char essid[32];

		/**
		 * 链接网络 @ref essid 对应密码。
		 */
		NK_Char passphrase[32];

		/**
		 * 隐藏 SSID 标志，仅在热点模式和中继模式有效。
		 */
		NK_Boolean hidden;

		/**
		 * 热点开放信道，对应 @ref NK_N1EthWiFiAccessChannel，\n
		 * 仅在热点模式和中继模式有效。
		 */
		NK_N1PropEnum AccessChannel;

	} _Station, _StationAlternative, _AccessPoint, *Station, *StationAlternative, *AccessPoint;

	/**
	 * 只读，仅在中继模式有效。
	 */
	NK_Int connection;
    /** 
     * 站点或中继模式下的速率
     */
	NK_Int stationsignal;
	/**
	 * 连接信息。
	 */
	struct {

		/**
		 * 连接设备的物理地址。
		 */
		NK_N1PropHwAddr BssID;

		/**
		 * 连接速率。
		 */
		NK_N1PropInteger Rate;

	} Connection[8];
 
	/// 临时修改标识，保留。
	NK_Boolean temporary;

} NK_N1EthConfig;


typedef enum Nk_N1LanSetupClassify
{

	NK_N1_LAN_SETUP_UNDEF = (-1),

	/**
	 * 设备信息。
	 */
	NK_N1_LAN_SETUP_INFO,

	/**
	 * 局域网时间配置，对应 NK_N1Lansetup::Time。
	 */
	NK_N1_LAN_SETUP_TIME,

	/**
	 * 局域网红外滤光片配置，对应 NK_N1Lansetup::IRCutFilter。
	 */
	NK_N1_LAN_SETUP_IRCUT,

	/**
	 * 局域网视频图像配置，对应 NK_N1Lansetup::VideoImage。
	 */
	NK_N1_LAN_SETUP_VIMG,

	/**
	 * 局域网视频编码配置，对应 NK_N1Lansetup::VideoEncoder。
	 */
	NK_N1_LAN_SETUP_VENC,

	/**
	 * 局域网云台控制配置，对应 NK_N1Lansetup::PanTiltZoom。
	 */
	NK_N1_LAN_SETUP_PTZ,

	NK_N1_LAN_SETUP_NET_WIRED,
	NK_N1_LAN_SETUP_NET_WIFI,

	/**
	 * DNS 配置。
	 */
	NK_N1_LAN_SETUP_DNS,

	/**
	 * P2P 配置。
	 */
	NK_N1_LAN_SETUP_P2P,

	/**
	 * 出厂复位配置。
	 */
	NK_N1_LAN_SETUP_FACTROY_RESET,

	/**
	 * RTSP 配置。
	 */
	NK_N1_LAN_SETUP_RTSP,

	/**
	 * 视频 OSD 配置。
	 */
	NK_N1_LAN_SETUP_VOSD,

	/**
	 * 视频隐私遮盖配置。
	 */
	NK_N1_LAN_SETUP_VPMSK,
	NK_N1_LAN_SETUP_HICONN,

	/**
	 * Onvif 协议配置。
	 */
	NK_N1_LAN_SETUP_ONVIF,

	/// 时间服务器同步设置。
	NK_N1_LAN_SETUP_NTP = 0x1021,
	
	/// 日历使用设置。
	NK_N1_LAN_SETUP_CALENDAR = 0x1060,
	
	/// 高级视频图像设置，对应 NK_N1LanSetup::VideoImageAdvanced。
	NK_N1_LAN_SETUP_VIM_ADV = 0x4010,

	/// 音频输入输出配置。
	NK_N1_LAN_SETUP_AIO = 0x6010,

	/// 音频编码配置，对应 NK_N1LanSetup::AudioEncoder。
	NK_N1_LAN_SETUP_AENC = 0x6040,


} NK_N1LanSetupClassify;


/**
 * N1 协议局域网配置相关数据结构。
 */
typedef struct Nk_N1LanSetup
{
	/**
	 * 配置分类。
	 */
	NK_N1LanSetupClassify classify;

	/**
	 * 通道号，在涉及到多通道配置时涉及到。
	 */
	NK_Int channel_id;

	/**
	 * 码流号，在涉及到某个通道下多码流配置时涉及到。
	 */
	NK_Int stream_id;

	/**
	 * 参考图像宽高（单位：像素）。
	 *
	 */
	NK_Size ref_width, ref_height;

	union {

		/**
		 * 设备信息。
		 * 当 classify 等于 NK_N1_LAN_SETUP_INFO 时有效。
		 */
		struct {

			/**
			 * 设备的云 ID 号。
			 */
			NK_Char cloud_id[32];

			/**
			 * 设备型号定义。
			 */
			NK_Char model[32];

			/**
			 * 设备版本号。
			 */
			NK_Char version[32];

			/**
			 * 设备直播通道数定义，最小为 1，最大为 256。
			 */
			NK_Size live_channels;

			/**
			 * 每个直播通道的属性，有效数量与 @ref live_channels 相对应。
			 */
			struct {
				/**
				 * 每个直播通道的码流数，最小为 1，最大为 8。
				 */
				NK_Size stream_channels;

			} LiveChannels[128];

			/**
			 * 设备硬件码，与升级策略相关。
			 */
			NK_UInt32 hardware_code;

			/**
			 * 生产厂家信息。
			 */
			NK_Char manufacturer[32];

		} Info;

		/**
		 * 设备时间配置相关数据结构。
		 * 当 classify 等于 NK_N1_LAN_SETUP_TIME 时有效。
		 */
		struct 	{

			/**
			 * UTC 时间（格林尼治相对公元1970年1月1日00时00分00秒时间戳）。
			 */
			NK_UTC1970 utc;

			/**
			 * 时间所在时区。
			 */
			NK_TimeZone gmt;

			/**
			 * 夏令时使用标识。
			 */
			NK_Boolean dst;

		} Time;

		struct {
			NK_Boolean jalaali;
		} Calendar;

		struct {
			NK_Boolean enabled;
			NK_Char domain[64];
		} NTP;

		/**
		 * 红外滤光片配置相关。
		 */
		struct {

			/**
			 * 详见 @ref NK_N1IRCutFilterMode。
			 */
			NK_N1PropEnum Mode;

			/**
			 * 白天模式到黑夜模式互相转换的持续判断时间（单位：秒）。
			 */
			NK_N1PropInteger DayToNightFilterTime, NightToDayFilterTime;

		} IRCutFilter;

		/**
		 * 视频图像配置相关数据结构。
		 * 当 classify 等于 NK_N1_LAN_SETUP_VIMG 时有效。
		 */
		struct 	NK_VideoImage{

			/**
			 * 视频图像输入频率。
			 */
			NK_N1PropInteger PowerLineFrequenceMode;

			/**
			 * 视频图像输入分辨率。
			 */
			NK_N1PropEnum CaptureResolution;

			/**
			 * 视频图像输入帧率。
			 */
			NK_N1PropInteger CaptureFrameRate;

			/**
			 * 输入图像色彩调节。
			 * 与设备传感器或者 ISP 处理器相关。
			 */
			NK_N1PropInteger BrightnessLevel, ContrastLevel, SharpnessLevel, SaturationLevel, HueLevel;

			/**
			 * 水平、垂直反转设置标识。
			 */
			NK_N1PropBoolean Flip, Mirror;

			/**
			 * 视频图像标题。
			 */
			struct {

				/**
				 * 标题显示标识。
				 */
				NK_N1PropBoolean Show;

				/**
				 * 标题文本编码。
				 */
				NK_N1PropEnum TextEncoding;

				/**
				 * 标题文本。
				 */
				NK_N1PropString Text;

			} Title;

			/**
			 * 视频图像运动侦测。
			 */
			struct NK_MotionDetection{
				/**
				 * 启动运动侦测标识。
				 */
				NK_N1PropBoolean Enabled;

				/**
				 * 检测灵敏度。
				 */
				NK_N1PropInteger SensitivityLevel;

				/**
				 * 检测区域掩码。
				 * 配置视频运动侦测除了配置灵敏度以外还须要配置检测掩码，
				 * 当掩码活动区域全部为 True 的时候表示整个视频运动检测，
				 * 否则则为掩码区域活动的区域检测，检测掩码最大颗粒为 32x24，如果设备运动这测区域实现的颗粒大于此值则须要进行适配。
				 *
				 */
				struct {
					/**
					 * 小于等于 32x24。
					 */
					NK_Size width, height;
					NK_Byte matrix[24][32];
				} Mask;

			} MotionDetection;

			/**
			 * 图像颜色风格，用户可以预定义多种风格，供客户选择。
			 */
			NK_N1PropInteger ColorStyle;

		} VideoImage;

		///
		/// 高级图像设置接口，对应 NK_N1_LAN_SETUP_VIM_ADV 配置选项。
		///
		struct {

			NK_N1PropEnum SceneMode;
			NK_N1PropInteger Denoise3D;
			NK_N1PropInteger DigitalWDR;
			/// 曝光模式。
			NK_N1PropEnum ExposureMode;
			/// 自动白平衡模式。
			NK_N1PropEnum AutoWBMode;
			/// 手动锐度。
			NK_N1PropInteger ManualSharpen;
			//手动锐度开关
			NK_N1PropBoolean ManualSharpenSwitch;
			/// 背光补偿。
			NK_N1PropEnum BacklightCompensation;
			///Lowlight
			NK_N1PropEnum LowlightMode;
		} VideoImageAdvanced;

		struct {

			NK_N1PropInteger SampleRate;
			NK_N1PropInteger SampleBitWidth;
			NK_N1PropInteger InputVolume;
			NK_N1PropInteger OutputVolume;
			NK_N1PropEnum InputMode;

		} AudioIO;

		struct {
			NK_Boolean enabled;
			NK_N1PropEnum Encodec;
		} AudioEncoder;

		/**
		 * 视频编码配置相关数据结构。
		 */
		NK_N1VideoEncoder VideoEncoder;

		/**
		 * 视频隐私区域配置相关数据结构。
		 */
		struct {

            NK_N1PropBoolean Enabled;

			/**
			 * 遮挡区域，与 @ref_width 和 @ref_height 参数相关。
			 */
			struct {

				/**
                 * 开启标识。
                 */
				NK_N1PropBoolean Enabled;

                NK_Int Color;

				NK_DFloat x,y,width,height;

			} Mask[4];

		} VideoPrivacyMask;

		/**
		 * 视频编辑层配置。
		 */
		struct {

			/**
			 * 显示模式。
			 */
			NK_N1PropEnum Method;

			struct {

				/**
				 * 显示标识。
				 */
				NK_N1PropBoolean Enabled;

				/**
				 * 显示位置。
				 */
				NK_Size x, y;

				/**
				 * 文本编码标识，
				 * 为 True 时则 @ref Text 编码为 UTF-8，否则为 GB2312。
				 */
				NK_Boolean textUTF8;

				/**
				 * 传入文本。
				 */
				NK_N1PropString Text;

			} Title;

			struct {

				/**
				 * 显示标识。
				 */
				NK_N1PropBoolean Enabled;

				/**
				 * 显示位置。
				 */
				NK_Size x, y;

				/**
				 * 显示星期标识。
				 */
				NK_N1PropBoolean DisplayWeekday;

				/**
				 * 显示时间 12 小时制标识。
				 */
				NK_N1PropBoolean TimeFormt12HRs;

				/**
				 * 日期显示格式。
				 */
				NK_N1PropEnum DateFormat;

			} Time;

		} VideoOnScreenDisplay;

		/**
		 * 云台控制配置相关数据结构。
		 * 当 classify 等于 NK_N1_LAN_SETUP_PTZ 时有效。
		 */
		struct {

			NK_N1PTZCommand command;

			/**
			 * 单步执行标识，对于 NK_N1_LAN_SETUP_PTZ_CMD_TILT_* 或 NK_N1_LAN_SETUP_PTZ_CMD_PAN_* 命令，\n
			 * 单步执行后，设备执行单步执行后自动停止，\n
			 * 非单步执行时，须要客户端再次发命令 NK_N1_LAN_SETUP_PTZ_CMD_STOP 才能停止。
			 *
			 */
			NK_Boolean step;

			/**
			 * 预置位号，当 command 为 NK_N1_LAN_SETUP_PTZ_CMD_*_PRESET 时指示操作的对应预置位。
			 */
			NK_Integer preset_position;

			/**
			 * 云台运动速度。
			 */
			NK_N1PropInteger Speed;

		} PanTiltZoom;


		/**
		 * 连接无线 NVR 配置。\n
		 * 根据无线 NVR 发起请求配置设备无线连接对应的无线网卡。
		 */
		NK_N1EthConfig NetWired, NetWiFi;

		/**
		 * DNS 地址配置。
		 */
		struct {

			/**
			 * 首选的 DNS 地址。
			 */
			NK_N1PropIPv4 Preferred;

			/**
			 * 备用的 DNS 地址。
			 */
			NK_N1PropIPv4 Alternative;

		} DNS;


		/**
		 * P2P 配置。
		 */
		struct {

			NK_N1PropBoolean Enabled;

		} PtoP;

		/**
		 * Onvif 配置。
		 */
		struct {

			/**
			 * IP 自适应使能标识。
			 * 记录到配置文件。
			 */
			NK_N1PropBoolean IPAutoAdaption;

			/**
			 * 临时配置变量，不用保存到配置，
			 * 此标识和 @ref IPAutoAdaption 没有绝对关系，表示当前状态模块内期望临时开启、关闭自适应功能。
			 * 在上层实现中，仅当 @ref IPAutoAdaption 和 @ref ipAutoAdaptionActived 同时使能时 IP 自适应个功能有效。
			 *
			 */
			NK_Boolean ipAutoAdaptionActived;


		} Onvif;
	};

} NK_N1LanSetup;


/**
 * 終端打印 NK_N1LanSetup 数据结构，用于调试。
 *
 */
static inline NK_Void
NK_N1_LAN_SETUP_DUMP(NK_N1LanSetup *__LanSetup)
{
	NK_Int i = 0, ii = 0;
	NK_TermTable Table;
	NK_Char text[64];
	NK_CHECK_POINT();
	NK_TermTbl_BeginDraw(&Table, "Lan Setup Data Field", 80, 4);

	if (NK_N1_LAN_SETUP_TIME == (__LanSetup)->classify) {
		NK_TermTbl_PutText(&Table, NK_True, "%-48s", "Time");
		NK_TermTbl_PutKeyValue(&Table, NK_False, "UTC", "%u", (__LanSetup)->Time.utc);
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Time Zone", "%d", (__LanSetup)->Time.gmt);
		NK_TermTbl_PutKeyValue(&Table, NK_True, "DST", "%s", (__LanSetup)->Time.dst ? "Enabled" : "Disabled");
	} else if (NK_N1_LAN_SETUP_VIMG == (__LanSetup)->classify) {
		NK_TermTbl_PutText(&Table, NK_True, "%-48s", "Video Image");
		NK_TermTbl_PutKeyValue(&Table, NK_True, "Channel", "%d", (__LanSetup)->channel_id);
		NK_TermTbl_PutText(&Table, NK_True, "%-48s", "Title");
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Show", "%s", (__LanSetup)->VideoImage.Title.Show.val ? "Yes" : "No");
		NK_TermTbl_PutKeyValue(&Table, NK_True, "Text", "%s", (__LanSetup)->VideoImage.Title.Text.val);
		NK_TermTbl_PutText(&Table, NK_True, "%-48s", "Color");
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Hue Level", "%d [%d, %d]",
			(__LanSetup)->VideoImage.HueLevel.val, (__LanSetup)->VideoImage.HueLevel.min, (__LanSetup)->VideoImage.HueLevel.max);
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Sharpness Level", "%d [%d, %d]",
			(__LanSetup)->VideoImage.SharpnessLevel.val, (__LanSetup)->VideoImage.SharpnessLevel.min, (__LanSetup)->VideoImage.SharpnessLevel.max);
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Contrast Level", "%d [%d, %d]",
			(__LanSetup)->VideoImage.ContrastLevel.val, (__LanSetup)->VideoImage.ContrastLevel.min, (__LanSetup)->VideoImage.ContrastLevel.max);
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Brightness Level", "%d [%d, %d]",
			(__LanSetup)->VideoImage.BrightnessLevel.val, (__LanSetup)->VideoImage.BrightnessLevel.min, (__LanSetup)->VideoImage.BrightnessLevel.max);
		NK_TermTbl_PutKeyValue(&Table, NK_True, "Saturation Level", "%d [%d, %d]",
			(__LanSetup)->VideoImage.SaturationLevel.val, (__LanSetup)->VideoImage.SaturationLevel.min, (__LanSetup)->VideoImage.SaturationLevel.max);
		NK_TermTbl_PutKeyValue(&Table, NK_True, "Flip / Mirror", "%s / %s",
			(__LanSetup)->VideoImage.Flip.val ? "Yes" : "No", (__LanSetup)->VideoImage.Mirror.val ? "Yes" : "No");
		NK_TermTbl_PutText(&Table, NK_True, "%-48s", "Motion Detection");
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Enabled", "%s", (__LanSetup)->VideoImage.MotionDetection.Enabled.val ? "Yes" : "No");
		NK_TermTbl_PutKeyValue(&Table, NK_True, "Sensitivity Level", "%d [%d, %d]",
			(__LanSetup)->VideoImage.MotionDetection.SensitivityLevel.val,
			(__LanSetup)->VideoImage.MotionDetection.SensitivityLevel.min,
			(__LanSetup)->VideoImage.MotionDetection.SensitivityLevel.max);
		NK_TermTbl_PutText(&Table, NK_False, "%s (%u x %u)", "Mask",
				(__LanSetup)->VideoImage.MotionDetection.Mask.width, (__LanSetup)->VideoImage.MotionDetection.Mask.height);
		for (i = 0; i < (NK_Int)((__LanSetup)->VideoImage.MotionDetection.Mask.height); ++i) {
			NK_Char mask[80];
			NK_BZERO(mask, sizeof(mask));
			for (ii = 0; ii < (NK_Int)((__LanSetup)->VideoImage.MotionDetection.Mask.width); ++ii) {
				mask[ii * 2] = mask[ii * 2 + 1] = (__LanSetup)->VideoImage.MotionDetection.Mask.matrix[i][ii] ? 'O' : '.';
			}
			NK_TermTbl_PutText(&Table, NK_False, "%s", mask);
		}

	} else if (NK_N1_LAN_SETUP_VENC == (__LanSetup)->classify) {
		NK_TermTbl_PutText(&Table, NK_True, "%-48s", "Video Encoder");
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Channel", "%d", (__LanSetup)->channel_id);
		NK_TermTbl_PutKeyValue(&Table, NK_True, "Stream", "%d", (__LanSetup)->stream_id);
		if (NK_N1_VENC_CODEC_H264 == (__LanSetup)->VideoEncoder.Codec.val) {
			NK_TermTbl_PutKeyValue(&Table, NK_False, "Resolution", "%d", (__LanSetup)->VideoEncoder.H264.Resolution.val);
			NK_TermTbl_PutKeyValue(&Table, NK_False, "Bit Rate", "%d", (__LanSetup)->VideoEncoder.H264.BitRate.val);
			NK_TermTbl_PutKeyValue(&Table, NK_False, "Frame Rate", "%d", (__LanSetup)->VideoEncoder.H264.FrameRate.val);
			NK_TermTbl_PutKeyValue(&Table, NK_False, "Key Frame Interval", "%d", (__LanSetup)->VideoEncoder.H264.KeyFrameInterval.val);
			NK_TermTbl_PutKeyValue(&Table, NK_False, "Bit Rate Control Mode", "%s", NK_ENUM_MAP(N1BitRateCtrlMode, (NK_N1BitRateCtrlMode)((__LanSetup)->VideoEncoder.H264.BitRateCtrlMode.val)));
		}
	} else if (NK_N1_LAN_SETUP_VPMSK == (__LanSetup)->classify) {

		NK_TermTbl_PutText(&Table, NK_True, "%-48s", "Video Privacy Mask");
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Channel", "%d", (__LanSetup)->channel_id);
		NK_TermTbl_PutKeyValue(&Table, NK_True, "Enabled", "%s", (__LanSetup)->VideoPrivacyMask.Enabled.val ? "Yes" : "No");
		for (i = 0; i < sizeof((__LanSetup)->VideoPrivacyMask.Mask) / sizeof((__LanSetup)->VideoPrivacyMask.Mask[0]); ++i) {
			NK_TermTbl_PutKeyValue(&Table, NK_False, "Mask", "%u,%u,%u,%u",
					(NK_UInt32)((__LanSetup)->VideoPrivacyMask.Mask[i].x),
					(NK_UInt32)((__LanSetup)->VideoPrivacyMask.Mask[i].y),
					(NK_UInt32)((__LanSetup)->VideoPrivacyMask.Mask[i].width),
					(NK_UInt32)((__LanSetup)->VideoPrivacyMask.Mask[i].height));
		}
	} else if (NK_N1_LAN_SETUP_VOSD == (__LanSetup)->classify) {

		NK_TermTbl_PutText(&Table, NK_True, "%-48s", "Video OSD");
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Channel", "%d", (__LanSetup)->channel_id);
		NK_TermTbl_PutKeyValue(&Table, NK_True, "Reference Size", "%ux%u", (NK_UInt32)((__LanSetup)->ref_width), (NK_UInt32)((__LanSetup)->ref_height));
		NK_TermTbl_PutText(&Table, NK_True, "Title");
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Show", "%s", (__LanSetup)->VideoOnScreenDisplay.Title.Enabled.val ? "Yes" : "No");
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Position", "(%u, %u)",
				(NK_UInt32)((__LanSetup)->VideoOnScreenDisplay.Title.x), (NK_UInt32)((__LanSetup)->VideoOnScreenDisplay.Title.y));
		NK_TermTbl_PutKeyValue(&Table, NK_True, "Text", "%s", (__LanSetup)->VideoOnScreenDisplay.Title.Text.val);
		NK_TermTbl_PutText(&Table, NK_True, "Time");
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Show", "%s", (__LanSetup)->VideoOnScreenDisplay.Time.Enabled.val ? "Yes" : "No");
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Show Weekday", "%s", (__LanSetup)->VideoOnScreenDisplay.Time.DisplayWeekday.val ? "Yes" : "No");
//		NK_TermTbl_PutKeyInt32(&Table, NK_False, "Date Format", (__LanSetup)->VideoOnScreenDisplay.osdType);
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Time Format", "%s", (__LanSetup)->VideoOnScreenDisplay.Time.TimeFormt12HRs.val ? "12HRs" : "24HRs");
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Position", "(%u, %u)",
				(NK_UInt32)((__LanSetup)->VideoOnScreenDisplay.Time.x),
				(NK_UInt32)((__LanSetup)->VideoOnScreenDisplay.Time.y));

	}else if (NK_N1_LAN_SETUP_PTZ == (__LanSetup)->classify) {

		NK_TermTbl_PutText(&Table, NK_True, "%-48s", "Pan Tilt Zoom");
		NK_TermTbl_PutKeyValue(&Table, NK_True, "Channel", "%d", (__LanSetup)->channel_id);
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Command", NK_ENUM_MAP(N1PTZCommand, (__LanSetup)->PanTiltZoom.command));
		if (NK_N1_PTZ_CMD_SET_PRESET == (__LanSetup)->PanTiltZoom.command
				|| NK_N1_PTZ_CMD_GOTO_PRESET == (__LanSetup)->PanTiltZoom.command
				|| NK_N1_PTZ_CMD_CLEAR_PRESET == (__LanSetup)->PanTiltZoom.command) {
			NK_TermTbl_PutKeyValue(&Table, NK_False, "Preset Position", "%d", (__LanSetup)->PanTiltZoom.preset_position);
		}
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Speed", "%d [%d, %d]", (__LanSetup)->PanTiltZoom.Speed.val,
				(__LanSetup)->PanTiltZoom.Speed.min, (__LanSetup)->PanTiltZoom.Speed.max);
	} else if (NK_N1_LAN_SETUP_NET_WIRED == (__LanSetup)->classify
			|| NK_N1_LAN_SETUP_NET_WIFI == (__LanSetup)->classify) {

		if (NK_N1_LAN_SETUP_NET_WIFI == (__LanSetup)->classify) {

			if ((NK_N1_ETH_WIFI_MODE_STA == (__LanSetup)->NetWiFi.WiFiMode.val
					|| NK_N1_ETH_WIFI_MODE_REP == (__LanSetup)->NetWiFi.WiFiMode.val)
					&& NK_Nil != (__LanSetup)->NetWiFi.Station) {
				NK_TermTbl_PutText(&Table, NK_True, "%s", "Wi-Fi Station");
				NK_TermTbl_PutKeyValue(&Table, NK_False, "EssID", "%s", (__LanSetup)->NetWiFi.Station->essid);
				NK_TermTbl_PutKeyValue(&Table, NK_True, "Passphrase", "%s", (__LanSetup)->NetWiFi.Station->passphrase);
			}

			if (NK_N1_ETH_WIFI_MODE_REP == (__LanSetup)->NetWiFi.WiFiMode.val
					&& NK_Nil != (__LanSetup)->NetWiFi.StationAlternative) {
				NK_TermTbl_PutText(&Table, NK_True, "%s", "Wi-Fi Station Alternative");
				NK_TermTbl_PutKeyValue(&Table, NK_False, "EssID", "%s", (__LanSetup)->NetWiFi.StationAlternative->essid);
				NK_TermTbl_PutKeyValue(&Table, NK_True, "Passphrase", "%s", (__LanSetup)->NetWiFi.StationAlternative->passphrase);
			}

			if ((NK_N1_ETH_WIFI_MODE_AP == (__LanSetup)->NetWiFi.WiFiMode.val
					|| NK_N1_ETH_WIFI_MODE_REP == (__LanSetup)->NetWiFi.WiFiMode.val)
					&& NK_Nil != (__LanSetup)->NetWiFi.AccessPoint) {
				NK_TermTbl_PutText(&Table, NK_True, "%s", "Wi-Fi Access Point");
				NK_TermTbl_PutKeyValue(&Table, NK_False, "EssID", "%s", (__LanSetup)->NetWiFi.AccessPoint->essid);
				NK_TermTbl_PutKeyValue(&Table, NK_True, "Passphrase", "%s", (__LanSetup)->NetWiFi.AccessPoint->passphrase);
			}

		} else {
			NK_TermTbl_PutText(&Table, NK_True, "%-48s", "Wired Net");
		}
		NK_N1_PROP_HWADDR_STR(&(__LanSetup)->NetWiFi.HwAddr, text, sizeof(text));
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Hardware Address", "%s", text);
		NK_TermTbl_PutKeyValue(&Table, NK_False, "DHCP", "%s", (__LanSetup)->NetWiFi.EnableDHCP.val ? "Enabled" : "Disabled");
		NK_N1_PROP_IPV4_NTOA(&(__LanSetup)->NetWiFi.IPAddress, text, sizeof(text));
		NK_TermTbl_PutKeyValue(&Table, NK_False, "IP Address", "%s", text);
		NK_N1_PROP_IPV4_NTOA(&(__LanSetup)->NetWiFi.Netmask, text, sizeof(text));
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Netmask", "%s", text);
		NK_N1_PROP_IPV4_NTOA(&(__LanSetup)->NetWiFi.Gateway, text, sizeof(text));
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Gateway", "%s", text);
		NK_N1_PROP_IPV4_NTOA(&(__LanSetup)->NetWiFi.DomainNameServer, text, sizeof(text));
		NK_TermTbl_PutKeyValue(&Table, NK_False, "DNS", "%s", text);
	} else if (NK_N1_LAN_SETUP_DNS == (__LanSetup)->classify) {
		NK_N1_PROP_IPV4_NTOA(&(__LanSetup)->DNS.Preferred, text, sizeof(text));
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Preferred DNS", "%s", text);
		NK_N1_PROP_IPV4_NTOA(&(__LanSetup)->DNS.Alternative, text, sizeof(text));
		NK_TermTbl_PutKeyValue(&Table, NK_False, "Alternative DNS", "%s", text);
	} else if (NK_N1_LAN_SETUP_P2P == (__LanSetup)->classify) {
		NK_TermTbl_PutKeyValue(&Table, NK_False, "P2P", "%s", (__LanSetup)->PtoP.Enabled.val ? "Enabled" : "Disabled");
	} else if (NK_N1_LAN_SETUP_ONVIF == (__LanSetup)->classify) {
		NK_TermTbl_PutText(&Table, NK_True, "%-48s", "Onvif");
		NK_TermTbl_PutKeyValue(&Table, NK_False, "IP Audio Adaption Enabled", "%s", (__LanSetup)->Onvif.IPAutoAdaption.val ? "Enabled" : "Disabled");
		NK_TermTbl_PutKeyValue(&Table, NK_False, "IP Audio Adaption Actived", "%s", (__LanSetup)->Onvif.ipAutoAdaptionActived ? "Actived" : "Deactived");
	}

	NK_TermTbl_EndDraw(&Table);
}

/**
 * N1 通知类型。
 *
 */
typedef NK_Size Nk_N1NotificationType;
#define NK_N1_NOTF_NA                 (0)     ///< 无效通知
#define NK_N1_NOTF_MOTION_DETECTED    (0x200) ///< 运动侦测通知。

#define NK_N1_NOTF_VIDEO_LOSS         (0x400) ///< 视频丢失通知
#define NK_N1_NOTF_VIDEO_SHELTER      (0x402) ///< 视频遮挡通知

#define NK_N1_NOTF_HDD_NOT_FOUND      (0x700)  ///< 未发现硬盘通知
#define NK_N1_NOTF_HDD_FULL           (0x702)  ///< 硬盘满通知
#define NK_N1_NOTF_HDD_RECORD_ERR     (0x704)  ///< 硬盘录像异常通知

#define NK_N1_NOTF_REMOTE_KEYPAD      (0x10000) ///< 遥控相关通知
#define NK_N1_NOTF_DOOR_MAGNETIC      (0x11000) ///< 门磁感应相关通知

#define NK_N1_NOTF_PIR_DETECTED       (0x12000) ///< 人体红外感应通知
#define NK_N1_NOTF_SMOKE_DETECTED     (0x13000) ///< 烟雾探测通知
	

/**
 * N1 通知数据结构。
 */
typedef struct Nk_N1Notification
{
	Nk_N1NotificationType type;

	union {

		/// 运动侦测通知，当 @ref NK_N1Notification::type 为 NK_N1_NOTF_MOTION_DETECTED 有效。
		struct {
			NK_UInt32 reserved;
		} MotionDetected;

		/// 人体红外感应检测，当 @ref NK_N1Notification::type 为 NK_N1_NOTF_PIR_DETECTED 有效。
		struct {
			NK_UInt32 reserved;
		} PIRDetected;
	};

} NK_N1Notification;



/**
 * 无线热点数据结构定义。
 */
typedef struct Nk_WiFiHotSpot
{
	/**
	 * 无线热点的 BSSID。
	 */
	NK_Char bssid[32];
	/**
	 * 无线热点通信信道，0 表示自动。
	 */
	NK_Int channel;
	/**
	 * 无线热点信号强度。
	 */
	NK_Int dBm, sdBm;
	/**
	 * 无线热点生存期。
	 */
	NK_Int age;

	/**
	 * 无线热点的 ESSID。
	 */
	NK_Char essid[128];

	/**
	 * 无线热点的 PSK。
	 */
	NK_Char psk[32];

} NK_WiFiHotSpot;


/**
 * 打印 NK_WiFiHotSpot 数据结构。
 */
#define NK_N1_WIFI_HOTSPOT_DUMP(__HotSpot) \
	do{\
		NK_TermTable Table;\
		NK_TermTbl_BeginDraw(&Table, "N1 Hot Spot", 64, 4);\
		NK_TermTbl_PutKeyValue(&Table, NK_True, "BSSID", "%s", (__HotSpot)->bssid);\
		NK_TermTbl_PutKeyValue(&Table, NK_True, "Channel", "%d", (__HotSpot)->channel);\
		NK_TermTbl_PutKeyValue(&Table, NK_True, "dBm / SdBm", "%d / %d", (__HotSpot)->dBm, (__HotSpot)->sdBm);\
		NK_TermTbl_PutKeyValue(&Table, NK_True, "Age", "%d", (__HotSpot)->age);\
		NK_TermTbl_PutKeyValue(&Table, NK_True, "ESSID", "%s", (__HotSpot)->essid);\
		NK_TermTbl_PutKeyValue(&Table, NK_True, "PSK", "%s", (__HotSpot)->psk);\
		NK_TermTbl_EndDraw(&Table);\
	} while(0)


NK_CPP_EXTERN_END
#endif /* NK_N1_DEF_H_ */
