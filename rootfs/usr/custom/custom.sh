#!/bin/sh

echo "Running Hack Scripts"

export LD_LIBRARY_PATH=/usr/custom/lib


/usr/custom/rtsp &
/usr/custom/web_interface/start_web_interface.sh &



