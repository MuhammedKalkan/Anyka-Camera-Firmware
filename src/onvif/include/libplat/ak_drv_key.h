#ifndef _EXTDRV_KEY_H_
#define _EXTDRV_KEY_H_

enum key_stat{
  PRESS = 1,
  RELEASE,
};

struct key_event{
   int code;
   enum key_stat stat;
};


/**
 * ak_drv_key_get_version - get drv key version
 * return: version string
 */
const char* ak_drv_key_get_version(void);

/** 
 * ak_drv_key_open: open key driver 
 * return: key handle , or NULL; 
 */
void *ak_drv_key_open(void);

/** 
 * ak_drv_key_get_event: get key event . 
 * @handle[IN]:  key handle
 * @key[OUT]:  key event
 * @ms[IN]:  time out,  msecond.
 * return:0 - success; otherwise -1;  
 */
int ak_drv_key_get_event(void *handle, struct key_event *key,long ms);

/** 
 * ak_drv_key_close: close key handle . 
 * @handle[IN]:  key handle
 * return: 0 - success; otherwise -1; 
 */	
int ak_drv_key_close(void *handle);

#endif
