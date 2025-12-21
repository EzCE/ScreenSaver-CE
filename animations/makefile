NAME = ANIMATE
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
	$(Q)convbin -j 8x -k asm -i $(BINDIR)\$(TARGET8XP) -o ..\app\src\asm\$(OUTPUTASM) -n $(NAME)