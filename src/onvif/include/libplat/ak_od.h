#ifndef _AK_OD_H_
#define _AK_OD_H_

#define MAX_AF_STATICS	5
#define MAX_RGB_HIST	256

struct od_info {
	unsigned int af_statics[MAX_AF_STATICS];
	unsigned int rgb_hist[MAX_RGB_HIST];
};

/**
 * ak_od - get occlusion detection params
 * @vi_handle[IN]: opened video input handle
 * @od[OUT]: od params 
 * return: 0 success, -1 failed
 * notes:
 */
int ak_od(const void *vi_handle, struct od_info *od);

#endif
