subdirs-y := sensor fft connector mediafreq pm

all:
	@$(foreach dir,$(subdirs-y),make -C $(dir) all;)
	@echo "Make MPP Kernel done."

clean:
	@$(foreach dir,$(subdirs-y),make -C $(dir) clean;)
	@echo "Make MPP Kernel clean done."
