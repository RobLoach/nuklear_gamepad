compile: configure
	cmake --build build

configure:
	cmake -B build -S .

clean:
	rm -rf build

test: compile
	./build/nuklear_gamepad_demo_raylib
