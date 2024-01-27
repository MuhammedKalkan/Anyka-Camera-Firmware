#!/bin/sh


insmod /usr/modules/sensor_gc1054.ko
insmod /usr/modules/akcamera.ko
insmod /usr/modules/ak_info_dump.ko


/usr/custom/custom.sh &
/etc/jffs2/play.sh &



