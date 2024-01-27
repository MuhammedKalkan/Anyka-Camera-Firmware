/*************************************************

dvs.h
used by dvs

**************************************************/

#ifndef DVS_H
#define DVS_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <pthread.h>

#define ONVIF

//#define SUPPORT_YATAI
//#define MACRO_VASION
//#define VERSION                      "IPCAM-MV-V1.0.0.4"
#define VERSION                      "IPCAM-ONVIF-V1.1.0.1"
//#define OLD_IRCUT
#define MAX_CHUNNEL_NUM              1
#define MAX_VI_CHUNNEL               1
#define MAX_VO_CHUNNEL               1
#define MAX_VD_CHUNNEL               1
#define MAX_FRAMERATE                15
#define MIN_FRAMERATE                1
#define MAX_IO_CHUNNEL               4
#define MAX_RELAY_CHUNNEL            2
#define MAX_AUDIO_CHUNNEL            4
#define TEMP_STR_LEN                 1024
#define MAX_COMMAND_LEN              1024
#define MAX_NAME_LEN                 128
#define MAX_BUF_NUM                  100
#define MAX_CLIENT_NUM               5
#define MAX_MOBILE_NUM               1
#define MAX_ALARM_NUM                16
#define MOTION_AREA_STEP             8
#define MAX_USER_NUM                 100
#define MAX_IPADDR_LEN               20
#define MAX_OSD_CHARS                12
#define PAL                          0
#define NTSC                         1
#define VIDEO_ENCODE_TYPE            1
#define AUDIO_ENCODE_TYPE            0
#define MAX_PRESET_SCAN_NUM          16
#define VO_DEV_HD                    0
#define VO_DEV_AD                    2	//1
#define VO_DEV_SD                    1	//2
#define VOU_LAYER_G0                 0
#define VOU_LAYER_G1                 1
#define VOU_LAYER_G2                 2
#define VOU_LAYER_G3                 3
#define VOU_LAYER_HC                 4
#define VO_BKGRD_BLUE       0x0000FF
#define TW2865_FILE         "/dev/tw2865dev"
#define TW2864_FILE         "/dev/tw2864dev"
#define BT656_WORKMODE               VI_WORK_MODE_4D1

#define CMDSIZE                      (1024*2)
#define SEND_BUF_SIZE                (512*1024)
#define RECV_BUF_SIZE                2048
// #define MAX_VIDEO_ENCODE_BUF_SIZE    (512*1024)
#define MAX_VIDEO_ENCODE_BUF_SIZE    (1024*1024)
#define MAX_AUDIO_BUF_SIZE           8192

#define QCIF_WIDTH                   176
#define CIF_WIDTH                    352
#define D1_WIDTH                     704
#define QCIF_HEIGHT                  144
#define QCIF_HEIGHT_NTSC             120
#define CIF_HEIGHT                   288
#define CIF_HEIGHT_NTSC              240
#define D1_HEIGHT                    576
#define D1_HEIGHT_NTSC               480

#define OV9715                       1
#define MT9P031                      2
#define MT9M034                      3
#define OV2715                       4
#define TW9900                       5
#define HM1375                       6
#define OV10633                      8
#define RES1080P                     0
#define RES960P                      1
#define RES720P                      2
#define RESVGA                       3
#define RESQVGA                      4
#define RESD1                        5


#ifdef  SUPPORT_YATAI
#define MAX_YATAI_CLIENT_NUM         4
#endif
#define MAX_HTTP_CLIENT_NUM          16
#define MAX_RTSP_CLIENT_NUM          8
#define IP_ADDR_LEN                  16
#define RTP_BUF_SIZE                 1500
#define RTP_PKG_SIZE                 1440
#define RTSP_LISTEN_PORT             554
#define RTP_BASE_PORT                59694
#define IFRAME                       0x02
#define PFRAME                       0x00

#ifdef  MACRO_VASION
#define IRCUT_DELAY                  100
#else
#define IRCUT_DELAY                  2
#endif

#define  MAXSTRLEN 1024

struct stringlist
{
  char string[MAXSTRLEN];
  struct stringlist *next;
};

/**************************************************
base structs
**************************************************/
typedef struct
{
  unsigned short bf_type;
  unsigned int bf_size;
  unsigned short bf_reserved1;
  unsigned short bf_reserved2;
  unsigned int bf_offbytes;
} __attribute__ ((packed)) bitmap_file_header_t;

typedef struct
{
  unsigned int bi_size;
  unsigned long bi_width;
  unsigned long bi_height;
  unsigned short bi_planes;
  unsigned short bi_bitcount;
  unsigned int bi_compression;
  unsigned int bi_sizeimage;
  unsigned long bi_xpelspermeter;
  unsigned long bi_ypelspermeter;
  unsigned int bi_clrused;
  unsigned int bi_clrimportant;
} __attribute__ ((packed)) bitmap_info_header_t;

typedef struct
{
  int left;
  int top;
  int width;
  int height;
} rect_t;

/**************************************************
config structs
**************************************************/
typedef struct
{
  char name[MAX_NAME_LEN];
  int status;
} config_camerainfo_t;

typedef struct
{
  int displayname;
  int nameposition;
  int displaydate;
  int dateposition;
} config_displayinfo_t;

typedef struct
{
  int comport;
  int baudrate;
  int databit;
  int parity;
  int stopbit;
  int speed;
} config_comport_t;

typedef struct
{
  int brightness;
  int contrast;
  int saturation;
  int hue;
} config_videocolor_t;

typedef struct
{
  int enable;
  int preset;
  int stay_time;
} preset_scan_t;

typedef struct
{
  preset_scan_t preset_scan[MAX_PRESET_SCAN_NUM];
} config_preset_scan_t;

typedef struct
{
  int protocol;
  int comport;
  int address;
  int baudrate;
  int databit;
  int parity;
  int stopbit;
  int speed;
} config_ptz_t;

typedef struct
{
  int resolution;
  int quality;
  int bitrate;
  int framerate;
  int recordaudio;
  int dual_stream;
} config_videoprofile_t;

typedef struct
{
  int enable;
  int relating_io_in;
  int relating_io_out;
  int record;
  int notifyclient;
  int output;
} config_io_alarm_t;

typedef struct
{
  int enable;
  int sensitivity;
  int relating_io_out;
  int notifyclient;
  int record;
  int output;
  int motion_area[MOTION_AREA_STEP][MOTION_AREA_STEP];
} config_motiondetect_t;

typedef struct
{
  int enable;
  int relating_io_out;
  int notifyclient;
  int output;
} config_videolose_t;

typedef struct
{
  int minute;
  int video_seek;
  int audio_seek;
  int record_type;
  int firstrecord;
  int inteval;
  int second_video_seeks[60];	//60s
  int second_audio_seeks[60];	//60s
} record_log_t;

typedef struct
{
  char name[MAX_NAME_LEN];
  char passwd[MAX_NAME_LEN];
  char discript[MAX_NAME_LEN];
  int browse;
  int playback;
  int admin;
  int control;
  int ctrl_ptz;
  int control_record;
#if 0
  int control_alarm;
  int liveview[MAX_CHUNNEL_NUM];
  int control_ptz[MAX_CHUNNEL_NUM];
  int playrecord[MAX_CHUNNEL_NUM];
#endif
} config_user_t;

typedef struct
{
  int num;
  config_user_t user[MAX_USER_NUM];
} config_users_t;

typedef struct
{
  int type;
  int auto_get_dns;
  char ip[MAX_IPADDR_LEN];
  char netmask[MAX_IPADDR_LEN];
  char gateway[MAX_IPADDR_LEN];
  char dns1[MAX_IPADDR_LEN];
  char dns2[MAX_IPADDR_LEN];
} config_network_t;

typedef struct
{
  char username[MAX_NAME_LEN];
  char password[MAX_NAME_LEN];
  int connect;
} config_adsl_t;

typedef struct
{
  int server;
  char ddns[MAX_NAME_LEN];
  char username[MAX_NAME_LEN];
  char password[MAX_NAME_LEN];
  int connect;
} config_ddns_t;

typedef struct
{
  int interval;
  char receiver[MAX_NAME_LEN];
  char dvrname[MAX_NAME_LEN];
} config_mail_t;

typedef struct
{
  int auto_adjust;
  int adjust_interval;
  int server_port;
  char server_addr[TEMP_STR_LEN];
} config_ntp_t;

typedef struct
{
  int webport;
  int videoport;
  int mobileport;
} config_port_t;

typedef struct
{
  config_camerainfo_t camerainfo;
  config_displayinfo_t displayinfo;
  config_ptz_t ptz;
  config_preset_scan_t preset_scans;
  config_videocolor_t videocolor;
  config_videocolor_t default_color;
  config_videoprofile_t videoprofile;
  config_videoprofile_t videoprofile_small;
  config_io_alarm_t io_alarm;
  config_videolose_t videolose;
  config_motiondetect_t motiondetect;
} config_camera_t;

typedef struct
{
  config_users_t users;
  config_network_t network;
  config_ntp_t ntp;
  config_adsl_t adsl;
  config_ddns_t ddns;
  config_mail_t mail;
  config_port_t port;
} config_t;

typedef struct
{
  void *start;
  size_t length;
  int type;
  int frame_index;
  int chunnel;
  unsigned int pts;
} buffer_t;

typedef struct
{
  int rear;
  int front;
  int bufnum;
  int bufsize;
  int inteval;
  pthread_mutex_t mutex;
  buffer_t buf[MAX_BUF_NUM];
} buffers_t;

typedef struct
{
  int display;
  int record;
  int motiondetect;
  int alarm;
  int send;
  int motion_alarming;
  int io_alarming;
  int motion_alarm_record;
  int io_alarm_record;
  int schedule_record;
  int schedule_motiondetect;
  int recordmode;
} control_status_t;

typedef struct
{
  char name[MAX_NAME_LEN];
  pthread_t enc_thread;
  pthread_t save_thread;
  pthread_t dispense_thread;

  buffers_t audio_send_bufs;
  buffers_t audio_save_bufs;
  buffers_t video_save_bufs;
  buffers_t video_send_bufs;
  buffers_t video_send_bufs_small;
  unsigned char *videobuf;
  unsigned char *video_send_buf;
  unsigned char *video_send_buf_small;
  unsigned char *video_save_buf;
  unsigned char *audio_save_buf;
  unsigned char *audio_send_buf;
  unsigned char *videolose_buf;
  unsigned char *video_alarm_buf;
  unsigned char *video_alarm_jpeg;
  unsigned char *video_alarm_buf2;
  unsigned char *mark_name;
  unsigned char *last_video;
  int mark_name_width;
  control_status_t status;
  config_camera_t camera;
  int motion_alarm;
  int motion_alarm_timeout[MOTION_AREA_STEP][MOTION_AREA_STEP];

  int io_alarm;
  int io_alarm_timeout;
  int enable_preset_scan;
  int cur_preset_scan;
  int preset_scan_timeout;
  int video_lost;
  int video_lost_alarm_times;
  int get_alarm_picture;
  int ircut_status;
  int ircut_direction;
  int handle_name_osd;
  int handle_name_osd_small;
  int handle_date_osd;
  int handle_date_osd_small;
  int osd_name_changed;
  int camera_name_changed;
  rect_t rect_date;
  rect_t rect_date_small;
  int encode_fps;
  int encode_fps_small;
  int motion_result[MOTION_AREA_STEP][MOTION_AREA_STEP];
  pthread_mutex_t mutex;
  pthread_mutex_t mutex_buf;
  int enc_fd;
  int enc_fd_small;
  unsigned char *bs_buf;
  unsigned char *bs_buf_small;
//  uint8_t *hzbits;
//  uint8_t hzfont[MAX_OSD_CHARS][36];
  int enc_buf_size;
  int enc_buf_size_small;
} chunnel_t;

typedef struct
{
  int used;
  int timeout;
  int sock_fd;
  int send_pos;
  int send_len;
  int recv_pos;
  int recv_len;
  int logined;
  int chunnel[MAX_CHUNNEL_NUM];
  int chunnel_small[MAX_CHUNNEL_NUM];
  int audio_chunnel[MAX_CHUNNEL_NUM];
  int record_video_canceled[MAX_CHUNNEL_NUM];
  int record_audio_canceled[MAX_CHUNNEL_NUM];
  int video_width;
  int video_height;
  int video_width_small;
  int video_height_small;
  int get_motion_alarm[MAX_CHUNNEL_NUM];

  config_user_t cur_user;
  pthread_t recv_thread;
  pthread_t send_thread;
  unsigned char *send_buf;
  unsigned char *recv_buf;
  unsigned char *sending_buf;
  buffers_t video_send_bufs;
  buffers_t audio_send_bufs;
  pthread_mutex_t mutex_buf;
  pthread_mutex_t mutex_send;
} client_t;

typedef struct
{
  int used;
  int timeout;
  int sock_fd;
  int send_pos;
  int send_len;
  int recv_pos;
  int recv_len;
  pthread_t rs_thread;
  unsigned char *send_buf;
  unsigned char *recv_buf;
  unsigned char *sending_buf;
  unsigned char *image_buf;
  unsigned char *jpeg_buf;
  int jpeg_buflen;
} mobile_client_t;

typedef struct
{
  int timeout_encode;
  int timeout_encode_small;
} wdt_t;

typedef struct
{
  unsigned int Version;
  unsigned int Paddig;
  unsigned int Extension;
  unsigned int CSRCCount;
  unsigned int Marker;
  unsigned int PayloadType;
  unsigned int SequenceNumber;
  unsigned int TimeStamp;
  unsigned int SSRC;
} RTP_HEADER_T;

typedef struct
{
  /* byte 0 */
  unsigned char csrc_len:4;	/* expect 0 */
  unsigned char extension:1;	/* expect 1, see RTP_OP below */
  unsigned char padding:1;	/* expect 0 */
  unsigned char version:2;	/* expect 2 */

  /* byte 1 */
  unsigned char payloadtype:7;	/* RTP_PAYLOAD_RTSP */
  unsigned char marker:1;	/* expect 1 */

  /* bytes 2,3 */
  unsigned short seq_no;
  /* bytes 4-7 */
  unsigned int timestamp;
  /* bytes 8-11 */
  unsigned int ssrc;		/* stream number is used here. */
} RTP_FIXED_HEADER;


typedef struct
{
  //byte 0
  unsigned char TYPE:5;
  unsigned char NRI:2;
  unsigned char F:1;
} NALU_HEADER;			// 1 BYTE

/*
+---------------+
|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+
|F|NRI|  Type   |
+---------------+
*/
typedef struct
{
  //byte 0
  unsigned char TYPE:5;
  unsigned char NRI:2;
  unsigned char F:1;
} FU_INDICATOR;			// 1 BYTE

/*
+---------------+
|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+
|S|E|R|  Type   |
+---------------+
*/
typedef struct
{
  //byte 0
  unsigned char TYPE:5;
  unsigned char R:1;
  unsigned char E:1;
  unsigned char S:1;
} FU_HEADER;			// 1 BYTES

typedef struct
{
  int used;
  int sock_fd;
  int recv_len;
  int rtp_send;
  int rtp_tcp;
  unsigned short sq_no;
  int  rtp_port;
  char client_ip[IP_ADDR_LEN];
  int fd_rtp;
  int fd_rtcp;
  int client_changed;
  int wait_iframe;
  int index;
  unsigned int seq_no;
  unsigned int payload_sum;
  unsigned int pkg_sum;
  unsigned int timestamp;
  int vops;
  int ssrc;
  int chunnel;
  unsigned char *send_buf;
  unsigned char *recv_buf;
  unsigned char *rtp_buf;
  unsigned char *sending_buf;
  buffers_t video_send_bufs;
  buffers_t audio_send_bufs;
  pthread_mutex_t mutex_buf;
  pthread_mutex_t mutex_send;
} rtsp_client_t;

typedef struct
{
	char identification_name[256];
	char identification_location[256];
} onvif_data_t ;

typedef struct
{
  int ch_num;
  int disk_num;
  int format;
  int language;
  int watchdog;
  int restart;
  int restart_hour;
  int restart_minute;
  int restart_day;
  int time_zone;
  int scan_interval;
  int videolose_alarm_times;
  int io_alarm_times;
  int motion_alarm_times;
  int osd_date_handle;
  int sensor;
  int source_resolution;
  int video_width;
  int video_height;
  int video_width_small;
  int video_height_small;
  int video_src_height;
  int video_src_height_small;
  config_t config;
  int tw286x_fd;
  int cpu_gpio_fd;
  int wdt_fd;
  int dvr_fd;
  wdt_t wdt;
  chunnel_t chunnel[MAX_CHUNNEL_NUM];
  int aux_output_chunnel[MAX_CHUNNEL_NUM];
  client_t client[MAX_CLIENT_NUM];
  mobile_client_t mobile_client[MAX_MOBILE_NUM];
#ifdef SUPPORT_YATAI
  client_t yatai_client[MAX_YATAI_CLIENT_NUM];
#endif
  client_t http_client[MAX_HTTP_CLIENT_NUM];
  rtsp_client_t rtsp_client[MAX_RTSP_CLIENT_NUM];

  pthread_mutex_t mutex_mark_date;
  pthread_mutex_t mutex_serialport;
  config_user_t cur_user;
  char macaddr[MAX_IPADDR_LEN];

  onvif_data_t onvif_data[MAX_CHUNNEL_NUM];
} dvs_t;

typedef struct
{
  int index;
  pthread_mutex_t mutex;
  dvs_t *dvs;
} dvs_params_t;

int write_log (dvs_t * dvs, char *str);
int send_cmd (dvs_t * dvs, char *cmdstr);

int dvs_init_buffers (buffers_t * bufs, int bufsize, int bufnum);
void buffers_clear (dvs_t * dvs, buffers_t * bufs, int chunnel);
int buffers_put_iframe_data (void *data, int length, buffers_t * bufs,
			     int type, int chunnel, int frame_index);
int buffers_put_pframe_data (void *data, int length, buffers_t * bufs,
			     int type, int chunnel, int frame_index);
int buffers_get_data (void *data, int *length, buffers_t * bufs, int *type,
		      int *chunnel, int *frame_index);
int buffers_put_data (void *data, int length, buffers_t * bufs, int type,
		      int chunnel, int frame_index);
int buffers_get_data_with_pts (void *data, int *length, buffers_t * bufs, int *type,
		      int *chunnel, int *frame_index, unsigned int *pts);
int buffers_put_data_with_pts (void *data, int length, buffers_t * bufs, int type,
		      int chunnel, int frame_index, unsigned int pts);
int buffers_put_iframe_data_with_pts (void *data, int length, buffers_t * bufs,
			     int type, int chunnel, int frame_index, unsigned int pts);
int buffers_put_pframe_data_with_pts (void *data, int length, buffers_t * bufs,
			     int type, int chunnel, int frame_index, unsigned int pts);
void buffers_clear_data (buffers_t * bufs);

int init_onvif(int width, int height);
void *start_onvif();
extern dvs_t g_dvs;
extern dvs_params_t g_param;
#ifdef __cplusplus
}
#endif

#endif
