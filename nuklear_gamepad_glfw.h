#ifndef NUKLEAR_GAMEPAD_GLFW_H__
#define NUKLEAR_GAMEPAD_GLFW_H__

#ifndef NK_GAMEPAD_MAX
#define NK_GAMEPAD_MAX GLFW_JOYSTICK_LAST
#endif  // NK_GAMEPAD_MAX

#ifdef __cplusplus
extern "C" {
#endif

NK_API void nk_gamepad_glfw_update(struct nk_gamepads* gamepads, void* user_data);
NK_API const char* nk_gamepad_glfw_name(struct nk_gamepads* gamepads, int num, void* user_data);
NK_API struct nk_gamepad_input_source nk_gamepad_glfw_input_soure(void* user_data);

#ifdef __cplusplus
}
#endif

#endif

#if defined(NK_GAMEPAD_IMPLEMENTATION) && !defined(NK_GAMEPAD_HEADER_ONLY)
#ifndef NUKLEAR_GAMEPAD_GLFW_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_GLFW_IMPLEMENTATION_ONCE

#ifndef NK_GAMEPAD_DEFAULT_INPUT_SOURCE
#define NK_GAMEPAD_DEFAULT_INPUT_SOURCE nk_gamepad_glfw_input_soure
#endif

#ifdef __cplusplus
extern "C" {
#endif

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

void nk_gamepad_glfw_update(struct nk_gamepads* gamepads, void* user_data) {
    NK_UNUSED(user_data);
    if (!gamepads) {
        return;
    }

    GLFWgamepadstate state;
    for (int num = 0; num < GLFW_JOYSTICK_LAST; num++) {
        if ((glfwJoystickPresent(num) == GLFW_FALSE) ||
            (glfwJoystickIsGamepad(num) == GLFW_FALSE) ||
            (glfwGetGamepadState(num, &state) == GLFW_FALSE)) {
            gamepads->gamepads[num].available = nk_false;
            continue;
        }

        gamepads->gamepads[num].available = nk_true;
        for (int i = NK_GAMEPAD_BUTTON_FIRST; i < NK_GAMEPAD_BUTTON_LAST; i++) {
            int glfwButton = nk_gamepad_glfw_map_button(i);
            if (glfwButton >= 0 && state.buttons[glfwButton]) {
                nk_gamepad_button(gamepads, num, (enum nk_gamepad_button)i, nk_true);
            }
        }
    }
}

const char* nk_gamepad_glfw_name(struct nk_gamepads* gamepads, int num, void* user_data) {
    NK_UNUSED(user_data);
    const char* name = glfwGetGamepadName(num);
    if (name == NULL || name[0] == '\0') {
        return gamepads->gamepads[num].name;
    }

    return name;
}

NK_API struct nk_gamepad_input_source nk_gamepad_glfw_input_soure(void* user_data) {
    struct nk_gamepad_input_source source = {
        user_data,
        NULL,
        &nk_gamepad_glfw_update,
        NULL,
        &nk_gamepad_glfw_name,
    };
    return source;
}

#ifdef __cplusplus
}
#endif

#endif
#endif
