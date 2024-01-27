#ifndef _AK_DRV_I2C_H_
#define _AK_DRV_I2C_H_

struct ak_i2c_dev{
	int fd;
	int device_no;
};

struct i2c_msg {
	unsigned short addr; 		/* slave address */
	unsigned short flags;
	unsigned short len; 	 	/* msg length */
	unsigned char *buf; 	 	/* pointer to msg data */
};

/* This is the structure as used in the I2C_RDWR ioctl call */
struct i2c_rdwr_ioctl_data {
	struct i2c_msg  *msgs;	 /* pointers to i2c_msgs */
	unsigned long nmsgs;	 /* number of i2c_msgs */
};

/** 
 * ak_drv_key_open: open key driver 
 * return: key handle , or NULL; 
 */
void* ak_drv_i2c_open(int device_no);

/** 
 * ak_drv_key_open: open key driver 
 * return: key handle , or NULL; 
 */
int ak_drv_i2c_read(void *handle, const unsigned short reg_addr,
	unsigned char *data, unsigned int len);

/** 
 * ak_drv_key_get_event: get key event . 
 * @handle[IN]:  key handle
 * @key[OUT]:  key event
 * @ms[IN]:  time out,  msecond.
 * return:0 - success; otherwise -1;  
 */
int ak_drv_i2c_write(void *handle, const unsigned short reg_addr, 
	const unsigned char *data, unsigned int len);

/** 
 * ak_drv_key_close: close key handle . 
 * @handle[IN]:  key handle
 * return: 0 - success; otherwise -1; 
 */	
int ak_drv_i2c_close(void *handle);

#endif
