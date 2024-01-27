#ifndef _AK_DVR_REPLAY_H_
#define _AK_DVR_REPLAY_H_

#include "list.h"
#include "ak_dvr_common.h"
#include "ak_demux.h"

typedef void (* FP_RECORD_REPLAY_CALLBACK)(void *param, int type,
			struct demux_stream *stream, int encode_type);

struct dvr_replay_request {
    unsigned long start_time;//replay start time
    unsigned long end_time;  //replay end time, Beijing time(timestamp+timezone)
    void *user_data;         //user data
    enum dvr_file_type record_type;	//record type
};

struct dvr_replay_entry {
	unsigned long start_time;       //record file start time, unit: second
    unsigned long total_time;       //record file total time, unit: millisecond
    unsigned long play_start_pos;   //play start position
    unsigned long play_duration;    //play duration
	char *path;						//including path and file full name
	struct list_head list;			//list head
};

struct dvr_replay_param {
	unsigned long start_time;		//replay request start time, unit: second
	unsigned long end_time;			//replay request end time, unit: second
	enum dvr_file_type type;		//replay record file type
};

/**
 * replay_pause - pause replay
 * return: void
 * notes:
 */
void ak_dvr_replay_pause(void);

/**
 * ak_dvr_replay_resume - resume replay
 * return: void
 * notes:
 */
void ak_dvr_replay_resume(void);

/**
 * ak_dvr_replay_stop - stop replay
 * @usr_data: user data
 * return: void
 * notes:
 */
void ak_dvr_replay_stop(void *usr_data);

/**
 * ak_dvr_replay_start - start record replay
 * @request[IN]: replay request info, you must set [start_time, end_time]
 * @send_record[IN]: send record data callback
 * return: 0 success, -1 failed
 * notes: 1. we'll send replay media data via "send_record" callback
 * 		2. replay media data just in [start_time, end_time]
 */
int ak_dvr_replay_start(struct dvr_replay_request *request,
                        FP_RECORD_REPLAY_CALLBACK send_record);

/**
 * ak_dvr_replay_fetch_list - fetch replay record file list
 * @param[IN]: replay param, according to [start_time, end_time] and type
 * @replay_list[OUT]: fetched replay list, entry type "struct dvr_replay_entry"
 * return: entry number in replay_list, 0 failed
 * notes: 1.call ak_dvr_record_init before fetch list
 * 		2.just return replay entry list in [start_time, end_time]
 *		3.you MUST free fetch list atfer used
 *		4.we'll save a complete replay entry list internal
 */
int ak_dvr_replay_fetch_list(const struct dvr_replay_param *param,
							struct list_head **replay_list);

/**
 * ak_dvr_replay_free_fetch_list - free fetched replay list
 * @fetch_list: fetched list by ak_dvr_replay_fetch_list
 * return: 0 success,  -1 failed
 * notes: you MUST free fetched list after call ak_dvr_replay_fetch_list success
 */
int ak_dvr_replay_free_fetch_list(struct list_head *fetch_list);

/**
 * ak_dvr_replay_init - init record replay env and start replay thread.
 * @void
 * return: 0 success; othrewise -1
 * notes:
 */
int ak_dvr_replay_init(void);

/**
 * ak_dvr_replay_exit - clear record replay env and exit replay thread.
 * @void
 * return: 0 success; othrewise -1
 * notes:
 */
int ak_dvr_replay_exit(void);

#endif