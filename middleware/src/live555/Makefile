dir-y := UsageEnvironment BasicUsageEnvironment groupsock liveMedia mediaServer

# Add directories to the build system
dirs := $(sort $(dir-y))

all:
	@$(foreach dir,$(dirs),$(MAKE) -C $(dir) all || exit $?;)

clean:
	@$(foreach dir,$(dirs),$(MAKE) -C $(dir) clean || exit $?;)

.PHONY: all clean
