#ifndef NUKLEAR_GAMEPAD_PNTR_H__
#define NUKLEAR_GAMEPAD_PNTR_H__

NK_API void nk_gamepad_pntr_update(struct nk_gamepads* gamepads);

#endif

#ifdef NK_GAMEPAD_IMPLEMENTATION
#ifndef NUKLEAR_GAMEPAD_PNTR_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_PNTR_IMPLEMENTATION_ONCE

#ifndef NK_GAMEPAD_MFREE
    #define NK_GAMEPAD_MFREE(unused, ptr) pntr_unload_memory(ptr)
#endif

#ifndef NK_GAMEPAD_MALLOC
    #define NK_GAMEPAD_MALLOC(unused, old, size) pntr_load_memory((size_t)(size))
#endif

#define NK_GAMEPAD_UPDATE nk_gamepad_pntr_update

int nk_gamepad_pntr_to_nk_gamepad(int button) {
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
        default: return PNTR_APP_GAMEPAD_BUTTON_UNKNOWN;
    }
}

void nk_gamepad_pntr_update(struct nk_gamepads* gamepads) {
    if (!gamepads || !gamepads->user_data) {
        return;
    }

    if (gamepads->gamepads == NULL) {
        nk_gamepad_init_gamepads(gamepads, PNTR_APP_MAX_GAMEPADS);
    }

    for (int num = 0; num < PNTR_APP_MAX_GAMEPADS; num++) {
        for (int i = 0; i < NK_GAMEPAD_BUTTON_MAX; i++) {
            if (pntr_app_gamepad_button_down(gamepads->user_data, num, nk_gamepad_pntr_to_nk_gamepad(i))) {
                nk_gamepad_button(gamepads, num, i, nk_true);
            }
        }
    }
}

#endif
#endif
