#ifndef NUKLEAR_GAMEPAD_RAYLIB_H__
#define NUKLEAR_GAMEPAD_RAYLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

NK_API void nk_gamepad_raylib_update(struct nk_gamepads* gamepads, void* user_data);
NK_API const char* nk_gamepad_raylib_name(struct nk_gamepads* gamepads, int num, void* user_data);
NK_API struct nk_gamepad_input_source nk_gamepad_raylib_input_soure(void* user_data);

#ifdef __cplusplus
}
#endif

#endif

#if defined(NK_GAMEPAD_IMPLEMENTATION) && !defined(NK_GAMEPAD_HEADER_ONLY)
#ifndef NUKLEAR_GAMEPAD_RAYLIB_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_RAYLIB_IMPLEMENTATION_ONCE

#ifndef NK_GAMEPAD_DEFAULT_INPUT_SOURCE
#define NK_GAMEPAD_DEFAULT_INPUT_SOURCE nk_gamepad_raylib_input_soure
#endif

#ifdef __cplusplus
extern "C" {
#endif

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
        case NK_GAMEPAD_BUTTON_RB: return GAMEPAD_BUTTON_RIGHT_TRIGGER_1;
        case NK_GAMEPAD_BUTTON_BACK: return GAMEPAD_BUTTON_MIDDLE_LEFT;
        case NK_GAMEPAD_BUTTON_START: return GAMEPAD_BUTTON_MIDDLE_RIGHT;
        case NK_GAMEPAD_BUTTON_GUIDE: return GAMEPAD_BUTTON_MIDDLE;
        default: return -1;
    }
}

void nk_gamepad_raylib_update(struct nk_gamepads* gamepads, void* user_data) {
    NK_UNUSED(user_data);
    if (!gamepads) {
        return;
    }

    for (int num = 0; num < NK_GAMEPAD_MAX; num++) {
        if (!IsGamepadAvailable(num)) {
            gamepads->gamepads[num].available = nk_false;
            continue;
        }

        gamepads->gamepads[num].available = nk_true;
        for (int i = NK_GAMEPAD_BUTTON_FIRST; i < NK_GAMEPAD_BUTTON_LAST; i++) {
            if (IsGamepadButtonDown(num, nk_gamepad_raylib_map_button(i))) {
                nk_gamepad_button(gamepads, num, (enum nk_gamepad_button)i, nk_true);
            }
        }
    }
}

const char* nk_gamepad_raylib_name(struct nk_gamepads* gamepads, int num, void* user_data) {
    NK_UNUSED(user_data);
    const char* name = GetGamepadName(num);
    if (name == NULL || TextLength(name) == 0) {
        return gamepads->gamepads[num].name;
    }

    return name;
}

NK_API struct nk_gamepad_input_source nk_gamepad_raylib_input_soure(void* user_data) {
    struct nk_gamepad_input_source source = {
        user_data,
        NULL,
        &nk_gamepad_raylib_update,
        NULL,
        &nk_gamepad_raylib_name,
    };
    return source;
}

#ifdef __cplusplus
}
#endif

#endif
#endif
