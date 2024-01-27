#ifndef _AK_WDT_H_
#define _AK_WDT_H_

/**
 * ak_drv_wdt_get_version - get drv wdt version
 * return: version string
 */
const char* ak_drv_wdt_get_version(void);

/** 
 * ak_drv_wdt_open - open watch dog and watch dog start work. 
 * @feed_time: [in] second  
 * return: 0 - success; otherwise -1; 
 * note:   max of feed_time limit to 8 second.
 */
int ak_drv_wdt_open(unsigned int feed_time);

/** 
 * ak_drv_wdt_feed - feed watch dog. 
 * return:0 - success; otherwise -1; 
 */
int ak_drv_wdt_feed(void);

/** 
 * ak_drv_wdt_close - close watch dog. 
 * return:0 - success; otherwise -1; 
 */
int ak_drv_wdt_close(void);

#endif	
