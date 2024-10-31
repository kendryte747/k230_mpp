include $(MPP_SRC_DIR)/userapps/src/mpp.mk
include $(MPP_SRC_DIR)/userapps/src/rt-smart.mk

include $(SDK_SRC_ROOT_DIR)/.config

CURRECT_DIR_NAME=$(shell basename `pwd`)
LOCAL_SRC_DIR = $(shell pwd)
LIB = $(MPP_SRC_DIR)/userapps/lib/lib$(CURRECT_DIR_NAME).a

LOCAL_CFLAGS = -I$(LOCAL_SRC_DIR)/

obj-y := mpi_connector.o
obj-$(CONFIG_MPP_ENABLE_DSI_DEBUGGER) += debugger.o

OBJS = $(obj-y)

all: $(LIB)
	@-rm -f $(OBJS)
	echo "${PWD}/Makefile all"

$(OBJS): %.o : %.c
	@$(CC) $(CC_CFLAGS) $(LOCAL_CFLAGS) $(BSP_CFLGAS) $(RTSMART_CFLAGS) $(MPP_USER_CFLGAS) -c $< -o $@

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

clean:
	echo "${PWD}/Makefile clean"
	-rm -rf $(LIB)
	-rm -f $(OBJS)

.PHONY: all clean