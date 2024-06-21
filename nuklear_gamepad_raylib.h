#ifndef NUKLEAR_GAMEPAD_RAYLIB_H__
#define NUKLEAR_GAMEPAD_RAYLIB_H__

NK_API void nk_gamepad_raylib_update(struct nk_gamepads* gamepads);
NK_API const char* nk_gamepad_raylib_name(struct nk_gamepads* gamepads, int num);

#endif

#ifdef NK_GAMEPAD_IMPLEMENTATION
#ifndef NUKLEAR_GAMEPAD_RAYLIB_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_RAYLIB_IMPLEMENTATION_ONCE

#ifndef NK_GAMEPAD_MALLOC
    #define NK_GAMEPAD_MALLOC(unused, old, size) MemAlloc((unsigned int)(size))
#endif

#ifndef NK_GAMEPAD_MFREE
    #define NK_GAMEPAD_MFREE(unused, ptr) MemFree(ptr)
#endif

#ifndef NK_GAMEPAD_RAYLIB_GAMEPAD_MAX
#define NK_GAMEPAD_RAYLIB_GAMEPAD_MAX 4
#endif

#define NK_GAMEPAD_UPDATE nk_gamepad_raylib_update
#define NK_GAMEPAD_NAME nk_gamepad_raylib_name

int nk_gamepad_raylib_map_button(int button) {
    switch (button) {
        case NK_GAMEPAD_BUTTON_UP: return GAMEPAD_BUTTON_LEFT_FACE_UP;
        case NK_GAMEPAD_BUTTON_DOWN: return GAMEPAD_BUTTON_LEFT_FACE_DOWN;
        case NK_GAMEPAD_BUTTON_LEFT: return GAMEPAD_BUTTON_LEFT_FACE_LEFT;
        case NK_GAMEPAD_BUTTON_RIGHT: return GAMEPAD_BUTTON_LEFT_FACE_RIGHT;
        case NK_GAMEPAD_BUTTON_A: return GAMEPAD_BUTTON_RIGHT_FACE_DOWN;
        case NK_GAMEPAD_BUTTON_B: return GAMEPAD_BUTTON_RIGHT_FACE_RIGHT;
        case NK_GAMEPAD_BUTTON_X: return GAMEPAD_BUTTON_RIGHT_FACE_LEFT;
        case NK_GAMEPAD_BUTTON_Y: return GAMEPAD_BUTTON_RIGHT_FACE_UP;
        case NK_GAMEPAD_BUTTON_LB: return GAMEPAD_BUTTON_LEFT_TRIGGER_1;
        case NK_GAMEPAD_BUTTON_RB: return GAMEPAD_BUTTON_LEFT_TRIGGER_2;
        case NK_GAMEPAD_BUTTON_BACK: return GAMEPAD_BUTTON_MIDDLE_LEFT;
        case NK_GAMEPAD_BUTTON_START: return GAMEPAD_BUTTON_MIDDLE_RIGHT;
        default: return -1;
    }
}

void nk_gamepad_raylib_update(struct nk_gamepads* gamepads) {
    if (!gamepads) {
        return;
    }

    if (gamepads->gamepads == NULL) {
        if (nk_gamepad_init_gamepads(gamepads, NK_GAMEPAD_RAYLIB_GAMEPAD_MAX) == nk_false) {
            return;
        }
    }

    gamepads->gamepads_count = 0;
    for (int num = 0; num < NK_GAMEPAD_RAYLIB_GAMEPAD_MAX; num++) {
        if (!IsGamepadAvailable(num)) {
            break;
        }

        gamepads->gamepads_count++;

        for (int i = NK_GAMEPAD_BUTTON_FIRST; i < NK_GAMEPAD_BUTTON_LAST; i++) {
            if (IsGamepadButtonDown(num, nk_gamepad_raylib_map_button(i))) {
                nk_gamepad_button(gamepads, num, i, nk_true);
            }
        }
    }
}

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
