include ../mkenv.mk

include $(MPP_SRC_DIR)/middleware/mpp.mk
include $(MPP_SRC_DIR)/middleware/rt-smart.mk

dir-y := kdmedia live555 mp4_format mp4_player rtsp_client  rtsp_server

# dir-y += rtsp_pusher

# Add directories to the build system
dirs := $(sort $(dir-y))

all:
	@rm -rf $(MPP_MIDDLEWARE_LIB_INSTALL_PATH)/*
	@$(foreach dir,$(dirs),$(MAKE) -C $(dir) all || exit $?;)
	@echo "Make MPP middleware libraries done."

clean:
	@rm -rf $(MPP_MIDDLEWARE_LIB_INSTALL_PATH)/*
	@$(foreach dir,$(dirs),$(MAKE) -C $(dir) clean || exit $?;)
	@echo "Clean MPP middleware libraries done."

.PHONY: all clean
