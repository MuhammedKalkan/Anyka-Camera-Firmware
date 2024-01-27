#ifndef _AK_ONVIF_SELF_H_
#define _AK_ONVIF_SELF_H_

#include "ak_vi.h"

struct onvif_net_param {
	int dhcp;			//0:Fixed IP  1:dynamic ip
	char ipaddr[16];	//local ip
	char netmask[16];	//net mask
	char gateway[16];
	char firstdns[16];	//main dns
	char backdns[16];	//second dns
};

struct onvif_video_param {
    enum video_channel channel;

	/* camera config */
    int main_width;         /* main channel res width */
    int main_height;        /* main channel res height */
    int sub_width;          /* sub channel res width */
    int sub_height;         /* sub channel res height */

    int main_fps;
    int main_kbps;
    int sub_fps;
    int sub_kbps;
};

/* callback for writing net param to config file */
typedef void (*onvif_set_net_cb)(const struct onvif_net_param *net);
/* callback for writing video param to config file */
typedef void (*onvif_set_video_cb)(const struct onvif_video_param *video);


struct onvif_param {
    void *vi_handle;        /* opened vi handle */

    /* camera config */
    int main_width;         /* main channel res width */
    int main_height;        /* main channel res height */
    int sub_width;          /* sub channel res width */
    int sub_height;         /* sub channel res height */

    /* osd config */
	int osd_rate;		    /* display rate info OSD */
	int osd_position;
    int osd_switch;
    char osd_name[50];
    int time_switch;
    int date_format;
    int hour_format;
    int week_format;

    /* encode config*/
    int gop_len;            /* encode GOP len */
    int min_qp;
    int max_qp;
    int main_video_mode;
    int sub_video_mode;
    int method;

    /* onvif config */
    int main_fps;
    int main_kbps;
    int sub_fps;
    int sub_kbps;

    /* callback */
    onvif_set_net_cb net_cb;
    onvif_set_video_cb video_cb;
};

#endif
