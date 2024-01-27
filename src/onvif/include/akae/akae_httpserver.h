/**
 * HTTP 协议基础工具集定义。
 *
 */

#include <akae_typedef.h>
#include <akae_httputils.h>
#include <akae_verbose.h>
#include <akae_tosser.h>


#if !defined(AKAE_HTTPSERVER_H_)
# define AKAE_HTTPSERVER_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * 服务器最大能处理的 HTTP 头大小，包括请求与回复。
 */
#define AK_HTTP_SERVER_MAX_HEADFIELD_LEN   (1024 * 8)

/**
 * 服务器支持解析最大 URL 地址长度.
 */
#define AK_HTTP_SERVER_MAX_URL_LEN         (256)

/**
 * 服务器保持链接等待时间。
 */
#define AK_HTTP_SERVER_KEEP_ALIVE_SEC      (16)

/**
 * 服务器保持链接次数。
 */
#define AK_HTTP_SERVER_KEEP_ALIVE_MAX      (128)





/**
 * @brief
 *  HTTP 服务器配置数据结构。
 */
#pragma pack (push, 4)
typedef struct _AK_HttpServerProperty {

	/**
	 * 资源可写标识，缺省为 AK_false。
	 */
	AK_boolean resource_writables;

	/**
	 * 使用 Keep-Alive 保留时间,\n
	 * 如果该属性为 0 则表示不使用 Keep-Alive.\n
	 * 在 HTTP1.1 协议中缺省打开, 数值为 60, 即默认进行 60 秒超时 Keep-Alive.
	 */
	AK_size keepalive_sec;

	/**
	 * Keep-Alive 等待次数，\n
	 * 当 @ref keepalive_sec 大于 0 此参数有效，\n
	 */
	AK_size keepalive_max;

	AK_size maxAttachedCache;

} AK_HttpServerProperty;


typedef struct _AK_HttpServerSession {

	/// 服务器使用到的内存分配器。
	AK_Object Malloc;

	/// TCP 链接套接字
	AK_Socket tcp;

	/// Keep-Alive 参数。
	struct {

		/// 保留会话的剩余秒数。
		AK_ssize seconds;

		/// 保留会话的最大次数。
		AK_ssize max;

	} Keepalive;

	/**
	 * 当前 HTTP 会话校验用户。
	 */
//	NK_HTTPAccessUsers *AccessUsers;

	struct {

		/// 请求头域报文原始内容。
		AK_char headfield[AK_HTTP_SERVER_MAX_HEADFIELD_LEN + 1];

		/// 请求头域报文有效长度。
		AK_ssize headfieldlen;

		/// 请求头域数据结构。
		AK_Object Headfield;

		/// 请求数据内容长度。
		AK_size content_length;

	} Request;

	struct {

		/**
		 * 回复报文头域数据结构。
		 */
		AK_Object Headfield;

		/**
		 * 回复数据内容。
		 */
		struct {

			/**
			 * 服务器回复数据区起始地址。\n
			 * @ref NK_HTTP_RESP_PUT_DATA() 等宏调用会影响该数值。
			 */
			AK_bytptr data;

			/**
			 * 回复数据为引用数据时的数据内存起始地址。
			 * @ref NK_HTTP_RESP_PUT_DATA_REF() 等宏调用会影响该数值。
			 */
			AK_bytptr dataRef;

			/**
			 * 服务器回复数据长度，及缓冲最大长度。\n
			 */
			AK_size datalen, capacity;

			/**
			 * 服务器回复的文件，\n
			 * @ref NK_HTTP_RESP_PUT_FILE() 宏调用会影响该数值。
			 */
			AK_chrptr datafile;

		} Content;

//		/**
//		 * OPTIONS 请求回复标识。
//		 */
//		NK_UInt32 optionsAllow;

	} Response;

} AK_HttpServerSession;
#pragma pack (pop)


/**
 * 服务器 CGI 用户事件定义.
 *
 * @param[IN] Session
 *  HTTP 服务器会话上下文.
 *
 * @param[IN] th
 *  HTTP 服务器线程会话句柄
 *
 * @param[IN] sock
 *  HTTP 服务器 TCP 套接字句柄
 *
 * @param[IN] userdata
 *  用户数据, 在 @ref akae_http_server_add_cgi() 或 @ref akae_http_server_add_cgi_prefix() 传入.
 *
 */
typedef AK_void (*AK_HttpServerCgiHandle)(AK_HttpServerSession *Session, AK_Thread th, AK_voidptr userdata);


/**
 * 创建 HTTP 服务器
 *
 * @param[IN] Malloc
 *  创建 HTTP 服务器内存分配器, 该内存分配器必须支持线程安全. 否则会创建失败.
 *
 * @return
 *  创建成功返回模块对象, 否则返回 AK_null.
 */
AK_API AK_Object akae_http_server_create (AK_Object Malloc);


/**
 * 释放 HTTP 服务器对象句柄.
 *
 * @param[IN] Object
 *  HTTP 服务器对象模块句柄.
 *
 * @retval AK_OK
 *  释放成功.
 * @retval AK_ERR_INVAL_OBJECT
 *  释放失败, 可能是模块对象句柄异常.
 */
AK_API AK_int akae_http_server_release (AK_Object Object);

/**
 * 开启/关闭调试打印信息。
 */
AK_API AK_int akae_http_server_verbose (AK_Object Object, AK_boolean enabled);

/**
 * 设置请求路径别名.
 */
AK_API AK_int akae_http_server_alias_path (AK_Object Object, AK_chrcptr origin_path, AK_chrcptr alias_path);

AK_API AK_int akae_http_server_redirect_path (AK_Object Object, AK_chrcptr origin_path, AK_chrcptr redirect_path);

/**
 * HTTP 服务器句柄绑定 Tosser 模块工作.\n
 * HTTP 服务器可以绑定 Tosser 模块监听, 也可以自己独立监听, 见 @ref akae_http_server_start()
 *
 * @param[IN] Object
 *  HTTP 服务器对象模块句柄
 * @param[IN] Tosser
 *  Tosser 对象模块句柄.
 *
 * @retval AK_OK
 *  操作成功.
 * @retval AK_ERR_INVAL_OBJECT
 *  操作失败, 句柄异常
 *
 */
AK_API AK_int akae_http_server_bind_tosser (AK_Object Object, AK_Object Tosser);

/**
 * 解除 HTTP 服务器绑定
 *
 * @param[IN] Object
 *  HTTP 服务器对象模块句柄
 *
 * @retval AK_OK
 *  操作成功.
 * @retval AK_ERR_INVAL_OBJECT
 *  操作失败, 句柄异常
 */
AK_API AK_int akae_http_server_unbind_tosser (AK_Object Object);

AK_API AK_int akae_http_server_start (AK_Object Object, AK_uint16 port);

AK_API AK_int akae_http_server_finish (AK_Object Object);


AK_API AK_int akae_http_server_add_cgi (AK_Object Object, AK_chrcptr path, AK_HttpServerCgiHandle CgiHandle, AK_voidptr userdata);

AK_API AK_int akae_http_server_add_cgi_prefix (AK_Object Object, AK_chrcptr path_prefix, AK_HttpServerCgiHandle CgiHandle, AK_voidptr userdata);

AK_API AK_int akae_http_server_drop_cgi (AK_Object Object, AK_chrcptr path);

AK_API AK_int akae_http_server_set_resource_fspath (AK_Object Object, AK_chrcptr fspath);

AK_API AK_chrptr akae_http_server_get_resource_fspath (AK_Object Object, AK_chrptr stack, AK_size stacklen);





AK_API AK_void akae_http_server_session_response_disable_keepalive (AK_HttpServerSession *Session);

AK_API AK_void akae_http_server_session_response_put_result (AK_HttpServerSession *Session, AK_int status, AK_chrcptr phrase);

#define akae_http_server_session_response_put_option(__Session, __opt, __fmt...) \
	({\
		if (!(__Session)->Response.Headfield) {\
			akae_http_server_session_response_put_result (__Session, AK_HTTP_CODE_OK, AK_null);\
		}\
		akae_http_headfield_add_option ((__Session)->Response.Headfield, AK_false, __opt, ##__fmt);\
	})

/**
 * 更新 HTTP 服务器会话上下文的回复内容。
 *
 * @param[IN] Session
 *  会话上下文。
 * @param[IN] data
 *  HTTP 回复内容数据所在内存起始地址。
 * @param[IN] datalen
 *  HTTP 回复内容数据长度。
 *
 */
AK_API AK_void akae_http_server_session_response_put_content (AK_HttpServerSession *Session, AK_voidcptr data, AK_size datalen);

AK_API AK_void akae_http_server_session_response_put_file (AK_HttpServerSession *Session, AK_chrcptr filepath);


AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_HTTPSERVER_H_

