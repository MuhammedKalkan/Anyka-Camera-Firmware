# Anyka-Camera-Firmware


![296179109-84c04be3-5052-445e-8a76-39865668989a](https://github.com/user-attachments/assets/293939b6-6a93-491c-bee3-4f25dc9262fd)
![296247365-8d110c89-1de7-4595-9225-d89119ab2856](https://github.com/user-attachments/assets/d60caff8-3413-479a-b534-25606e0d964f)


This repo contains firmware applications to use with Anyka based cameras. While this software is developed and tested spesifically on E27 bulb cameras.
applications should work on most cases

E27 Lightbulb and derivation cameras are packaged to be used with online software only named YIIOT or KAMI software. 

But with this repo , you can turn them into a network video recoding camera

There are
- RTSP application which provides AUDIO,VIDEO stream, Auto DAY/NIGHT switching
- PTZ Daemon to manage Camera Movement. Also storing ptz position
- And a WEBUI to control PTZ and LED lights
- Audible wifi connection status
- Daemon to check network and rtsp status, restart if needed


After new firmware, you will have full telnet access. As long as you are connected to wifi you dont have to go into hardware mode.

Developed on a platform which MCU is AK3918v200EN080, if you have different MCU, try to run repo apps. If they dont work, find your toolchain and compile for your MCU. I ve seen v300 and v330 Anyka MCUs are used. So learn your MCU type

There are two methods to run custom apps on camera.

# 1- SDCARD Method

This method does not alter your camera software and runs on SD card.

Simply copy Factory folder to your SD Card and reboot the camera.

If you have SD card inserted into your camera, you can copy that folder through FTP. This is good if your camera is out of easy reach

# 2- Overwriting Camera Software

This is good if you dont want to buy an SD card for each of your cameras.

But make sure you try SD Card method first and see apps and scripts are working for you.

Then you can create your custom firmware with your drivers or if you are sure that you dont have any different drivers as listed below use the one in Releases.

If you happen to have different drivers, your camera will not work, even if you tried SDcard method and all worked flawlessly.


Camera Sensors included in this repo are gc1084,gc1054, gc1034, H62 , H63. If yours is different (check /usr/modules and look for .ko files), use your own drivers . Look at [here](https://github.com/MuhammedKalkan/Anyka-Camera-Firmware?tab=readme-ov-file#different-hardware)

Wifi Sensors included in this repo are rtl8188fu,atbm603x_wifi_usb, ssv6x5x. If yours is different  (check /usr/modules and look for .ko files), use your own drivers . Look at [here](https://github.com/MuhammedKalkan/Anyka-Camera-Firmware?tab=readme-ov-file#different-hardware)

If you see different .ko file names, open an issue and attach your original /usr /etc/init.d and /etc/jffs2 folders. And wait for response

Also note that anyka_ipc app wont be on flash. There is just not enough room. You can use it via sdcard if you like later

## Before Beginning

Plug your camera to power , insert sdcard and connect it to whatever software it came with. Most probably YİIOT or such.

Just connect to wifi and see your camera is working. View it on your vendor app and make sure it is working.
  
You should backup your camera software incase any other unwanted situations. Steps after this one will modify your camera software permanently ,thus use at your own risk



================

**Most important folders are /usr and /etc/jffs2 . I advise  you to get these at least**. 

================


Also get /var/log/messages file. This will be of help

Now there are two ways to get files inside camera 

1 - FTP is usually enabled, you can download your camera software like

  `wget --user=username --password=password -r --preserve-permissions ftp://cameraip`

  This command will download all files in ftp. Sym links will be converted to regular files, you will have much more bigger sized folder.

2 - You can modify /etc/jffs2/time_zone.sh file and add `telnetd &` at the end of file, and restart it. After restart you will have telnet enabled
  
  Connect camera via `telnet cameraip` and then dump all 6 partitions using 
  `cat /dev/mtd0 > mtd0.bin`
  .
  .
  .
  `cat /dev/mtd6 > mtd6.bin`
  
  Move these dumps to your local pc. If your partitions are more or less.figure that out in /dev/mtd*. Just dump them all.

Getting into camera is documented very well by the links at the bottom.

## Quick Install

NOTE: If your camera is not lightbulb variant, cross check scripts in your dumped camera files and files inside rootfs folder of this repository. If they are almost same then you can proceed for quick install

0 - Get your /etc/jffs2/isp_xxx.conf file. And save it somewhere safe. It is needed in step 5

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

or install VLC on snap. Regular ubuntu VLC does not have rtsp support.

`sudo snap install vlc` and then play it as newtwork stream

![296181423-aebebf78-db28-423f-a360-02d6c11f5fe1](https://github.com/user-attachments/assets/0fcf88d6-cbd5-484e-a683-977566508686)


## Custom Scripting

In order to make scripting easy and without flashing, i have added a method.

If you create /etc/jffs2/custom.sh script , camera will run this on start. It wont start any apps.

Wifi will run before this custom script so avoid messing with wifi here, or you lock yourself out your camera (No telnet)

Just echo something if you want to try it, but i suggest dont do anything unless you know how to recover in lockout mode.(Meaning hardware connection)

## Settings Update

If you want to update wifi settings,change etc/jffs2/anyka_conf.ini file and place it in update folder sdcard. Restart the camera and settings will be updated. This is useful when you want to change wifi settings

## Creating Custom Update File

If you have a dump you can create your own update file

Form rootfs folder, copy contained files and folders to your dumps.

There are two different parts.

1- rootfs - bin and etc will go into this one and pack it with

`mksquashfs squashfs-root root.sqsh4 -comp xz -Xdict-size 100%`

2- usr   - custom wil lgo into this one and pack it with

`mksquashfs squashfs-usr usr.sqsh4 -comp xz -Xdict-size 100%`

Then with fw_version file pack them . You can also pack seperately(make sure it is above your current cam version or it wont update)

`tar cvf update.tar  usr.sqsh4   fw_version`


## Other Anyka Based Cameras (This part will need some sort of expertise)

- Connect to telnet using instructions above
- Kill anyka_ipc, cmd_serverd and daemon
- Copy binaries and libs in the repo
- Set LD_LIBRARY_PATH to libs folder
- Try to run software and see if it is working

  1- If it works , build your usr and rootfs and write them to flash
  2- If not, try to compile source and make it work somehow. Your camera must be different

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




