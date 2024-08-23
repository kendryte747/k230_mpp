include $(SDK_SRC_ROOT_DIR)/.config

subdirs-y := sensor fft connector mediafreq

subdirs-$(CONFIG_MPP_ENABLE_PM)	+= pm

all:
	@$(foreach dir,$(subdirs-y),make -C $(dir) all;)
	@echo "Make MPP Kernel done."

clean:
	@$(foreach dir,$(subdirs-y),make -C $(dir) clean;)
	@echo "Make MPP Kernel clean done."
