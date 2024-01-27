#!/bin/bash
export PATH=$PATH:/opt/arm-anykav200-crosstool/usr/bin
arm-anykav200-linux-uclibcgnueabi-gcc *.cpp -std=c++11 -D_GNU_SOURCE   -Llib  -lplat_drv -lplat_thread   -lplat_common  -lpthread   -D_GLIBCXX_USE_CXX11_ABI=0 -lstdc++ -o ptz_daemon.out


