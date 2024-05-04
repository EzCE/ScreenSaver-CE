appvar:
	@$(MAKE) -C appvar all

program:
	@$(MAKE) -C program all

clean:
	@$(MAKE) -C appvar clean
	@$(MAKE) -C program clean

all: program appvar

.PHONY: appvar program clean all
