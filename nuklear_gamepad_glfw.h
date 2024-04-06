#ifndef NUKLEAR_GAMEPAD_GLFW_H__
#define NUKLEAR_GAMEPAD_GLFW_H__

NK_API void nk_gamepad_glfw_update(struct nk_gamepads* gamepads);
NK_API const char* nk_gamepad_glfw_name(struct nk_gamepads* gamepads, int num);

#endif

#ifdef NK_IMPLEMENTATION
#ifndef NUKLEAR_GAMEPAD_GLFW_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_GLFW_IMPLEMENTATION_ONCE

void nk_gamepad_glfw_update(struct nk_gamepads* gamepads) {
    if (!gamepads) {
        return;
    }

    if (gamepads->gamepads == NULL) {
        nk_handle unused;
        gamepads->gamepads = (struct nk_gamepad*)nk_malloc(unused, NULL, GLFW_JOYSTICK_LAST * sizeof(struct nk_gamepad));
        nk_zero(gamepads->gamepads, GLFW_JOYSTICK_LAST * sizeof(struct nk_gamepad));
    }

    int button_mapping[NK_GAMEPAD_BUTTON_MAX] = {
        GLFW_GAMEPAD_BUTTON_DPAD_UP, /* NK_GAMEPAD_BUTTON_UP */
        GLFW_GAMEPAD_BUTTON_DPAD_DOWN, /* NK_GAMEPAD_BUTTON_DOWN */
        GLFW_GAMEPAD_BUTTON_DPAD_LEFT, /* NK_GAMEPAD_BUTTON_LEFT */
        GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, /* NK_GAMEPAD_BUTTON_RIGHT */
        GLFW_GAMEPAD_BUTTON_A, /* NK_GAMEPAD_BUTTON_A */
        GLFW_GAMEPAD_BUTTON_B, /* NK_GAMEPAD_BUTTON_B */
        GLFW_GAMEPAD_BUTTON_X, /* NK_GAMEPAD_BUTTON_X */
        GLFW_GAMEPAD_BUTTON_Y, /* NK_GAMEPAD_BUTTON_Y */
        GLFW_GAMEPAD_BUTTON_LEFT_BUMPER, /* NK_GAMEPAD_BUTTON_LB */
        GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER, /* NK_GAMEPAD_BUTTON_RB */
        GLFW_GAMEPAD_BUTTON_BACK, /* NK_GAMEPAD_BUTTON_BACK */
        GLFW_GAMEPAD_BUTTON_START /* NK_GAMEPAD_BUTTON_START */
    };

    gamepads->gamepads_count = 0;
    for (int num = 0; num < GLFW_JOYSTICK_LAST; num++) {
        if (glfwJoystickPresent(num) == GLFW_FALSE) {
            break;
        }

        gamepads->gamepads_count++;
        if (glfwJoystickIsGamepad(num) == GLFW_FALSE) {
            continue;
        }

     	GLFWgamepadstate state;
        if (glfwGetGamepadState(num, &state) == GLFW_TRUE) {
            for (int i = 0; i < NK_GAMEPAD_BUTTON_MAX; i++) {
                if (state.buttons[button_mapping[i]]) {
                    nk_gamepad_button(gamepads, num, i, nk_true);
                }
            }
        }
    }
}

const char* nk_gamepad_glfw_name(struct nk_gamepads* gamepads, int num) {
    if (!gamepads || num < 0 || num >= gamepads->gamepads_count) {
        return NULL;
    }

    const char* name = glfwGetGamepadName(num);
    if (!name) {
        return "Controller";
    }
    else {
        return name;
    }
}

#endif
#endif
