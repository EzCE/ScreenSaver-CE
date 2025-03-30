all: appvar program

appvar:
	@$(MAKE) -C appvar all

program:
	@$(MAKE) -C program all

clean:
	@$(MAKE) -C appvar clean
	@$(MAKE) -C program clean

gfx:
	@$(MAKE) -C appvar gfx

.PHONY: appvar program clean gfx all
