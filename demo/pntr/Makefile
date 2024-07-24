compile: build/Makefile
	cmake --build build

build/Makefile:
	cmake -B build -S .

test: compile
	./build/nuklear_gamepad_pntr_raylib

clean:
	rm -rf build
