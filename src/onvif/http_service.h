/*************************************************

httpd.h
used by onvif

**************************************************/

#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H
#ifdef __cplusplus
extern "C"
{
#endif

#define HTTP_TEMP_STR_LEN            10240
#define HTTP_CMDSIZE                 10240
#define HTTP_SEND_BUF_SIZE           10240
#define HTTP_RECV_BUF_SIZE           10240
#define MAX_SOAP_SIZE                20480
#define HTTP_LISTEN_PORT             80 

static char *http_head_str = "HTTP/1.1 200 OK\r\nServer: gSOAP/2.8\r\nContent-Type: application/soap+xml; charset=utf-8\r\nContent-Length: %d\r\nConnection: close\r\n\r\n";

static char *xml_version = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

static char *soap_env_head = "<env:Envelope xmlns:env=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:soapenc=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" xmlns:timg=\"http://www.onvif.org/ver20/imaging/wsdl\" xmlns:tev=\"http://www.onvif.org/ver10/events/wsdl\" xmlns:tptz=\"http://www.onvif.org/ver20/ptz/wsdl\" xmlns:tan=\"http://www.onvif.org/ver20/analytics/wsdl\" xmlns:tst=\"http://www.onvif.org/ver10/storage/wsdl\" xmlns:ter=\"http://www.onvif.org/ver10/error\" xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:tns1=\"http://www.onvif.org/ver10/topics\" xmlns:tmd=\"http://www.onvif.org/ver10/deviceIO/wsdl\" xmlns:wsdl=\"http://schemas.xmlsoap.org/wsdl\" xmlns:wsoap12=\"http://schemas.xmlsoap.org/wsdl/soap12\" xmlns:http=\"http://schemas.xmlsoap.org/wsdl/http\" xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:wsadis=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:xop=\"http://www.w3.org/2004/08/xop/include\" xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" xmlns:wsa=\"http://www.w3.org/2005/08/addressing\" xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" xmlns:wsrf-bf=\"http://docs.oasis-open.org/wsrf/bf-2\" xmlns:wsntw=\"http://docs.oasis-open.org/wsn/bw-2\" xmlns:wsrf-rw=\"http://docs.oasis-open.org/wsrf/rw-2\" xmlns:wsaw=\"http://www.w3.org/2006/05/addressing/wsdl\" xmlns:wsrf-r=\"http://docs.oasis-open.org/wsrf/r-2\" xmlns:tnshik=\"http://www.hikvision.com/2011/event/topics\">";

int http_init_client (client_t * client);
void *http_client_recv (void *data);
void *http_client_listen (void *data);

#ifdef __cplusplus
}
#endif
#endif
