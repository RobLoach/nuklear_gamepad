glfw:
	$(MAKE) -C demo/glfw test

sdl:
	$(MAKE) -C demo/sdl test

sdl3:
	$(MAKE) -C demo/sdl3 test

none:
	$(MAKE) -C demo/none test

pntr:
	$(MAKE) -C demo/pntr test

raylib:
	$(MAKE) -C demo/raylib test

none:
	$(MAKE) -C demo/none test
