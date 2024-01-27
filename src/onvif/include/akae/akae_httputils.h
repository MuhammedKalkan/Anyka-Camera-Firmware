/**
 * HTTP 协议基础工具集定义。
 *
 */

#include <akae_typedef.h>
#include <akae_object.h>
//#include <NkUtils/types.h>
//#include <NkUtils/allocator.h>
//#include <NkUtils/str_list.h>
//#include <NkUtils/assert.h>
#include <akae_verbose.h>


#if !defined(AKAE_HTTPUTILS_H_)
# define AKAE_HTTPUTILS_H_
AK_C_HEADER_EXTERN_C_BEGIN

/**
 * HTTP 换行符定义。
 */
#define AK_CRLF "\r\n"

/**
 * @macro
 *  类 HTTP 协议的协议名称最大长度。
 */
#define AK_HTTP_LIKE_PROTO_NAME_MAX_LEN  (16)

/**
 * 最大域名长度
 */
#define AK_HTTP_HOST_MAX_LEN             (256)

/**
 * 最大路径长度
 */
#define AK_HTTP_PATH_MAX_LEN             (256)

/**
 * 最大协议查询字符长度
 */
#define AK_HTTP_QUERY_STRING_MAX_LEN     (1024 * 4)

/**
 * HTTP 回复码定义。
 */
enum {

	AK_HTTP_CODE_CONTINUE = (100),
	AK_HTTP_CODE_SWITCHING_PROTOCOLS = (101),
	AK_HTTP_CODE_OK = (200),
	AK_HTTP_CODE_CREATED = (201),
	AK_HTTP_CODE_ACCEPTED = (202),
	AK_HTTP_CODE_NON_AUTHORITATIVE_INFOMATION = (203),
	AK_HTTP_CODE_NO_CONTENT = (204),
	AK_HTTP_CODE_RESET_CONTENT = (205),
	AK_HTTP_CODE_PARTIAL_CONTENT = (206),
	AK_HTTP_CODE_MULTIPLE_CHOICES = (300),
	AK_HTTP_CODE_MOVED_PERMANENTLY = (301),
	AK_HTTP_CODE_FOUND = (302),
	AK_HTTP_CODE_SEE_OTHER = (303),
	AK_HTTP_CODE_NOT_MODIFIED = (304),
	AK_HTTP_CODE_USE_PROXY = (305),
	AK_HTTP_CODE_TEMPORARY_REDIRECT = (307),
	AK_HTTP_CODE_BAD_REQUEST = (400),
	AK_HTTP_CODE_UNAUTHORIZED = (401),
	AK_HTTP_CODE_PAYMENT_REQUIRED = (402),
	AK_HTTP_CODE_FORBIDDEN = (403),
	AK_HTTP_CODE_NOT_FOUND = (404),
	AK_HTTP_CODE_METHOD_NOT_ALLOWED = (405),
	AK_HTTP_CODE_NOT_ACCEPTABLE = (406),
	AK_HTTP_CODE_PROXY_AUTHENTICATION_REQUIRED = (407),
	AK_HTTP_CODE_REQUEST_TIME_OUT = (408),
	AK_HTTP_CODE_CONFLICT = (409),
	AK_HTTP_CODE_GONE = (410),
	AK_HTTP_CODE_LENGTH_REQUIRED = (411),
	AK_HTTP_CODE_PRECONDITION_FAILED = (412),
	AK_HTTP_CODE_REQUEST_ENTITY_TOO_LARGE = (413),
	AK_HTTP_CODE_REQUEST_URI_TOO_LARGE = (414),
	AK_HTTP_CODE_UNSUPPORTED_MEDIA_TYPE = (415),
	AK_HTTP_CODE_REQUESTED_RANGE_NOT_SATISFIABLE = (416),
	AK_HTTP_CODE_EXPECTATION_FAILED = (417),
	AK_HTTP_CODE_INTERNAL_SERVER_ERROR = (500),
	AK_HTTP_CODE_NOT_IMPLEMENTED = (501),
	AK_HTTP_CODE_BAD_GATEWAY = (502),
	AK_HTTP_CODE_SERVICE_UNAVAILABLE = (503),
	AK_HTTP_CODE_GATEWAY_TIME_OUT = (504),
	AK_HTTP_CODE_HTTP_VERSION_NOT_SUPPORTED = (505),

};


/**
 * 通过文件名称获取文件的 MIME。\n
 * 接口根据文件后缀获取 MIME，因此文件名称必须带有后缀，如 file.txt 则返回 text/plain.
 *
 * @return
 *  返回文件名称对应的 MIME 信息, 如果文件名称异常或者该文件后缀未能被识别则返回 application/octet-stream.
 *
 */
AK_API AK_chrcptr akae_http_file_mime (AK_chrcptr file_name);

/**
 * 获取 HTTP 返回码缺省消息。
 *
 * @param[IN]
 *  code
 *  HTTP 返回码。
 *
 * @return
 *  返回码对应消息，返回码不存在返回 AK_null。
 */
AK_API AK_chrptr akae_http_default_reason_phrase (AK_int code);

/**
 * 对字符串进行 URI 编码。
 *
 * @details
 *  不对 @ * / + 符号进行转码。
 *
 * @param[IN]
 *  str
 *  原始字符串。
 *
 * @param[OUT]
 *  stack
 *  编码后的结果所在内存起始位置。
 *
 * @param stacklen [in,out]
 *  传入编码内存大小，返回编码后字符串长度。
 *
 * @return
 *  编码成功返回编码输出字符串长度，并在 @ref trans 内存区域返回编码结果，失败返回 -1。
 *
 */
AK_API AK_ssize akae_http_str_escape (AK_chrptr str, AK_chrptr stack, AK_size stacklen);

/**
 * 解码 URI 字符串。\n
 * 不支持 UTF-8 编码转换。\n
 * 传入的 @ref enc 和 @ref uri 可以指向同一块内存块。
 *
 * @param str [IN]
 *  原始解码字符串。
 * @param trans [out]
 *  解码后的结果所在内存起始位置。
 * @param trans_len [in,out]
 *  传入解码内存大小，返回解码后字符串长度。
 *
 * @return
 *  解码成功返回 0，并在 @ref trans 内存区域返回解码结果，失败返回 -1。
 */
AK_API AK_ssize akae_http_str_unescape (AK_chrptr str, AK_chrptr stack, AK_size stacklen);

/**
 * 编码 URI 字符串，\n
 * 不支持 UTF-8 编码转换。
 *
 * @details
 *  不对符号 ~ ! @ # $ & * ( ) = : / , ; ? + 进行转码。
 *
 * @param str [IN]
 *  原始字符串。
 * @param trans [out]
 *  编码后的结果所在内存起始位置。
 * @param trans_len [in,out]
 *  传入编码内存大小，返回编码后字符串长度。
 *
 * @return
 *  编码成功返回 0，并在 @ref trans 内存区域返回编码结果，失败返回 -1。
 */
AK_API AK_ssize akae_http_encode_uri (AK_chrptr str, AK_chrptr stack, AK_size stacklen);

/**
 * 解码 URI 字符串。\n
 * 不支持 UTF-8 编码转换。\n
 * 传入的 @ref enc 和 @ref uri 可以指向同一块内存块。
 *
 * @param str [IN]
 *  原始解码字符串。
 * @param trans [out]
 *  解码后的结果所在内存起始位置。
 * @param trans_len [in,out]
 *  传入解码内存大小，返回解码后字符串长度。
 *
 * @return
 *  解码成功返回 0，并在 @ref trans 内存区域返回解码结果，失败返回 -1。
 */
AK_API AK_ssize akae_http_decode_uri (AK_chrcptr str, AK_chrptr stack, AK_size stacklen);


/**
 *
 * @details
 *  不对符号 ~ ! * ( ) 进行转码。
 *
 * @param str [IN]
 *  原始字符串。
 * @param trans [out]
 *  编码后的结果所在内存起始位置。
 * @param trans_len [in,out]
 *  传入编码内存大小，返回编码后字符串长度。
 *
 * @return
 *  编码成功返回 0，并在 @ref trans 内存区域返回编码结果，失败返回 -1。
 *
 */
AK_API AK_ssize akae_http_encode_uri_component (AK_chrptr str, AK_chrptr stack, AK_size stacklen);

/**
 * 解码 URI 字符串。\n
 * 不支持 UTF-8 编码转换。\n
 * 传入的 @ref enc 和 @ref uri 可以指向同一块内存块。
 *
 *
 * @param str [IN]
 *  原始解码字符串。
 * @param trans [out]
 *  解码后的结果所在内存起始位置。
 * @param trans_len [in,out]
 *  传入解码内存大小，返回解码后字符串长度。
 *
 * @return
 *  解码成功返回 0，并在 @ref trans 内存区域返回解码结果，失败返回 -1。
 */
AK_API AK_ssize akae_http_decode_uri_component (AK_chrptr str, AK_chrptr stack, AK_size stacklen);


/**
 * @brief
 *  压缩 URI。去掉 @ref uri 所在字符串开始和结束位置连续的空格，\n
 *  去掉字符串中间出现连续的 / 和 \ 字符。\n
 *
 * @param[IN]
 *  uri
 *  传入 URI 字符串。\n
 *
 * @param[OUT]
 *  stack
 *  栈区内存位置。\n
 *
 * @param[IN]
 *  stacklen
 *  栈区内存长度。\n
 *
 * @retval >=0
 *  压缩后的 URI 长度，结果在 @ref stack。\n
 *
 * @retval -1。
 *  压缩失败。\n
 */
AK_API AK_ssize akae_http_strip_uri (AK_chrcptr uri, AK_chrptr stack, AK_size stacklen);



/**
 * URL 数据结构, 用于结构化 URL 字段内容.
 */
#pragma pack(push, 4)
typedef struct _AK_HttpUrl {

	/// 协议名称
	AK_char proto[AK_HTTP_LIKE_PROTO_NAME_MAX_LEN + 1];

	/// 域名
	AK_char host[AK_HTTP_HOST_MAX_LEN + 1];

	/// 端口
	AK_int port;

	/// 路径
	AK_char path[AK_HTTP_PATH_MAX_LEN + 1];

	/// 查询字段
	AK_char query[AK_HTTP_QUERY_STRING_MAX_LEN + 1];

} AK_HttpUrl;
#pragma pack(pop)


/**
 * 解析 URL 字符串成数据结构 @ref AK_HttpUrl.\n
 * 用户可以通过该接口拆分字符串内 URL 的个个元素用做其他数据输入.
 *
 * @retval AK_OK
 *  解析成功, 通过 @ref Url 数据结构获取结果。
 * @retval AK_ERR_INVAL_PARAM
 *  解析失败, 可能是参数错误。
 * @retval AK_ERR_INVAL_OPERATE
 *  解析失败, 可能传入了一个内容不能解析的字符串。
 */
AK_API AK_int akae_http_parse_url (AK_chrcptr url, AK_HttpUrl *Url);

/**
 * 序列化 URL 字符串.\n
 * 接口 @ref akae_http_parse_url() 反向操作.
 *
 * @return
 *  序列化成功返回字符串长度, 字符串保存在 @ref stack 所在内存地址上.
 * @retval AK_ERR_INVAL_PARAM
 *  序列化失败, 可能传入了错误的参数.
 *
 */
AK_API AK_ssize akae_http_stringify_url (AK_HttpUrl *Url, AK_chrptr stack, AK_size stacklen);


/**
 * 打印 @ref AK_HttpUrl 数据结构。
 */
#define AK_HTTP_URL_DUMP(__Url) \
	do{\
		AK_VerboseForm Form;\
		akae_verbose_form_init (&Form, "URL", 64, 4);\
		akae_verbose_form_put_kv (&Form, AK_false, "Protocol", "%s", (__Url)->proto);\
		akae_verbose_form_put_kv (&Form, AK_false, "Host", "%s", (__Url)->host);\
		akae_verbose_form_put_kv (&Form, AK_false, "Port", "%d", (AK_int)((__Url)->port));\
		akae_verbose_form_put_kv (&Form, AK_false, "Path", "%s", (__Url)->path);\
		akae_verbose_form_finish (&Form);\
	} while(0)


#if 0





/**
 * @macro
 *  打印输出数据结构 AK_HTTPHeadField。
 */
#define AK_HTTP_HEAD_FIELD_DUMP(__Object) \
	do{\
		AK_TermTable Table;\
		AK_Size number = 0;\
		AK_chrptr key = AK_AK_null;\
		AK_chrptr value = AK_AK_null;\
		AK_Int i = 0;\
		\
		AK_CHECK_POINT();\
		AK_TermForm_BeginDraw(&Table, "HTTP Head Filed", 96, 4);\
		AK_TermForm_PutKeyValue(&Table, AK_True, "Protocol", "%s %u.%u",\
				(__Object)->protocol, (__Object)->ver_maj, (__Object)->ver_min);\
		if ((__Object)->isRequest) {\
			AK_TermForm_PutKeyValue(&Table, AK_False, "Method", "%s", (__Object)->method);\
			AK_TermForm_PutKeyValue(&Table, AK_True, "Absolute Path", "%s", (__Object)->abs_path);\
			number = (__Object)->numberOfQuery((__Object));\
			AK_TermForm_PutText(&Table, AK_True, "Query String (%u)", number);\
			for (i = 0; i < (AK_Int)number; ++i) {\
				if (0 == (__Object)->indexOfQuery((__Object), i, &key, &value)) {\
					AK_TermForm_PutKeyValue(&Table, (i == number - 1), key, "%s", value);\
				}\
			}\
		} else {\
			AK_TermForm_PutKeyValue(&Table, AK_False, "Code", "%u", (__Object)->code);\
			AK_TermForm_PutKeyValue(&Table, AK_True, "Reason Phrase", "%s", (__Object)->reason_phrase);\
		}\
		\
		number = (__Object)->numberOfOption((__Object));\
		AK_TermForm_PutText(&Table, AK_True, "Option Tag (%u)", number);\
		for (i = 0; i < (AK_Int)number; ++i) {\
			if (0 == (__Object)->indexOfOption((__Object), i, &key, &value)) {\
				AK_TermForm_PutKeyValue(&Table, (i == number - 1), key, "%s", value);\
			}\
		}\
		AK_TermForm_EndDraw(&Table);\
	} while(0)




/**
 * @macro
 *  探测 HTTP 头域快速调用。
 */
#define AK_HTTPUtils_DetectHTTPHeadField(__package, __len, __ver_maj, __ver_min) \
	AK_HTTPUtils_DetectIndicatedHeadField((__package), (__len), "HTTP", (__ver_maj), (__ver_min));

/**
 * @macro
 *  探测 RTSP 头域快速调用。
 */
#define AK_HTTPUtils_DetectRTSPHeadField(__package, __len, __ver_maj, __ver_min) \
	AK_HTTPUtils_DetectIndicatedHeadField((__package), (__len), "RTSP", (__ver_maj), (__ver_min));




/**
 * @brief
 *  创建一个简单的报文。
 * @detais
 *  该方法会会把 @ref Headfield 头域数据结构展开成报文，并合并请求内容到用户指定的内存空间 @ref stack。\n
 *  数据结构展开以后 @ref Headfield 数据结构不会被释放，用户需要在外部对其进行释放，\n
 *  方法内部会更新 @ref Headfield 数据结构内的 Content-Type 标签。
 *
 * @param Headfield [IN]
 *  头域数据结构。
 * @param content [IN]
 *  发送内容内存起始地址，传入 AK_null 则表示没有报文内容。
 * @param contentlen [IN]
 *  发送内容的长度，传入 0 则表示没有报文。
 * @param stack [out]
 *  返回报文的所在内存起始地址，当打包报文成功以后从这里获取报文结果。
 * @param stacklen [IN]
 *  返回报文的可用内存空间大小，如果空间不足以存放头域和内容则打包失败。
 *
 * @return
 *  创建成功返回报文长度，创建失败返回 -1。
 *
 */
AK_API AK_SSize
AK_HTTPUtils_SimplePacket(AK_HTTPHeadField *Headfield, AK_PByte content, AK_Size contentlen, AK_PByte stack, AK_Size stacklen);

/**
 * @brief
 *  创建一个简单的报文。
 * @detais
 *  该方法会会把 @ref Headfield 头域数据结构展开成报文，并合并请求内容到用户指定的内存空间 @ref stack。\n
 *  数据结构展开以后 @ref Headfield 数据结构不会被释放，用户需要在外部对其进行释放，\n
 *  方法内部会更新 @ref Headfield 数据结构内的 Content-Type 标签。
 *
 * @param ver [IN]
 *  报文版本号（如：1.0），传入 AK_null 则默认 1.1 版本。
 * @param method [IN]
 *  请求方式。
 * @param abspath [IN]
 *  请求绝对路径（如：http://192.168.0.1:10080/index.html?var=0），内部会解析其协议以及地址。
 * @param agent [IN]
 *  代理名称，与报文头域 User-Agent 标签相关，传入 AK_null 则表示不使用。
 * @param contenttype [IN]
 *  发送内容类型，与报文头域 Content-Type 标签相关，传入 AK_null 则表示不使用。
 * @param content [IN]
 *  发送内容内存起始地址，传入 AK_null 则表示没有报文内容。
 * @param contentlen [IN]
 *  发送内容的长度，传入 0 则表示没有报文。
 * @param stack [out]
 *  返回报文的所在内存起始地址，当打包报文成功以后从这里获取报文结果。
 * @param stacklen [IN]
 *  返回报文的可用内存空间大小，如果空间不足以存放头域和内容则打包失败。
 *
 * @return
 *  创建成功返回报文长度，创建失败返回 -1。
 *
 */
AK_API AK_SSize
AK_HTTPUtils_SimplePacketRequest(AK_chrptr ver, AK_chrptr method, AK_chrptr abspath, AK_chrptr agent, AK_chrptr contenttype, AK_PByte content, AK_Size contentlen, AK_PByte stack, AK_Size stacklen);


/**
 * @brief
 *  创建一个简单的报文。
 * @detais
 *  该方法会会把 @ref Headfield 头域数据结构展开成报文，并合并请求内容到用户指定的内存空间 @ref stack。\n
 *  数据结构展开以后 @ref Headfield 数据结构不会被释放，用户需要在外部对其进行释放，\n
 *  方法内部会更新 @ref Headfield 数据结构内的 Content-Type 标签。
 *
 * @param ver [IN]
 *  报文版本号（如：1.0），传入 AK_null 则默认 1.1 版本。
 * @param protocol [IN]
 *  请求协议，传入 AK_null 默认 HTTP。
 * @param code [IN]
 *  回复码。
 * @param server [IN]
 *  服务名称，与报文头域 Server 标签相关，传入 AK_null 则表示不使用。
 * @param contenttype [IN]
 *  发送内容类型，与报文头域 Content-Type 标签相关，传入 AK_null 则表示不使用。
 * @param content [IN]
 *  发送内容内存起始地址，传入 AK_null 则表示没有报文内容。
 * @param contentlen [IN]
 *  发送内容的长度，传入 0 则表示没有报文。
 * @param stack [out]
 *  返回报文的所在内存起始地址，当打包报文成功以后从这里获取报文结果。
 * @param stacklen [IN]
 *  返回报文的可用内存空间大小，如果空间不足以存放头域和内容则打包失败。
 *
 * @return
 *  创建成功返回报文长度，创建失败返回 -1。
 *
 */
AK_API AK_SSize
AK_HTTPUtils_SimplePacketResponse(AK_chrptr ver, AK_chrptr protocol, AK_HTTPCode code, AK_chrptr server, AK_chrptr contenttype, AK_PByte content, AK_Size contentlen, AK_PByte stack, AK_Size stacklen);


#endif



/**
 * 创建 HTTPHeadField 模块句柄。
 *
 * @return
 *  创建成功返回 HTTP 头域句柄，否则返回 AK_null。
 */
AK_API AK_Object akae_http_headfield_create (AK_Object Malloc);

/**
 * @brief
 *  销毁 HTTPHeadField 模块句柄。
 *
 * @return
 *  创建成功返回 HTTP 头域句柄，否则返回 AK_null。
 */
AK_API AK_int akae_http_headfield_release (AK_Object Object);
#define akae_http_headfield_release_and_clear(__Object) \
	({\
		AK_int res = akae_http_headfield_release (__Object);\
		if (AK_OK == res) {\
			__Object = AK_null;\
		}\
		res;\
	})


/**
 * 获取当前模块使用的内存分配器。
 */
AK_API AK_Object akae_http_headfield_get_malloc (AK_Object Object);




/**
 * 设置协议。
 */
AK_API AK_int akae_http_headfield_setup_protocol (AK_Object Object, AK_chrcptr proto, AK_int version_major, AK_int version_minor);

/**
 * 获取当前数据结构内协议名称以及版本号。
 */
AK_API AK_int akae_http_headfield_protocol (AK_Object Object, AK_chrptr proto, AK_size protolen, AK_int *version_major, AK_int *version_minor);



/**
 * 设置成请求数据头。
 */
AK_API AK_int akae_http_headfield_setup_request (AK_Object Object, AK_chrcptr method, AK_chrcptr path, AK_chrcptr query);

/**
 * 获取当前数据结构协议请求方式。
 */
AK_API AK_chrptr akae_http_headfield_get_request_method (AK_Object Object, AK_chrptr stack, AK_size stacklen);


/**
 * 获取当前数据结构请求路径，结果存放在 @ref stack 所指向内存位置，并通过返回值以字符串的方式返回。
 *
 * @param[IN] Object
 *  HTTP 头域对象句柄。
 * @param[OUT] stack
 *  返回结果内存的起始地址。
 * @param[IN] stacklen
 *  返回结构的内存大小。
 *
 * @return
 *  获取成功结果存放在 @ref 所在内存并返回，失败返回 AK_null。
 *
 */
AK_API AK_chrptr akae_http_headfield_get_request_path (AK_Object Object, AK_chrptr stack, AK_size stacklen);

/**
 * 设置当前数据结构请求路径。
 */
AK_API AK_int akae_http_headfield_set_request_path (AK_Object Object, AK_chrcptr path);

/**
 * 设置成回复数据头。
 */
AK_API AK_int akae_http_headfield_setup_response (AK_Object Object, AK_int code, AK_chrcptr reason_phrase);

AK_API AK_int akae_http_headfield_get_response_result (AK_Object Object);

AK_API AK_int akae_http_headfield_add_query (AK_Object Object, AK_chrcptr opt, AK_chrcptr fmt, ...);


AK_API AK_int akae_http_headfield_drop_query (AK_Object Object, AK_chrcptr opt);


AK_API AK_int akae_http_headfield_locate_query (AK_Object Object, AK_int pos, AK_chrptr *opt, AK_chrptr *data);


AK_API AK_chrcptr akae_http_headfield_indexof_query (AK_Object Object, AK_chrcptr opt, AK_chrcptr def);

AK_API AK_size akae_http_headfield_count_query (AK_Object Object);


/**
 * 头域加入选项信息。\n
 * 加入方式有分别有追加和更新两种，通过 @ref append 标识决定．\n
 * 追加方式是在原有对应选项基础上追加数据, 前后数据之间通过分号分隔,\n
 * 如 option: value1 追加 value2 数据之后更新为 option: value1; value2.\n
 * 更新方式是使用新的内容替换原来选项内容.\n
 * 如 option: value1 更新 value2 数据之后更新为 option: value2.\n
 * 用户可根据个人不同选项的特征选择使用对应的方式进行加入选项信息.
 *
 * @param[IN] Object
 *  模块对象实例
 *
 * @param[IN] append
 *  加入方式指定, 当为 AK_true 时表示追加方式, 为 AK_false 时表示更新方式.
 *
 * @param[IN] opt
 *  选项名称, 如 Content-Length, Content-Type, Server 等.
 *
 * @param[IN] fmt
 *  选项变量, 格式方式输入.
 *
 * @retval AK_OK
 *  操作成功, 选项对应的数值已经被更新.
 * @retval AK_ERR_INVAL_OBJECT
 *  操作失败, 可能是传入错误的句柄.
 * @retval AK_ERR_INVAL_PARAM
 *  操作失败, 可能是传入错误的参数.
 *
 */
AK_API AK_int akae_http_headfield_add_option (AK_Object Object, AK_boolean append, AK_chrcptr opt, AK_chrcptr fmt, ...);

/**
 * 见 @ref akae_http_headfield_add_option().\n
 * 更新模式的快速宏调用.
 */
#define akae_http_headfield_update_option(__Object, __opt, __fmt...) \
		akae_http_headfield_add_option(__Object, AK_false, __opt, ##__fmt)

/**
 * 见 @ref akae_http_headfield_add_option().\n
 * 追加模式的快速宏调用.
 */
#define akae_http_headfield_append_option(__Object, __opt, __fmt...) \
		akae_http_headfield_add_option(__Object, AK_true, __opt, ##__fmt)


/**
 * 移除选项信息.\n
 * 注: 当传入 @ref opt 选项不存在时, 该接口仍然返回成功.
 *
 * @param[IN] Object
 *  模块对象实例
 *
 * @param[IN] opt
 *  选项名称, 如 Content-Length, Content-Type, Server 等.
 *
 * @retval AK_OK
 *  操作成功, 选项对应的数值已经被更新.
 * @retval AK_ERR_INVAL_OBJECT
 *  操作失败, 可能是传入错误的句柄.
 * @retval AK_ERR_INVAL_PARAM
 *  操作失败, 可能是传入错误的参数.
 *
 */
AK_API AK_int akae_http_headfield_drop_option (AK_Object Object, AK_chrptr opt);

/**
 * 索引选项信息。
 *
 * @param[IN] Object
 *  模块对象实例
 *
 * @param[IN] opt
 *  选项名称, 如 Content-Length, Content-Type, Server 等.
 *
 * @param[IN] def
 *  缺省数值, 当选项不存在时, 会返回缺省数值作为结果.
 *
 * @return
 *  返回选项数值, 如果选项不存在则返回 @ref def, 如果传入非法参数则返回 AK_null.
 *
 */
AK_API AK_chrptr akae_http_headfield_indexof_option (AK_Object Object, AK_chrcptr opt, AK_chrcptr def);


/**
 * 通过加入先后的次序索引选项信息。一般该方法用于遍历所有的选项爱那个信息.\n
 * 索引成功以后通过 @ref opt 与 @data 参数返回结果.
 *
 * @param[IN] Object
 *  模块对象实例
 *
 * @param[IN] pos
 *  选项加入次序, 从 0 开始, 第一个加入的选项是 0, 第二个是 1, 如此类推.\n
 *  可以传入负数作反向索引, 例如传入 -1 则检索倒数第一个.
 *
 * @param[OUT] opt
 *  选项名称返回, 当索引成功以后该指针会指向选项名称.
 *
 * @param[OUT] data
 *  选项名称数值, 当索引成功以后该指针会指向选项数值.
 *
 * @retval AK_OK
 *  操作成功, 可通过 @ref opt 与 @ref data 获取结果.
 * @retval AK_ERR_INVAL_OBJECT
 *  操作失败, 可能是传入错误的句柄.
 * @retval AK_ERR_INVAL_PARAM
 *  操作失败, 可能是传入错误的参数.
 * @retval AK_ERR_OUT_OF_RANGE
 *  操作失败, @ref pos 所在位置不在选项可检索范围内.
 *
 */
AK_API AK_int akae_http_headfield_locate_option (AK_Object Object, AK_int pos, AK_chrptr *opt, AK_chrptr *data);

/**
 * 获取选项的个数.
 *
 * @return
 *  返回选项个数, 注: 如果传入异常句柄, 该接口会返回 0.
 */
AK_API AK_size akae_http_headfield_count_option (AK_Object Object);


/**
 * 序列化头域数据。对对象 @ref Object 的头域数据进行字符串序列化。\n
 * 结果输出到 @ref stack 所在内存位置，长度不超过 @ref stacklen。
 *
 * @return
 *  序列化成功返回字符串有效长度，结果存放在 @ref stack 所在内存位置。
 *
 * @retval AK_ERR_INVAL_OBJECT
 *  操作失败, 可能是传入错误的句柄.
 * @retval AK_ERR_INVAL_PARAM
 *
 */
AK_API AK_ssize akae_http_headfield_stringify (AK_Object Object, AK_chrptr stack, AK_size stacklen);

/**
 * 从字符串中扫描类 HTTP 报文, 并通过接口返回该头域的长度,\n
 * 通过参数返回协议的名称以及版本号.
 *
 * @param[IN] str
 * 	字符串的内存起始位置。
 * @param[IN] len
 * 	字符串 @ref str 的长度。
 * @param[OUT] proto
 * 	协议名称，当扫描发现到报文存在时, 通过该参数返回协议名称。
 * @param[IN] protolen
 * 	协议名称内存最大长度, 如果检测到协议大于该值, 返回结果将按最大长度截断
 * @param[OUT] version_major
 * 	协议主版本号，当扫描发现到报文存在时, 通过该参数返回协议主版本号。
 * @param[OUT] version_minor
 * 	协议主版本号，当扫描发现到报文存在时, 通过该参数返回协议次版本号。
 *
 * @return
 *  当扫描到类 HTTP 报文时, 返回报文长度, 并通过参数返回协议的名称以及版本号.\n
 *  扫描失败时返回对应结果.
 *
 * @retval AK_ERR_INVAL_OBJECT
 *  扫描失败, 传入了非法的对象
 * @retval AK_ERR_INVAL_PARAM
 *  扫描失败, 传入了非法参数
 * @retval AK_ERR_NOT_EXIST
 *  扫描失败, 字符串中没有发现类 HTTP 报文
 *
 */
AK_API AK_ssize akae_http_headfield_scan_string (AK_chrcptr str, AK_size len,
		AK_chrptr proto, AK_size protolen, AK_int *version_major, AK_int *version_minor);


/**
 * 在字符串中尝试指定探测对应协议的类 HTTP 报文,\n
 * 内部会调用 @ref akae_http_headfield_scan_string() 扫描.
 *
 * @param[IN] str
 * 	字符串的内存起始位置。
 * @param[IN] len
 * 	字符串 @ref str 的长度。
 * @param[IN] proto
 * 	指定的协议名称，可传入 AK_null 标识忽略协议匹配。
 *
 * @return
 *  在字符串内探测到与 @ref proto 匹配的协议返回 AK_true, 探测不到以及i其余的异常均返回 AK_false.。
 */
AK_API AK_boolean akae_http_headfield_probe_string (AK_chrcptr str, AK_size len, AK_chrcptr proto);



/**
 * 从字符串中解析类 HTTP 头数据并创建对象实例.\n
 * 内部会调用 @ref akae_http_headfield_scan_string() 和 @ref akae_http_headfield_create() 创建一个对象实例.\n
 * 创建出来的对象通过 @ref akae_http_headfield_release() 是放.
 * 解析时候会尝试区匹配 @ref protocol 协议名称，如果协议名称传入 AK_null 则不指定协议解析，\n
 * 如果协议名称不匹配，接口会返回失败。
 *
 * @param[IN] Malloc
 *  用于分配对象实例内存的分配器
 *
 * @param[IN] str
 *  字符串内存起始地址.
 * @param[IN] len
 *  字符串长度.
 * @param[IN] proto
 *  指定解析协议名称, 如果指定协议不匹配则返回失败, 协议名称大小写不敏感.
 * @param[OUT] fieldlen
 *  创建实例成功返回头域实际长度, 传入 AK_null 不会影响结果, 但用户无法获取头域实际长度.
 *
 * @return
 *  成功解析字符串中的类 HTTP 头域则创建对象实例并返回.
 *
 */
AK_API AK_Object akae_http_headfield_parse_string (AK_Object Malloc, AK_chrcptr str, AK_size len, AK_chrcptr proto, AK_size *fieldlen);

/**
 * @macro
 *  加入 Connection 选项标签。
 *
 * @param __Object
 *  头域模块对象实例
 * @param __sec
 *  传入 0 表示不使用 Keep-Alive, 即可加入 Connection: close.\n
 *  传入对应秒数表示保持连 @ref __sec 超时.
 * @param __max
 *  保持连接的最大次数, 仅当 @ref __sec 大于 0 时有效.
 */
#define akae_http_headfield_set_connection(__Object, __sec, __max) \
	do {\
		if (__sec > 0) {\
			akae_http_headfield_add_option ((__Object), AK_false, "Connection", "keep-alive");\
			/*akae_http_headfield_add_option ((__Object), AK_false, "Keep-Alive", "timeout=%u, max=%u", __sec, __max);*/\
		} else {\
			akae_http_headfield_add_option ((__Object), AK_false, "Connection", "close");\
			akae_http_headfield_drop_option ((__Object), "Keep-Alive");\
		}\
	} while(0)

/**
 * @macro
 *  加入 Content-Length 选项标签。
 */
#define akae_http_headfield_set_content_length(__Object, __len) \
	akae_http_headfield_add_option ((__Object), AK_false, "Content-Length", "%u", (__len))


/**
 * @macro
 *  获取 Content-Length 选项标签。
 */
#define akae_http_headfield_get_content_length(__Object) \
	akae_atoi(akae_http_headfield_indexof_option ((__Object), "Content-Length", "0"))


/**
 * @macro
 *  加入 Content-Type 选项标签。
 */
#define akae_http_headfield_set_content_type(__Object, __type) \
	akae_http_headfield_add_option ((__Object), AK_false, "Content-Type", "%s", (__type))


/**
 * @macro
 *  获取 Content-Type 选项标签。
 */
#define akae_http_headfield_get_content_type(__Object) \
	akae_http_headfield_indexof_option ((__Object), "Content-Type", akae_http_file_mime (AK_null))

/**
 * @macro
 *  加入 Host 选项标签。
 */
#define akae_http_headfield_set_host(__Object, __host) \
	akae_http_headfield_add_option ((__Object), AK_false, "Host", "%s", __host)


/**
 * @macro
 *  加入 Server 选项标签。
 */
#define akae_http_headfield_set_server(__Object, __server) \
	do {\
		if (AK_null != (__server)) {\
			akae_http_headfield_add_option((__Object), AK_false, "Server", (__server));\
		} else {\
			akae_http_headfield_drop_option ((__Object), "Server");\
		}\
	} while(0)


/**
 * @macro
 *  向头域加入 Content-Encoding 选项标签。
 */
#define akae_http_headfield_set_content_encoding(__Object, __encoding) \
	do {\
		if (AK_null != (__encoding)) {\
			akae_http_headfield_add_option((__Object), AK_false, "Content-Encodeing", "%s", (__encoding));\
		} else {\
			akae_http_headfield_drop_option ((__Object), "Content-Encodeing");\
		}\
	} while(0)


/**
 * @macro
 *  向头域加入 Content-Disposition 选项标签，重定义对端下载文件名称。
 */
#define akae_http_headfield_set_content_disposition(__Object, __filename) \
	do {\
		if (AK_null != (__filename)) {\
			akae_http_headfield_add_option((__Object), AK_false, "Content-Disposition", "inline;filename=%s", (__filename));\
		} else {\
			akae_http_headfield_drop_option ((__Object), "Content-Disposition");\
		}\
	} while(0)



AK_C_HEADER_EXTERN_C_END
#endif

