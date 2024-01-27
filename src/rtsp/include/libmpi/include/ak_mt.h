#ifndef _AK_MT_H_
#define _AK_MT_H_


#define MT_LIB_VERSION	"MT Lib V1.0.07" 


#define MD_SIZE  32  // horizontal and vertical size of vpss_md matrix

typedef struct
{
    short px;
	short py;
}MT_POINT;

typedef struct
{
	MT_POINT pt;    // center coordinate of motion region
	char width;
	char height;
}MT_RECTANGLE;

/**
 * ak_mt_init - init motion tracking parameters
 * return: motion tracking parameter handle
 * notes:
 */
void *ak_mt_init(void);

/**
 * ak_mt_destroy - release motion tracking parameter handle
 * @mt_handle[IN]: motion tracking parameter handle
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_destroy(void *mt_handle);

/**
 * ak_mt_set_fps - set encode frames per second
 * @mt_handle[IN]: motion tracking parameter handle
 * @fps[IN]: encode frames per second
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_set_fps(void *mt_handle, short fps);

/**
 * ak_mt_set_max_invalid_frames - set max frames to determine invalid motion
 * @mt_handle[IN]: motion tracking parameter handle
 * @nFrames[IN]: max frames to determine invalid motion
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_set_max_invalid_frames(void *mt_handle, unsigned short nFrames);

/**
 * ak_mt_set_max_decision_frames - set max frames to determine camera rotation
 * @mt_handle[IN]: motion tracking parameter handle
 * @nFrames[IN]: max frames to determine camera rotation
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_set_max_decision_frames(void *mt_handle, unsigned short nFrames);

/**
 * ak_mt_set_wait_frames - set wait frames to wait camera rotation 
 * @mt_handle[IN]: motion tracking parameter handle
 * @nFrames[IN]: frames to wait camera rotation 
 * return: 0 on success; -1 on failed
 * notes:
 */
 int ak_mt_set_wait_frames(void *mt_handle, unsigned short nFrames);

/**
 * ak_mt_set_wait_time - set waiting time for camera rotation 
 * @mt_handle[IN]: motion tracking parameter handle
 * @sec[IN]: seconds to wait camera rotation 
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_set_wait_time(void *mt_handle, unsigned short sec);


/**
 * ak_mt_set_invalid_distance - set distance to seperate invalid and valid motion
 * @mt_handle[IN]: motion tracking parameter handle
 * @dis[IN]: distance to seperate invalid and valid motion 
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_set_invalid_distance(void *mt_handle, unsigned char dis);

/**
 * ak_mt_set_camera_rotation_range - set camera accessible rotation angle
 * @mt_handle[IN]: motion tracking parameter handle
 * @min_agl_h[IN]: min accessible angle of camera in horizontal direction
 * @max_agl_h[IN]: max accessible angle of camera in horizontal direction
 * @min_agl_v[IN]: min accessible angle of camera in vertical direction
 * @max_agl_v[IN]: max accessible angle of camera in vertical direction
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_set_camera_rotation_range(void *mt_handle, short min_agl_h, short max_agl_h,
	short min_agl_v, short max_agl_v);

/**
 * ak_mt_set_mrd_filters - set filters for motion region detection
 * @mt_handle[IN]: motion tracking parameter handle
 * @flt_big[IN]: big filter to roughly detect motion region
 * @flt_small[IN]: small filter to expand rough detection result
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_set_mrd_filters(void *mt_handle, unsigned short flt_big, unsigned short flt_small);

/**
 * ak_mt_set_camera_init_angle - set camera init angle
 * @mt_handle[IN]: motion tracking parameter handle
 * @init_agl_h[IN]: init camera angle in horizontal direction
 * @init_agl_v[IN]: init camera angle in vertical direction
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_set_camera_init_angle(void *mt_handle, short init_agl_h, short init_agl_v);

/**
 * ak_mt_set_camera_rotation_size_h - set camera rotation angle per rotation in horizontal direction
 * @mt_handle[IN]: motion tracking parameter handle
 * @agl_h[IN]: rotation angle in horizontal direction
 * return: 0 on success; -1 on failed
 * notes:
 */
//int ak_mt_set_camera_rotation_size_h(void *mt_handle, short agl_h);

/**
 * ak_mt_set_camera_rotation_factor - set camera rotation factor
 * @mt_handle[IN]: motion tracking parameter handle
 * @factor[IN]: camera rotation factor
 * return: 0 on success; -1 on failed
 * notes: if offset caused by motion speed isn't essential, set the input parameter factor = x(x is a negative number, example -1) 
 */
int ak_mt_set_camera_rotation_factor(void *mt_handle, int factor);


/**
 * ak_mt_update_camera_angle - update current camera direction angle
 * @mt_handle[IN]: motion tracking parameter handle
 * @agl_h[IN]: current camera angle in horizontal direction
 * @agl_v[IN]: current camera angle in vertical direction
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_update_camera_angle(void *mt_handle, short agl_h, short agl_v);

/**
 * ak_mt_open_camera_rotation_switch_v - open camera rotation switch in vertical direction
 * @mt_handle[IN]: motion tracking parameter handle
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_open_camera_rotation_switch_v(void *mt_handle);

/**
 * ak_mt_close_camera_rotation_switch_v - close camera rotation switch in vertical direction
 * @mt_handle[IN]: motion tracking parameter handle
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_close_camera_rotation_switch_v(void *mt_handle);

/**
 * ak_mt_reverse_rotation_direction_h - reverse camera rotation direction in horizontal direction
 * @mt_handle[IN]: motion tracking parameter handle
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_reverse_rotation_direction_h(void *mt_handle);

/**
 * ak_mt_reverse_rotation_direction_v - reverse camera rotation direction in vertical direction
 * @mt_handle[IN]: motion tracking parameter handle
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_reverse_rotation_direction_v(void *mt_handle);

/**
 * ak_mt_reverse_main_decision_direction_v - reverse main decision direction in vertical direction
 * @mt_handle[IN]: motion tracking parameter handle
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_reverse_main_decision_direction_v(void *mt_handle);

/**
 * ak_mt_set_params - set motion tracking parameters from outer input
 * @mt_handle[IN]: motion tracking parameter handle
 * @len[IN]: VPSS_MD_DIMENSION in vertical direction
 * @valid_size_min[IN]: min size of valid motion region
 * @valid_size_max[IN]: max size of valid motion region
 * @center_size_h[IN]: horizontal size of video frame center
 * @center_size_v[IN]: vertical size of video frame center
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_set_motion_region_params(void *mt_handle, short len, short valid_size_min,  short valid_size_max,
	short boundary_size_h, short boundary_size_v);

/**
 * ak_mt_wait_camera_rotation - wait camera rotation
 * @mt_handle[IN]: motion tracking parameter handle
 * return: 0 on success; -1 on failed
 * notes:
 */
int ak_mt_wait_camera_rotation(void *mt_handle);

/**
 * mt_mrd - detect motion region
 * @mt_handle[IN]: motion tracking parameter handle
 * @md[IN]: motion information
 * @len[IN]: VPSS_MD_DIMENSION in vertical direction
 * @rect[OUT]: motion region
 * return: 1 effect motion region is detected; 0 no motion region; -1 parameter error
 * notes:
 */
int ak_mt_mrd(void *mt_handle, unsigned short md[][MD_SIZE], short len, MT_RECTANGLE *rect);

/**
 * ak_mt_tracking - track the desired motion region
 * @mt_handle[IN]: motion tracking parameter handle
 * @md[IN]: motion information
 * @len[IN]: VPSS_MD_DIMENSION in vertical direction
 * @rect[IN]: rectangle box of the largest motion region
 * @rt_angle[OUT]: absolute rotation angel in horizontal and vertical direction
 * return: 0 need no rotation; 1 need rotation; 2 need rotation and reset to inital position, -1 param error, no effect
 * notes:
 */
int ak_mt_tracking(void *mt_handle, unsigned short md[][MD_SIZE], short len, MT_RECTANGLE *rect, MT_POINT *rt_angle);

#endif

