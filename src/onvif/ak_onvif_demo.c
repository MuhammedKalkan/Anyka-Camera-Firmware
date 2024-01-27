/* system heads */
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>

/* onvif heads */
#include "dvs.h"
#include "onvif_service.h"

/* project heads */
#include "ak_common.h"
#include "ak_thread.h"
#include "ak_ipc_srv.h"
#include "ak_vi.h"
#include "ak_cmd_exec.h"
#include "ak_venc.h"
#include "ak_osd.h"
#include "ak_osd_ex.h"
#include "ak_net.h"
//#include "ak_vpss.h"
#include "onvif_demo.h"
#include "onvif_cmd.h"
#include "onvif_discovery.h"

/* ISP config file path */
#define FIRST_PATH       				"/etc/jffs2/"
#define BACK_PATH        				"/usr/local/"

#define ONVIF_GET_STREAM_TIME   0
#define SHOW_ONVIF_PARAM        1
#define ONVIF_TEST_ISP_EFFECT	0

struct onvif_stream {
    void *stream_handle;

#if ONVIF_GET_STREAM_TIME
    int fps;
    struct ak_timeval get_time;
#endif
};

/* onvif global control handle struct */
struct ak_onvif_t {
	int onvif_started;	//run flag
	void *vi_handle;	//vi handle
	void *venc_main;	//venc main channel handle
	void *venc_sub;		//venc sub channel handle
	void *stream_main;	//main stream handle
	void *stream_sub;	//sub stream handle
	ak_pthread_t onvif_tid;	//onvif main thread id

	struct onvif_param param;
};

/* global control handle and value definition */
static struct ak_onvif_t onvif_ctrl = {0};
static void *vi_handle = NULL;
static int frame_index_m = 0;
static int frame_index_s = 0;
static unsigned char onvif_run_flag = 0;

/*----------- osd code -----------*/
#define AK_ONVIF_OSD_ENABLE (0)

#if AK_ONVIF_OSD_ENABLE
/* onvif osd struct */
struct ak_onvif_osd {
	ak_pthread_t tid;	//osd thread id
	int run_flag;		//osd thread run flag
	int osd_switch;		//osd on/off flag
	struct osd_ex_info ex;	//osd extend infomartion
};

struct ak_onvif_osd onvif_osd = {0};
#endif

static const char onvif_version[] = "onvif demo V1.0.01";

#if SHOW_ONVIF_PARAM
static void show_onvif_param(void)
{
    ak_print_normal("*** %s ***\n", __func__);
	ak_print_normal("vi_handle=%p\n", onvif_ctrl.param.vi_handle);

	ak_print_normal("main: width=%d, height=%d\n",
		onvif_ctrl.param.main_width, onvif_ctrl.param.main_height);
	ak_print_normal("sub: width=%d, height=%d\n",
		onvif_ctrl.param.sub_width, onvif_ctrl.param.sub_height);

	ak_print_normal("osd_rate=%d, osd_position=%d\n",
		onvif_ctrl.param.osd_rate, onvif_ctrl.param.osd_position);
	ak_print_normal("osd_switch=%d, time_switch=%d\n",
		onvif_ctrl.param.osd_switch, onvif_ctrl.param.time_switch);
	ak_print_normal("osd_name: %s\n", onvif_ctrl.param.osd_name);
    ak_print_normal("date_format=%d, hour_format=%d, week_format=%d\n",
            onvif_ctrl.param.date_format,
            onvif_ctrl.param.hour_format,
            onvif_ctrl.param.week_format);

    ak_print_normal("min_qp=%d, max_qp=%d\n",
		onvif_ctrl.param.min_qp, onvif_ctrl.param.max_qp);
	ak_print_normal("gop_len=%d, main vdmode=%d, sub vdmode=%d, method=%d\n",
		onvif_ctrl.param.gop_len,
		onvif_ctrl.param.main_video_mode,
		onvif_ctrl.param.sub_video_mode,
		onvif_ctrl.param.method);

	ak_print_normal("main_fps=%d, main_kbps=%d\n",
		onvif_ctrl.param.main_fps, onvif_ctrl.param.main_kbps);
	ak_print_normal("sub_fps=%d, sub_kbps=%d\n",
		onvif_ctrl.param.sub_fps, onvif_ctrl.param.sub_kbps);
	ak_print_normal("*** %s End ***\n\n", __func__);
}
#endif

/**
 * ak_onvif_get_version - get onvif version
 * return: version string
 */
const char* ak_self_onvif_get_version(void)
{
	return onvif_version;
}

#if AK_ONVIF_OSD_ENABLE
/*
 * onvif_osd_thread - onvif osd thread, draw osd
 */
static void *onvif_osd_thread(void *arg)
{
	unsigned char osd_init_flag = AK_FALSE;
	void *vi_handle = arg;
	ak_thread_set_name("onvif_osd");

	ak_print_normal_ex("thread id: %ld\n", ak_thread_get_tid());
	if (ak_osd_ex_init_env(vi_handle)) {
		goto onvif_osd_end;
	}
	osd_init_flag = AK_TRUE;

	/* iterate two channel */
	int chn;
	for (chn = 0; chn < 2; chn++) {
		int width, height, max_w, max_h, ret;

		/* set osd buff */
		ret	= ak_osd_get_max_rect(chn, &max_w, &max_h);
		if (ret < 0) {
			ak_print_error_ex("chn %d get max rect fail!\n", chn);
			goto onvif_osd_end;
		}
		ak_print_normal_ex("chn %d get max rect ok, w: %d h: %d\n",
				chn, max_w, max_h);
		width  = max_w;
		height = max_h;
		/* set osd rectangle first */
		ret = ak_osd_set_rect(vi_handle, chn, 0, 0, 0, width, height);
		if (ret < 0) {
			ak_print_error_ex("chn %d set rect fail!\n", chn);
			goto onvif_osd_end;
		}
		ak_print_normal_ex("chn %d set rect ok!\n", chn);
	}

	/* set display position and format */
	struct osd_ex_info osd_ex = {0};

	memcpy(osd_ex.name, onvif_osd.ex.name, sizeof(onvif_osd.ex.name));
	osd_ex.on_right_side = onvif_osd.ex.on_right_side;
	osd_ex.date_format = onvif_osd.ex.date_format;
	osd_ex.hour_format= onvif_osd.ex.hour_format;
	osd_ex.week_format = onvif_osd.ex.week_format;
	osd_ex.xpos_main = onvif_osd.ex.xpos_main;
	osd_ex.ypos_main = onvif_osd.ex.ypos_main;
	osd_ex.xpos_sub = onvif_osd.ex.xpos_sub;
	osd_ex.ypos_sub = onvif_osd.ex.ypos_sub;

	/*
	 * osd name will not change frequent,
	 * so we set one times
	 */
	if (1 == onvif_osd.osd_switch) {
		ak_osd_ex_disp_name(&osd_ex);
	}

	/*
	 * the date and times will change frequently,
	 * so we update it by a specified interval time.
	 */
	onvif_osd.run_flag = 1;
	while (onvif_osd.run_flag && onvif_osd.ex.time_switch) {
		if (ak_osd_ex_disp_date_time(&osd_ex) < 0)
			break;
		ak_sleep_ms(200);
	}

onvif_osd_end:
	/* resources release */
	if (osd_init_flag) {
		ak_osd_destroy();
	}
	ak_print_normal_ex("thread tid: %ld exit\n", ak_thread_get_tid());
	ak_thread_exit();
	return NULL;
}
/*
 * onvif_osd_init - init osd
 * vi_handle[IN]: video input handle
 */
static void onvif_osd_init(void *vi_handle)
{
	if (onvif_osd.run_flag == 1)
		return;

	onvif_osd.ex.position = onvif_ctrl.param.osd_position;
	onvif_osd.osd_switch = onvif_ctrl.param.osd_switch;
	strcpy(onvif_osd.ex.name, onvif_ctrl.param.osd_name);
	onvif_osd.ex.time_switch = onvif_ctrl.param.time_switch;
	onvif_osd.ex.date_format = onvif_ctrl.param.date_format;
	onvif_osd.ex.hour_format = onvif_ctrl.param.hour_format;
	onvif_osd.ex.week_format = onvif_ctrl.param.week_format;

	/* adapt camera setting */
	int main_width = onvif_ctrl.param.main_width;
 	int main_height = onvif_ctrl.param.main_height;
	int sub_width = onvif_ctrl.param.sub_width;
	int sub_height = onvif_ctrl.param.sub_height;

	/* select position */
	switch (onvif_osd.ex.position)
	{
	case 1:	//leftdown
		onvif_osd.ex.xpos_main	= OSD_EX_FONT_SIZE * 2;
		onvif_osd.ex.ypos_main	= main_height - OSD_EX_FONT_SIZE * 2 * 2;
		onvif_osd.ex.xpos_sub	= OSD_EX_FONT_SIZE;
		onvif_osd.ex.ypos_sub	= sub_height - OSD_EX_FONT_SIZE * 2;
		onvif_osd.ex.on_right_side	= 0;
		break;
	case 2:	//leftup
		onvif_osd.ex.xpos_main	= OSD_EX_FONT_SIZE * 2;
		onvif_osd.ex.ypos_main	= 0;
		onvif_osd.ex.xpos_sub	= OSD_EX_FONT_SIZE;
		onvif_osd.ex.ypos_sub	= 0;
		onvif_osd.ex.on_right_side	= 0;
		break;
	case 3:	//rightup
		onvif_osd.ex.xpos_main	= main_width;
		onvif_osd.ex.ypos_main	= 0;
		onvif_osd.ex.xpos_sub	= sub_width;
		onvif_osd.ex.ypos_sub	= 0;
		onvif_osd.ex.on_right_side	= 1;
		break;
	case 4:	//rightdown
		onvif_osd.ex.xpos_main	= main_width;
		onvif_osd.ex.ypos_main	= main_height - OSD_EX_FONT_SIZE * 2 * 2;
		onvif_osd.ex.xpos_sub	= sub_width;
		onvif_osd.ex.ypos_sub	= sub_height - OSD_EX_FONT_SIZE * 2;
		onvif_osd.ex.on_right_side	= 1;
		break;
	default:
		break;
	}

	/* create osd thread to update osd */
	ak_thread_create(&onvif_osd.tid, onvif_osd_thread,
		vi_handle, 100*1024, -1);
}
#endif

#if ONVIF_TEST_ISP_EFFECT
/* isp effect test function, show menu */
static void show_select_menu(void)
{
	printf("\n--------------- user set menu ---------------\n");
	ak_print_normal("\t 0. effect HUE, value range: [-50, 50]\n");
	ak_print_normal("\t 1. effect brightness, value range: [-50, 50]\n");
	ak_print_normal("\t 2. effect saturation, value range: [-50, 50]\n");
	ak_print_normal("\t 3. effect contrast, value range: [-50, 50]\n");
	ak_print_normal("\t 4. effect sharp, value range: [-50, 50]\n");
	ak_print_normal("\t 5. isp style ID, value range: [0, 2]\n");
	ak_print_normal("\t 6. power Hz, value range: 50 or 60\n");
	ak_print_normal("\t 7. flip mirror\n");
	ak_print_normal("\t 8. exit menu\n");
	ak_print_normal("--------------- user menu end ---------------\n");
}

/* isp effect test thread */
static void *test_isp_effect(void *arg)
{
	pthread_detach(pthread_self());
	ak_print_normal_ex("thread id: %ld\n", ak_thread_get_tid());
	ak_thread_set_name("isp_effect");

	int menu_no = -1;
	int value = -1;

	/* get user set input info */
	while (onvif_ctrl.onvif_started) {
		show_select_menu();
		ak_print_normal("please select your function number:\n");
		if(scanf("%d", &menu_no) <= 0){
			getchar();
			continue;
		}
		getchar();

		/* parse input and do difference operate */
		switch(menu_no){
		case CTRL_TYPE_EFFECT_HUE:			//hue
		case CTRL_TYPE_EFFECT_BRIGHTNESS:	//brightness
		case CTRL_TYPE_EFFECT_SATURATION:	//saturation
		case CTRL_TYPE_EFFECT_CONTRAST:		//contrast
		case CTRL_TYPE_EFFECT_SHARP:		//sharp
		case CTRL_TYPE_STYLE_ID:			//select style id
		case CTRL_TYPE_POWER_HZ:			//power frequence
			ak_print_normal("please set your value:\n");
			if(scanf("%d", &value) <= 0){
				getchar();
				continue;
			}
			getchar();
			ak_print_normal("your input value is: %d\n", value);
			ak_vi_set_effect(onvif_ctrl.vi_handle, menu_no, value);
			break;
		case 7:	/* flip or error */
			{
				int flip = -1;
				int mirror = -1;

				ak_print_normal("please set flip & mirror value:\n");
				if(scanf("%d%d", &flip, &mirror) <= 0){
					getchar();
					continue;
				}
				getchar();
				ak_print_normal("your input value, flip=%d, mirror=%d\n",
					flip, mirror);
				/* set flip and mirror effect */
				ak_vi_set_flip_mirror(onvif_ctrl.vi_handle, flip, mirror);
			}
			break;
		case 8:
			goto isp_effect_end;	//exit
		default:
			ak_sleep_ms(500);
			break;
		}
		/* reset value */
		menu_no = -1;
		value = -1;
	}

isp_effect_end:
	ak_print_normal("%s thread stopped\n", __func__);
	ak_thread_exit();
	return NULL;
}
#endif

/*
 * send_stream2onvif - send stream to onvif
 * chn[IN]: indicate main or sub channel, 0 -> main, 1 -> sub
 * stream[IN]: pointer to stream which would be sent to onvif
 */
static void send_stream2onvif(int chn, struct video_stream *stream)
{
    int iframe_flag, res;
    unsigned int tv_sec, tv_usec, pts;
	int *frame_index = NULL;
	buffers_t *send_bufs = NULL;

	/* drop invalid frame */
	if (stream->len == 0 || stream->ts == 0) {
		ak_print_error_ex("Error!!, len: %u, ts: %llu\n",
			   	stream->len, stream->ts);
		return ;
	}

	/* get buf and index */
	if (chn == 0) {
		send_bufs = &(g_dvs.chunnel[0].video_send_bufs);
		frame_index = &frame_index_m;
	} else if (chn == 1) {
		send_bufs = &(g_dvs.chunnel[0].video_send_bufs_small);
		frame_index = &frame_index_s;
	}
	/* timestamp convert */
    tv_sec = stream->ts / 1000;
    tv_usec = (stream->ts % 1000) * 1000;
    pts = tv_sec * 90000 + tv_usec *9 / 100;

	/* I or p frame flag */
    if (stream->frame_type == 1) {	//I frame
		iframe_flag = 0x02;
    	*frame_index = 0;
    } else {
       	iframe_flag = 0;	//p frame
    	*frame_index+=1;
    }
	/* send to onvif internal buffer */
    res = buffers_put_data_with_pts (stream->data, stream->len,
      		send_bufs, iframe_flag, 0, *frame_index, stream->ts);
      		//send_bufs, iframe_flag, 0, *frame_index, pts);
    if (res < 1) { //buffer full
    	if (*frame_index == 0)	{ //IFrame
    		ak_print_error_ex("onvif buffers full, clear_data !\r\n");
    		buffers_clear_data (send_bufs);
    		buffers_put_data_with_pts (stream->data, stream->len,
        		send_bufs, iframe_flag, 0, *frame_index, pts);
    	}
    }
}

/*
 * onvif_init_venc - init onvif video encode
 * index[IN]: to indicate main or sub channel
 * return: on success, ruturn venc handle, failed return NULL;
 */
static void *onvif_init_venc(int index)
{
	struct encode_param param = {0};

	switch (index) {
	case ENCODE_MAINCHN_NET:	/* main channel */
		param.width = onvif_ctrl.param.main_width;
		param.height = onvif_ctrl.param.main_height;
		param.minqp = onvif_ctrl.param.min_qp;
		param.maxqp = onvif_ctrl.param.max_qp;
		param.fps = onvif_ctrl.param.main_fps;
		param.goplen = onvif_ctrl.param.main_fps * onvif_ctrl.param.gop_len;
		param.bps = onvif_ctrl.param.main_kbps;	//kbps
		param.profile = PROFILE_MAIN;	//default main profile
		param.use_chn = ENCODE_MAIN_CHN;
		param.enc_grp = ENCODE_MAINCHN_NET;
		param.br_mode = onvif_ctrl.param.main_video_mode;
		param.enc_out_type = H264_ENC_TYPE;
		break;
	case ENCODE_SUBCHN_NET:	 /* sub channel */
		param.width = onvif_ctrl.param.sub_width;
		param.height = onvif_ctrl.param.sub_height;
		param.minqp = onvif_ctrl.param.min_qp;
		param.maxqp = onvif_ctrl.param.max_qp;
		param.fps = onvif_ctrl.param.sub_fps;
		param.goplen = onvif_ctrl.param.sub_fps * onvif_ctrl.param.gop_len;
		param.bps = onvif_ctrl.param.sub_kbps;	//kbps
		param.profile = PROFILE_MAIN;
		param.use_chn = ENCODE_SUB_CHN;
		param.enc_grp = ENCODE_SUBCHN_NET;
		param.br_mode = onvif_ctrl.param.sub_video_mode;
		param.enc_out_type = H264_ENC_TYPE;
		break;
	default:
		return NULL;
		break;
	}

	/* init venc */
	return ak_venc_open(&param);
}

#if ONVIF_GET_STREAM_TIME
static void calc_get_stream_time(int ch, struct onvif_stream *onvif_str,
                                struct video_stream *stream)
{
    struct ak_timeval cur_time;
    long over_time = 130;

    if (onvif_str->fps > 0) {
        over_time = ((1000 / onvif_str->fps) * 2);
    }

    ak_get_ostime(&cur_time);
    long stream_time = ak_diff_ms_time(&cur_time, &(onvif_str->get_time));
    if (stream_time > over_time) {
        ak_print_normal("[ak_onvif]: channel=%d, get stream time=%ld(ms), "
            "seq_no=%lu, %s frame\n",
            ch, stream_time, stream->seq_no,
            (FRAME_TYPE_I == stream->frame_type)?"I" : "P");
    }
	ak_get_ostime(&(onvif_str->get_time));
}
#endif

/* onvif_get_data - onvif main working thread, get stream to onvif */
static void *onvif_get_data(void *arg)
{
	ak_print_normal_ex("thread id: %ld\n", ak_thread_get_tid());
	ak_thread_set_name("onvif_data");

	int chn = VIDEO_CHN_NUM;
	int i = 0;
	struct video_stream stream = {0};
	struct onvif_stream onvif_str[VIDEO_CHN_NUM];

	/* request main and sub stream */
	onvif_ctrl.stream_main = ak_venc_request_stream(onvif_ctrl.vi_handle,
		   	onvif_ctrl.venc_main);
	onvif_ctrl.stream_sub = ak_venc_request_stream(onvif_ctrl.vi_handle,
		   	onvif_ctrl.venc_sub);
	ak_print_notice_ex("stream_main handle=%p, stream_sub handle=%p\n\n",
					onvif_ctrl.stream_main, onvif_ctrl.stream_sub);

	if (!onvif_ctrl.stream_main)
		onvif_ctrl.onvif_started = 0;
	if (!onvif_ctrl.stream_sub)
		chn = 1;
	onvif_str[VIDEO_CHN_MAIN].stream_handle = onvif_ctrl.stream_main;
	onvif_str[VIDEO_CHN_SUB].stream_handle = onvif_ctrl.stream_sub;

#if AK_ONVIF_OSD_ENABLE
	/* osd config */
	int osd_rate = onvif_ctrl.param.osd_rate;
	struct osd_ex_info osd_ex = {0};
	memcpy(osd_ex.name, onvif_osd.ex.name, sizeof(onvif_osd.ex.name));

	osd_ex.on_right_side = onvif_osd.ex.on_right_side;
	osd_ex.xpos_main = onvif_osd.ex.xpos_main;
	osd_ex.ypos_main = onvif_osd.ex.ypos_main;
	osd_ex.xpos_sub = onvif_osd.ex.xpos_sub;
	osd_ex.ypos_sub = onvif_osd.ex.ypos_sub;
#endif

#if ONVIF_GET_STREAM_TIME
    onvif_str[VIDEO_CHN_MAIN].fps = onvif_ctrl.param.main_fps;
    onvif_str[VIDEO_CHN_SUB].fps = onvif_ctrl.param.sub_fps;
    for (i = 0; i < VIDEO_CHN_NUM; i++) {
        ak_get_ostime(&onvif_str[i].get_time);
    }
#endif

	/* iterate main and sub channel, get stream and set to onvif */
	while (onvif_ctrl.onvif_started) {
		for (i = 0; i < chn; i++) {
			/* get stream until success */
			if (ak_venc_get_stream(onvif_str[i].stream_handle, &stream)) {
				ak_sleep_ms(10);
				continue;
			}
#if ONVIF_GET_STREAM_TIME
            calc_get_stream_time(i, &onvif_str[i], &stream);
#endif
#if AK_ONVIF_OSD_ENABLE
			/* show osd rate info */
			if (osd_rate) {
				void *venc_stream = ((VIDEO_CHN_MAIN == i) ?
				    onvif_ctrl.stream_main : onvif_ctrl.stream_sub);
				ak_osd_ex_stat_video(i, venc_stream);
			}
#endif
			/* send stream */
			send_stream2onvif(i, &stream);
			/* release resource */
			ak_venc_release_stream(onvif_str[i].stream_handle, &stream);
		}
	}
	ak_print_normal_ex("### thread id: %ld exit ###\n", ak_thread_get_tid());
	ak_thread_exit();
	return NULL;
}

/* onvif_stop_video - stop onvif video, release resource */
static void onvif_stop_video(void)
{
#if AK_ONVIF_OSD_ENABLE
	/* release osd resource */
	ak_print_normal_ex("join onvif osd thread ...\n");
	onvif_osd.run_flag = 0;
	ak_sleep_ms(10);
	ak_thread_join(onvif_osd.tid);
	ak_print_normal_ex("onvif osd thread exit!\n");
#endif

	onvif_ctrl.onvif_started = 0;
	ak_sleep_ms(10);

	/* wait onvif main stream thread exit */
	ak_print_normal_ex("join onvif data thread ...\n");
	ak_thread_join(onvif_ctrl.onvif_tid);
	ak_print_normal_ex("onvif data thread exit!\n");

	/* onvif ops handle unregister */
	onvif_server_unregister_handle();

	/* cancel streams */
	if (onvif_ctrl.venc_main && onvif_ctrl.stream_main) {
		ak_print_normal_ex("stop onvif main channel video\n");
		ak_venc_cancel_stream(onvif_ctrl.stream_main);
		onvif_ctrl.stream_main = NULL;

		/* close video encode */
		ak_venc_close(onvif_ctrl.venc_main);
		onvif_ctrl.venc_main = NULL;
	}

	/* cancel streams */
	if (onvif_ctrl.venc_sub && onvif_ctrl.stream_sub) {
		ak_print_normal_ex("stop onvif sub channel video\n");
		ak_venc_cancel_stream(onvif_ctrl.stream_sub);
		onvif_ctrl.stream_sub = NULL;

		/* close video encode */
		ak_venc_close(onvif_ctrl.venc_sub);
		onvif_ctrl.venc_sub = NULL;
	}
}
/*
 * onvif_start_video - start onvif video service
 * vi_handle[IN]:	video input handle
 * return: 0 on success, -1 failed
 */
static int onvif_start_video(void *vi_handle)
{
	onvif_ctrl.vi_handle = vi_handle;
	if (!onvif_ctrl.vi_handle) {
		ak_print_error_ex("open vi handle failed\n");
		return -1;
	}

	/* init main channal */
	onvif_ctrl.venc_main = onvif_init_venc(ENCODE_MAINCHN_NET);
	if (!onvif_ctrl.venc_main) {
		ak_print_error_ex("open vi handle failed\n");
		return -1;
	}

	/* set video control method */
	ak_venc_set_method(onvif_ctrl.venc_main, onvif_ctrl.param.method);

	/* init sub channal */
	onvif_ctrl.venc_sub = onvif_init_venc(ENCODE_SUBCHN_NET);
	if (!onvif_ctrl.venc_sub) {
		ak_print_error_ex("open sub vi handle failed\n");
		ak_venc_close(onvif_ctrl.venc_main);
		return -1;
	}
	/* set video control method */
	ak_venc_set_method(onvif_ctrl.venc_sub, onvif_ctrl.param.method);

	/* create onvif main stream thread */
	onvif_ctrl.onvif_started = 1;
	ak_thread_create(&onvif_ctrl.onvif_tid, onvif_get_data, NULL,
		100*1024, -1);

	struct video_handle_t video = {NULL};

	video.vi_handle = vi_handle;
	video.main_chn = onvif_ctrl.venc_main;
	video.sub_chn = onvif_ctrl.venc_sub;
	video.start_func = onvif_start_video;
	video.stop_func = onvif_stop_video;

	/* register onvif ops callbacks */
	if (onvif_server_register_handle(&video)) {
		ak_print_error_ex("register video handle for onvif failed\n");
	}

#if AK_ONVIF_OSD_ENABLE
	/* init onvif osd if needed */
	onvif_osd_init(vi_handle);
#endif

	return 0;
}

/**
 * ak_onvif_init - init onvif app
 * @param[IN]: onvif config param
 * return: void
 */
static void ak_self_onvif_init(const struct onvif_param *param)
{
    ak_print_normal_ex("enter...\n"
   			"##########################################\n"
			"## ak onvif version: %s ##\n"
   			"##########################################\n",
			ak_self_onvif_get_version());

	if (onvif_ctrl.onvif_started) {
		ak_print_error_ex("onvif has been opened before\n");
		return;
	}

    memcpy(&onvif_ctrl.param, param, sizeof(onvif_ctrl.param));
    onvif_server_init_video_param(&onvif_ctrl.param);

#if SHOW_ONVIF_PARAM
    show_onvif_param();
#endif

	/* init onvif */
	int ret = init_onvif(param->main_width, param->main_height);
	ak_print_notice_ex("init onvif, ret: %d\n", ret);
	if (ret) {
		ak_print_error_ex("\n\t---- init onvif failed ---- !\n");
		return;
	}

	/* start onvif lib service */
	start_onvif();

	/* start video service */
	if (onvif_start_video(param->vi_handle)) {
		ak_print_error_ex("onvif start video failed\n");
		return;
	}

#if ONVIF_TEST_ISP_EFFECT
	/* create test isp effect thread if needed */
	ak_pthread_t effect_tid;
	ak_thread_create(&effect_tid, test_isp_effect, NULL,
		ANYKA_THREAD_NORMAL_STACK_SIZE, -1);
#endif
}

/* onvif switch gop len */
void ak_onvif_switch_gop(void)
{
	int fps = 0;
	int goplen = 0;

	/* get main channal fps and calculate goplen, then switch it */
	if (onvif_ctrl.venc_main) {
		fps = ak_venc_get_fps(onvif_ctrl.venc_main);
		goplen = fps * onvif_ctrl.param.gop_len;
		ak_venc_set_gop_len(onvif_ctrl.venc_main, goplen);
		ak_print_normal_ex("main chn goplen: %d, fps: %d, %d\n",
				goplen, fps, onvif_ctrl.param.gop_len);
	}

	/* get sub channal fps and calculate goplen, then switch it */
	if (onvif_ctrl.venc_sub) {
		fps = ak_venc_get_fps(onvif_ctrl.venc_sub);
		goplen = fps * onvif_ctrl.param.gop_len;
		ak_venc_set_gop_len(onvif_ctrl.venc_sub, goplen);
		ak_print_normal_ex("sub chn goplen: %d, fps: %d, %d\n",
				goplen, fps, onvif_ctrl.param.gop_len);
	}
}

/* ak_onvif_exit - exit onvif video service */
static void ak_self_onvif_exit(void)
{
#if AK_ONVIF_OSD_ENABLE
	onvif_osd.run_flag = 0;
#endif

	if (onvif_ctrl.onvif_started) {
		onvif_stop_video();
		ak_print_normal("***** onvif exit ok *****\n");
	}
}

static void process_signal(unsigned int sig, siginfo_t *si, void *ptr)
{
	if (onvif_run_flag) {
		char res[2] = {0};
		ak_cmd_exec("rm -f /tmp/core_*", res, 2);

 		ak_backtrace(sig, si, ptr);
 	}

	if((SIGSEGV == sig) || (SIGTERM == sig) || (SIGINT == sig)){
		onvif_run_flag = 0;
	}
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

static int init_vi(void)
{
	/* open device */
	vi_handle = ak_vi_open(VIDEO_DEV0);
	if (NULL == vi_handle) {
		ak_print_error_ex("vi open failed\n");
		return AK_FAILED;
	}
	//ak_vpss_init(vi_handle, VIDEO_DEV0);

	struct video_resolution resolution = {0};
	if (ak_vi_get_sensor_resolution(vi_handle, &resolution))
		ak_print_error_ex("get sensor resolution failed\n");

	/* set crop information */
	struct video_channel_attr attr;
	memset(&attr, 0x00, sizeof(attr));
	attr.res[VIDEO_CHN_MAIN].width = resolution.width;
	attr.res[VIDEO_CHN_MAIN].height = resolution.height;
	attr.res[VIDEO_CHN_SUB].width = 640;
	attr.res[VIDEO_CHN_SUB].height = 480;
	attr.res[VIDEO_CHN_MAIN].max_width = 640;
	attr.res[VIDEO_CHN_MAIN].max_height = 480;
	attr.res[VIDEO_CHN_SUB].max_width = 640;
	attr.res[VIDEO_CHN_SUB].max_height = 480;
	attr.crop.left = 0;
	attr.crop.top = 0;
	attr.crop.width = resolution.width;
	attr.crop.height = resolution.height;

	if (ak_vi_set_channel_attr(vi_handle, &attr)) {
		ak_print_error_ex("set channel attribute failed\n");
	}

	/* get crop */
	struct video_channel_attr cur_attr;

	memset(&cur_attr, 0x00, sizeof(cur_attr));
	if (ak_vi_get_channel_attr(vi_handle, &cur_attr)) {
		ak_print_normal("ak_vi_get_channel_attr failed!\n");
	}

	ak_print_normal_ex("capture fps: %d\n", ak_vi_get_fps(vi_handle));
	ak_print_normal("\nvideo input info:\n"
			"\tmain_w[%d], main_h[%d], sub_w[%d], sub_h[%d]\n\n",
			cur_attr.res[VIDEO_CHN_MAIN].width,
			cur_attr.res[VIDEO_CHN_MAIN].height,
			cur_attr.res[VIDEO_CHN_SUB].width,
		   	cur_attr.res[VIDEO_CHN_SUB].height);

	return AK_SUCCESS;
}

/* match sensor according to ISP config file path */
static int match_sensor(const char *isp_cfg_path)
{
	DIR *dir = opendir(isp_cfg_path);
	if (NULL == dir) {
		ak_print_normal_ex("it fails to open directory %s\n", isp_cfg_path);
		return 0;
	}

	int ret = AK_FAILED;
	char isp_file[255] = {0};
	char *tmp = NULL;
	char *isp_cfg = NULL;
	struct dirent *dir_ent = NULL;

	while (NULL != (dir_ent = readdir(dir))) {
		if (!dir_ent->d_name)
			continue;
		/* fine next when we get dir */
        if ((dir_ent->d_type & DT_DIR))
            continue;

		/* make sure use isp_*.conf file to match */
		tmp = strstr(dir_ent->d_name, "isp_");
		if (!tmp)
			continue;
		isp_cfg = strstr(tmp, ".conf");
		if (!isp_cfg)
			continue;

		sprintf(isp_file, "%s%s", isp_cfg_path, dir_ent->d_name);
		/* get sensor id, match config file */
		if(AK_SUCCESS == ak_vi_match_sensor(isp_file)) {
			ak_print_notice_ex("ak_vi_match_sensor OK\n");
			ret = AK_SUCCESS;

			if (strcmp(isp_cfg_path, FIRST_PATH)) {
				char cmd[128] = {0};
				char result[2] = {0};

				sprintf(cmd, "cp %s %s", isp_file, FIRST_PATH);
				ak_cmd_exec(cmd, result, 2);

				sprintf(isp_file, "%s%s", FIRST_PATH, dir_ent->d_name);
				ak_vi_match_sensor(isp_file);
			}
			break;
		}
	}
	closedir(dir);

	return ret;
}

static int init_video(void)
{
	/* match sensor at the first step */
	if (AK_FAILED == match_sensor(FIRST_PATH)) {
		ak_print_warning_ex("match sensor FIRST_PATH failed\n");
		if (AK_FAILED == match_sensor(BACK_PATH)) {
			ak_print_error_ex("match sensor BACK_PATH failed\n");
			return AK_FAILED;
		}
	}

	int ret = AK_FAILED;

	/* one video input device, only open one time vi for encode */
	if (init_vi()) {
		ak_print_error_ex("video input init faild, exit\n");
	} else {
		ak_print_notice_ex("start capture ...\n");
		if(ak_vi_capture_on(vi_handle)) {
			ak_print_error_ex("vi capture on failed\n");
		} else {
			ret = AK_SUCCESS;
		}
	}

	if ((AK_FAILED == ret) && (NULL != vi_handle)) {
		ak_vi_close(vi_handle);
		vi_handle = NULL;
		//ak_vpss_destroy(VIDEO_DEV0);
	}

	return ret;
}

static void set_onvif_net_param(const struct onvif_net_param *net)
{
	ak_print_normal_ex("set net: dhcp=%d, ip=%s, netmask=%s, gateway=%s"
			"dns1=%s, dns2=%s\n", net->dhcp, net->ipaddr, net->netmask,
			net->gateway, net->firstdns, net->backdns);
	/* do not set on demo */
}

static void set_onvif_video_param(const struct onvif_video_param *video)
{
    if (VIDEO_CHN_MAIN == video->channel) {
		ak_print_normal_ex("set main channel: fps=%d, kbps=%d, w=%d, h=%d\n",
				video->main_kbps, video->main_fps,
				video->main_width, video->main_height);
    } else {
		ak_print_normal_ex("set sub channel: fps=%d, kbps=%d, w=%d, h=%d\n",
				video->sub_kbps, video->sub_fps,
				video->sub_width, video->sub_height);
    }

	/* do not set on demo */
}


static void init_onvif_demo(void)
{
    struct onvif_param param = {0};
    /* camera config */
    param.main_width = 1280;
    param.main_height = 720;
    param.sub_width = 640;
    param.sub_height = 480;

    /* osd config */
	param.osd_rate = 0;
	param.osd_position = 0;
    param.osd_switch = 0;
    strcpy(param.osd_name, "onvif demo");
    param.time_switch = 1;
    param.date_format = 1;
    param.hour_format = 1;
    param.week_format = 1;

    /* encode config*/
    param.gop_len = 2;
    param.min_qp = 20;
    param.max_qp = 51;
    param.main_video_mode = 0;	//cbr
    param.sub_video_mode = 0;	//cbr
    param.method = 0;

    /* onvif config */
    param.main_fps = 25;
    param.main_kbps = 1500;
    param.sub_fps = 25;
    param.sub_kbps = 800;

    param.net_cb = set_onvif_net_param;
    param.video_cb = set_onvif_video_param;
    param.vi_handle = vi_handle;

    ak_self_onvif_init(&param);
}

static void onvif_demo_exit(void)
{
	ak_print_normal_ex("********** entering **********\n");
	/* set log level to info, to dump more message */
	int old_level = ak_print_set_level(LOG_LEVEL_INFO);

	ak_self_onvif_exit();

	ak_print_notice_ex("*** ready to close vi ***\n");
	//ak_vpss_destroy(VIDEO_DEV0);
	if (vi_handle) {
		ak_vi_close(vi_handle);
		vi_handle = NULL;
	}
	ak_cmd_server_unregister(ANYKA_IPC_PORT);
	ak_print_set_level(old_level);

	ak_print_normal_ex("********** onvif demo exit OK **********\n");
}

int main(void)
{
	register_signal();

	/* init ipc_srv */
	//ak_cmd_server_register(ANYKA_IPC_PORT, "onvif_demo");
	/* just let we know what interface we use now */
	ak_net_get_cur_iface(NULL);

	onvif_run_flag = 1;

	if (init_video()) {
		ak_print_error_ex("init video failed, exit...\n");
		return -1;
	}

	//onvif_cmd_init();
	onvif_discovery_init();
	init_onvif_demo();

	while (onvif_run_flag) {
		ak_sleep_ms(1000);
	}

	//onvif_cmd_exit();
	onvif_discovery_exit();
	onvif_demo_exit();
}
