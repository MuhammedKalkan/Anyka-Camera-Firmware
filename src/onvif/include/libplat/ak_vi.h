#ifndef _AK_VIDEO_INPUT_H_
#define _AK_VIDEO_INPUT_H_

#include "ak_global.h"

#define SENSOE_IF_PATH		"/sys/ak_info_dump/sensor_if"

enum video_dev_type {
	VIDEO_DEV0 = 0x00,
	VIDEO_DEV_NUM
};

enum encode_use_type {
	VIDEO_ENCODE = 0x00,
	MOVE_DETECT,
	RAW_DATA
};

enum video_channel {
	VIDEO_CHN_MAIN,
	VIDEO_CHN_SUB,
	VIDEO_CHN_NUM
};

enum video_daynight_mode {

	VI_MODE_DAY,
	VI_MODE_NIGHT,
	VI_MODE_CUSTOM_2,
	VI_MODE_CUSTOM_3,
	VI_MODE_CUSTOM_4,

	VI_MODE_NUM
};

enum video_work_scene {
	VIDEO_SCENE_UNKNOWN = -1,
	VIDEO_SCENE_INDOOR = 0x00,
	VIDEO_SCENE_OUTDOOR
};

struct video_resolution {
	int		width;
	int		height;
	int		max_width;
	int		max_height;
};

struct crop_info {
	int		left;	// x position of crop
	int		top;	// y position of crop
	int		width;	// width of crop
	int		height;	// height of crop
};

typedef struct crop_info video_rect;
/*box of motion rect*/

/* video input module config */
struct video_channel_attr {
	struct crop_info crop;
	struct video_resolution res[VIDEO_CHN_NUM];
};

struct video_input_frame {
	struct frame vi_frame[VIDEO_CHN_NUM];
	void *mdinfo;
};

/**
 * ak_vi_get_version - get video in version
 * return: version string
 */
const char* ak_vi_get_version(void);

/**  
 * ak_vi_match_sensor: match sensor according to appointed config file.
 * @config_file[IN]: it can be config file absolutely path, or a directory name.
 * If it is a directory name, it will search config file which format is 
 * correct to match.
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_match_sensor(const char *config_file);

/**
 * ak_vi_open: open video input device
 * @dev[IN]: video input device ID, for more detail, see defination of 
 *          'enum video_dev_type'
 * return: 0 success, -1 failed
 * notes: must call this after 'ak_vi_match_sensor()' success.
 */
void* ak_vi_open(enum video_dev_type dev);

/**
 * ak_vi_get_channel_attr: get channel attribution
 * @handle[IN]: return by 'ak_vi_open()'
 * @attr[OUT]: channel attribution
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_get_channel_attr(void *handle, struct video_channel_attr *attr);

/**
 * ak_vi_set_channel_attr: set channel attribution
 * @handle[IN]: return by 'ak_vi_open()'
 * @attr[IN]: channel attribution
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_set_channel_attr(void *handle,
		const struct video_channel_attr *attr);

/**
 * ak_vi_change_channel_attr - change channel attribution
 * @handle[IN]: return by 'ak_vi_open()'
 * @attr[IN]: new channel attribution
 * return: 0 success, -1 failed
 * notes: IMPORTANT-you can change channel attribution in real time.
 */
int ak_vi_change_channel_attr(const void *handle,
		const struct video_channel_attr *attr);

/**
 * ak_vi_get_flip_mirror - get flip and mirror
 * @handle[IN]: return by 'ak_vi_open()'
 * @flip_enable[OUT]: flip flag
 * @mirror_enable[OUT]: mirror flag
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_get_flip_mirror(void *handle, int *flip_enable, int *mirror_enable);

/**
 * ak_vi_set_flip_mirror - set flip and mirror
 * @handle[IN]: return by 'ak_vi_open()'
 * @flip_enable[IN]:flip flag
 * @mirror_enable[IN]:mirror flag
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_set_flip_mirror(void *handle, int flip_enable, int mirror_enable);

/**
 * ak_vi_capture_on - open isp capture
 * @handle[IN], return by 'ak_vi_open()'
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_capture_on(void *handle);

/*
 * ak_vi_capture_off - close isp capture
 * @handle[IN], return by 'ak_vi_open()'
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_capture_off(void *handle);

/**
 * ak_vi_reset - reset vi device
 * @handle[IN]: return by ak vi open
 * return: 0 success, -1 failed
 */
int ak_vi_reset(void *handle);

/**
 * ak_vi_get_frame: get frame
 * @handle[IN]: return by 'ak_vi_open()'
 * @frame[OUT]: store frames
 * return: 0 success, otherwise failed
 */
int ak_vi_get_frame(void *handle, struct video_input_frame *frame);

/**
 * ak_vi_release_frame: release frame buffer after used
 * @handle[IN]: return by 'ak_vi_open()'
 * @frame[IN]: store frames
 * return: 0 success, otherwise failed
 * notes:
 */
int ak_vi_release_frame(void *handle, struct video_input_frame *frame);

/**
 * ak_vi_get_fps - get current capture fps
 * @handle[IN]: return by 'ak_vi_open()'
 * return: fps value
 * notes:this fps is sensor fps, not video encode fps
 */
int ak_vi_get_fps(void *handle);

/**
 * ak_vi_set_fps - set capture fps
 * @handle[IN]: return by 'ak_vi_open()'
 * @fps[IN]: the fps value to be set
 * return: 0 success, -1 failed
 * notes: this fps is sensor fps , not video encode fps;
 * 		 the value of 'fps' do not out of range [1, sensor capture max fps]
 */
int ak_vi_set_fps(void *handle, int fps);

/**
 * ak_vi_switch_mode - switch day/night mode
 * @handle[IN]: return by 'ak_vi_open()'
 * @mode[IN]: for more detail see defination of enum video_daynight_mode 
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_switch_mode(void *handle, enum video_daynight_mode mode);

/**
 * ak_vi_get_sensor_resolution - get sensor max resolution supported
 * @handle[IN]: return by 'ak_vi_open()'
 * @res[OUT]: store sensor resolution information
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_get_sensor_resolution(void *handle, struct video_resolution *res);

/**
 * ak_vi_set_switch_fps_enable - enable or disable auto-switch fps function 
 * @handle[IN]: return by 'ak_vi_open()' 
 * @enable[IN]: enable = 1, disable = 0
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_set_switch_fps_enable(void *handle, int enable);

/**
 * ak_vi_get_handle - get vi handle
 * @dev[IN]:device in enum video_dev_type
 * return: handle pointer, NULL failed
 * notes:
 */
void *ak_vi_get_handle(enum video_dev_type dev);

/**
 * ak_vi_get_work_scene - get appointed video device's current work scene
 * @dev[IN]: device in enum video_dev_type
 * return: current work scene
 * notes:
 */
enum video_work_scene ak_vi_get_work_scene(enum video_dev_type dev);

/**
 * ak_vi_clear_buffer - clear vi capture buffer
 * @handle[IN]: return by ak vi open
 * return: 0 success, -1 failed
 * notes: if stopped encoding at all
 *		you can call this function before encoding again
 */
int ak_vi_clear_buffer(void *handle);

/**
 * ak_vi_close - close vi and release resource
 * @handle[IN]: return by 'ak_vi_open()'
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_close(void *handle);



/**
 * Put rectangle on video image with RGB @ref rgb24 value
 *
 * @handle[IN]: video device id
 * @channel[IN]: video channel
 * @rect[IN]:indicated the rect where put,if NULL is clear rect
 * @rgb24[IN]:rgb24 color of rectangle.
 *
 * return: 0 success, otherwise failed
 * notes:
 */
int ak_vi_put_rect (void *handle, enum video_channel channel,video_rect *rect,unsigned int rgb24);

/**
 * Quick call for clear the rectangle @ak_vi_put_rect in clear funtion.
 */
#define ak_vi_clear_rect(__handle,__channel) ak_vi_put_rect (__handle, __channel, NULL, 0)

 /**
 * ak_vi_set_box_rect - set box rect info
 * @x[IN]: left of rect
 * @y[IN]: top of rect
 * @width[IN]: width of rect
 * @height[IN]: height of rect
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_set_box_rect(int x, int y, int width, int height);

#endif
