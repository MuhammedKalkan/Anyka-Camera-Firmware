#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>

#include "ak_thread.h"
#include "ak_common.h"
#include "ak_vi.h"
#include "ak_venc.h"
#include "ak_rtsp.h"
#include "ak_ai.h"
#include "ak_aenc.h"
#include "ak_drv_ir.h"

#define IRCUT_A_FILE_NAME       "/sys/user-gpio/gpio-ircut_a"
#define IRLED_FILE_NAME		"/sys/user-gpio/IR_LED"
#define STRING_LEN	            128

enum day_ctrl_level {
	DAY_LEVEL_RESERVED = 0x00,
	DAY_LEVEL_HH,		//high-high
	DAY_LEVEL_HL,		//high-low
	DAY_LEVEL_LH,		//low-high
	DAY_LEVEL_LL,		//low-low
	DAY_LEVEL_MAX
};

enum day_night_switch_mode {
	SET_NIGHT_MODE,
	SET_DAY_MODE,
	SET_AUTO_MODE,
	SET_CLOSE_MODE
};

enum ak_photosensitive_mode {
	HARDWARE_PHOTOSENSITIVE,	// hardware photosensitive
	AUTO_PHOTOSENSITIVE		// auto ir switch
};


struct ak_misc {
    int ircut_run_flag;	    //photosensitive and ircut switch run flag
    int ps_switch_enable;   //store photosensitive switch status
    int day_level_ctrl;		//day level default led-ircut config
    int ipc_run_flag;		//ipc run flag
    int pre_status;			//current state,0 night, 1 day
    void *vi_handle;		//global vi handle
    ak_pthread_t ircut_tid;
	int cur_set_mode;
	int thread_run;
	enum ak_photosensitive_mode ps_mode;
	thread_func ps_fun_thread;// //photosensitive thread function
};


static struct ak_misc misc_ctrl = {0};


#define FIRST_PATH                  "/etc/jffs2/"
#define LEN_HINT                    512
#define DEFAULT_MAIN_WIDTH          1280
#define DEFAULT_MAIN_HEIGHT         720
#define DEFAULT_SUB_WIDTH           640
#define DEFAULT_SUB_HEIGHT          480
#define DEFAULT_MAIN_FPS            25
#define DEFAULT_SUB_FPS             25
#define DEFAULT_MINQP               20
#define DEFAULT_MAXQP               51
#define DEFAULT_GOP                 2 
#define DEFAULT_MAIN_MODE           BR_MODE_CBR                                                     //BR_MODE_CBR,BR_MODE_VBR
#define DEFAULT_SUB_MODE            BR_MODE_CBR
#define DEFAULT_MAIN_TYPE           H264_ENC_TYPE	//H264_ENC_TYPE    //HEVC_ENC_TYPE,HEVC_ENC_TYPE
#define DEFAULT_SUB_TYPE            H264_ENC_TYPE	//H264_ENC_TYPE
#define DEFAULT_MAIN_KBPS           2000
#define DEFAULT_SUB_KBPS            200
#define DEFAULT_MAIN_SUFFIX         "vs0"
#define DEFAULT_SUB_SUFFIX          "vs1"

char ac_option_hint[  ][ LEN_HINT ] = {
	"       HELP" ,
	"[NUM]  ( DEFAULT: 1920 )" ,
	"[NUM]  ( DEFAULT: 1080 )" ,
	"[NUM]  ( DEFAULT: 640 )" ,
	"[NUM]  ( DEFAULT: 360 )" ,
	"[NUM]  ( DEFAULT: 2000 )" ,
	"[NUM]  ( DEFAULT: 200 )" ,
	"[NAME] ( DEFAULT: 'vs0' )" ,
	"[NAME] ( DEFAULT: 'vs1' )" ,
	"[NUM]  ( DEFAULT: 50 )" ,
	"[NUM]  ( DEFAULT: 28 )" ,
	"[NUM]  ( DEFAULT: 42 )" ,
};

struct option option_long[ ] = {
	{ "help"             , no_argument       , NULL , 'h' } ,                                   //"       HELP" ,
	{ "main-width"       , required_argument , NULL , 'a' } ,                                   //"[NUM]  ( DEFAULT: 1920 )" ,
	{ "main-height "     , required_argument , NULL , 'b' } ,                                   //"[NUM]  ( DEFAULT: 1080 )" ,
	{ "sub-width"        , required_argument , NULL , 'c' } ,                                   //"[NUM]  ( DEFAULT: 640 )" ,
	{ "sub-height"       , required_argument , NULL , 'd' } ,                                   //"[NUM]  ( DEFAULT: 360 )" ,
	{ "main-kbps"        , required_argument , NULL , 'e' } ,                                   //"[NUM]  ( DEFAULT: 2000 )" ,
	{ "sub-kbps"         , required_argument , NULL , 'f' } ,                                   //"[NUM]  ( DEFAULT: 200 )" ,
	{ "main-chn-name"    , required_argument , NULL , 'n' } ,                                   //"[NAME] ( DEFAULT: 'vs0' )" ,
	{ "sub-chn-name"     , required_argument , NULL , 'o' } ,                                   //"[NAME] ( DEFAULT: 'vs1' )" ,
	{ "gop"              , required_argument , NULL , 'p' } ,                                   //"[NUM]  ( DEFAULT: 50 )" ,
	{ "minqp"            , required_argument , NULL , 'q' } ,                                   //"[NUM]  ( DEFAULT: 28 )" ,
	{ "maxqp"            , required_argument , NULL , 'r' } ,                                   //"[NUM]  ( DEFAULT: 42 )" ,
	/*
	{ "main-fps"         , required_argument , NULL , 'g' } ,                                   //"[NUM]  ( DEFAULT: 25 )" ,
	{ "sub-fps"          , required_argument , NULL , 'i' } ,                                   //"[NUM]  ( DEFAULT: 25 )" ,
	{ "main-video-mode"  , required_argument , NULL , 'j' } ,                                   //"[0|1]  BR_MODE_CBR: 0 | BR_MODE_VBR: 1( DEFAULT: 0 )" ,
	{ "sub-video-mode"   , required_argument , NULL , 'k' } ,                                   //"[0|1]  BR_MODE_CBR: 0 | BR_MODE_VBR: 1( DEFAULT: 0 )" ,
	{ "main-enc-type"    , required_argument , NULL , 'l' } ,                                   //"[0|2]  H264_ENC_TYPE: 0 | HEVC_ENC_TYPE: 2( DEFAULT: 0 )" ,
	{ "sub-enc-type"     , required_argument , NULL , 'm' } ,                                   //"[0|2]  H264_ENC_TYPE: 0 | HEVC_ENC_TYPE: 2( DEFAULT: 0 )" ,
	*/
};

int i_main_width = DEFAULT_MAIN_WIDTH;                                                             //��ͨ��������
int i_main_height = DEFAULT_MAIN_HEIGHT;
int i_sub_width = DEFAULT_SUB_WIDTH;                                                               //��ͨ���ֱ���
int i_sub_height = DEFAULT_SUB_HEIGHT;
int i_main_kbps = DEFAULT_MAIN_KBPS;                                                               //����
int i_sub_kbps = DEFAULT_SUB_KBPS;
int i_main_fps = DEFAULT_MAIN_FPS;                                                                 //֡��
int i_sub_fps = DEFAULT_SUB_FPS;
int i_main_mode = DEFAULT_MAIN_MODE;
int i_sub_mode = DEFAULT_SUB_MODE;
int i_main_type = DEFAULT_MAIN_TYPE;
int i_sub_type = DEFAULT_SUB_TYPE;
char *pc_main_name = DEFAULT_MAIN_SUFFIX;
char *pc_sub_name = DEFAULT_SUB_SUFFIX;
int i_gop = DEFAULT_GOP;                                                                           //I֡�����
int i_minqp = DEFAULT_MINQP;                                                                       //ѹ���ʷ�Χ
int i_maxqp = DEFAULT_MAXQP;
char *pc_prog_name = NULL ;

static int run_flag = AK_FALSE;
void *vi_handle;

/**
* @brief   initialize the video input handle
* @param   no
* @return  the video input handle
*/
static void *ak_rtsp_vi_init(void)
{
	/* match sensor */
	if (ak_vi_match_sensor(FIRST_PATH) < 0) {
		ak_print_error_ex("match sensor failed\n");
		return NULL;
	}

	/* open device */
	void *handle = ak_vi_open(VIDEO_DEV0);
	if (handle == NULL) {
		ak_print_error_ex("vi open failed\n");
		return NULL;
	}

	/* get camera resolution */
	struct video_resolution resolution = {0};
	if (ak_vi_get_sensor_resolution(handle, &resolution)) {
		ak_print_error_ex("get sensor resolution failed\n");
		ak_vi_close(handle);
		return NULL;
	} else
		ak_print_normal("sensor resolution height:%d, width:%d.\n",
				resolution.height, resolution.width);

	/* set crop information */
	struct video_channel_attr attr;
	memset(&attr, 0, sizeof(struct video_channel_attr));

	//set default crop
	attr.crop.left = 0;
	attr.crop.top = 0;
	attr.crop.width = resolution.width;
	attr.crop.height = resolution.height;

	//set channel default pixel
	attr.res[VIDEO_CHN_MAIN].width = i_main_width;
	attr.res[VIDEO_CHN_MAIN].height = i_main_height;
	attr.res[VIDEO_CHN_MAIN].max_width = 640;
	attr.res[VIDEO_CHN_MAIN].max_height = 480;

	attr.res[VIDEO_CHN_SUB].width = i_sub_width;
	attr.res[VIDEO_CHN_SUB].height= i_sub_height;
	attr.res[VIDEO_CHN_SUB].max_width = 640;
	attr.res[VIDEO_CHN_SUB].max_height = 480;

	if (ak_vi_set_channel_attr(handle, &attr)) {
		ak_print_error_ex("set channel attribute failed\n");
		ak_vi_close(handle);
		return NULL;
	}
	ak_print_notice_ex("start capture ...\n");
	if(ak_vi_capture_on(handle)) {
		ak_print_error_ex("start capture failed\n");
		ak_vi_close(handle);
		return NULL;
	}

	return handle;
}

static int help_hint(void)                                                                          //����option_long��ac_option_hint��ӡ������Ϣ
{
	int i ;
	printf( "%s\n" , pc_prog_name ) ;
	for( i = 0 ; i < sizeof( option_long ) / sizeof( struct option ) ; i ++ ) {
		printf( "\t--%-16s -%c %s\n" , option_long[ i ].name , option_long[ i ].val , ac_option_hint[ i ] ) ;
	}
	printf( "\n\n" ) ;
	return 0 ;
}

static void process_signal(unsigned int sig, siginfo_t *si, void *ptr)
{
	system("rm -f /tmp/core_*");

	ak_backtrace(sig, si, ptr);
	run_flag = AK_FALSE;
}

static int register_signal(void)
{
	struct sigaction s;

	s.sa_flags = SA_SIGINFO;
	s.sa_sigaction = (void *)process_signal;

	/* register signals that we should handle */
	sigaction(SIGSEGV, &s, NULL);
	sigaction(SIGINT, &s, NULL);
	sigaction(SIGTERM, &s, NULL);
	sigaction(SIGUSR1, &s, NULL);
	sigaction(SIGUSR2, &s, NULL);
	sigaction(SIGALRM, &s, NULL);
	sigaction(SIGHUP, &s, NULL);
	sigaction(SIGPIPE, &s, NULL);

	signal(SIGCHLD, SIG_IGN);

	return 0;
}

static int camera_set_ir(int value, char *name)
{
	char cmd[STRING_LEN];
	char result[STRING_LEN];

	memset(result, '\0', STRING_LEN);
	sprintf(cmd, "echo %d > %s", value, name);
	system(cmd);

	return 0;
}


/**
 * ak_misc_set_video_day_night: set video day or night mode, according to IR value
 * @vi_handle: opened vi handle
 * @ir_val: IR value, [0, 1]
 * @day_level: day control level, [1, 4]
 * return: 0 success, -1 failed
 */
int ak_misc_set_video_day_night(void *vi_handle, int ir_val, int day_level)
{
	if (!vi_handle) {
		return AK_FAILED;
	}

	int day_val = 0;
	int ir_switch_val = 0;

	switch (day_level) {
	case DAY_LEVEL_HH:
		ir_switch_val = ir_val;
		day_val = ir_val;
		break;
	case DAY_LEVEL_HL:
		ir_switch_val = !ir_val;
		day_val = ir_val;
		break;
	case DAY_LEVEL_LH:
		ir_switch_val = !ir_val;
		day_val = !ir_val;
		break;
	case DAY_LEVEL_LL:
		ir_switch_val = ir_val;
		day_val = !ir_val;
		break;
	default:
		return AK_FAILED;
	}

	int ret = AK_FAILED;
	if (day_val) {
		ak_print_notice("now set to day\n");
		camera_set_ir(1, IRCUT_A_FILE_NAME);			
		ret = ak_vi_switch_mode(vi_handle, VI_MODE_DAY);
		camera_set_ir(!ir_val, IRLED_FILE_NAME);
	} else {
		ak_print_notice("now set to night\n");
		camera_set_ir(!ir_val, IRLED_FILE_NAME);
		ret = ak_vi_switch_mode(vi_handle, VI_MODE_NIGHT);
		camera_set_ir(0, IRCUT_A_FILE_NAME);	
	}

	ak_sleep_ms(300);
	/* to notify av mode to modify gop */


	return ret;
}


static void *photosensitive_switch_th_ex(void *arg)
{
	long int tid = ak_thread_get_tid();
	int ir_val = 0;

	ak_print_normal_ex("Thread start, id: %ld\n", tid);
	ak_thread_set_name("PS_switch");

	misc_ctrl.thread_run = AK_TRUE;
	/* get ir state and switch day-night */
	while (misc_ctrl.thread_run) {		
		//ak_print_normal_ex("misc_ctrl.cur_set_mode=%d\n", misc_ctrl.cur_set_mode);
		if (misc_ctrl.cur_set_mode != SET_AUTO_MODE)
			break;

		ir_val = ak_drv_ir_get_input_level();
		
		if (ir_val != -1 && misc_ctrl.pre_status != ir_val) {
	        ak_print_notice("prev_state=%d, ir_val=%d\n",
		        misc_ctrl.pre_status, ir_val);
			ak_misc_set_video_day_night(vi_handle, !ir_val,
			    DAY_LEVEL_HH);
			misc_ctrl.pre_status = ir_val;
		}
		
		ak_sleep_ms(5000);
	}

	ak_print_normal_ex("Thread exit, id: %ld\n", tid);
	ak_thread_exit();
	return NULL;
}

int ak_misc_start_photosensitive_switch_ex(enum ak_photosensitive_mode ps_mode, 
							 enum day_night_switch_mode day_night_mode)
{

	if (misc_ctrl.ircut_run_flag) {
		ak_print_error_ex("misc already start\n");
		return AK_SUCCESS;
	}
	misc_ctrl.vi_handle = ak_vi_get_handle(VIDEO_DEV0);
	misc_ctrl.ircut_run_flag = AK_TRUE;
	
	misc_ctrl.cur_set_mode = day_night_mode;
	misc_ctrl.ps_mode = ps_mode;
	misc_ctrl.pre_status = -1;

	int ret = AK_SUCCESS;
	
		
		misc_ctrl.ps_fun_thread = photosensitive_switch_th_ex;
		
		ret = ak_thread_create(&misc_ctrl.ircut_tid, misc_ctrl.ps_fun_thread,
			NULL, 100 *1024, -1);
	
	return ret;
}





static void init_other_platform(void)
{

	ak_drv_ir_init();
	/* start photosensitive ircut detect service */
    ak_misc_start_photosensitive_switch_ex(HARDWARE_PHOTOSENSITIVE,
										2);
	

}
/**
 * ak_rtsp_demo
 * input url and connect at client tool,such as: "rtsp://192.168.1.119:554/720p"
 * return: void
 */
int main(int argc, char **argv)
{
	int ret , i_option ;
	

	pc_prog_name = argv[ 0 ] ;

	register_signal();

	while( ( i_option = getopt_long( argc , argv , "ha:b:c:d:e:f:g:i:j:k:l:m:n:o:p:q:r:" , option_long , NULL ) ) != -1 ) {
		switch( i_option ) {
			case 'h' :
				help_hint( ) ;
				return 0 ;
			case 'a' :
				i_main_width = atoi( optarg ) ;
				break;
			case 'b' :
				i_main_height = atoi( optarg ) ;
				break;
			case 'c' :
				i_sub_width = atoi( optarg ) ;
				break;
			case 'd' :
				i_sub_height = atoi( optarg ) ;
				break;
			case 'e' :
				i_main_kbps = atoi( optarg ) ;
				break;
			case 'f' :
				i_sub_kbps = atoi( optarg ) ;
				break;
			case 'g' :
				i_main_fps = atoi( optarg ) ;
				break;
			case 'i' :
				i_sub_fps = atoi( optarg ) ;
				break;
			case 'j' :
				i_main_mode = atoi( optarg ) ;
				break;
			case 'k' :
				i_sub_mode = atoi( optarg ) ;
				break;
			case 'l' :
				i_main_type = atoi( optarg ) ;
				break;
			case 'm' :
				i_sub_type = atoi( optarg ) ;
				break;
			case 'n' :
				pc_main_name = optarg ;
				break;
			case 'o' :
				pc_sub_name = optarg ;
				break;
			case 'p' :
				i_gop = atoi( optarg ) ;
				break;
			case 'q' :
				i_minqp = atoi( optarg ) ;
				break;
			case 'r' :
				i_maxqp = atoi( optarg ) ;
				break;
		}
	}

	signal(SIGPIPE, SIG_IGN);

	/* init camera*/
	vi_handle = ak_rtsp_vi_init();
	if(vi_handle == NULL) {
		ak_print_error("vi init fail\n");
		return -1;
	}
	ak_print_notice("vi init ok\n");

	struct rtsp_param param = {{{0}}};

	/* main channel config */
	param.rtsp_chn[0].current_channel = 0;
	param.rtsp_chn[0].width 	= i_main_width;
	param.rtsp_chn[0].height 	= i_main_height;

	param.rtsp_chn[0].fps 	 	= i_main_fps;
	param.rtsp_chn[0].max_kbps 	= i_main_kbps;
	param.rtsp_chn[0].min_qp	= i_minqp;
	param.rtsp_chn[0].max_qp 	= i_maxqp;
	param.rtsp_chn[0].gop_len	= i_gop;

	param.rtsp_chn[0].video_enc_type = i_main_type;
	param.rtsp_chn[0].video_br_mode  = i_main_mode;

	param.rtsp_chn[0].vi_handle = vi_handle;
	strcpy(param.rtsp_chn[0].suffix_name, "vs0");

	/* main channel config */
	param.rtsp_chn[1].current_channel = 1;
	param.rtsp_chn[1].width 	= i_sub_width;
	param.rtsp_chn[1].height 	= i_sub_height;

	param.rtsp_chn[1].fps 	 	= i_sub_fps;
	param.rtsp_chn[1].max_kbps 	= i_sub_kbps;
	param.rtsp_chn[1].min_qp	= i_minqp;
	param.rtsp_chn[1].max_qp 	= i_maxqp;
	param.rtsp_chn[1].gop_len	= i_gop;

	param.rtsp_chn[1].video_enc_type = i_sub_type;
	param.rtsp_chn[1].video_br_mode  = i_sub_mode;

	param.rtsp_chn[1].vi_handle = vi_handle;
	strcpy(param.rtsp_chn[1].suffix_name, "vs1");



	/* init rtsp */
	ret = ak_rtsp_init(&param);
	ak_print_notice("init rtsp, ret: %d\n", ret);
	if (ret) {
		ak_print_error_ex("\n\t---- init rtsp failed ---- !\n");
		return -1;
	}
	/* start rtsp service */
	ak_rtsp_start(VIDEO_CHN_MAIN);
	ak_rtsp_start(VIDEO_CHN_SUB);

	init_other_platform();

	run_flag = AK_TRUE;

	while (run_flag) {
		ak_sleep_ms(1000);
	}
	ak_rtsp_stop(VIDEO_CHN_MAIN);
	ak_rtsp_stop(VIDEO_CHN_SUB);

	ak_rtsp_exit();

	return 0;
}
