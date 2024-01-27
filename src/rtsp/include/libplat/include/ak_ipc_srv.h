#ifndef _AK_IPC_SRV_H_
#define _AK_IPC_SRV_H_

#define ANYKA_IPC_PORT		(7000)
#define DAEMON_PORT			(7001)
#define CMD_SERVERD_PORT	(7002)

#define DELIMIT			"#"

typedef int (ipc_msg_cb)(char *arg, unsigned int arg_len, int sock);

struct ak_ipc_msg_t {
	char *cmd;			//pointer to cmd
	ipc_msg_cb *msg_cb;	//cmd's deal-with callback
};

/* 
 * ak_cmd_server_register - server register, bind port and name 
 * port[IN]: server's port number
 * name[IN]: server's name
 * return: 0 on success, -1 failed
 */
int ak_cmd_server_register(unsigned int port, char *name);

/* 
 * ak_cmd_server_unregister - server unregister, remove server from set
 * port[IN]: server's port number
 * return: 0 on success, -1 failed
 */
int ak_cmd_server_unregister(unsigned int port);

/* 
 * ak_cmd_send - send commamd/message
 * port[IN]: server's port number
 * cmd[IN]: pointer to command buffer
 * cmd_len[IN]: indicate command buffer len
 * result[OUT]: store cmd execute result
 * res_len[IN]: indicate result buffer len
 * tv_out[IN/OUT]: pointer to time_out value, NULL means block;
 * 	   			   on select succeed, 
 * 	   			   tv_out was modified to reflect the amount of time not slept.
 * return: 0 on success, -1 failed
 */
int ak_cmd_send(unsigned int port,
		const char *cmd, unsigned int cmd_len,
	   	char *result, unsigned int res_len,
		long int *tv_out);

/* 
 * ak_cmd_register_msg_handle - register message handle
 * port[IN]: server's port number
 * msg[IN]: pointer to message structure
 * return: 0 on success, -1 failed
 */
int ak_cmd_register_msg_handle(unsigned int port,
		struct ak_ipc_msg_t *msg);


/* 
 * ak_cmd_unregister_msg_handle - unregister message handle
 * port[IN]: server's port number
 * msg[IN]: pointer to message structure
 * return: 0 on success, -1 failed
 */
int ak_cmd_unregister_msg_handle(unsigned int port,
		char *cmd);

/* 
 * ak_cmd_result_response - cmd response
 *                          send the executed result of cmd to client
 * port[IN]: server's port number
 * msg[IN]: pointer to message structure
 */
void ak_cmd_result_response(char *result, int len, int sock);

/* 
 * ak_cmd_register_module - register module
 * port[IN]: server's port number
 * name[IN]: pointer to module name
 */
void ak_cmd_register_module(unsigned int port, const char *name);

/* 
 * ak_cmd_unregister_module - unregister module
 * port[IN]: server's port number
 * name[IN]: pointer to module name
 */
void ak_cmd_unregister_module(unsigned int port, const char *name);

/* 
 * ak_cmd_show_working_modules - show all registed module
 * cli_sock[IN]: use to send result back to client
 */
void ak_cmd_show_working_modules(int sock);

/* 
 * ak_ipcsrv_get_version - get module version
 * return: pointer to version string
 */
const char *ak_ipcsrv_get_version(void);

#endif
