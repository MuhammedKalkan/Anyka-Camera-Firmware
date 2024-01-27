#ifndef _AK_SD_CARD_H_
#define _AK_SD_CARD_H_

#ifdef __cplusplus
extern "C" {
#endif

/* SD card status */
enum sd_card_status {
	SD_STATUS_UNMOUNT = 0X00,			//unmount
	SD_STATUS_MOUNTED = 0x01,			//SD card has mounted(must have inserted)
	SD_STATUS_CARD_INSERT = 0x02,		//SD card has inserted
	SD_STATUS_CARD_REMOVED = 0x04,		//SD card has removed
	SD_STATUS_READ_ONLY = 0x08,			//read only
	SD_STATUS_NO_FREE_SPACE = 0x10,		//no free space
};

/**
 * ak_sd_check_insert_status - check SD card insert status
 * @void
 * @return: SD card insert status
 * 		SD_STATUS_CARD_INSERT or SD_STATUS_CARD_REMOVED
 */
int ak_sd_check_insert_status(void);

/**
 * ak_sd_check_mount_status - check SD card mount status
 * @void
 * @return: 0 sd mounted; -1 sd unmount
 */
int ak_sd_check_mount_status(void);

/**
 * ak_sd_mount - mount sd card to appointed point
 * @mount_point[IN]: mount point, such as /mnt
 * @return: 0 mount success; -1 mount failed
 */
int ak_sd_mount(const char *mount_point);

/**
 * ak_sd_umount - umount the sd card from appointed point
 * @mount_point[IN]: mount point, such as /mnt
 * @return: 0 umount success; -1 umount failed
 */
int ak_sd_umount(const char *mount_point);

/**
 * ak_sd_set_status - set system sd status
 * @status[IN]: new sd card status
 * @mount_point[IN]: mount point, such as /mnt
 * @return: 0 mount success; -1 mount failed
 */
int ak_sd_set_status(enum sd_card_status status, const char *mount_point);

/**
 * ak_sd_get_status - get system sd status
 * @void
 * return: the sd card status, may multi status combination
 */
int ak_sd_get_status(void);

/**
* @brief sd_init_status
		 init system sd status
* @author aj
* @date
* @param[in] void
* @return void
* @retval
*/

void sd_init_status(void);

#ifdef __cplusplus
}
#endif

#endif
