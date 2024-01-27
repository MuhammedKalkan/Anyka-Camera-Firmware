#ifndef _AK_DVR_DISK_H_
#define _AK_DVR_DISK_H_

/**
 * ak_dvr_repair_ro_disk - try to repair readonly SD card
 * @bad_file[IN]: bad file full path, including file name
 * return: 0 success, -1 failed
 * notes:
 */
int ak_dvr_repair_ro_disk(void);

/**
 * ak_dvr_check_sd_ro - check SD card readonly status
 * @void
 * return: 1 read only, 0 rw, -1 failed
 * notes:
 */
int ak_dvr_check_sd_ro(void);


#endif
