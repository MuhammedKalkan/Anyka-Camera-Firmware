#!/bin/sh



/usr/sbin/camera.sh setup

# ANYKA IPC SECTION ENABLE THESE IF YOU WANT THAT
#/usr/sbin/service.sh start &
# ANYKA IPC SECTION ENABLE THESE IF YOU WANT THAT


# RTSP APP ENABLE THESE IF YOU WANT THAT
/usr/custom/custom.sh &
/etc/jffs2/play.sh &
# RTSP APP ENABLE THESE IF YOU WANT THAT



