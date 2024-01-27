#ifndef _AK_ATS_H_
#define _AK_ATS_H_

/**
 * ak_ats_get_version - get audio tool server in version
 * return: version string
 * notes:
 */
const char* ak_ats_get_version(void);

/**
 * ak_ats_start - audio tool server start
 * @port[IN]: server port id
 * return: 0 success, -1 failed
 * notes:
 */
int ak_ats_start(unsigned int port);

/**
 * ak_ats_stop - audio tool server stop
 * return: 0 success, -1 failed
 * notes:
 */
void ak_ats_stop(void);


#endif
