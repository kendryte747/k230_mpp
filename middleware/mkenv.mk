ifneq ($(MKENV_INCLUDED),1)
export SDK_SRC_ROOT_DIR := $(realpath $(dir $(realpath $(lastword $(MAKEFILE_LIST))))/../../../../)

include $(SDK_SRC_ROOT_DIR)/tools/mkenv.mk

export MPP_SRC_DIR := $(SDK_RTSMART_SRC_DIR)/mpp/
export RTSMART_SRC_DIR := $(SDK_RTSMART_SRC_DIR)/rtsmart/
export MPP_BUILD_DIR := $(SDK_RTSMART_BUILD_DIR)/mpp/
endif

export MPP_MIDDLEWARE_LIB_INSTALL_PATH := $(MPP_SRC_DIR)/middleware/lib/
$(shell if [ ! -e $(MPP_MIDDLEWARE_LIB_INSTALL_PATH) ];then mkdir -p $(MPP_MIDDLEWARE_LIB_INSTALL_PATH); fi)

export MPP_MIDDLEWARE_ELF_INSTALL_PATH := $(MPP_SRC_DIR)/middleware/elf/
$(shell if [ ! -e $(MPP_MIDDLEWARE_ELF_INSTALL_PATH) ];then mkdir -p $(MPP_MIDDLEWARE_ELF_INSTALL_PATH); fi)

include $(SDK_TOOLS_DIR)/toolchain_rtsmart.mk
export PATH:=$(CROSS_COMPILE_DIR):$(PATH)
