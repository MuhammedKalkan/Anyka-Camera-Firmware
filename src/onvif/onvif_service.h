#ifndef _ONVIF_SERVICE_H_
#define _ONVIF_SERVICE_H_

#include "onvif_demo.h"

struct netinfo_t {
	unsigned short IpType;//0:Fixed IP  1:dynamic ip
	unsigned short DnsType;//0:no  1:yes
	char cIp[16];//local ip
	char cNetMask[16];//net mask
	char cGateway[16];
	char cMDns[16];//main dns
	char cSDns[16];//second dns
	unsigned short chttpPort;//http port
};

typedef int (*onvif_video_start)(void *vi_handle);
typedef void (*onvif_video_stop)(void);

struct onvif_service_param {
    struct onvif_video_param video;

    /* callback */
    onvif_set_net_cb net_cb;
    onvif_set_video_cb video_cb;
};

struct video_handle_t {
	void *vi_handle;
	void *main_chn;
	void *sub_chn;
	onvif_video_start start_func;
	onvif_video_stop stop_func;
	int onvif_server_runflag;
};

/**
 * NAME        SetNet
 * @BRIEF	set net data

 * @PARAM	struct  netinfo_t *resp, info you want to set
 * @RETURN	int
 * @RETVAL  0
 */
int SetNet(struct  netinfo_t *resp);

/**
 * NAME        init_network
 * @BRIEF	init net

 * @PARAM	dvs_t * dvs, strore info
 * @RETURN	int
 * @RETVAL	0
 */
int init_network(dvs_t * dvs);

/**
 * NAME        GetNet
 * @BRIEF	get net data

 * @PARAM	struct  netinfo_t *resp, strore info
 * @RETURN	int
 * @RETVAL	if return 0 success, otherwise failed
 */
int GetNet(struct  netinfo_t *resp);

/**
 * NAME        hal_set_brightness
 * @BRIEF	set brightness

 * @PARAM	dvs_t *dvs,
 			int index,
 			int brightness
 * @RETURN	int
 * @RETVAL	0
 */
int hal_set_brightness(dvs_t *dvs, int index, int brightness);

/**
 * NAME        hal_set_contrast
 * @BRIEF	set contrast

 * @PARAM	dvs_t *dvs,
 			int index,
 			int brightness
 * @RETURN	int
 * @RETVAL	0
 */
int hal_set_contrast(dvs_t *dvs, int index, int contrast);

/**
 * NAME        hal_set_saturation
 * @BRIEF	set saturation

 * @PARAM	dvs_t *dvs,
 			int index,
 			int brightness
 * @RETURN	int
 * @RETVAL	0
 */
int hal_set_saturation(dvs_t *dvs, int index, int saturation);

/**
 * NAME        GetVideoPara
 * @BRIEF	get vadie para

 * @PARAM	int *fps,  fps
 			int *bitrate, bitrate
 			int *quality, quality
 			int *flage, flage
			int *width, width
			int *height, height
			int main_small, main or small
 * @RETURN	int
 * @RETVAL	0
 */
int GetVideoPara(int *fps, int *bitrate, int *quality, int *flage,
        int *width, int *height, int main_small);

/**
 * NAME        SetVideoPara
 * @BRIEF	set vadie para

 * @PARAM	int *fps,  fps
 			int *bitrate, bitrate
 			int *quality, quality
 			int *flage, flage
			int *width, width
			int *height, height
			int main_small, main or small
 * @RETURN	int
 * @RETVAL	0
 */
int SetVideoPara(int fps, int bitrate, int quality,
        int width, int height, int flage);

/**
 * NAME        IniReadString
 * @BRIEF	read config ini file string

 * @PARAM	const char *section, section name
 			const char *indent,	indent name
	       	const char *defaultresult,  default result when not found the string
	       	char *result, pointer to result
	       	int resultlen, lengh of result
	       	char *inifilename, INI file name
 * @RETURN	int
 * @RETVAL	if return 0 success, otherwise failed
 */
int IniReadString(const char *section, const char *indent,
        const char *defaultresult, char *result, int resultlen,
        char *inifilename);

/**
 * NAME        IniReadInteger
 * @BRIEF	read config ini file interger

 * @PARAM	const char *section, section name
 			const char *indent,	indent name
	       	const char *defaultresult,  default result when not found the string
	       	char *inifilename, INI file name
 * @RETURN	int
 * @RETVAL	if return 0 success, otherwise failed
 */
int IniReadInteger(const char *section, const char *indent, int defaultresult,
        char *inifilename);

/**
 * NAME        IniWriteString
 * @BRIEF	write config ini file string

 * @PARAM	const char *section, section name
 			const char *indent,	indent name
	       	const char *value, value which you want to write
	       	char *inifilename, INI file name
 * @RETURN	int
 * @RETVAL	if return 0 success, otherwise failed
 */
int IniWriteString(const char *section, const char *indent, const char *value,
        char *inifilename);

/**
 * NAME        IniWriteInteger
 * @BRIEF	write config ini file interger

 * @PARAM	const char *section, section name
 			const char *indent,	indent name
	       	const char *value, value which you want to write
	       	char *inifilename, INI file name
 * @RETURN	int
 * @RETVAL	if return 0 success, otherwise failed
 */
int IniWriteInteger(const char *section, const char *indent, int value,
        char *inifilename);

/**
 * NAME        config_load_videocolor
 * @BRIEF	load video recorder config

 * @PARAM	config_videocolor_t * videocolor, store info
 			int index, intex
 * @RETURN	int
 * @RETVAL	0
 */
int config_load_videocolor(config_videocolor_t * videocolor, int index);

/**
 * NAME        GetOnvifData
 * @BRIEF	get onvfi config

 * @PARAM	onvif_data_t *onvif_data

 * @RETURN	void
 * @RETVAL
 */
int GetOnvifData(onvif_data_t *onvif_data);

/**
 * NAME        SetOnvifData
 * @BRIEF	set onvfi config

 * @PARAM	onvif_data_t *onvif_data

 * @RETURN	void
 * @RETVAL
 */
int SetOnvifData(onvif_data_t *onvif_data);

int onvif_server_init_video_param(struct onvif_param *param);

int onvif_server_register_handle(struct video_handle_t *video);

void onvif_server_unregister_handle(void);

#endif
