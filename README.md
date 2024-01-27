# Anyka-Camera-Firmware


![190591214-fce64616-8f5d-40e2-b1f2-7ab6d3802d3b](https://github.com/MuhammedKalkan/E27-Camera-Hack-Anyka3918/assets/19736494/84c04be3-5052-445e-8a76-39865668989a)
![222040166-b6145183-1f35-4915-a977-f99d149d1128](https://github.com/MuhammedKalkan/E27-Lightbulb-Camera-Hack/assets/19736494/8d110c89-1de7-4595-9225-d89119ab2856)


This repo contains firmware applications to use with Anyka based cameras. While this software is developed and tested spesifically on E27 bulb cameras.
applications should work on most cases

E27 Lightbulb and derivation cameras are packaged to be used with online software only named YIIOT or KAMI software. 

But with this repo , you can turn them into a network video recoding camera

There are
- RTSP application which provides AUDIO,VIDEO stream, Auto DAY/NIGHT switching
- PTZ Daemon to manage Camera Movement
- And a WEBUI to control PTZ and LED lights

## Before Beginning

Plug your camera to power , insert sdcard and connect it to whatever software it came with. Most probably YİIOT or such.

Just connect to wifi and see your camera is working. View it on your vendor app and make sure it is working.
  
You should backup your camera software incase any other unwanted situations. Steps after this one will modify your camera software permanently ,thus use at your own risk

Most important folders are /usr and /etc/jffs2 . I advise  you to get these at least. Also get /var/log/messages file. This will be of help

Now there are two ways to get files inside camera 

1 - FTP is usually enabled, you can download your camera software like

  `wget --user=username --password=password -r --preserve-permissions ftp://cameraip`

  This command will download all files in ftp. Sym links will be converted to regular files, you will have much more bigger sized folder.

2 - You can modify /etc/jffs2/time_zone.sh file and add `telnetd &` at the end of file, and restart it. After restart you will have telnet enabled
  
  Connect camera via `telnet cameraip` and then dump all 6 partitions using `cat /dev/mtd0 > mtd0.bin`. Move these dumps to your local pc

Getting into camera is documented very well by the links at the bottom.

## Quick Install

NOTE: If you are not sure that this will work for your camera, cross check scripts in your dumped camera files and files inside rootfs folder of this repository. If they are almost same then you can proceed for quick install

1- Download the update file in Releases and copy it to sdcard. Create a folder named `update` and place update.tar inside that folder.

2- Restart the camera, you will hear 'Firmware Update Started' warning and beeping afterwards.

3- When it is done you will hear 'Update complete.Restarting'

4- After that remove sdcard, or you will update same file over again

5- Camera will start but you will need to copy your isp_xxxxxx.conf file inside original camera folder of /etc/jffs2

   Update process will delete this file, only first and last time you will need to copy this to your /etc/jffs2 folder again.
   
   If you are not sure which file, try to investigate /var/log/messages and locate same file name to see which is yours.
   
6- After copying from your original /etc/jffs2/isp_xxxx.conf file to your newly flashed /etc/jffs2 folder restart camera

7- Your RTSP feed will be broadcasted at

`rtsp://ipcamera:554/vs0`  for high res
`rtsp://ipcamera:554/vs1`  for low res

8- Your camera controls will be enabled at

`http://ipcamera:8080`


After running software you should be seeing your stream using 

ffplay "rtsp://IP:554/vs0" and vs1

![image](https://github.com/MuhammedKalkan/E27-Camera-Hack-Anyka3918/assets/19736494/aebebf78-db28-423f-a360-02d6c11f5fe1)

## Settings Update

If you want to update wifi settings,change etc/jffs2/anyka_conf.ini file and place it in update folder sdcard. Restart the camera and settings will be updated. This is useful when you want to change wifi settings

## Other Anyka Based Cameras (This part will need some sort of expertise)

- Connect to telnet using instructions above
- Kill anyka_ipc, cmd_serverd and daemon
- Copy binaries and libs in the repo
- Set LD_LIBRARY_PATH to libs folder
- Try to run software and see if it is working

  1- If it works , build your usr and rootfs and write them to flash
  2- If not, try to compile source and make it work somehow. Your camera must be different

For all these steps Gerge documented very well, you should go read it . Link is at the bottom

## Different Hardware

Normally these cameras come with many different Camera Sensor / Wifi Adapter and CPU

Current software is tested for Anyka3918v200 MCU , GCXXXX, HXX Sensor and Realtek, ATM WIFI. 

Other than these hardware 

if you have different MCU and this does not work for you, you can try to compile using your MCU toolchain (current Anyka3918v200)

If you have different Camera Sensor, use your stock ISP file (current /etc/jffs2/sensor_gc1054.conf) and sensor driver (current /usr/modules/sensor_gc1054.ko)

If you have different WIFI, use stock for that aswell. This repo should work for 2 different WIFI adapters as far as i know and you should be fine because havent heard other than these two.

## Onvif

There is also onvif app but it lacks ptz and sound as well as auto day night. So that is a choice

## Important Links
Thanks for the work of these ppl, because without them this would not have been possible

https://github.com/ricardojlrufino/anyka_v380ipcam_experiments

https://gitea.raspiweb.com/Gerge/Anyka_ak3918_hacking_journey

https://github.com/e27-camera-hack/E27-Camera-Hack/discussions/1


