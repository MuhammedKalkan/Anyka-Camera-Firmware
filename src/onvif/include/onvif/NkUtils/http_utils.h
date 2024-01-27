/**
 * HTTP Э��������߼����塣
 *
 */

#include <NkUtils/types.h>
#include <NkUtils/allocator.h>
//#include <NkUtils/str_list.h>
#include <NkUtils/assert.h>

#ifndef NK_HTTP_UTILS_H_
# define NK_HTTP_UTILS_H_
NK_CPP_EXTERN_BEGIN


/**
 * HTTP ���з����塣
 */
#define NK_HTTP_CRLF "\r\n"

/**
 * HTTP �ظ��붨�塣
 */
typedef enum Nk_HTTPCode
{
	NK_HTTP_CODE_CONTINUE = (100),                       //!< NK_HTTP_CODE_CONTINUE
	NK_HTTP_CODE_SWITCHING_PROTOCOLS = (101),            //!< NK_HTTP_CODE_SWITCHING_PROTOCOLS
	NK_HTTP_CODE_OK = (200),                             //!< NK_HTTP_CODE_OK
	NK_HTTP_CODE_CREATED = (201),                        //!< NK_HTTP_CODE_CREATED
	NK_HTTP_CODE_ACCEPTED = (202),                       //!< NK_HTTP_CODE_ACCEPTED
	NK_HTTP_CODE_NON_AUTHORITATIVE_INFOMATION = (203),   //!< NK_HTTP_CODE_NON_AUTHORITATIVE_INFOMATION
	NK_HTTP_CODE_NO_CONTENT = (204),                     //!< NK_HTTP_CODE_NO_CONTENT
	NK_HTTP_CODE_RESET_CONTENT = (205),                  //!< NK_HTTP_CODE_RESET_CONTENT
	NK_HTTP_CODE_PARTIAL_CONTENT = (206),                //!< NK_HTTP_CODE_PARTIAL_CONTENT
	NK_HTTP_CODE_MULTIPLE_CHOICES = (300),               //!< NK_HTTP_CODE_MULTIPLE_CHOICES
	NK_HTTP_CODE_MOVED_PERMANENTLY = (301),              //!< NK_HTTP_CODE_MOVED_PERMANENTLY
	NK_HTTP_CODE_FOUND = (302),                          //!< NK_HTTP_CODE_FOUND
	NK_HTTP_CODE_SEE_OTHER = (303),                      //!< NK_HTTP_CODE_SEE_OTHER
	NK_HTTP_CODE_NOT_MODIFIED = (304),                   //!< NK_HTTP_CODE_NOT_MODIFIED
	NK_HTTP_CODE_USE_PROXY = (305),                      //!< NK_HTTP_CODE_USE_PROXY
	NK_HTTP_CODE_TEMPORARY_REDIRECT = (307),             //!< NK_HTTP_CODE_TEMPORARY_REDIRECT
	NK_HTTP_CODE_BAD_REQUEST = (400),                    //!< NK_HTTP_CODE_BAD_REQUEST
	NK_HTTP_CODE_UNAUTHORIZED = (401),                   //!< NK_HTTP_CODE_UNAUTHORIZED
	NK_HTTP_CODE_PAYMENT_REQUIRED = (402),               //!< NK_HTTP_CODE_PAYMENT_REQUIRED
	NK_HTTP_CODE_FORBIDDEN = (403),                      //!< NK_HTTP_CODE_FORBIDDEN
	NK_HTTP_CODE_NOT_FOUND = (404),                      //!< NK_HTTP_CODE_NOT_FOUND
	NK_HTTP_CODE_METHOD_NOT_ALLOWED = (405),             //!< NK_HTTP_CODE_METHOD_NOT_ALLOWED
	NK_HTTP_CODE_NOT_ACCEPTABLE = (406),                 //!< NK_HTTP_CODE_NOT_ACCEPTABLE
	NK_HTTP_CODE_PROXY_AUTHENTICATION_REQUIRED = (407),  //!< NK_HTTP_CODE_PROXY_AUTHENTICATION_REQUIRED
	NK_HTTP_CODE_REQUEST_TIME_OUT = (408),               //!< NK_HTTP_CODE_REQUEST_TIME_OUT
	NK_HTTP_CODE_CONFLICT = (409),                       //!< NK_HTTP_CODE_CONFLICT
	NK_HTTP_CODE_GONE = (410),                           //!< NK_HTTP_CODE_GONE
	NK_HTTP_CODE_LENGTH_REQUIRED = (411),                //!< NK_HTTP_CODE_LENGTH_REQUIRED
	NK_HTTP_CODE_PRECONDITION_FAILED = (412),            //!< NK_HTTP_CODE_PRECONDITION_FAILED
	NK_HTTP_CODE_REQUEST_ENTITY_TOO_LARGE = (413),       //!< NK_HTTP_CODE_REQUEST_ENTITY_TOO_LARGE
	NK_HTTP_CODE_REQUEST_URI_TOO_LARGE = (414),          //!< NK_HTTP_CODE_REQUEST_URI_TOO_LARGE
	NK_HTTP_CODE_UNSUPPORTED_MEDIA_TYPE = (415),         //!< NK_HTTP_CODE_UNSUPPORTED_MEDIA_TYPE
	NK_HTTP_CODE_REQUESTED_RANGE_NOT_SATISFIABLE = (416),//!< NK_HTTP_CODE_REQUESTED_RANGE_NOT_SATISFIABLE
	NK_HTTP_CODE_EXPECTATION_FAILED = (417),             //!< NK_HTTP_CODE_EXPECTATION_FAILED
	NK_HTTP_CODE_INTERNAL_SERVER_ERROR = (500),          //!< NK_HTTP_CODE_INTERNAL_SERVER_ERROR
	NK_HTTP_CODE_NOT_IMPLEMENTED = (501),                //!< NK_HTTP_CODE_NOT_IMPLEMENTED
	NK_HTTP_CODE_BAD_GATEWAY = (502),                    //!< NK_HTTP_CODE_BAD_GATEWAY
	NK_HTTP_CODE_SERVICE_UNAVAILABLE = (503),            //!< NK_HTTP_CODE_SERVICE_UNAVAILABLE
	NK_HTTP_CODE_GATEWAY_TIME_OUT = (504),               //!< NK_HTTP_CODE_GATEWAY_TIME_OUT
	NK_HTTP_CODE_HTTP_VERSION_NOT_SUPPORTED = (505),     //!< NK_HTTP_CODE_HTTP_VERSION_NOT_SUPPORTED

} NK_HTTPCode;


/**
 * ͨ���ļ����ƻ�ȡ�ļ��� MIME��\n
 * �ӿڸ����ļ���׺��ȡ MIME������ļ����Ʊ�����к�׺���� file.txt��
 *
 */
extern NK_PChar
NK_HTTPUtils_FileMIME(NK_PChar file_name);


/**
 * ��ȡ HTTP �������Ӧ��Ĭ����Ϣ��
 *
 * @param[in]		code		HTTP �����롣
 *
 * @return	�������Ӧ��Ϣ��
 */
extern NK_PChar
NK_HTTPUtils_ReasonPhrase(NK_UInt32 code);

/**
 * ���� URI �ַ�����\n
 * ��֧�� UTF-8 ����ת����
 *
 * @param[in]			uri				URI �ı���
 * @param[out]			enc				���� URI �ı����壬������ɹ�ʱ�Ӵ˻����л�ȡ URI �����ı���
 * @param[in,out]		enc_len			���� URI �����ջ�����ȣ�������ɹ�ʱ�Ӵ˷��� URI �����ı����ȡ�
 *
 * @return		����ɹ����� 0��ʧ�ܷ��� -1��
 */
extern NK_Int
NK_HTTPUtils_EncodeURI(NK_PChar uri, NK_PChar enc, NK_Size *enc_len);

/**
 * ���� URI �ַ�����\n
 * ��֧�� UTF-8 ����ת����\n
 * ����� @ref enc �� @ref uri ����ָ��ͬһ���ڴ�顣
 *
 * @param[in]			enc				����� URI �ı���
 * @param[out]			uri				��ԭ URI �ı����壬������ɹ�ʱ�Ӵ˻����л�ȡ URI �ı���
 * @param[in,out]		uri_len			���� URI �����ջ�����ȣ�������ɹ�ʱ�Ӵ˷��� URI �ı����ȡ�
 *
 * @return		����ɹ����� 0��ʧ�ܷ��� -1��
 */
extern NK_Int
NK_HTTPUtils_DecodeURI(NK_PChar enc, NK_PChar uri, NK_Size *uri_len);

/**
 * URL ���ݽṹ��
 */
typedef struct Nk_HTTPURL
{
	NK_PChar protocol;
	NK_PChar host;
	NK_UInt16 port;
	NK_PChar abs_path;
	NK_PChar query;

	/**
	 * �������塣
	 */
	NK_Byte reserved[1024 * 5];
} NK_HTTPURL;

/**
 * ��ӡ @ref NK_HTTPURL ���ݽṹ��
 */
#define NK_HTTP_URL_DUMP(__URL) \
	do{\
		NK_TermTable Tbl;\
		NK_TermTbl_BeginDraw(&Tbl, "URL", 96, 4);\
		NK_TermTbl_PutKeyValue(&Tbl, NK_True, "Protocol", "%s", (__URL)->protocol);\
		NK_TermTbl_PutKeyValue(&Tbl, NK_True, "Host", "%s", (__URL)->host);\
		NK_TermTbl_PutKeyValue(&Tbl, NK_True, "Port", "%d", (NK_Int)((__URL)->port));\
		NK_TermTbl_PutKeyValue(&Tbl, NK_True, "Absolute Path", "%s", (__URL)->abs_path);\
		if (NK_Nil != (__URL)->query) {\
			NK_TermTbl_PutKeyValue(&Tbl, NK_True, "Query String", "%s", (__URL)->query);\
		}\
		NK_TermTbl_EndDraw(&Tbl);\
	} while(0)


/**
 * ���� URL �ַ�����
 */
extern NK_Int
NK_HTTPUtils_ParseURL(NK_PChar url, NK_HTTPURL *URL);


/**
 * HTTPHeadField ģ������
 */
typedef struct Nk_HTTPHeadField {
#define THIS struct Nk_HTTPHeadField *const

	/**
	 * ģ������ӿڡ�
	 */
	NK_Object Object;

	/**
	 * ����/�ظ���ʶ��
	 */
	NK_Boolean isRequest;

	/**
	 * Э�顣
	 */
	NK_PChar protocol;

	/**
	 * �汾�š�
	 */
	NK_UInt32 ver_maj, ver_min;

	/**
	 * �� isRequest Ϊ True ʱ��Ч��
	 */
	struct {
		NK_PChar method;
		NK_PChar abs_path;
		NK_PChar query;
	};
	/**
	 * �� isRequest Ϊ False ʱ��Ч��
	 */
	struct {
		NK_UInt32 code;
		NK_PChar reason_phrase;
	};

	/**
	 * ����Э�顣
	 */
	NK_Int
	(*setProtocol)(THIS, NK_PChar protocol, NK_Int ver_maj, NK_Int ver_min);

	/**
	 * ���ó���������ͷ��
	 */
	NK_Int
	(*setRequest)(THIS, NK_PChar method, NK_PChar abs_path, NK_PChar query);

	/**
	 * ���óɻظ�����ͷ��
	 */
	NK_Int
	(*setResponse)(THIS, NK_UInt32 status_code, NK_PChar reason_phrase);

	/**
	 * ͷ������ѯѶϢ��\n
	 * ����ģʽʱ��Ч�����򷵻� -1��
	 *
	 */
	NK_Int
	(*addQuery)(THIS, NK_PChar key, NK_PChar fmt, ...);

	/**
	 * ͷ������ѯѶϢ��\n
	 * ����ģʽʱ��Ч�����򷵻� -1��
	 *
	 * @return		�ؼ��ֲ����ڻ���ɾ���ɹ����� 0����������ʱ���� -1��
	 */
	NK_Int
	(*dropQuery)(THIS, NK_PChar key);

	/**
	 * ��ȡ Query ��������
	 */
	NK_Size
	(*numberOfQuery)(THIS);

	/**
	 * ͨ����Ż�ȡ Query ��Ϣ��
	 *
	 * @param[in]			id				Query ����������š�
	 * @param[out]			key				Query �������ڹؼ��֡�
	 * @param[out]			value			Query �����ؼ��ֶ�Ӧ��ֵ��
	 *
	 * @return		��ȡ�ɹ����� 0�����򷵻� -1��
	 */
	NK_Int
	(*indexOfQuery)(THIS, NK_Int id, NK_PChar *key, NK_PChar *value);

	/**
	 * ��ȡͷ���ѯѶϢ��\n
	 * ����ģʽʱ��Ч�����򷵻� -1��
	 *
	 */
	NK_PChar
	(*getQuery)(THIS, NK_PChar key, NK_PChar def);

	/**
	 * ��ȡͷ��ĳһ��ѯѶϢ�ĸ�����
	 *
	 */
	NK_Boolean
	(*hasQuery)(THIS, NK_PChar key);

	/**
	 * ͷ�����ѡ����Ϣ��
	 *
	 */
	NK_Int
	(*addOption)(THIS, NK_Boolean overwrite, NK_PChar opt, NK_PChar fmt, ...);

	/**
	 * ͷ�����ѡ����Ϣ��
	 *
	 * @return		�ؼ��ֲ����ڻ���ɾ���ɹ����� 0����������ʱ���� -1��
	 */
	NK_Int
	(*dropOption)(THIS, NK_Boolean all, NK_PChar opt);

	/**
	 * ��ȡ��ǩ������
	 *
	 * @return		��ǩѡ���������
	 */
	NK_Size
	(*numberOfOption)(THIS);

	/**
	 * ͨ����ǩ��Ż�ȡ��ǩ���ƺ���Ϣ��
	 *
	 * @param[in]			id				��ǩѡ����š�
	 * @param[out]			key				��Ŷ�Ӧ�±�ǩ���ơ�
	 * @param[out]			value			��Ŷ�Ӧ�±�ǩ��Ϣ��
	 *
	 * @return		��ȡ�ɹ����� 0���� @ref opt �� @ref value �л�ȡ��ǩѡ������
	 */
	NK_Int
	(*indexOfOption)(THIS, NK_Int id, NK_PChar *key, NK_PChar *value);


	/**
	 * ͨ����ǩѡ�����ƻ�ȡ����Ϣ��
	 *
	 * @param[in]			key				��ǩѡ������ơ�
	 * @param[in]			def				Ĭ�ϱ�ǩѡ�����ֵ���� @ref opt �����ڵ�ʱ���ʹ��Ĭ��ֵ��Ϊ���أ����Դ��� Nil��
	 *
	 * @return		���ر�ǩѡ�����ֵ������ǩ�����ڵ�ʱ��᷵�� @ref def Ĭ��ֵ��
	 */
	NK_PChar
	(*getOption)(THIS, NK_PChar key, NK_PChar def);

	/**
	 * ��ȡͷ��ĳһѡ��ĸ�����
	 *
	 */
	NK_SSize
	(*hasOption)(THIS, NK_PChar opt);

	/**
	 * �����ı����ݡ�
	 *
	 */
	NK_Int
	(*toText)(THIS, NK_PChar text, NK_Size *text_len);

#undef THIS
} NK_HTTPHeadField;

/**
 * ��ӡ������ݽṹ NK_HTTPHeadField��
 */
#define NK_HTTP_HEAD_FIELD_DUMP(__HeadField) \
	do{\
		NK_TermTable Table;\
		NK_Size number = 0;\
		NK_PChar key = NK_Nil;\
		NK_PChar value = NK_Nil;\
		NK_Int i = 0;\
		\
		NK_CHECK_POINT();\
		NK_TermTbl_BeginDraw(&Table, "HTTP Head Filed", 96, 4);\
		NK_TermTbl_PutKeyValue(&Table, NK_True, "Protocol", "%s %u.%u",\
				(__HeadField)->protocol, (__HeadField)->ver_maj, (__HeadField)->ver_min);\
		if ((__HeadField)->isRequest) {\
			NK_TermTbl_PutKeyValue(&Table, NK_False, "Method", "%s", (__HeadField)->method);\
			NK_TermTbl_PutKeyValue(&Table, NK_True, "Absolute Path", "%s", (__HeadField)->abs_path);\
			number = (__HeadField)->numberOfQuery((__HeadField));\
			NK_TermTbl_PutText(&Table, NK_True, "Query String (%u)", number);\
			for (i = 0; i < number; ++i) {\
				if (0 == (__HeadField)->indexOfQuery((__HeadField), i, &key, &value)) {\
					NK_TermTbl_PutKeyValue(&Table, (i == number - 1), key, "%s", value);\
				}\
			}\
		} else {\
			NK_TermTbl_PutKeyValue(&Table, NK_False, "Code", "%u", (__HeadField)->code);\
			NK_TermTbl_PutKeyValue(&Table, NK_True, "Reason Phrase", "%s", (__HeadField)->reason_phrase);\
		}\
		\
		number = (__HeadField)->numberOfOption((__HeadField));\
		NK_TermTbl_PutText(&Table, NK_True, "Option Tag (%u)", number);\
		for (i = 0; i < number; ++i) {\
			if (0 == (__HeadField)->indexOfOption((__HeadField), i, &key, &value)) {\
				NK_TermTbl_PutKeyValue(&Table, (i == number - 1), key, "%s", value);\
			}\
		}\
		NK_TermTbl_EndDraw(&Table);\
	} while(0)



/**
 * ���� Host ѡ���ǩ��
 */
#define NK_HTTP_HEAD_FIELD_HOST(__HeadField, __str_host) \
	do {\
		if (NK_Nil != (__HeadField)) {\
			(__HeadField)->dropOption((__HeadField), NK_True, "Host");\
			(__HeadField)->addOption((__HeadField), NK_False, "Host", "%s", __str_host);\
		}\
	} while(0)

/**
 * ���� Server ѡ���ǩ��
 */
#define NK_HTTP_HEAD_FIELD_SERVER(__HeadField, __str_server, __uint_ver1, __uint_ver2) \
	do {\
		if (NK_Nil != (__HeadField)) {\
			(__HeadField)->dropOption((__HeadField), NK_True, "Server");\
			(__HeadField)->addOption((__HeadField), NK_False, "Server", "%s / %u.%u", (__str_server), (__uint_ver1), (__uint_ver2));\
		}\
	} while(0)

/**
 * ���� Content-Type ѡ���ǩ��
 */
#define NK_HTTP_HEAD_FIELD_CONTENT_TYPE(__HeadField, __str_type) \
	do {\
		if (NK_Nil != (__HeadField)) {\
			(__HeadField)->dropOption((__HeadField), NK_True, "Content-Type");\
			(__HeadField)->addOption((__HeadField), NK_False, "Content-Type", "%s", (__str_type));\
		}\
	} while(0)

/**
 * ���� Content-Length ѡ���ǩ��
 */
#define NK_HTTP_HEAD_FIELD_CONTENT_LENGTH(__HeadField, __uint_len) \
	do {\
		if (NK_Nil != (__HeadField)) {\
			(__HeadField)->dropOption((__HeadField), NK_True, "Content-Length");\
			(__HeadField)->addOption((__HeadField), NK_False, "Content-Length", "%u", (__uint_len));\
		}\
	} while(0)

/**
 * ���� Content-Length ѡ���ǩ��
 */
#define NK_HTTP_HEAD_FIELD_CONNECTION(__HeadField, __uint_alive_s) \
	do {\
		if (NK_Nil != (__HeadField)) {\
			(__HeadField)->dropOption((__HeadField), NK_True, "Connection");\
			(__HeadField)->dropOption((__HeadField), NK_True, "Keep-Alive");\
			(__HeadField)->addOption((__HeadField), NK_False, "Connection", "%s", (__uint_alive_s > 0) ? "keep-alive" : "close");\
			if (__uint_alive_s > 0) {\
				NK_UInt32 timeout = __uint_alive_s;\
				timeout = timeout < 60U ? timeout : 60U;\
				(__HeadField)->addOption((__HeadField), NK_False, "Keep-Alive", "timeout=%u, max=%u", timeout, 60U);\
			}\
		}\
	} while(0)



/**
 * ��ȡ������ HTTP ͷ�Ĵ�С��
 *
 * @param[in]				protocol			Э�����ƣ����� Nil ʱ��Ĭ�ϳ� HTTP��
 * @param[in]				package				���ڷ��������ݰ���
 * @param[in,out]			pack_size			���� @package ���ݰ����ȣ����ؽ�������ͷ���С��
 *
 * @return		�������Ϊ HTTP ͷ���� HTTP ͷ�ĳ��ȣ���������в����� HTTP ͷ�򷵻� -1��
 */
extern NK_Int
NK_HTTPUtils_ExtractHeadField(NK_PChar protocol, NK_PChar package, NK_Size *pack_size);

/**
 * �ӱ����н��� HTTP ͷ�����ݽṹ����ȡ�����\n
 * �ڲ������ @ref NK_HTTPUtils_ExtractHeadField() �� @ref NK_HTTPUtils_CreateHeadField()��\n
 * ����ʱ��᳢����ƥ�� @ref protocol Э�����ƣ����Э�����ƴ��� Nil ��Ĭ��Ϊ HTTP��\n
 * ���Э�����Ʋ�ƥ�䣬�ӿڻ᷵��ʧ�ܡ�
 *
 */
extern NK_HTTPHeadField *
NK_HTTPUtils_ParseHeadField(NK_Allocator *Alloctr, NK_PChar protocol, NK_PChar package, NK_Size *len);

/**
 * ���� HTTPHeadField ģ������
 */
extern NK_HTTPHeadField *
NK_HTTPUtils_CreateHeadField(NK_Allocator *Alloctr, NK_PChar protocol, NK_UInt32 ver_maj, NK_UInt32 ver_min);

/**
 * ���� HTTPHeadField ģ������
 */
extern NK_Int
NK_HTTPUtils_FreeHeadField(NK_HTTPHeadField **Field_r);



NK_CPP_EXTERN_END
#endif /* NK_HTTP_UTILS_H_ */

