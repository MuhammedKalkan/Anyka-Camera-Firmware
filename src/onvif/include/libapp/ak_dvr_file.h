#ifndef _AK_DVR_FILE_H_
#define _AK_DVR_FILE_H_

#include "list.h"
#include "ak_common.h"
#include "ak_dvr_common.h"


/* dvr file param */
struct dvr_file_param {
	unsigned char cyc_flag;        			//cycle record flag
	enum dvr_file_type type;				//record file type
	char rec_prefix[DVR_FILE_PREFIX_MAX_LEN];//record file name prefix
	char rec_path[DVR_FILE_PATH_MAX_LEN];//record file saving path
	char rec_tmppath[DVR_FILE_PATH_MAX_LEN]; //record file saving tmp path
	char rec_tmp_file[DVR_PAIR_NUM][DVR_FILE_PATH_MAX_LEN];//record file saving full path tmp file name
	char rec_tmp_idx[DVR_PAIR_NUM][DVR_FILE_PATH_MAX_LEN];//record file saving full path idx file name
	char rec_tmp_moov[DVR_PAIR_NUM][DVR_FILE_PATH_MAX_LEN];//record file saving full path idx file name
	char rec_tmp_stbl[DVR_PAIR_NUM][DVR_FILE_PATH_MAX_LEN];//record file saving full path idx file name
	char rec_info_file[DVR_FILE_PATH_MAX_LEN];//record file saving full path info file name
};

#ifdef USE_PTR_PATH
struct dvr_file_entry {
	time_t calendar_time;		//calendar time, seconds from 1970-1-1 00:00:00
	unsigned long size;			//file size
	unsigned long total_time;	//record file total time
	char *path;					//including path and file full name
	struct list_head list;		//list head
};
#else

#define DVR_PATH_LEN	(60)

struct dvr_file_entry {                                                         //静态分配结构体方式
	time_t calendar_time;		//calendar time, seconds from 1970-1-1 00:00:00
	unsigned long size;			//file size
	unsigned long total_time;	//record file total time
	char path[DVR_PATH_LEN];	//file relatively path
	struct list_head list;		//list head
	int using_flg;				//using flag, 1 is using
};
#endif

/**
 * ak_dvr_file_init - init record file manage env and start record file thread.
 * @param: record file param
 * return: 0 success; othrewise -1
 * notes: record file type, prefix and path must provide.
 */
int ak_dvr_file_init(const struct dvr_file_param *param);

/**
 * ak_dvr_file_delete_tmp_file - delete record temp file
 * @void
 * return: none
 * notes: delete record temp file if you wish.
 */
void ak_dvr_file_delete_tmp(void);

/**
 * ak_dvr_file_create_list - wakeup get history record file thread and fetch list
 * @void
 * return: 0 success, -1 failed
 * notes: call again after SD card reinsert and mounted
 */
int ak_dvr_file_create_list(void);

/**
 * ak_dvr_file_exit - clear record file manage env and stop record file thread.
 * @void
 * return: none
 */
int ak_dvr_file_exit(void);
int ak_record_exist( time_t start_time, time_t end_time, enum dvr_file_type type );
int get_entry_total_time(const char *entry_path);
int check_time_axis( time_t i_request_start , time_t i_request_end , time_t i_mark_start , time_t i_mark_end );
#endif
