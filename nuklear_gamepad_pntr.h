#ifndef NUKLEAR_GAMEPAD_PNTR_H__
#define NUKLEAR_GAMEPAD_PNTR_H__

#ifndef NK_GAMEPAD_MAX
#define NK_GAMEPAD_MAX PNTR_APP_MAX_GAMEPADS
#endif  // NK_GAMEPAD_MAX


#ifdef __cplusplus
extern "C" {
#endif

NK_API void nk_gamepad_pntr_update(struct nk_gamepads* gamepads, void* user_data);
NK_API struct nk_gamepad_input_source nk_gamepad_pntr_input_soure(void* user_data);

#ifdef __cplusplus
}
#endif

#endif

#if defined(NK_GAMEPAD_IMPLEMENTATION) && !defined(NK_GAMEPAD_HEADER_ONLY)
#ifndef NUKLEAR_GAMEPAD_PNTR_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_PNTR_IMPLEMENTATION_ONCE

#ifndef NK_GAMEPAD_DEFAULT_INPUT_SOURCE
#define NK_GAMEPAD_DEFAULT_INPUT_SOURCE nk_gamepad_pntr_input_soure
#endif

#ifdef __cplusplus
extern "C" {
#endif

int nk_gamepad_pntr_map_button(int button) {
    switch (button) {
        case NK_GAMEPAD_BUTTON_UP: return PNTR_APP_GAMEPAD_BUTTON_UP;
        case NK_GAMEPAD_BUTTON_DOWN: return PNTR_APP_GAMEPAD_BUTTON_DOWN;
        case NK_GAMEPAD_BUTTON_LEFT: return PNTR_APP_GAMEPAD_BUTTON_LEFT;
        case NK_GAMEPAD_BUTTON_RIGHT: return PNTR_APP_GAMEPAD_BUTTON_RIGHT;
        case NK_GAMEPAD_BUTTON_A: return PNTR_APP_GAMEPAD_BUTTON_A;
        case NK_GAMEPAD_BUTTON_B: return PNTR_APP_GAMEPAD_BUTTON_B;
        case NK_GAMEPAD_BUTTON_X: return PNTR_APP_GAMEPAD_BUTTON_X;
        case NK_GAMEPAD_BUTTON_Y: return PNTR_APP_GAMEPAD_BUTTON_Y;
        case NK_GAMEPAD_BUTTON_LB: return PNTR_APP_GAMEPAD_BUTTON_LEFT_SHOULDER;
        case NK_GAMEPAD_BUTTON_RB: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_SHOULDER;
        case NK_GAMEPAD_BUTTON_BACK: return PNTR_APP_GAMEPAD_BUTTON_SELECT;
        case NK_GAMEPAD_BUTTON_START: return PNTR_APP_GAMEPAD_BUTTON_START;
        case NK_GAMEPAD_BUTTON_GUIDE: return PNTR_APP_GAMEPAD_BUTTON_MENU;
        default: return PNTR_APP_GAMEPAD_BUTTON_UNKNOWN;
    }
}

void nk_gamepad_pntr_update(struct nk_gamepads* gamepads, void* user_data) {
    if (!gamepads || !gamepads->input_source.user_data) {
        return;
    }

    for (int num = 0; num < PNTR_APP_MAX_GAMEPADS; num++) {
        gamepads->gamepads[num].available = nk_true;
        for (int i = NK_GAMEPAD_BUTTON_FIRST; i < NK_GAMEPAD_BUTTON_LAST; i++) {
            if (pntr_app_gamepad_button_down(gamepads->input_source.user_data, num, nk_gamepad_pntr_map_button(i))) {
                nk_gamepad_button(gamepads, num, (enum nk_gamepad_button)i, nk_true);
            }
        }
    }
}

NK_API struct nk_gamepad_input_source nk_gamepad_pntr_input_soure(void* user_data) {
    struct nk_gamepad_input_source source = {
        user_data,
        NULL,
        &nk_gamepad_pntr_update,
        NULL,
        NULL,
    };
    return source;
}

#ifdef __cplusplus
}
#endif

#endif
#endif
