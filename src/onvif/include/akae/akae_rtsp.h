/**
 * RTSP 协议基本数据类型定义。
 *
 */

#include <akae_typedef.h>
#include <akae_rtp.h>
#include <akae_rtcp.h>


#if !defined(AKAE_RTSP_H_)
# define AKAE_RTSP_H_
AK_C_HEADER_EXTERN_C_BEGIN


#define AK_RTSP_CODE_CONTINUE                              (100)
#define AK_RTSP_CODE_OK                                    (200)
#define AK_RTSP_CODE_CREATED                               (201)
#define AK_RTSP_CODE_LOW_ON_STORAGE_SPACE                  (250)
#define AK_RTSP_CODE_MULTIPLE_CHOICES                      (300)
#define AK_RTSP_CODE_MOVED_PERMANENTLY                     (301)
#define AK_RTSP_CODE_MOVED_TEMPORARILY                     (302)
#define AK_RTSP_CODE_SEE_OTHER                             (303)
#define AK_RTSP_CODE_NOT_MODIFIED                          (304)
#define AK_RTSP_CODE_USE_PROXY                             (305)
#define AK_RTSP_CODE_BAD_REQUEST                           (400)
#define AK_RTSP_CODE_UNAUTHORIZED                          (401)
#define AK_RTSP_CODE_PAYMENT_REQUIRED                      (402)
#define AK_RTSP_CODE_FORBIDDEN                             (403)
#define AK_RTSP_CODE_NOT_FOUND                             (404)
#define AK_RTSP_CODE_METHOD_NOT_ALLOWED                    (405)
#define AK_RTSP_CODE_NOT_ACCEPTABLE                        (406)
#define AK_RTSP_CODE_PROXY_AUTHENTICATION_REQUIRED         (407)
#define AK_RTSP_CODE_REQUEST_TIME_OUT                      (408)
#define AK_RTSP_CODE_GONE                                  (410)
#define AK_RTSP_CODE_LENGTH_REQUIRED                       (411)
#define AK_RTSP_CODE_PRECONDITION_FAILED                   (412)
#define AK_RTSP_CODE_REQUEST_ENTITY_TOO_LARGE              (413)
#define AK_RTSP_CODE_REQUEST_URI_TOO_LARGE                 (414)
#define AK_RTSP_CODE_UNSUPPORTED_MEDIA_TYPE                (415)
#define AK_RTSP_CODE_PARAMETER_NOT_UNDERSTOOD              (451)
#define AK_RTSP_CODE_CONFERENCE_NOT_FOUND                  (452)
#define AK_RTSP_CODE_NOT_ENOUGH_BANDWIDTH                  (453)
#define AK_RTSP_CODE_SESSION_NOT_FOUND                     (454)
#define AK_RTSP_CODE_METHOD_NOT_VALID_IN_THIS_STATE        (455)
#define AK_RTSP_CODE_HEADER_FIELD_NOT_VALID_FOR_RESOURCE   (456)
#define AK_RTSP_CODE_INVALID_RANGE                         (457)
#define AK_RTSP_CODE_PARAMETER_IS_READ_ONLY                (458)
#define AK_RTSP_CODE_AGGREGATE_OPERATION_NOT_ALLOWED       (459)
#define AK_RTSP_CODE_ONLY_AGGREGATE_OPERATION_ALLOWED      (460)
#define AK_RTSP_CODE_UNSUPPORTED_TRANSPORT                 (461)
#define AK_RTSP_CODE_DESTINATION_UNREACHABLE               (462)
#define AK_RTSP_CODE_INTERNAL_SERVER_ERROR                 (500)
#define AK_RTSP_CODE_NOT_IMPLEMENTED                       (501)
#define AK_RTSP_CODE_BAD_GATEWAY                           (502)
#define AK_RTSP_CODE_SERVICE_UNAVAILABLE                   (503)
#define AK_RTSP_CODE_GATEWAY_TIME_OUT                      (504)
#define AK_RTSP_CODE_RTSP_VERSION_NOT_SUPPORTED            (505)
#define AK_RTSP_CODE_OPTION_NOT_SUPPORTED                  (551)

#define AK_RTSP_RESPONSE_REASON_PHRASE(__code) \
	(\
		(AK_RTSP_CODE_CONTINUE                              ) == (__code) ? "Continue" :\
		(AK_RTSP_CODE_OK                                    ) == (__code) ? "OK" :\
		(AK_RTSP_CODE_CREATED                               ) == (__code) ? "Created" :\
		(AK_RTSP_CODE_LOW_ON_STORAGE_SPACE                  ) == (__code) ? "Low on Storage Space" :\
		(AK_RTSP_CODE_MULTIPLE_CHOICES                      ) == (__code) ? "Multiple Choices" :\
		(AK_RTSP_CODE_MOVED_PERMANENTLY                     ) == (__code) ? "Moved Permanently" :\
		(AK_RTSP_CODE_MOVED_TEMPORARILY                     ) == (__code) ? "Moved Temporarily" :\
		(AK_RTSP_CODE_SEE_OTHER                             ) == (__code) ? "See Other" :\
		(AK_RTSP_CODE_NOT_MODIFIED                          ) == (__code) ? "Not Modified" :\
		(AK_RTSP_CODE_USE_PROXY                             ) == (__code) ? "Use Proxy" :\
		(AK_RTSP_CODE_BAD_REQUEST                           ) == (__code) ? "Bad Request" :\
		(AK_RTSP_CODE_UNAUTHORIZED                          ) == (__code) ? "Unauthorized" :\
		(AK_RTSP_CODE_PAYMENT_REQUIRED                      ) == (__code) ? "Payment Required" :\
		(AK_RTSP_CODE_FORBIDDEN                             ) == (__code) ? "Forbidden" :\
		(AK_RTSP_CODE_NOT_FOUND                             ) == (__code) ? "Not Found" :\
		(AK_RTSP_CODE_METHOD_NOT_ALLOWED                    ) == (__code) ? "Method Not Allowed" :\
		(AK_RTSP_CODE_NOT_ACCEPTABLE                        ) == (__code) ? "Not Acceptable" :\
		(AK_RTSP_CODE_PROXY_AUTHENTICATION_REQUIRED         ) == (__code) ? "Proxy Authentication Required" :\
		(AK_RTSP_CODE_REQUEST_TIME_OUT                      ) == (__code) ? "Request Time-out" :\
		(AK_RTSP_CODE_GONE                                  ) == (__code) ? "Gone" :\
		(AK_RTSP_CODE_LENGTH_REQUIRED                       ) == (__code) ? "Length Required" :\
		(AK_RTSP_CODE_PRECONDITION_FAILED                   ) == (__code) ? "Precondition Failed" :\
		(AK_RTSP_CODE_REQUEST_ENTITY_TOO_LARGE              ) == (__code) ? "Request Entity Too Large" :\
		(AK_RTSP_CODE_REQUEST_URI_TOO_LARGE                 ) == (__code) ? "Request-URI Too Large" :\
		(AK_RTSP_CODE_UNSUPPORTED_MEDIA_TYPE                ) == (__code) ? "Unsupported Media Type" :\
		(AK_RTSP_CODE_PARAMETER_NOT_UNDERSTOOD              ) == (__code) ? "Parameter Not Understood" :\
		(AK_RTSP_CODE_CONFERENCE_NOT_FOUND                  ) == (__code) ? "Conference Not Found" :\
		(AK_RTSP_CODE_NOT_ENOUGH_BANDWIDTH                  ) == (__code) ? "Not Enough Bandwidth" :\
		(AK_RTSP_CODE_SESSION_NOT_FOUND                     ) == (__code) ? "Session Not Found" :\
		(AK_RTSP_CODE_METHOD_NOT_VALID_IN_THIS_STATE        ) == (__code) ? "Method Not Valid in This State" :\
		(AK_RTSP_CODE_HEADER_FIELD_NOT_VALID_FOR_RESOURCE   ) == (__code) ? "Header Field Not Valid for Resource" :\
		(AK_RTSP_CODE_INVALID_RANGE                         ) == (__code) ? "Invalid Range" :\
		(AK_RTSP_CODE_PARAMETER_IS_READ_ONLY                ) == (__code) ? "Parameter Is Read-Only" :\
		(AK_RTSP_CODE_AGGREGATE_OPERATION_NOT_ALLOWED       ) == (__code) ? "Aggregate operation not allowed" :\
		(AK_RTSP_CODE_ONLY_AGGREGATE_OPERATION_ALLOWED      ) == (__code) ? "Only aggregate operation allowed" :\
		(AK_RTSP_CODE_UNSUPPORTED_TRANSPORT                 ) == (__code) ? "Unsupported transport" :\
		(AK_RTSP_CODE_DESTINATION_UNREACHABLE               ) == (__code) ? "Destination unreachable" :\
		(AK_RTSP_CODE_INTERNAL_SERVER_ERROR                 ) == (__code) ? "Internal Server Error" :\
		(AK_RTSP_CODE_NOT_IMPLEMENTED                       ) == (__code) ? "Not Implemented" :\
		(AK_RTSP_CODE_BAD_GATEWAY                           ) == (__code) ? "Bad Gateway" :\
		(AK_RTSP_CODE_SERVICE_UNAVAILABLE                   ) == (__code) ? "Service Unavailable" :\
		(AK_RTSP_CODE_GATEWAY_TIME_OUT                      ) == (__code) ? "Gateway Time-out" :\
		(AK_RTSP_CODE_RTSP_VERSION_NOT_SUPPORTED            ) == (__code) ? "RTSP Version not supported" :\
		(AK_RTSP_CODE_OPTION_NOT_SUPPORTED                  ) == (__code) ? "Option not supported" :\
		"Undefined"\
	)




AK_C_HEADER_EXTERN_C_END
#endif ///< AKAE_RTSP_H_
