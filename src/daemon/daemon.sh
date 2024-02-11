#!/bin/sh

RTSP_URL="rtsp://127.0.0.1:554/vs0"
PING_HOST="8.8.8.8"  # Google's public DNS server or your router ip 192.168.1.1 if no internet available
RTSP_APP_PID=0
RTSP_APP_NAME="\/(?:[^\/]*\/)*[^\/]*rtsp$"  # Replace with the actual name of your RTSP application

# Function to check if RTSP stream is available
check_rtsp_stream() {
    if ./ffprobe -v error -select_streams v:0 -show_entries stream=codec_type -of default=noprint_wrappers=1:nokey=1 "$RTSP_URL" &> /dev/null; then
        echo "RTSP stream is available."
        return 0
    else
        echo "RTSP stream is not available."
        return 1
    fi
}

# Function to check network connectivity
check_network_connectivity() {
    if ping -c 1 "$PING_HOST" &> /dev/null; then
        echo "Network connectivity is available."
        return 0
    else
        echo "Network connectivity is not available."
        return 1
    fi
}

# Function to stop the RTSP application
stop_rtsp_application() {
    if [ "$RTSP_APP_PID" -ne 0 ]; then
        echo "Stopping RTSP application...$RTSP_APP_PID"
        kill -9 "$RTSP_APP_PID" 2>/dev/null
    fi
}

# Function to start the RTSP application
start_rtsp_application() {
    echo "Starting RTSP application..."
    export LD_LIBRARY_PATH=/usr/custom/lib
    /usr/custom/rtsp &
    # Add command to start your RTSP application here
    # Example: /path/to/your/rtsp/application &
    # Ensure the process ID of the application is captured correctly
}

# Main function
main() {
    if  ! check_network_connectivity; then    
       echo "Reboot Device"
       reboot                                                                                     
    elif ! check_rtsp_stream; then
        echo "Restart RTSP app"
        stop_rtsp_application
        start_rtsp_application
    else
        echo "RTSP stream and network connectivity are available."
    fi
}


# Loop to run the main function every 10 seconds
while true; do
    main
    sleep 60
done

