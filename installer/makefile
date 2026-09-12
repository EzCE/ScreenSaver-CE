# ----------------------------
# Makefile Options
# ----------------------------

NAME = SCRNSAVR
ICON = icon.png
DESCRIPTION = "Screensaver Installer"
COMPRESSED = YES
COMPRESSED_MODE = zx0
ARCHIVED = YES
LTO = NO
HAS_PRINTF = NO

APPVAR_PREFIX = "ScrnSav"
APPVAR_SPLIT_SIZE = 65200

CFLAGS = -Wall -Wextra -Oz -DAPPVAR_PREFIX=\"$(APPVAR_PREFIX)\" -DAPPVAR_SPLIT_SIZE=$(APPVAR_SPLIT_SIZE)
CXXFLAGS = -Wall -Wextra -Oz -DAPPVAR_PREFIX=\"$(APPVAR_PREFIX)\" -DAPPVAR_SPLIT_SIZE=$(APPVAR_SPLIT_SIZE)

# ----------------------------

include $(shell cedev-config --makefile)
