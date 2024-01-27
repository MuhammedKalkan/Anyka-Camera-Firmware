#ifndef _ANYKA_ERROR_H_
#define _ANYKA_ERROR_H_

/* the error no. will be overwritten by next error */
int ak_get_error_no(void);

/** ak_get_error_str - get error string by errno
 * @error_no[IN]: appointed error no.
 * return: error description of errno
 * note: You can get ONLY ONE error string once.
 */
char* ak_get_error_str(int error_no);

#endif
