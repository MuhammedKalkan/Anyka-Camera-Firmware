#ifndef _AK_CMD_EXEC_H_
#define _AK_CMD_EXEC_H_

/* 
 * ak_cmd_exec - execute command, if result is not NULL, store
 * executed result to it.
 * cmd[IN], command which you want to execute
 * result[OUT], store result
 * res_len[OUT], indicate the lenght of result buf
 * return: on success 0, -1 fail
 */
int ak_cmd_exec(const char *cmd, char *result, int res_len);

#endif
