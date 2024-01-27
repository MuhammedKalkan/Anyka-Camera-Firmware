#ifndef _AK_MD_H_
#define _AK_MD_H_

/*
 * ak_md_get_version - get lib md  version
 */
const char* ak_md_get_version(void);

/**
 * submit - md init. set defaut fps ,global sensitivity
 * @vi_handle[IN]:   video in handle.
 * 
 * return:  0 - success;  -1  - fail;  
 * note: md can work in 2 mode: global mode and area mode,
 * global mode, md give result that the have moving detection , 
 * but donot give which area of whole area; otherwise,area mode
 * is to divide the whole area into many (such as 2*4) area, md give
 * which area have moving detection.
 */
int ak_md_init(void *vi_handle);

/**
 * submit -  set how much frame to do md in one second
 * @fps[IN]:     how much frame to do md in one second. 
 * return:  0 - success;  -1  - fail;  
 * note: this fps should be no more than fps of sensor.
 */
int ak_md_set_fps(int fps);

/**
 * submit -  get fps
 * @fps[OUT]:   how much frame that md do  in one second. 
 * return:  > 0 -  fps ;  -1  - fail;  
 */
int ak_md_get_fps(int *fps);

/**
 * submit -  set the sensitivity that  md do
 * @sensitivity[IN]:   sensitivity to do global md. 
 * return:  0 - success;  -1  - fail;  
 */
int ak_md_set_global_sensitivity(int sensitivity);

/**
 * submit -  get global sensitivity
 * @sensitivity[OUT]:   how sensitivity that md do   
 * return: > 0 - sensitivity;  -1  - fail;  
 */
int ak_md_get_global_sensitivity(int *sensitivity);

/**
 * submit - set dimension area thart md to do.
 * @horizon_num[IN]:   dimension number of horizontal.
 * @vertical_num[IN]:   dimension number of vertial.
 * @area_sensitivity[IN]:   each area sensitivity 
 * return:  0 - success;  -1  - fail;  
 */
int ak_md_set_area_sensitivity(int horizon_num, int vertical_num, 
							int *area_sensitivity);

/**
 * submit - get dimension max thart md support.
 * @horizon_num[OUT]:   dimension number of horizontal.
 * @vertical_num[OUT]:   dimension number of vertial.
 * return:  0 - success;  -1  - fail;  
 */
int ak_md_get_dimension_max(int *horizon_num, int *vertical_num);

/**
 * submit - get md result.
 * @md_sec[OUT]:  md trigger time,calendar time,second.
 * @area_md[OUT]:	if enable area md, output area md result.
 * @msec[IN]:  msec <0  block mode, ==0 non-block mode, >0 waittime
 * return: 1 - md trigger; 0 - no md;  -1  - fail;  
 */
int ak_md_get_result(int *md_sec, char *area_md, int msec);

/** 
 * ak_md_enable - start or stop md . 
 * @enable[IN]:  [0,1],  0 -> stop md; 1 -> start md 
 * return: 0 - success; otherwise -1; 
 */
int ak_md_enable(int enable);

/* 
 * ak_md_destroy - free md resource and quit md. 
 * return: none 
 */
void ak_md_destroy(void);

#endif
