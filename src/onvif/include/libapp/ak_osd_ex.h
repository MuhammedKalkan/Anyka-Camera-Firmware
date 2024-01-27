#ifndef _AK_OSD_EX_H_
#define _AK_OSD_EX_H_

#define OSD_EX_NAME_LEN		50
#define OSD_EX_VIDEO_CH		2

enum osd_rect{
	OSD_NAME_RECT = 0,
	OSD_TIME_RECT,
	OSD_STAT_RECT,
	OSD_MAX_RECT,
};

struct osd_ex_info {
	time_t time_bak;
	/* value can be [0,4], 0 -> off, 2-"leftup",1-"leftdown",3-"rightup",4-"rightdown"*/
	int position[OSD_MAX_RECT];
	char name[OSD_EX_NAME_LEN];
	int date_format;
	int hour_format;
	int week_format;
	int x_width[OSD_EX_VIDEO_CH];
	int y_height[OSD_EX_VIDEO_CH];
	int osd_disp_size[OSD_EX_VIDEO_CH]; /*disp font size*/
	int on_right_side[OSD_MAX_RECT];
};

/**
 * ak_osd_ex_get_version - get osd ex version
 * return: version string
 */
const char* ak_osd_ex_get_version(void);


/**
 * ak_osd_ex_u2g - utf8 convert to gbk
 * @inbuf[IN]: utf8 string
 * @inlen[IN]: utf8 string len
 * @outbuf[IN]: gbk string buf
 * @outlen[IN]: gbk string buf len
 * return: 0 - success, fail return -1.
 */
int ak_osd_ex_u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

/**
 * ak_osd_ex_g2u - gbk convert to utf8
 * @inbuf[IN]: gbk string
 * @inlen[IN]: gbk string len
 * @outbuf[IN]: utf8 string buf
 * @outlen[IN]: utf8 string buf len
 * return: 0 - success, fail return -1.
 */
int ak_osd_ex_g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

/**
 * ak_osd_ex_turn_on - turn on to draw osd on screen of ja platform
 * @channel[IN]: video channel
 * return: 0 - success, fail return -1.
 */
int ak_osd_ex_turn_on(int channel);

/**
 * ak_osd_ex_turn_off - turn off to draw osd on screen of ja platform
 * @channel[IN]: video channel
 * return: 0 - success, fail return -1.
 */
int ak_osd_ex_turn_off(int channel);

/**
 * ak_osd_ex_stat_video - display video stat info via osd
 * channel[IN]: channel no.
 * stream_handle[IN]: stream handle
 * return: 0 success, -1 failed
 */
int ak_osd_ex_stat_video(int channel, void *stream_handle);

/**
 * ak_osd_ex_update_name - update osd name on screen of ja platform
 * @osd_name[IN]: osd name to be update
 * return: 0 - success, fail return -1.
 */
int ak_osd_ex_update_name(char *osd_name);


/**
 * ak_osd_ex_init - create thread to draw osd to screen on ja platform
 * @vi[IN]: vi handle
 * return: 0 - success, fail return -1.
 */
int ak_osd_ex_init(void *vi,  struct osd_ex_info *info);

/**
 * ak_osd_ex_exit - exit osd thread which is for ja platform
 * return: void.
 */
void ak_osd_ex_exit(void);

#endif
