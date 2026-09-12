ifeq ($(OS),Windows_NT)
SHELL = cmd.exe
RMDIR = ( rmdir /s /q $(subst /,\,$(1)) 2>nul || call )
MKDIR = ( mkdir $(subst /,\,$(1)) 2>nul || call )
else
RMDIR = rm -rf $1
MKDIR = mkdir -p $1
endif

ANIMATIONS = \
	aod \
	beziers \
	baubles \
	colors \
	fish \
	matrix \
	merth \
	pipes \
	rain \
	snow \
	spirograph \
	starfield \
	strands \
	tiles \
	toasters \
	triss \
	walk

build: animations app

app:
	@$(MAKE) -C app appvar
	@$(MAKE) -C installer

gfx: $(addprefix gfx-,$(ANIMATIONS))

clean: $(addprefix clean-,$(ANIMATIONS))
	@$(call RMDIR,animations/bin)
	@$(MAKE) -C app clean
	@$(MAKE) -C installer clean

animations: $(addprefix build-,$(ANIMATIONS))
	@$(call MKDIR,animations/bin)

$(addprefix build-,$(ANIMATIONS)):
	@$(MAKE) -C animations/$(patsubst build-%,%,$@) build

$(addprefix clean-,$(ANIMATIONS)):
	@$(MAKE) -C animations/$(patsubst clean-%,%,$@) clean

$(addprefix gfx-,$(ANIMATIONS)):
	@$(MAKE) -C animations/$(patsubst gfx-%,%,$@) gfx

.PHONY: animations app clean gfx build
.PHONY: $(addprefix build-,$(ANIMATIONS))
.PHONY: $(addprefix clean-,$(ANIMATIONS))
.PHONY: $(addprefix gfx-,$(ANIMATIONS))
