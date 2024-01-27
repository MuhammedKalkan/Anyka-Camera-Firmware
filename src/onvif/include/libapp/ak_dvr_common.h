#ifndef _AK_DVR_COMMON_H_
#define _AK_DVR_COMMON_H_

#define DVR_FILE_PREFIX_MAX_LEN  20		//DVR file prefix max len in bytes
#define DVR_FILE_PATH_MAX_LEN    255	//DVR file path max len in bytes
#define DVR_PAIR_NUM 2

/*  record prefix and path */
#define RECORD_DEFAULT_PATH	        "/mnt/CYC_DV/"
#define RECORD_DEFAULT_PREFIX	    "CYC_DV_"

/* dvr trigger type */
enum dvr_record_type {
	RECORD_TRIGGER_TYPE_RESERVED = 0x00,        //dvr trigger type reserved
	RECORD_TRIGGER_TYPE_PLAN,                  	//plan record trigger
	RECORD_TRIGGER_TYPE_ALARM,                  //alarm record trigger
	RECORD_TRIGGER_TYPE_MAX,
};

/* dvr file type */
enum dvr_file_type {
	DVR_FILE_TYPE_AVI = 0x00,
	DVR_FILE_TYPE_MP4,
	DVR_FILE_TYPE_NUM
};

/* dvr exception */
enum dvr_exception {
	DVR_EXCEPT_NONE = 0x00,	    	//no exception
	DVR_EXCEPT_SD_REMOVED = 0x01,	//SD card removed
	DVR_EXCEPT_SD_UMOUNT = 0x02,	//SD card unmount
	DVR_EXCEPT_SD_NO_SPACE = 0x04,	//SD card space not enough
	DVR_EXCEPT_SD_RO = 0x08,		//SD card read only
	DVR_EXCEPT_NO_VIDEO = 0x10,	    //can't capture video data
	DVR_EXCEPT_NO_AUDIO = 0x20,	    //can't capture audio data
	DVR_EXCEPT_MUX_SYSERR = 0x40, 	//mux system error
	DVR_EXCEPT_REC_STOPPED = 0x80, 	//record stopped
	DVR_EXCEPT_SD_WRITE_ERROR = 0x100, 	//sd record write error
};

#endif
