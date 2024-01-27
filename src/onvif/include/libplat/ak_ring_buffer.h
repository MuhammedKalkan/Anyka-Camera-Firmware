#ifndef _AK_RING_BUFFER_H_
#define _AK_RING_BUFFER_H_ 

/** 
 * ak_rb_init - init appointed size ring buffer
 * @rb_size[IN]: appointed ring buffer size
 * return: new ring buffer handle; NULL failed
 */
void* ak_rb_init(const unsigned int rb_size);

/** 
 * ak_rb_reset - reset ring buffer, all of the data will be lost
 * @rb_handle[IN]: ring buffer handle
 * return: 0 success; -1 failed
 */
int ak_rb_reset(void *rb_handle);

/** 
 * ak_rb_get_data_len - get ring buffer current data len
 * @rb_handle[IN]: ring buffer handle
 * return: >=0 current data len; -1 failed
 */
int ak_rb_get_data_len(void *rb_handle);

/** 
 * ak_rb_read - read ring buffer data
 * @rb_handle[IN]: ring buffer handle
 * @to[OUT]: read data buffer
 * @read_len[IN]: read data len
 * return: >=0 real read len; -1 failed
 */
int ak_rb_read(void *rb_handle, unsigned char *to, int read_len);

/** 
 * ak_rb_read - read ring buffer data
 * @rb_handle[IN]: ring buffer handle
 * @from[IN]: write data buffer
 * @write_len[IN]: write data len
 * return: >=0 real read len; -1 failed
 */
int ak_rb_write(void *rb_handle, const unsigned char *from, int write_len);

/** 
 * ak_rb_release - init appointed size ring buffer
 * @rb_handle[IN]: ring buffer handle
 * return: 0 success; -1 failed
 */
int ak_rb_release(void *rb_handle);

#endif
