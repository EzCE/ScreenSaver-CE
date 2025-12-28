all: animations app

animations:
# 	@$(MAKE) -C animations all

app:
	@$(MAKE) -C app

clean:
	@$(MAKE) -C animations clean
	@$(MAKE) -C app clean

gfx:
	@$(MAKE) -C animations gfx

.PHONY: animations app clean gfx all
