all:
	@$(MAKE) -C src all || exit $?;
	@$(MAKE) -C sample all || exit $?;

clean:
	@$(MAKE) -C src clean || exit $?;
	@$(MAKE) -C sample clean || exit $?;

.PHONY: all clean
