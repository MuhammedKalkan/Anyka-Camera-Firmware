#! /bin/sh

restart_process()
{
  echo 'restarting web interface...'
  busybox httpd -p 8080 -h /usr/custom/web_interface/www
}

check_process_health()
{
  myresult=$( top -n 1 | grep snapshot | grep -v grep | grep -v daemon )
  echo $myresult
  if [[ ${#myresult} -lt 5 ]]; then
    restart_process
  fi
}


#while [ 1 ]; do
  #check_process_health
  restart_process
  sleep 5
  /usr/custom/ptz_daemon &
 
#done
