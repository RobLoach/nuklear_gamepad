#ifndef NUKLEAR_GAMEPAD_GLFW_H__
#define NUKLEAR_GAMEPAD_GLFW_H__

#ifndef NK_GAMEPAD_MAX
#define NK_GAMEPAD_MAX GLFW_JOYSTICK_LAST
#endif  // NK_GAMEPAD_MAX

NK_API void nk_gamepad_glfw_update(struct nk_gamepads* gamepads);
NK_API const char* nk_gamepad_glfw_name(struct nk_gamepads* gamepads, int num);

#endif

#if defined(NK_GAMEPAD_IMPLEMENTATION) && !defined(NK_GAMEPAD_HEADER_ONLY)
#ifndef NUKLEAR_GAMEPAD_GLFW_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_GLFW_IMPLEMENTATION_ONCE

#ifndef NK_GAMEPAD_MALLOC
    #define NK_GAMEPAD_MALLOC(unused, old, size) malloc(size)
#endif

#ifndef NK_GAMEPAD_MFREE
    // TODO: Switch to allow using GLFWallocator.
    #define NK_GAMEPAD_MFREE(unused, ptr) free(ptr)
#endif

#define NK_GAMEPAD_UPDATE nk_gamepad_glfw_update
#define NK_GAMEPAD_NAME nk_gamepad_glfw_name

int nk_gamepad_glfw_map_button(int button) {
    switch (button) {
        case NK_GAMEPAD_BUTTON_UP: return GLFW_GAMEPAD_BUTTON_DPAD_UP;
        case NK_GAMEPAD_BUTTON_DOWN: return GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
        case NK_GAMEPAD_BUTTON_LEFT: return GLFW_GAMEPAD_BUTTON_DPAD_LEFT;
        case NK_GAMEPAD_BUTTON_RIGHT: return GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;
        case NK_GAMEPAD_BUTTON_A: return GLFW_GAMEPAD_BUTTON_A;
        case NK_GAMEPAD_BUTTON_B: return GLFW_GAMEPAD_BUTTON_B;
        case NK_GAMEPAD_BUTTON_X: return GLFW_GAMEPAD_BUTTON_X;
        case NK_GAMEPAD_BUTTON_Y: return GLFW_GAMEPAD_BUTTON_Y;
        case NK_GAMEPAD_BUTTON_LB: return GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;
        case NK_GAMEPAD_BUTTON_RB: return GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;
        case NK_GAMEPAD_BUTTON_BACK: return GLFW_GAMEPAD_BUTTON_BACK;
        case NK_GAMEPAD_BUTTON_START: return GLFW_GAMEPAD_BUTTON_START;
        default: return -1;
    }
}

void nk_gamepad_glfw_update(struct nk_gamepads* gamepads) {
    if (!gamepads) {
        return;
    }

    GLFWgamepadstate state;
    for (int num = 0; num < GLFW_JOYSTICK_LAST; num++) {
        if ((glfwJoystickPresent(num) == GLFW_FALSE) ||
            (glfwJoystickIsGamepad(num) == GLFW_FALSE) ||
            glfwGetGamepadState(num, &state) == GLFW_FALSE) {
            gamepads->gamepads[num].connected = nk_false;
            break;
        }

        gamepads->gamepads[num].connected = nk_true;
        for (int i = NK_GAMEPAD_BUTTON_FIRST; i < NK_GAMEPAD_BUTTON_LAST; i++) {
            int glfwButton = nk_gamepad_glfw_map_button(i);
            if (glfwButton >= 0 && state.buttons[glfwButton]) {
                nk_gamepad_button(gamepads, num, i, nk_true);
            }
        }
    }
}

const char* nk_gamepad_glfw_name(struct nk_gamepads* gamepads, int num) {
    if (!gamepads || num < 0 || num >= NK_GAMEPAD_MAX) {
        return NULL;
    }

    const char* name = glfwGetGamepadName(num);
    if (!name) {
        return gamepads->gamepads[num].name;
    }

    return name;
}

#endif
#endif
