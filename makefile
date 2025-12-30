all: animations app

animations:
	@$(MAKE) -C animations/aod all
	@$(MAKE) -C animations/beziers all
	@$(MAKE) -C animations/baubles all
	@$(MAKE) -C animations/fish all
	@$(MAKE) -C animations/matrix all
	@$(MAKE) -C animations/merth all
	@$(MAKE) -C animations/pipes all
	@$(MAKE) -C animations/rain all
	@$(MAKE) -C animations/snow all
	@$(MAKE) -C animations/spirograph all
	@$(MAKE) -C animations/starfield all
	@$(MAKE) -C animations/strands all
	@$(MAKE) -C animations/tiles all
	@$(MAKE) -C animations/toasters all
	@$(MAKE) -C animations/triss all
	@$(MAKE) -C animations/walk all

app:
	@$(MAKE) -C app

clean:
	@$(MAKE) -C animations/aod clean
	@$(MAKE) -C animations/beziers clean
	@$(MAKE) -C animations/baubles clean
	@$(MAKE) -C animations/fish clean
	@$(MAKE) -C animations/matrix clean
	@$(MAKE) -C animations/merth clean
	@$(MAKE) -C animations/pipes clean
	@$(MAKE) -C animations/rain clean
	@$(MAKE) -C animations/snow clean
	@$(MAKE) -C animations/spirograph clean
	@$(MAKE) -C animations/starfield clean
	@$(MAKE) -C animations/strands clean
	@$(MAKE) -C animations/tiles clean
	@$(MAKE) -C animations/toasters clean
	@$(MAKE) -C animations/triss clean
	@$(MAKE) -C animations/walk clean
	@$(MAKE) -C app clean

gfx:
	@$(MAKE) -C animations/merth gfx
	@$(MAKE) -C animations/pipes gfx
	@$(MAKE) -C animations/tiles gfx
	@$(MAKE) -C animations/toasters gfx

.PHONY: animations app clean gfx all
