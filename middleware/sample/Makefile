include ../mkenv.mk

include $(MPP_SRC_DIR)/middleware/mpp.mk
include $(MPP_SRC_DIR)/middleware/rt-smart.mk

include $(SDK_SRC_ROOT_DIR)/.config

dir-y :=

dir-$(CONFIG_MPP_ENABLE_MIDDLEWARE_SAMPLES_DEMUXER)		+= sample_demuxer
dir-$(CONFIG_MPP_ENABLE_MIDDLEWARE_SAMPLES_MUXER)		+= sample_muxer
dir-$(CONFIG_MPP_ENABLE_MIDDLEWARE_SAMPLES_PLAYER)		+= sample_player
dir-$(CONFIG_MPP_ENABLE_MIDDLEWARE_SAMPLES_RTSP_CLIENT)	+= sample_rtspclient
dir-$(CONFIG_MPP_ENABLE_MIDDLEWARE_SAMPLES_RTSP_PUSHER)	+= sample_rtsppusher
dir-$(CONFIG_MPP_ENABLE_MIDDLEWARE_SAMPLES_RTSP_SERVER)	+= sample_rtspserver

# Add directories to the build system
dirs := $(sort $(dir-y))

.PHONY: all clean

all:
	@rm -rf $(MPP_MIDDLEWARE_ELF_INSTALL_PATH)/*.*
ifeq ($(CONFIG_MPP_ENABLE_MIDDLEWARE_SAMPLES),y)
	@$(foreach dir,$(dirs),make -C $(dir) all;)
endif
	@echo "Make MPP Middleware samples done."

clean:
	@rm -rf $(MPP_MIDDLEWARE_ELF_INSTALL_PATH)/*.*
ifeq ($(CONFIG_MPP_ENABLE_MIDDLEWARE_SAMPLES),y)
	@$(foreach dir,$(dirs),make -C $(dir) clean;)
endif
	@echo "Clean MPP Middleware samples done."
