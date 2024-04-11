# nuklear_gamepad

Gamepad API for [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear).

![Screenshot](demo/common/nuklear_gamepad_demo.png)

## Example

``` c
#define NK_GAMEPAD_SDL
//#define NK_GAMEPAD_GLFW
//#define NK_GAMEPAD_RAYLIB
#include "nuklear_gamepad.h"

// Initialize the gamepad system.
struct nk_gamepads* gamepads = nk_gamepad_init(ctx);

// Act on whehter or not the A button is pushed
if (nk_gamepad_is_button_down(gamepads, 0, NK_GAMEPAD_BUTTON_A)) {
    nk_label(ctx, "Button A is down", NK_TEXT_LEFT);
}

// Clean up the memory
nk_gamepad_free(gamepads);
```

## Platform Support

- [SDL](https://www.libsdl.org/)
- [GLFW](https://www.glfw.org/)
- [raylib](https://www.raylib.com/)
- Add more!

## API

``` c
struct nk_gamepads* nk_gamepad_init(struct nk_context* ctx);
void nk_gamepad_free(struct nk_gamepads* gamepads);
NK_API void nk_gamepad_init_gamepads(struct nk_gamepads* gamepads, int num);
nk_bool nk_gamepad_is_button_down(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button);
nk_bool nk_gamepad_is_button_pressed(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button);
nk_bool nk_gamepad_is_button_released(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button);
void nk_gamepad_button(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button, nk_bool down);
int nk_gamepad_count(struct nk_gamepads* gamepads);
const char* nk_gamepad_name(struct nk_gamepads* gamepads, int num);
```

## License

Unless stated otherwise, all works are:

- Copyright (c) 2024 [Rob Loach](https://robloach.net)

... and licensed under:

- [zlib License](LICENSE)
