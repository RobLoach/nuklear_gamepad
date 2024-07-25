#ifndef NUKLEAR_GAMEPAD_KEYBOARD_H__
#define NUKLEAR_GAMEPAD_KEYBOARD_H__

#ifndef NK_GAMEPAD_MAX
#define NK_GAMEPAD_MAX 1
#endif  // NK_GAMEPAD_MAX

/**
 * Keyboard mapping to gamepad buttons.
 *
 * @see nk_gamepad_keyboard_map_default
 * @see nk_gamepad_keyboard_input_source()
 */
struct nk_gamepad_keyboard_map {
    enum nk_keys keys[NK_GAMEPAD_BUTTON_LAST]; /** A mapping from an enum nk_gamepad_button to a enum nk_keys. */
    enum nk_gamepad_button chars[256]; /** Mapping between a char and a enum nk_gamepad_button. */
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Keyboard input source for the gamepad.
 *
 * Since Nuklear's text buffer is cleared every frame, this only captures button presses, not holds.
 *
 * @param user_data [nk_gamepad_keyboard_map] A custom keyboard map. If NULL, the default keyboard map is used.
 *
 * @return The input source for the keyboard.
 *
 * @see nk_gamepad_keyboard_map_default
 */
NK_API struct nk_gamepad_input_source nk_gamepad_keyboard_input_source(void* user_data);
NK_API nk_bool nk_gamepad_keyboard_init(struct nk_gamepads* gamepads, void* user_data);
NK_API void nk_gamepad_keyboard_update(struct nk_gamepads* gamepads, void* user_data);
NK_API const char* nk_gamepad_keyboard_name(struct nk_gamepads* gamepads, int num, void* user_data);

#ifdef __cplusplus
}
#endif

#endif

#if defined(NK_GAMEPAD_IMPLEMENTATION) && !defined(NK_GAMEPAD_HEADER_ONLY)
#ifndef NUKLEAR_GAMEPAD_KEYBOARD_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_KEYBOARD_IMPLEMENTATION_ONCE

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Default keyboard mapping for the gamepad.
 */
static struct nk_gamepad_keyboard_map nk_gamepad_keyboard_map_default;

NK_API void nk_gamepad_keyboard_update(struct nk_gamepads* gamepads, void* user_data) {
    if (!gamepads) {
        return;
    }

    // Grab the keyboard mapping.
    struct nk_gamepad_keyboard_map* map = (user_data == NULL) ? &nk_gamepad_keyboard_map_default : (struct nk_gamepad_keyboard_map*)user_data;

    // Keys
    for (int i = NK_GAMEPAD_BUTTON_FIRST; i < NK_GAMEPAD_BUTTON_LAST; i++) {
        enum nk_keys key = map->keys[i];
        if (key != NK_KEY_NONE) {
            nk_gamepad_button(gamepads, 0, (enum nk_gamepad_button)i, nk_input_is_key_down(&gamepads->ctx->input, key));
        }
    }

    // Text Buffer
    for (int i = 0; i < gamepads->ctx->input.keyboard.text_len; i++) {
        // Error correction.
        if (gamepads->ctx->input.keyboard.text[i] == '\0') {
            break;
        }

        int character = gamepads->ctx->input.keyboard.text[i];
        if (map->chars[character] != NK_GAMEPAD_BUTTON_INVALID) {
            nk_gamepad_button(gamepads, 0, map->chars[character], nk_true);
        }
    }
}

NK_API nk_bool nk_gamepad_keyboard_init(struct nk_gamepads* gamepads, void* user_data) {
    if (!gamepads) {
        return nk_false;
    }
    NK_UNUSED(user_data);

    // Only one keyboard is available.
    gamepads->gamepads[0].available = nk_true;
    for (int num = 1; num < NK_GAMEPAD_MAX; num++) {
        gamepads->gamepads[num].available = nk_false;
    }

    // Initialize the default keyboard mapping.
    nk_zero(&nk_gamepad_keyboard_map_default, sizeof(nk_gamepad_keyboard_map_default));

    // Keys
    nk_gamepad_keyboard_map_default.keys[NK_GAMEPAD_BUTTON_START] = NK_KEY_ENTER;
    nk_gamepad_keyboard_map_default.keys[NK_GAMEPAD_BUTTON_BACK] = NK_KEY_SHIFT;
    nk_gamepad_keyboard_map_default.keys[NK_GAMEPAD_BUTTON_UP] = NK_KEY_UP;
    nk_gamepad_keyboard_map_default.keys[NK_GAMEPAD_BUTTON_DOWN] = NK_KEY_DOWN;
    nk_gamepad_keyboard_map_default.keys[NK_GAMEPAD_BUTTON_LEFT] = NK_KEY_LEFT;
    nk_gamepad_keyboard_map_default.keys[NK_GAMEPAD_BUTTON_RIGHT] = NK_KEY_RIGHT;
    nk_gamepad_keyboard_map_default.keys[NK_GAMEPAD_BUTTON_B] = NK_KEY_BACKSPACE;

    // Text Buttons
    for (int i = 0; i < 256; i++) {
        nk_gamepad_keyboard_map_default.chars[i] = NK_GAMEPAD_BUTTON_INVALID;
    }
    nk_gamepad_keyboard_map_default.chars['Z'] = NK_GAMEPAD_BUTTON_A;
    nk_gamepad_keyboard_map_default.chars['z'] = NK_GAMEPAD_BUTTON_A;
    nk_gamepad_keyboard_map_default.chars[' '] = NK_GAMEPAD_BUTTON_A;
    nk_gamepad_keyboard_map_default.chars['X'] = NK_GAMEPAD_BUTTON_B;
    nk_gamepad_keyboard_map_default.chars['x'] = NK_GAMEPAD_BUTTON_B;
    nk_gamepad_keyboard_map_default.chars['A'] = NK_GAMEPAD_BUTTON_X;
    nk_gamepad_keyboard_map_default.chars['a'] = NK_GAMEPAD_BUTTON_X;
    nk_gamepad_keyboard_map_default.chars['S'] = NK_GAMEPAD_BUTTON_Y;
    nk_gamepad_keyboard_map_default.chars['s'] = NK_GAMEPAD_BUTTON_Y;
    nk_gamepad_keyboard_map_default.chars['Q'] = NK_GAMEPAD_BUTTON_LB;
    nk_gamepad_keyboard_map_default.chars['q'] = NK_GAMEPAD_BUTTON_LB;
    nk_gamepad_keyboard_map_default.chars['W'] = NK_GAMEPAD_BUTTON_RB;
    nk_gamepad_keyboard_map_default.chars['w'] = NK_GAMEPAD_BUTTON_RB;
    nk_gamepad_keyboard_map_default.chars['1'] = NK_GAMEPAD_BUTTON_START;
    nk_gamepad_keyboard_map_default.chars['!'] = NK_GAMEPAD_BUTTON_START;
    nk_gamepad_keyboard_map_default.chars['2'] = NK_GAMEPAD_BUTTON_BACK;
    nk_gamepad_keyboard_map_default.chars['@'] = NK_GAMEPAD_BUTTON_BACK;

    return nk_true;
}

NK_API struct nk_gamepad_input_source nk_gamepad_keyboard_input_source(void* user_data) {
    struct nk_gamepad_input_source source = {
        .user_data = user_data,
        .init = &nk_gamepad_keyboard_init,
        .update = &nk_gamepad_keyboard_update,
        .name = &nk_gamepad_keyboard_name,
        .input_source_name = "Keyboard",
    };
    return source;
}

NK_API const char* nk_gamepad_keyboard_name(struct nk_gamepads* gamepads, int num, void* user_data) {
    if (num == 0) {
        return "Keyboard";
    }
    NK_UNUSED(gamepads);
    NK_UNUSED(user_data);
    return NULL;
}

#ifdef __cplusplus
}
#endif

#endif
#endif
