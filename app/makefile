# ----------------------------
# Makefile Options
# ----------------------------

NAME = ScrnSavr
ICON = icon.png
APPVAR_PREFIX = ScrnSav
APPLICATION = YES
APPLICATION_DESCRIPTION = "(C)  2025-2026  RoccoLox  &  TIny_Hacker"
COMPRESSED = NO
ARCHIVED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)

appvar:
	@make
	@convbin --iformat 8ek --input bin/$(NAME).8ek --oformat 8xv-split --maxvarsize 65200 --output bin/$(APPVAR_PREFIX).8xv --name $(APPVAR_PREFIX)
