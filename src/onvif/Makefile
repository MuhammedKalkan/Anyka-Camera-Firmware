# appointed libs and their dir
LOCAL_SHARED_LIB := akuio akmedialib akv_encode akispsdk
LOCAL_STATIC_LIB := onvif

ifeq ($(COMPILE_SO), y)
LOCAL_SHARED_LIB+= plat_common plat_thread plat_vi plat_vpss plat_ipcsrv plat_venc_cb
LOCAL_SHARED_LIB+= mpi_venc mpi_osd
LOCAL_SHARED_LIB+= app_ini app_osd_ex app_net
else
# decide locate what kind of lib
PLAT_STATIC_LIB := plat_common plat_thread plat_vi plat_vpss \
	plat_drv plat_ipcsrv plat_venc_cb
PLAT_STATIC_LIB += 
ifeq ($(DEBUG), y)
	PLAT_STATIC_LIB += dmallocthcxx dmallocth
endif
MPI_STATIC_LIB := mpi_venc mpi_osd
APP_STATIC_LIB := app_ini app_osd_ex app_net
endif

# appointed link lib
LIBS := $(addprefix $(PLAT_LIB_DIR)/,$(patsubst %,lib%.a,$(PLAT_STATIC_LIB)))
LIBS += $(addprefix $(MPI_LIB_DIR)/,$(patsubst %,lib%.a,$(MPI_STATIC_LIB)))
LIBS += $(addprefix $(APP_LIB_DIR)/,$(patsubst %,lib%.a,$(APP_STATIC_LIB)))
LIBS += $(patsubst %,lib%.a,$(LOCAL_STATIC_LIB))

LDFLAGS := -L$(PLAT_LIB_DIR) -L$(MPI_LIB_DIR) -L$(APP_LIB_DIR)
LDFLAGS += $(addprefix -l,$(patsubst lib%,%,$(LOCAL_SHARED_LIB)))
LDFLAGS += $(STD_LDFLAGS)

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))
TARGET := ak_onvif_demo

.PHONY: all install clean

all:$(TARGET)

$(TARGET): $(OBJS)
	$(LDCXX) $^ $(LDFLAGS) -Xlinker "-(" $(LIBS) -Xlinker "-)" -o $@
	$(STRIP) $@

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@
	@echo ""
	
install:
	@$(CP) $(TARGET) $(ROOTFS_BIN_DIR)
	@echo ""

clean:	
	$(RM) *.o
	$(RM) $(TARGET)
