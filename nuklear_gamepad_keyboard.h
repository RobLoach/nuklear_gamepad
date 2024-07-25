#ifndef NUKLEAR_GAMEPAD_KEYBOARD_H__
#define NUKLEAR_GAMEPAD_KEYBOARD_H__

#ifndef NK_GAMEPAD_MAX
#define NK_GAMEPAD_MAX 1 // Only one keyboard
#endif  // NK_GAMEPAD_MAX

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Keyboard input source for the gamepad.
 *
 * Since Nuklear's text buffer is cleared every frame, this only captures button presses, not holds.
 */
NK_API struct nk_gamepad_input_source nk_gamepad_keyboard_input_source(void* user_data);
NK_API nk_bool nk_gamepad_keyboard_init(struct nk_gamepads* gamepads, void* user_data);
NK_API void nk_gamepad_keyboard_update(struct nk_gamepads* gamepads, void* user_data);

#ifdef __cplusplus
}
#endif

#endif

#if defined(NK_GAMEPAD_IMPLEMENTATION) && !defined(NK_GAMEPAD_HEADER_ONLY)
#ifndef NUKLEAR_GAMEPAD_KEYBOARD_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_KEYBOARD_IMPLEMENTATION_ONCE

#ifndef NK_GAMEPAD_DEFAULT_INPUT_SOURCE
    #define NK_GAMEPAD_DEFAULT_INPUT_SOURCE nk_gamepad_keyboard_input_source
#endif

#ifdef __cplusplus
extern "C" {
#endif

static enum nk_keys nk_gamepad_keyboard_map(enum nk_gamepad_button button) {
    switch (button) {
        case NK_GAMEPAD_BUTTON_BACK: return NK_KEY_SHIFT;
        case NK_GAMEPAD_BUTTON_START: return NK_KEY_ENTER;
        case NK_GAMEPAD_BUTTON_UP: return NK_KEY_UP;
        case NK_GAMEPAD_BUTTON_DOWN: return NK_KEY_DOWN;
        case NK_GAMEPAD_BUTTON_LEFT: return NK_KEY_LEFT;
        case NK_GAMEPAD_BUTTON_RIGHT: return NK_KEY_RIGHT;
        default: return NK_KEY_NONE;
    }
}

/**
 * Maps the gamepad button to a character. This is used to check if the key is in the text buffer.
 *
 * The text buffer is cleared each frame, so this is only capable when a button is pressed.
 */
static char nk_gamepad_keyboard_char(enum nk_gamepad_button button) {
    // TODO: Since the text buffer is cleared each frame, perhaps move A/B to CTRL/SHIFT?
    switch (button) {
        case NK_GAMEPAD_BUTTON_A: return 'Z';
        case NK_GAMEPAD_BUTTON_B: return 'X';
        case NK_GAMEPAD_BUTTON_X: return 'A';
        case NK_GAMEPAD_BUTTON_Y: return 'S';
        case NK_GAMEPAD_BUTTON_LB: return 'Q';
        case NK_GAMEPAD_BUTTON_RB: return 'W';
        default: return 0;
    }
}

NK_API void nk_gamepad_keyboard_update(struct nk_gamepads* gamepads, void* user_data) {
    if (!gamepads) {
        return;
    }
    NK_UNUSED(user_data);

    for (int i = NK_GAMEPAD_BUTTON_FIRST; i < NK_GAMEPAD_BUTTON_LAST; i++) {
        enum nk_keys key = nk_gamepad_keyboard_map(i);
        if (key != NK_KEY_NONE) {
            nk_gamepad_button(gamepads, 0, (enum nk_gamepad_button)i, nk_input_is_key_down(&gamepads->ctx->input, key));
            continue;
        }

        // Check if the key appears in the keyboard text buffer.
        char char_key = nk_gamepad_keyboard_char(i);
        for (int j = 0; j < gamepads->ctx->input.keyboard.text_len; j++) {
            if (gamepads->ctx->input.keyboard.text[j] == char_key || gamepads->ctx->input.keyboard.text[j] == char_key + 32) {
                nk_gamepad_button(gamepads, 0, (enum nk_gamepad_button)i, nk_true);
            }
        }
    }
}

NK_API nk_bool nk_gamepad_keyboard_init(struct nk_gamepads* gamepads, void* user_data) {
    if (!gamepads) {
        return nk_false;
    }
    NK_UNUSED(user_data);

    // Only one keyboard available.
    for (int num = 0; num < NK_GAMEPAD_MAX; num++) {
        gamepads->gamepads[num].available = nk_false;
    }
    gamepads->gamepads[0].available = nk_true;

    return nk_true;
}

NK_API struct nk_gamepad_input_source nk_gamepad_keyboard_input_source(void* user_data) {
    struct nk_gamepad_input_source source = {
        .user_data = user_data,
        .init = &nk_gamepad_keyboard_init,
        .update = &nk_gamepad_keyboard_update,
    };
    return source;
}

#ifdef __cplusplus
}
#endif

#endif
#endif
