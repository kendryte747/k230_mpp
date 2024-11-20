include ../mkenv.mk

include $(MPP_SRC_DIR)/userapps/sample/mpp.mk
include $(MPP_SRC_DIR)/userapps/sample/rt-smart.mk

BUILD := $(MPP_BUILD_DIR)/fastboot_app

all:
	@mkdir -p $(BUILD)
	@cmake -S . -B $(BUILD)
	@cmake --build $(BUILD)
	@cmake --install $(BUILD)

clean:
	@rm -rf $(BUILD)

.PHONY: all clean
