#ifndef _AK_DRV_PTZ_H_
#define _AK_DRV_PTZ_H_

enum ptz_device {
	PTZ_DEV_H = 0,      /* Horizontal(left, right) */
	PTZ_DEV_V,          /* Vertical(up, down) */
	PTZ_DEV_NUM         /* number of deivce */
};

enum ptz_status {
	PTZ_WAIT_INIT = 0,
	PTZ_INIT_OK,
	PTZ_SYS_WAIT
};

enum ptz_turn_direction {
    PTZ_TURN_RESERVED = 0x00,
	PTZ_TURN_LEFT,
	PTZ_TURN_RIGHT,
	PTZ_TURN_UP,
	PTZ_TURN_DOWN
};

enum ptz_feedback_pin {
	PTZ_FEEDBACK_PIN_NONE = 0,
	PTZ_FEEDBACK_PIN_EXIST
};

/*�Լ��ָ��*/
enum sc_step {
	SC_CLKNONE = 0,	/*δ����*/
	SC_CLKMAX,		/*˳ʱ�뵽��*/
	SC_ACLKMAX,		/*��ʱ�뵽��*/
	SC_CLKMID,		/*˳ʱ�뵽�м�*/
	SC_ACLKMID,		/*��ʱ�뵽�м�*/
};

/**
 * ak_drv_ptz_get_version - get drv ptz version
 * return: version string
 */
const char* ak_drv_ptz_get_version(void);

/**
 * ak_drv_ptz_open - init ptz, open motor device
 * return: 0 success; -1 failed
 */
int ak_drv_ptz_open(void);

/**
 * Setup the step parameter of pan motor.\n
 * Call after @ref ak_drv_ptz_open().
 *
 * @param[IN] motor_no
 *  Motor ID.
 *
 * @param[IN] cycle_step
 *  The motor steps of one cycle (360 degree)
 *
 * @param[IN] fulldst_step
 *  The motor work Full Distance Steps, Range [1, cycle_step].
 *
 * @param[IN] init_pos
 *  Current Initialize Step Position, Set -1 to Default.
 *
 * @return
 *  If Success Return 0, Else -1.
 */
int ak_drv_ptz_setup_step_param (enum ptz_device motor_no, int cycle_step, int fulldst_step, int init_pos);


/**
 * Get Motor Current Step Value.
 *
 * @param[IN] motor_no
 *  Motor ID.
 *
 * @return
 *  If Success Return the Step Value, Else -1.
 */
int ak_drv_ptz_get_step_pos (enum ptz_device motor_no);


/**
 * ak_drv_ptz_check_self - motor check it self
 * @pin_type[IN]: feedback pin type in 'enum ptz_feedback_pin'
 * return: 0 success; -1 failed
 */
int ak_drv_ptz_check_self(enum ptz_feedback_pin pin_type);

/**
 * ak_drv_ptz_check_self_ex - motor check it self
 * @pin_type[IN]: feedback pin type in 'enum ptz_feedback_pin'
 * @scs[IN]: self check step array
 * @scs_size[IN]: size of self check step array
 * return: 0 success; -1 failed
 */
int ak_drv_ptz_check_self_ex(enum ptz_feedback_pin pin_type,
		enum sc_step *scs, int scs_size);

/**
 * ak_drv_ptz_set_angle_rate deprecated > SDK1.8.00.
 */
#define ak_drv_ptz_set_angle_rate(__h_rate, __v_rate) \
	do {\
		ak_print_error_ex ("%s() deprecated > SDK1.8.00.\r\n", __func__);\
	} while (0)

/**
 * @ref ak_drv_ptz_setup_step_param Quick Call.
 */
#define ak_drv_ptz_set_degree(__h_degree, __v_degree) \
		(ak_drv_ptz_setup_step_param (PTZ_DEV_H, 2048, 2048 * (__h_degree) / 360, -1) == AK_SUCCESS                       \
		&& ak_drv_ptz_setup_step_param (PTZ_DEV_V, 2048, 2048 * (__v_degree) / 360, -1) == AK_SUCCESS ? AK_SUCCESS : AK_FAILED)

/**
 * ak_drv_ptz_reset_dg: reset degree for calibrate.
 * @motor_no[IN]: motor device No. in 'enum ptz_device'
 * return: 0 success, -1 failed
 */
int ak_drv_ptz_reset_dg(enum ptz_device motor_no);

/**
 * ak_drv_ptz_turn: command motor to turn to the relative position
 * @direct[IN]: ptz turn direction, [left,right,up,down]
 * @degree[IN]: relative degree of horizontal or vertical to current position
 * return: 0 success; -1 failed
 * note: degree of one circle is 360.
 */
int ak_drv_ptz_turn(enum ptz_turn_direction direct, int degree);

/**
 * ak_drv_ptz_turn_to_pos: command motor to turn to the object position
 * @left_degree[IN]:  absolute degree at  horizontal
 * @up_degree[IN]:  absolute degree of horizontal or vertical to start position
 * return: 0 success, -1 failed
 * note: degree of one circle is 360.
 */
int ak_drv_ptz_turn_to_pos(int left_degree, int up_degree);


/**
 * ak_drv_ptz_turn_stop: command motor to stop
 * @direct[IN]: ptz turn direction, [left,right,up,down]
 * return: 0 success, -1 failed
 */
int ak_drv_ptz_turn_stop(enum ptz_turn_direction direct);

/**
 * ak_drv_ptz_get_status: get appointed motor status.
 * @motor_no[IN]: motor device No. in 'enum ptz_device'
 * @status[OUT]: motor status
 * return: 0 success, -1 failed
 */
int ak_drv_ptz_get_status(enum ptz_device motor_no, enum ptz_status *status);

/**
 * ak_drv_ptz_set_speed: set appointed motor speed.
 * @motor_no[IN]: motor device No. in 'enum ptz_device'
 * @speed[IN]: speed, [15, 100]
 * return: 0 success, -1 failed
 */
int ak_drv_ptz_set_speed(enum ptz_device motor_no, int speed);

/**
 * ak_drv_ptz_close - close motor, release motor resource
 * return: 0 success, -1 failed
 */
int ak_drv_ptz_close(void);

#endif
