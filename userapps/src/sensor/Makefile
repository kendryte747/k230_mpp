include $(MPP_SRC_DIR)/userapps/src/mpp.mk
include $(MPP_SRC_DIR)/userapps/src/rt-smart.mk

ifneq ($(shell [ -d ${SDK_BUILD_IMAGES_DIR}/bin ] && echo 1 || echo 0),1)
$(shell mkdir -p ${SDK_BUILD_IMAGES_DIR}/bin)
endif

CURRECT_DIR_NAME=$(shell basename `pwd`)
LOCAL_SRC_DIR = $(shell pwd)
LIB = $(MPP_SRC_DIR)/userapps/lib/lib$(CURRECT_DIR_NAME).a

LOCAL_CFLAGS = -I$(LOCAL_SRC_DIR)/

SRCS = $(wildcard $(LOCAL_SRC_DIR)/*.c)

OBJS = $(patsubst %.c,%.o,$(SRCS))

YMLS = $(wildcard $(LOCAL_SRC_DIR)/dewarp/*.yml)

DWCFGS = $(patsubst %.yml,%.bin,$(YMLS))

all: $(LIB) $(DWCFGS)
	@-rm -rf $(OBJS)
	@rm -rf build
	@rm -rf ${SDK_BUILD_IMAGES_DIR}/bin/*
	@cp $(PWD)/config/* ${SDK_BUILD_IMAGES_DIR}/bin
	@cp $(PWD)/dewarp/*.bin ${SDK_BUILD_IMAGES_DIR}/bin
	@echo "${PWD}/Makefile all"

$(OBJS): %.o : %.c
	@$(CC) $(CC_CFLAGS) $(LOCAL_CFLAGS) $(BSP_CFLGAS) $(RTSMART_CFLAGS) $(MPP_USER_CFLGAS) -c $< -o $@

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

k230dwmapgen:
	cmake -S $(LOCAL_SRC_DIR)/dewarp/k230dwmapgen -B build
	cmake --build build -j

$(DWCFGS): %.bin : %.yml k230dwmapgen
	./build/k230dwmapgen-dump < $< > $@

clean:
	echo "${PWD}/Makefile clean"
	-rm -rf $(LIB)
	-rm -f $(OBJS)
	-rm -rf build

.PHONY: all clean k230dwmapgen
