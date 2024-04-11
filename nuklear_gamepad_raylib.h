#ifndef NUKLEAR_GAMEPAD_RAYLIB_H__
#define NUKLEAR_GAMEPAD_RAYLIB_H__

NK_API void nk_gamepad_raylib_update(struct nk_gamepads* gamepads);
NK_API const char* nk_gamepad_raylib_name(struct nk_gamepads* gamepads, int num);

#endif

#ifdef NK_IMPLEMENTATION
#ifndef NUKLEAR_GAMEPAD_RAYLIB_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_RAYLIB_IMPLEMENTATION_ONCE

#ifndef NK_GAMEPAD_RAYLIB_GAMEPAD_MAX
#define NK_GAMEPAD_RAYLIB_GAMEPAD_MAX 4
#endif

#define NK_GAMEPAD_UPDATE nk_gamepad_raylib_update
#define NK_GAMEPAD_NAME nk_gamepad_raylib_name

void nk_gamepad_raylib_update(struct nk_gamepads* gamepads) {
    if (!gamepads) {
        return;
    }

    if (gamepads->gamepads == NULL) {
        nk_gamepad_init_gamepads(gamepads, NK_GAMEPAD_RAYLIB_GAMEPAD_MAX);
    }

    int button_mapping[NK_GAMEPAD_BUTTON_MAX] = {
        GAMEPAD_BUTTON_LEFT_FACE_UP, /* NK_GAMEPAD_BUTTON_UP */
        GAMEPAD_BUTTON_LEFT_FACE_DOWN, /* NK_GAMEPAD_BUTTON_DOWN */
        GAMEPAD_BUTTON_LEFT_FACE_LEFT, /* NK_GAMEPAD_BUTTON_LEFT */
        GAMEPAD_BUTTON_LEFT_FACE_RIGHT, /* NK_GAMEPAD_BUTTON_RIGHT */
        GAMEPAD_BUTTON_RIGHT_FACE_DOWN, /* NK_GAMEPAD_BUTTON_A */
        GAMEPAD_BUTTON_RIGHT_FACE_RIGHT, /* NK_GAMEPAD_BUTTON_B */
        GAMEPAD_BUTTON_RIGHT_FACE_LEFT, /* NK_GAMEPAD_BUTTON_X */
        GAMEPAD_BUTTON_RIGHT_FACE_UP, /* NK_GAMEPAD_BUTTON_Y */
        GAMEPAD_BUTTON_LEFT_TRIGGER_1, /* NK_GAMEPAD_BUTTON_LB */
        GAMEPAD_BUTTON_LEFT_TRIGGER_2, /* NK_GAMEPAD_BUTTON_RB */
        GAMEPAD_BUTTON_MIDDLE_LEFT, /* NK_GAMEPAD_BUTTON_BACK */
        GAMEPAD_BUTTON_MIDDLE_RIGHT /* NK_GAMEPAD_BUTTON_START */
    };

    gamepads->gamepads_count = 0;
    for (int num = 0; num < NK_GAMEPAD_RAYLIB_GAMEPAD_MAX; num++) {
        if (!IsGamepadAvailable(num)) {
            break;
        }

        gamepads->gamepads_count++;

        for (int i = 0; i < NK_GAMEPAD_BUTTON_MAX; i++) {
            if (IsGamepadButtonDown(num, button_mapping[i])) {
                nk_gamepad_button(gamepads, num, i, nk_true);
            }
        }
    }
}

#include <stdio.h>
const char* nk_gamepad_raylib_name(struct nk_gamepads* gamepads, int num) {
    if (!gamepads || num < 0 || num >= gamepads->gamepads_count) {
        return NULL;
    }

    const char* name = GetGamepadName(num);
    if (!name || TextLength(name) == 0) {
        return gamepads->gamepads[num].name;
    }

    return name;
}

#endif
#endif
