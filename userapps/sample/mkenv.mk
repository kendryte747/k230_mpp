ifneq ($(MKENV_INCLUDED),1)
export SDK_SRC_ROOT_DIR := $(realpath $(dir $(realpath $(lastword $(MAKEFILE_LIST))))/../../../../../)

include $(SDK_SRC_ROOT_DIR)/tools/mkenv.mk

export MPP_SRC_DIR := $(SDK_RTSMART_SRC_DIR)/mpp/
export RTSMART_SRC_DIR := $(SDK_RTSMART_SRC_DIR)/rtsmart/
export MPP_BUILD_DIR := $(SDK_RTSMART_BUILD_DIR)/mpp/
endif

export MPP_USERAPP_SMAPLE_ELF_DIR := $(MPP_SRC_DIR)/userapps/sample/elf
$(shell if [ ! -e $(MPP_USERAPP_SMAPLE_ELF_DIR) ];then mkdir -p $(MPP_USERAPP_SMAPLE_ELF_DIR); fi)

include $(SDK_TOOLS_DIR)/toolchain_rtsmart.mk
export PATH:=$(CROSS_COMPILE_DIR):$(PATH)
