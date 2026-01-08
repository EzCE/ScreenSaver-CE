NAME = output
APPVAR_NAME = Tiles
ASM_SRC = tiles.asm
COMPRESSED = NO
OUTPUT_MAP = NO

# limit heap to only pixelShadow2
BSSHEAP_LOW = D052C6
BSSHEAP_HIGH = D07395

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)

OUTPUTASM = $(NAME).asm

all:
	$(Q)convbin -j 8x -k asm -i $(BINDIR)/$(TARGET8XP) -o $(BINDIR)/$(OUTPUTASM) -n $(NAME)
	$(Q)fasmg asm/$(ASM_SRC) ../$(BINDIR)/$(APPVAR_NAME).8xv
