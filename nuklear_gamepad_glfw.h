#ifndef NUKLEAR_GAMEPAD_GLFW_H__
#define NUKLEAR_GAMEPAD_GLFW_H__

#if !defined(NK_GAMEPAD_MAX) && defined(GLFW_JOYSTICK_LAST)
#define NK_GAMEPAD_MAX GLFW_JOYSTICK_LAST
#elif !defined(NK_GAMEPAD_MAX)
#define NK_GAMEPAD_MAX 4
#endif

#ifdef __cplusplus
extern "C" {
#endif

NK_API void nk_gamepad_glfw_update(struct nk_gamepads* gamepads, void* user_data);
NK_API const char* nk_gamepad_glfw_name(struct nk_gamepads* gamepads, int num, void* user_data);
NK_API struct nk_gamepad_input_source nk_gamepad_glfw_input_source(void* user_data);

#ifdef __cplusplus
}
#endif

#endif

#if defined(NK_GAMEPAD_IMPLEMENTATION) && !defined(NK_GAMEPAD_HEADER_ONLY)
#ifndef NUKLEAR_GAMEPAD_GLFW_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_GLFW_IMPLEMENTATION_ONCE

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
        case NK_GAMEPAD_BUTTON_GUIDE: return GLFW_GAMEPAD_BUTTON_GUIDE;
        default: return -1;
    }
}

void nk_gamepad_glfw_update(struct nk_gamepads* gamepads, void* user_data) {
    GLFWgamepadstate state;
    int num, i;
    NK_UNUSED(user_data);
    if (!gamepads) {
        return;
    }

    for (num = 0; num < NK_GAMEPAD_MAX; num++) {
        /* Make sure it's available. */
        if (glfwJoystickIsGamepad(num) == GLFW_FALSE) {
            gamepads->gamepads[num].available = nk_false;
            continue;
        }

        /* Grab the state. */
        if (glfwGetGamepadState(num, &state) == GLFW_FALSE) {
            gamepads->gamepads[num].available = nk_false;
            continue;
        }

        gamepads->gamepads[num].available = nk_true;
        for (i = NK_GAMEPAD_BUTTON_FIRST; i < NK_GAMEPAD_BUTTON_LAST; i++) {
            int glfwButton = nk_gamepad_glfw_map_button(i);
            if (glfwButton >= 0 && state.buttons[glfwButton]) {
                nk_gamepad_button(gamepads, num, (enum nk_gamepad_button)i, nk_true);
            }
        }

        /* Axes: GLFW sticks are -1..1, triggers are reported as -1..1 and remapped to 0..1 */
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_LEFT_X,       state.axes[GLFW_GAMEPAD_AXIS_LEFT_X]);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_LEFT_Y,       state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_RIGHT_X,      state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X]);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_RIGHT_Y,      state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_LEFT_TRIGGER,  (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER]  + 1.0f) * 0.5f);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_RIGHT_TRIGGER, (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f) * 0.5f);
    }
}

const char* nk_gamepad_glfw_name(struct nk_gamepads* gamepads, int num, void* user_data) {
    const char* name;
    NK_UNUSED(user_data);
    name = glfwGetGamepadName(num);
    if (name == NULL || name[0] == '\0') {
        return gamepads->gamepads[num].name;
    }

    return name;
}

NK_API struct nk_gamepad_input_source nk_gamepad_glfw_input_source(void* user_data) {
    struct nk_gamepad_input_source source;
    source.user_data = user_data;
    source.init = NULL;
    source.update = &nk_gamepad_glfw_update;
    source.free = NULL;
    source.name = &nk_gamepad_glfw_name;
    source.input_source_name = "glfw";
    source.id = NK_GAMEPAD_INPUT_SOURCE_GLFW;
    return source;
}

#ifdef __cplusplus
}
#endif

#endif
#endif
