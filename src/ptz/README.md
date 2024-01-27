# This is a modified version of
https://github.com/kuhnchris/IOT-ANYKA-PTZdaemon

## PTZ tracker/daemon

This is tailored to the functions in the module AK_MOTOR, wrapped by plat_drv, for ANYKA "CPU"s.

the PTZ_DAEMON_CPP app (that's the main thing) provides a fifo/pipe in /tmp to control the camera's motors.

How to start it?

`echo "init" >> /tmp/ptz.daemon`

This rotates and puts the motors back into original position.

Afterwards you can either set the speed of the camera or use the commands to oscilate the camera around.

```
- if (cmd[0] == "init")     --> Intializes the module
- if (cmd[0] == "setar")    --> set angle rate
- if (cmd[0] == "setdeg")   --> set degrees (?)
- if (cmd[0] == "t2p")      --> turn to position - main function to control cam
- if (cmd[0] == "setspeed") --> sets the speed at which the camera turns
- if (cmd[0] == "t")        --> turn 
- if (cmd[0] == "q")        --> quit, exits daemon
- if (cmd[0] == "ping")     --> returns "pong"
```

You can always go around and check the libplat_* files to check for more functions you may want to use.
This libplat_drv.so provided by the `LSC Smart Rotatable Camera` provides the following objects:

```
> arm-arm926ejs-linux-uclibcgnueabi-nm libs/libplat_drv.so | grep ak
         U ak_check_file_exist
         U ak_cmd_exec
00003af0 T ak_drv_i2c_close
00003804 T ak_drv_i2c_open
0000388c T ak_drv_i2c_read
00003984 T ak_drv_i2c_write
0000199c T ak_drv_ir_get_input_level
00001b5c T ak_drv_ir_get_threshold
00001990 T ak_drv_ir_get_version
00001d18 T ak_drv_ir_init
00001ebc T ak_drv_irled_get_version
00001f30 T ak_drv_irled_get_working_stat
00001ec8 T ak_drv_irled_init
00001fd8 T ak_drv_irled_set_working_stat
00001b48 T ak_drv_ir_set_check_mode
00001c1c T ak_drv_ir_set_ircut
00001bc4 T ak_drv_ir_set_threshold
00003660 T ak_drv_key_close
0000347c T ak_drv_key_get_event
00003414 T ak_drv_key_get_version
00003420 T ak_drv_key_open
00002770 T ak_drv_ptz_check_self
00002ad8 T ak_drv_ptz_check_self_ex
00003354 T ak_drv_ptz_close
00003224 T ak_drv_ptz_get_status
00002710 T ak_drv_ptz_get_step_pos
00002588 T ak_drv_ptz_get_version
00002594 T ak_drv_ptz_open
00002ee8 T ak_drv_ptz_reset_dg
00002ec0 T ak_drv_ptz_set_angle_rate
00002ed4 T ak_drv_ptz_set_degree
000032c0 T ak_drv_ptz_set_speed
0000269c T ak_drv_ptz_setup_step_param
00002f6c T ak_drv_ptz_turn
0000319c T ak_drv_ptz_turn_stop
00003064 T ak_drv_ptz_turn_to_pos
000037c0 T ak_drv_pwm_close
000036c0 T ak_drv_pwm_open
00003740 T ak_drv_pwm_set
00003c68 T ak_drv_wdt_close
00003c0c T ak_drv_wdt_feed
00003b10 T ak_drv_wdt_get_version
00003b1c T ak_drv_wdt_open
0000d1a8 d akmotor
```

To *compile* this application you need a viable compiler. I used crosstool-ng - the important parts are:

```
CT_ARCH="arm"
CT_ARCH_TUNE="arm926ej-s"
# Options for arm
CT_ARCH_ARM_MODE="arm"
CT_TARGET_VENDOR="arm926ejs"
CT_LIBC_UCLIBC_NG=y
CT_LIBC="uClibc-ng"
CT_CC_LANG_CXX=y
```

or, in words, uclibc, arm926ej-s tune, and c++.

You can easily adapt this project and do whatever you want with it, all of this is CC-BY-SA.
All the libs are copyright by their respective owners tho - so in this case I assume ANYKA? Or TUYA? Not entirely sure as there are various mixed licences in there.

Also, if you try to do it with a different ANYKA-based camera, please make sure to switch out the libs/ from the actual system (libdl, libm, libplat*, libpthread, libstdc++, libuclibc)

---

## Build: 
use `make`

---

## PTZ?

**P**an **T**ilt **Z**oom - basically the camera controls.
