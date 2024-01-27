/**
 * 日志信息控制单体模块。
 */

#include <NkUtils/types.h>

#if !defined(NK_UTILS_LOG_H_)
#define NK_UTILS_LOG_H_
NK_CPP_EXTERN_BEGIN

/**
 * @macro
 *  单行日志输出最大长度，当单行日志输出长度大于此值时会自动截断。
 */
#define NK_LOG_MAX_LINE_SZ (1024)

/**
 * @macro
 *  日志内部缓冲容量，当内部缓冲达到此容量时会触发 onFlush 事件清空缓冲。
 */
#define NK_LOG_CACHE_SZ    (1024 * 16)

/**
 * 日志处理等级。
 */
typedef enum Nk_LogLevel
{
	NK_LOG_ALL = (0),
	NK_LOG_LV_DEBUG,
	NK_LOG_LV_INFO,
	NK_LOG_LV_WARN,
	NK_LOG_LV_ERROR,
	NK_LOG_LV_ALERT,
	NK_LOG_NONE,

} NK_LogLevel;


/**
 * 日子标识定义
 */
#define NK_LOG_TAG(__level) \
	((NK_LOG_LV_ALERT == __level) ? "alert" : (\
			(NK_LOG_LV_ERROR == __level) ? "error" : (\
					(NK_LOG_LV_WARN == __level) ? "warn" : (\
							(NK_LOG_LV_INFO == __level) ? "info" : (\
									(NK_LOG_LV_DEBUG == __level) ? "debug" : "undef")))))


/**
 * Log 单体模块句柄。
 */
struct Nk_Log
{
	/**
	 * @brief
	 *  设置日志保存等级。\n
	 *  基于安全性考虑版本 >= 33 此功能屏蔽。
	 * @brief
	 *  设置等级后，要求满足输出等级小于等于设置等级才会进行保存，否则一律丢弃。\n
	 *  日志保存路径由接口 @ref setLogPath 进行设置。\n
	 *  如设置等级为 @ref NK_LOG_LV_NOTICE，则通过接口 @ref error 和接口 @ref notice 的日志模块才会处理，\n
	 *  而接口 @ref info 和接口 @ref debug 的日志模块会自动丢弃。\n
	 *  不设置的情况下，默认是 @ref NK_LOG_LV_ERROR 等级。
	 *
	 */
	NK_Void
	(*setLogLevel)(NK_LogLevel level);

	/**
	 * @brief
	 *  获取日志保存等级。\n
	 *  基于安全性考虑版本 >= 33 此功能屏蔽。
	 */
	NK_LogLevel
	(*getLogLevel)();

	/**
	 * @brief
	 *  设置日志控制台输出等级。\n
	 * @details
	 *  只有满足输出等级小于等于设置等级才会在控制台输出，否则忽略。\n
	 *  不设置的情况下，默认是 @ref NK_LOG_LV_INFO 等级。
	 *
	 */
	NK_Void
	(*setTerminalLevel)(NK_LogLevel level);

	/**
	 * @brief
	 *  获取日志控制台输出等级。
	 */
	NK_LogLevel
	(*getTerminalLevel)();

	/**
	 * 警戒日志记录。
	 */
	NK_Void
	(*alert)(const NK_Char* fmt, ...);

	/**
	 * 错误日志记录。
	 */
	NK_Void
	(*error)(const NK_Char* fmt, ...);

	/**
	 * 通知日志记录。
	 */
	NK_Void
	(*warn)(const NK_Char* fmt, ...);

	/**
	 * 信息日志记录。
	 */
	NK_Void
	(*info)(const NK_Char* fmt, ...);

	/**
	 * 调试信息记录。
	 */
	NK_Void
	(*debug)(const NK_Char* fmt, ...);

	/**
	 * @brief
	 *  基于安全性考虑版本 >= 33 此功能屏蔽。
	 * @details
	 *  模块内会保留一块日志缓冲，所有等级小于 Terminal Level 的日志将会被记录到缓冲以内，\n
	 *  当缓冲满时会自动触发此接口擦除，同时会触发 @ref onFlush 事件，\n
	 *  用户可以根据具体实现方案处理在 @ref onFlush 事件中处理缓冲的日志数据。
	 */
	NK_Void
	(*flush)();

	/**
	 * @brief
	 *  日志缓冲满冲刷事件。\n
	 *  基于安全性考虑版本 >= 33 此功能屏蔽。
	 * @details
	 *  参考 @ref flush。\n
	 *  须要注意的是，事件实现中不能调用 NK_Log 模块相关接口，否则由可能由于程序死循环造成的崩溃。
	 */
	NK_Void
	(*onFlush)(NK_PByte bytes, NK_Size len);

	/**
	 * @brief
	 *  日志输出事件。
	 *  基于安全性考虑版本 >= 33 此功能屏蔽。
	 * @details
	 *  每当用户调用有效日志输出时会出发次事件，\n
	 *  用户可以根据需要，把消息重定向到另外一个地方，如网络地址，文件保存等。\n
	 *  保留此事件为 Nil 则忽略。
	 *
	 * @param classify [in]
	 *  当前消息的等级，必须在有效等级以内。
	 * @param mesg [in]
	 *  消息内容。
	 *
	 */
	NK_Void
	(*onTrace)(NK_LogLevel level, NK_PChar msg);

	/**
	 * @brief
	 *  打印模块状态。
	 */
	NK_Void
	(*dump)();

	/**
	 * @brief
	 *  设置模块日志等级。
	 *  基于安全性考虑版本 >= 33 此功能屏蔽。
	 * @details
	 *  针对模块名称 @ref mod，配置模块的输出等级，\n
	 *  未配置的模块将默认为 @ref NK_LOG_NONE 等级，不作日志输出，\n
	 *  如果 @ref mod 名称已经存在与日志记录中则会更新该模块等级。\n
	 *  此接口非线程安全，建议在程序初始化时作全局调用，切忌在多线程打印日志时频繁调用。
	 *
	 * @param mod [in]
	 *  模块名称。
	 * @param level [in]
	 *  日志等级。
	 *
	 * @return
	 *  设置成功返回 True，失败返回 False。
	 */
	NK_Boolean
	(*setModLevel)(const NK_PChar mod, NK_LogLevel level);

	/**
	 * @brief
	 *  设置模块日志等级。
	 *  基于安全性考虑版本 >= 33 此功能屏蔽。
	 *
	 * @param mod [in]
	 *  模块名称。
	 *
	 * @return
	 *  返回该模块等级。
	 */
	NK_LogLevel
	(*getModLevel)(const NK_PChar mod);


	/**
	 * @brief
	 *  输出等级为 @ref NK_LOG_LV_ALERT 的日志。
	 *  基于安全性考虑版本 >= 33 此功能屏蔽。
	 * @details
	 *  针对模块 @ref mod 输出，\n
	 *  设置模块日志等级见 @ref setModLevel() 方法。
	 */
	NK_Void
	(*alertV2)(const NK_PChar mod, const NK_Char* fmt, ...);

	/**
	 * @brief
	 *  输出等级为 @ref NK_LOG_LV_ERROR 的日志。
	 *  基于安全性考虑版本 >= 33 此功能屏蔽。
	 * @details
	 *  针对模块 @ref mod 输出，\n
	 *  设置模块日志等级见 @ref setModLevel() 方法。
	 */
	NK_Void
	(*errorV2)(const NK_PChar mod, const NK_Char* fmt, ...);

	/**
	 * @brief
	 *  输出等级为 @ref NK_LOG_LV_WARN 的日志。
	 *  基于安全性考虑版本 >= 33 此功能屏蔽。
	 * @details
	 *  针对模块 @ref mod 输出，\n
	 *  设置模块日志等级见 @ref setModLevel() 方法。
	 */
	NK_Void
	(*warnV2)(const NK_PChar mod, const NK_Char* fmt, ...);

	/**
	 * @brief
	 *  输出等级为 @ref NK_LOG_LV_INFO 的日志。
	 *  基于安全性考虑版本 >= 33 此功能屏蔽。
	 * @details
	 *  针对模块 @ref mod 输出，\n
	 *  设置模块日志等级见 @ref setModLevel() 方法。
	 */
	NK_Void
	(*infoV2)(const NK_PChar mod, const NK_Char* fmt, ...);

	/**
	 * @brief
	 *  输出等级为 @ref NK_LOG_LV_DEBUG 的日志。
	 *  基于安全性考虑版本 >= 33 此功能屏蔽。
	 * @details
	 *  针对模块 @ref mod 输出，\n
	 *  设置模块日志等级见 @ref setModLevel() 方法。
	 */
	NK_Void
	(*debugV2)(const NK_PChar mod, const NK_Char* fmt, ...);

};

/**
 * 获取单体句柄。
 */
NK_API struct Nk_Log
*NK_Log();


/**
 * 终端输出表格句柄。
 * 存放表格输出过程中的上下文。
 */
typedef struct Nk_TermTable
{
	NK_Byte reserved[128];

} NK_TermTable;

/**
 * 开始绘制终端表格。
 * 初始化 @ref Tbl 句柄。
 */
NK_API NK_Int
NK_TermTbl_BeginDraw(NK_TermTable *Tbl, NK_PChar title, NK_Size width, NK_Size padding);

/**
 * 终端表格追加一行明文文本。
 *
 * @param[in]			end_ln			行结束标识，为 True 时明文追加后会加上底部边框。
 * @param[in]			fmt				文本输出格式。
 *
 * @retval 0		输出成功。
 * @retval -1		输出失败。
 *
 */
NK_API NK_Int
NK_TermTbl_PutText(NK_TermTable *Tbl, NK_Boolean end_ln, NK_PChar fmt, ...);

/**
 * 终端表格追加一行“键 - 值”格式的文本。。
 *
 * @param[in]			end_ln			行结束标识，为 True 时明文追加后会加上底部边框。
 * @param[in]			key				键文本。
 * @param[in]			fmt				值文本输出格式。
 *
 * @retval 0		输出成功。
 * @retval -1		输出失败。
 *
 */
NK_API NK_Int
NK_TermTbl_PutKeyValue(NK_TermTable *Tbl, NK_Boolean end_ln, NK_PChar key, NK_PChar fmt, ...);

/**
 * 终端表格快速输出一行“键 - 32 位整型”格式文本操作宏。
 *
 */
#define NK_TermTbl_PutKeyInt32(__Tbl, __end_ln, __key, __int32) \
	do{\
		NK_TermTbl_PutKeyValue(__Tbl, __end_ln, __key, "%d - %08x", (NK_Int)(__int32), (NK_UInt32)(__int32));\
	} while(0)

/**
 * 终端表格快速输出一行“键 - 字符串”格式文本操作宏。
 *
 */
#define NK_TermTbl_PutKeyInt64(__Tbl, __end_ln, __key, __int64) \
	do{\
		NK_TermTbl_PutKeyValue(__Tbl, __end_ln, __key, "%lld - %016x", (NK_Int64)(__int64), (NK_UInt64)(__int64));\
	} while(0)

/**
 * 终端表格快速输出一行“键 - 字符串”格式文本操作宏。
 *
 */
#define NK_TermTbl_PutKeyString(__Tbl, __end_ln, __key, __str) \
	do{\
		NK_TermTbl_PutKeyValue(__Tbl, __end_ln, __key, "%s", __str);\
	} while(0)


/**
 * 结束绘制。\n
 * 调用此接口会根据之前的输出情况追加一个底部边框，并复位句柄。
 *
 */
NK_API NK_Int
NK_TermTbl_EndDraw(NK_TermTable *Tbl);



NK_CPP_EXTERN_END
#endif /* NK_UTILS_LOG_H_ */
