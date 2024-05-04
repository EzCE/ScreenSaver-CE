all: appvar program

appvar:
	@$(MAKE) -C appvar all

program:
	@$(MAKE) -C program all

clean:
	@$(MAKE) -C appvar clean
	@$(MAKE) -C program clean

.PHONY: appvar program clean all
