#ifndef NUKLEAR_GAMEPAD_XINPUT_H__
#define NUKLEAR_GAMEPAD_XINPUT_H__

#if !defined(NK_GAMEPAD_MAX) && defined(XUSER_MAX_COUNT)
#define NK_GAMEPAD_MAX XUSER_MAX_COUNT
#elif !defined(NK_GAMEPAD_MAX)
#define NK_GAMEPAD_MAX 4
#endif

#ifdef __cplusplus
extern "C" {
#endif

NK_API nk_bool nk_gamepad_xinput_init(struct nk_gamepads* gamepads, void* user_data);
NK_API void nk_gamepad_xinput_update(struct nk_gamepads* gamepads, void* user_data);
NK_API const char* nk_gamepad_xinput_name(struct nk_gamepads* gamepads, int num, void* user_data);
NK_API struct nk_gamepad_input_source nk_gamepad_xinput_input_source(void* user_data);

#ifdef __cplusplus
}
#endif

#endif

#if defined(NK_GAMEPAD_IMPLEMENTATION) && !defined(NK_GAMEPAD_HEADER_ONLY)
#ifndef NUKLEAR_GAMEPAD_XINPUT_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_XINPUT_IMPLEMENTATION_ONCE

#ifdef __cplusplus
extern "C" {
#endif

NK_API nk_bool nk_gamepad_xinput_init(struct nk_gamepads* gamepads, void* user_data) {
    int i;
    NK_UNUSED(user_data);
    if (gamepads == NULL) {
        return nk_false;
    }
    for (i = 0; i < NK_GAMEPAD_MAX; i++) {
        XINPUT_STATE state;
        gamepads->gamepads[i].available = (XInputGetState(i, &state) == ERROR_SUCCESS) ? nk_true : nk_false;
    }
    return nk_true;
}

NK_API void nk_gamepad_xinput_update(struct nk_gamepads* gamepads, void* user_data) {
    int num;
    NK_UNUSED(user_data);
    if (gamepads == NULL) {
        return;
    }

    for (num = 0; num < NK_GAMEPAD_MAX; num++) {
        XINPUT_STATE state;
        WORD buttons;
        float lx, ly, rx, ry;

        if (XInputGetState(num, &state) != ERROR_SUCCESS) {
            gamepads->gamepads[num].available = nk_false;
            continue;
        }
        gamepads->gamepads[num].available = nk_true;

        buttons = state.Gamepad.wButtons;
        nk_gamepad_button(gamepads, num, NK_GAMEPAD_BUTTON_UP,    (buttons & XINPUT_GAMEPAD_DPAD_UP)         != 0);
        nk_gamepad_button(gamepads, num, NK_GAMEPAD_BUTTON_DOWN,  (buttons & XINPUT_GAMEPAD_DPAD_DOWN)       != 0);
        nk_gamepad_button(gamepads, num, NK_GAMEPAD_BUTTON_LEFT,  (buttons & XINPUT_GAMEPAD_DPAD_LEFT)       != 0);
        nk_gamepad_button(gamepads, num, NK_GAMEPAD_BUTTON_RIGHT, (buttons & XINPUT_GAMEPAD_DPAD_RIGHT)      != 0);
        nk_gamepad_button(gamepads, num, NK_GAMEPAD_BUTTON_A,     (buttons & XINPUT_GAMEPAD_A)               != 0);
        nk_gamepad_button(gamepads, num, NK_GAMEPAD_BUTTON_B,     (buttons & XINPUT_GAMEPAD_B)               != 0);
        nk_gamepad_button(gamepads, num, NK_GAMEPAD_BUTTON_X,     (buttons & XINPUT_GAMEPAD_X)               != 0);
        nk_gamepad_button(gamepads, num, NK_GAMEPAD_BUTTON_Y,     (buttons & XINPUT_GAMEPAD_Y)               != 0);
        nk_gamepad_button(gamepads, num, NK_GAMEPAD_BUTTON_LB,    (buttons & XINPUT_GAMEPAD_LEFT_SHOULDER)   != 0);
        nk_gamepad_button(gamepads, num, NK_GAMEPAD_BUTTON_RB,    (buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER)  != 0);
        nk_gamepad_button(gamepads, num, NK_GAMEPAD_BUTTON_BACK,  (buttons & XINPUT_GAMEPAD_BACK)            != 0);
        nk_gamepad_button(gamepads, num, NK_GAMEPAD_BUTTON_START, (buttons & XINPUT_GAMEPAD_START)           != 0);

        /* Normalize thumb sticks from -32768..32767 to -1..1 */
        lx = state.Gamepad.sThumbLX / 32767.0f;
        ly = state.Gamepad.sThumbLY / 32767.0f;
        rx = state.Gamepad.sThumbRX / 32767.0f;
        ry = state.Gamepad.sThumbRY / 32767.0f;
        if (lx >  1.0f) lx =  1.0f; if (lx < -1.0f) lx = -1.0f;
        if (ly >  1.0f) ly =  1.0f; if (ly < -1.0f) ly = -1.0f;
        if (rx >  1.0f) rx =  1.0f; if (rx < -1.0f) rx = -1.0f;
        if (ry >  1.0f) ry =  1.0f; if (ry < -1.0f) ry = -1.0f;
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_LEFT_X,  lx);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_LEFT_Y,  -ly);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_RIGHT_X, rx);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_RIGHT_Y, -ry);

        /* Normalize triggers from 0..255 to 0..1 */
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_LEFT_TRIGGER,  state.Gamepad.bLeftTrigger  / 255.0f);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_RIGHT_TRIGGER, state.Gamepad.bRightTrigger / 255.0f);
    }
}

NK_API const char* nk_gamepad_xinput_name(struct nk_gamepads* gamepads, int num, void* user_data) {
    NK_UNUSED(user_data);
    if (num < 0 || num >= NK_GAMEPAD_MAX || !gamepads->gamepads[num].available) {
        return NULL;
    }
    return gamepads->gamepads[num].name;
}

NK_API struct nk_gamepad_input_source nk_gamepad_xinput_input_source(void* user_data) {
    struct nk_gamepad_input_source source;
    source.user_data = user_data;
    source.init = &nk_gamepad_xinput_init;
    source.update = &nk_gamepad_xinput_update;
    source.free = NULL;
    source.name = &nk_gamepad_xinput_name;
    source.button_name = NULL;
    source.input_source_name = "XInput";
    source.id = NK_GAMEPAD_INPUT_SOURCE_XINPUT;
    return source;
}

#ifdef __cplusplus
}
#endif

#endif
#endif
