#ifndef _AK_ONVIF_CONFIG_H_
#define _AK_ONVIF_CONFIG_H_

struct onvif_sys_config {
    char dev_name[100];				//device name
	unsigned int soft_version;
	int	tzone;
};

struct onvif_venc_config {
	int main_enc_type;
	int sub_enc_type;
	int main_video_mode;
	int sub_video_mode;

	int main_fps;
    int sub_fps;
	int main_kbps;
    int sub_kbps;
	
    int main_goplen;
	int sub_goplen;

	int main_smart_mode;
	int sub_smart_mode;
	int main_smart_goplen;
	int sub_smart_goplen;
	
    int main_min_qp_264;
	int sub_min_qp_264;
	int main_min_qp_265;
	int sub_min_qp_265;
	
    int main_max_qp_264;
	int sub_max_qp_264;
	int main_max_qp_265;
	int sub_max_qp_265;

	int main_targetratio_264;
	int sub_targetratio_264;
	int main_targetratio_265;
	int sub_targetratio_265;
	
	int main_targetratio_264_smart;
	int sub_targetratio_264_smart;
	int main_targetratio_265_smart;
	int sub_targetratio_265_smart;

	int main_smart_quality_264;
	int sub_smart_quality_264;
	int main_smart_quality_265;
	int sub_smart_quality_265;
	
	int smart_static_value;
};

struct onvif_net_config {
	int dhcp;			//0:Fixed IP  1:dynamic ip
	char ipaddr[16];	//local ip
	char netmask[16];	//net mask
	char gateway[16];
	char firstdns[16];	//main dns
	char backdns[16];	//second dns
	int ip_adjust_en;
	int hikonport;
};

struct onvif_camera_config {
    int main_width;
    int main_height;
    int sub_width;
    int sub_height;
	int main_max_width;
    int main_max_height;
    int sub_max_width;
    int sub_max_height;
	int main_osd_size;
    int sub_osd_size;
	int name_position;
    int time_position;
	int rate_position;
    int osd_switch;
    char osd_name[50];
    int date_format;
    int hour_format;
    int week_format;
};

struct onvif_wifi_config {
	char ssid[32];
	char passwd[32];
};

struct onvif_image_config {
    int	hue;
	int	brightness;
	int	saturation;
	int	contrast;
	int	sharp;

	int	flip;
	int	mirror;

	int ircut_mode;
	int	force_anti_flicker_flag;
	int irled_mode;
};

struct onvif_audio_config {
    int	ai_source;
	int	aenc_type;
	int ai_enable;
};

struct onvif_alarm_config {
    int md_level_1;
    int md_level_2;
    int md_level_3;
	int md_level_4;
    int md_level_5;
};

/**
 * onvif_config_get_sys: get system info config
 * return: system info config struct pointer
 */
struct onvif_sys_config* onvif_config_get_sys(void);

/**
 * onvif_config_set_sys: set system info config
 * @sys[IN]: system info config
 * return: void
 */
void onvif_config_set_sys(const struct onvif_sys_config *sys);

/**
 * onvif_config_get_venc_param: get n1 video param config, title: video
 * @void
 * return: video param config info pointer
 */
struct onvif_venc_config* onvif_config_get_venc_param(void);

/**
 * onvif_config_set_venc_param: set video config param, title: video
 * @video[IN]: video config param
 * return: void
 */
void onvif_config_set_venc_param(const struct onvif_venc_config *video);

/**
 * onvif_config_get_net: get n1 network config
 * return: network config struct pointer
 */
struct onvif_net_config* onvif_config_get_net(void);

/**
 * onvif_config_set_net: set n1 network config
 * @net[IN]: network config
 * return: void
 */
void onvif_config_set_net(const struct onvif_net_config *net);

/**
 * onvif_config_get_wifi: get n1 wifi config
 * return: wifi config struct pointer
 */
struct onvif_wifi_config* onvif_config_get_wifi(void);

/**
 * onvif_config_set_wifi: set n1 wifi config
 * @wifi[IN]: wifi config
 * @save_ssid_f[IN]: save ssid or not
 * return: void
 */
void onvif_config_set_wifi(const struct onvif_wifi_config *wifi, int save_ssid);

/**
 * onvif_config_get_camera: get n1 camera config
 * return: camera config struct pointer
 */
struct onvif_camera_config* onvif_config_get_camera(void);

/**
 * onvif_config_set_camera: set camera config
 * @camera[IN]: camera config
 * return: void
 */
void onvif_config_set_camera(const struct onvif_camera_config *camera);

/** onvif_config_get_image: get n1 image config, title: image
 * @void
 * return: image config info pointer
 */
struct onvif_image_config* onvif_config_get_image(void);

/**
 * onvif_config_set_image: set n1 image config, title: image
 * @image[IN]: image config
 * return: void
 */
void onvif_config_set_image(const struct onvif_image_config *image);

/** onvif_config_get_audio: get n1 audio config, title: audio
 * @void
 * return: audio config info pointer
 */
struct onvif_audio_config* onvif_config_get_audio(void);

/**
 * onvif_config_set_audio: set audio config, title: audio
 * @audio[IN]: audio config
 * return: void
 */
void onvif_config_set_audio(struct onvif_audio_config *audio);

/** onvif_config_get_alarm: get n1 alarm config, title: alarm
 * @void
 * return: alarm config info pointer
 */
struct onvif_alarm_config* onvif_config_get_alarm(void);

/**
 * onvif_config_set_alarm: set alarm config, title: alarm
 * @audio[IN]: alarm config
 * return: void
 */
void onvif_config_set_alarm(struct onvif_alarm_config *alarm);

/**
 * onvif_config_init: init N1 config module
 * return: void
 */
void onvif_config_init(void);

/**
 * onvif_config_flush_data: flush data
 * return: void
 */
void onvif_config_flush_data(void);

/**
 * onvif_config_update_ssid:  set wifi ssid to config file
 * return: 0 -> success; -1 -> fail;
 * notes: wifi ssid configure change by script cmd, we need to
 *          update configure copy in memory.
 *          it is special.
 */
int onvif_config_update_ssid(void);

/**
 * onvif_config_release: destroy config module
 * return: void
 */
void onvif_config_release(void);

#endif
