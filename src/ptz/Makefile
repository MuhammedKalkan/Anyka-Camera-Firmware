PREFIX=arm-linux-gnueabi-
CC=${PREFIX}gcc
CPP=${PREFIX}g++
LINKERFLAG=-lm -lc -ldl -Wl,--allow-shlib-undefined
CPPFLAGS=-lm -lc -ldl -lstdc++ -L./libs -ldl -lplat_drv -lplat_common -lplat_thread -lpthread -D_GLIBCXX_USE_CXX11_ABI=0 -static-libgcc -static-libstdc++ --std=c++11
INCLUDES=libs/*

SRCS := ptz_driver.c ptz_daemon.c ptz_daemon_cpp.cpp

.PHONY = all clean

all: clean  ptz_daemon_cpp

%: %.cpp
	${CPP} ${INCLUDES} ${CPPFLAGS} $< -o $@
%: %.c
	${CC} ${INCLUDES} $< ${LINKERFLAG} -o $@

clean:
	rm -v ptz_driver ptz_daemon ptz_daemon_cpp | true
