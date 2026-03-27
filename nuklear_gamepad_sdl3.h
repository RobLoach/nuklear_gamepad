/**
 * @file nuklear_gamepad_sdl3.h
 * @brief SDL3 input source backend for nuklear_gamepad.
 *
 * @see nk_gamepad_sdl3_input_source()
 */
#ifndef NUKLEAR_GAMEPAD_SDL3_H__
#define NUKLEAR_GAMEPAD_SDL3_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Process an SDL3 event for gamepad hot-plug support.
 *
 * Call this from your SDL event loop to handle SDL_EVENT_GAMEPAD_ADDED and
 * SDL_EVENT_GAMEPAD_REMOVED events, keeping the gamepads state in sync with
 * connected hardware.
 *
 * @param gamepads The gamepads context to update.
 * @param event The SDL_Event to process.
 */
NK_API void nk_gamepad_sdl3_handle_event(struct nk_gamepads* gamepads, SDL_Event *event);

/**
 * Initialize the SDL3 gamepad backend.
 *
 * @param gamepads The gamepads context to initialize.
 * @param user_data Unused; pass NULL.
 * @return nk_true on success, nk_false on failure.
 */
NK_API nk_bool nk_gamepad_sdl3_init(struct nk_gamepads* gamepads, void* user_data);

/**
 * Update button and axis state for all connected SDL3 gamepads.
 *
 * @param gamepads The gamepads context to update.
 * @param user_data Unused; pass NULL.
 */
NK_API void nk_gamepad_sdl3_update(struct nk_gamepads* gamepads, void* user_data);

/**
 * Close all open SDL3 gamepad handles.
 *
 * Called automatically by nk_gamepad_free() when using the SDL3 input source.
 *
 * @param gamepads  The gamepads context to free.
 * @param user_data Unused; pass NULL.
 */
NK_API void nk_gamepad_sdl3_free(struct nk_gamepads* gamepads, void* user_data);

/**
 * Retrieve the name of the given SDL3 gamepad.
 *
 * @param gamepads The gamepads context.
 * @param num Zero-indexed gamepad slot.
 * @param user_data Unused; pass NULL.
 * @return A pointer to the gamepad name string, or NULL if the slot is empty.
 */
NK_API const char* nk_gamepad_sdl3_name(struct nk_gamepads* gamepads, int num, void* user_data);

/**
 * Build and return the SDL3 input source descriptor.
 *
 * @param user_data Optional user data pointer forwarded to each callback.
 * @return A fully populated nk_gamepad_input_source for SDL3.
 */
NK_API struct nk_gamepad_input_source nk_gamepad_sdl3_input_source(void* user_data);

#ifdef __cplusplus
}
#endif

#endif

#if defined(NK_GAMEPAD_IMPLEMENTATION) && !defined(NK_GAMEPAD_HEADER_ONLY)
#ifndef NUKLEAR_GAMEPAD_SDL3_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_SDL3_IMPLEMENTATION_ONCE

#ifdef __cplusplus
extern "C" {
#endif

NK_API void nk_gamepad_sdl3_handle_event(struct nk_gamepads* gamepads, SDL_Event *event) {
    switch (event->type) {
        case SDL_EVENT_GAMEPAD_ADDED: {
            SDL_JoystickID which = event->gdevice.which;
            if (SDL_IsGamepad(which)) {
                int first_free = -1;
                for (int i = 0; i < NK_GAMEPAD_MAX; i++) {
                    if (gamepads->gamepads[i].data != NULL &&
                        SDL_GetGamepadID((SDL_Gamepad*)gamepads->gamepads[i].data) == which) {
                        first_free = -1;
                        break;
                    }
                    if (first_free < 0 && gamepads->gamepads[i].data == NULL) {
                        first_free = i;
                    }
                }
                if (first_free >= 0) {
                    SDL_Gamepad* gamepad = SDL_OpenGamepad(which);
                    if (gamepad) {
                        gamepads->gamepads[first_free].data = gamepad;
                        gamepads->gamepads[first_free].available = nk_true;
                    }
                }
            }
            break;
        }
        case SDL_EVENT_GAMEPAD_REMOVED: {
            for (int i = 0; i < NK_GAMEPAD_MAX; i++) {
                if (gamepads->gamepads[i].data != NULL &&
                    SDL_GetGamepadID((SDL_Gamepad*)gamepads->gamepads[i].data) == event->gdevice.which) {
                    SDL_CloseGamepad((SDL_Gamepad*)gamepads->gamepads[i].data);
                    gamepads->gamepads[i].data = NULL;
                    gamepads->gamepads[i].available = nk_false;
                    break;
                }
            }
            break;
        }
    }
}

NK_API nk_bool nk_gamepad_sdl3_init(struct nk_gamepads* gamepads, void* user_data) {
    NK_UNUSED(user_data);
    if (gamepads == NULL) {
        return nk_false;
    }

    int count = 0;
    SDL_JoystickID* joysticks = SDL_GetGamepads(&count);
    if (joysticks) {
        int i;
        for (i = 0; i < count && i < NK_GAMEPAD_MAX; i++) {
            SDL_Gamepad* gamepad = SDL_OpenGamepad(joysticks[i]);
            if (gamepad != NULL) {
                gamepads->gamepads[i].data = gamepad;
                gamepads->gamepads[i].available = nk_true;
            }
        }
        SDL_free(joysticks);
    }

    return nk_true;
}

NK_API void nk_gamepad_sdl3_free(struct nk_gamepads* gamepads, void* user_data) {
    NK_UNUSED(user_data);
    if (!gamepads) {
        return;
    }

    for (int i = 0; i < NK_GAMEPAD_MAX; i++) {
        if (gamepads->gamepads[i].data != NULL) {
            SDL_CloseGamepad((SDL_Gamepad*)gamepads->gamepads[i].data);
            gamepads->gamepads[i].data = NULL;
            gamepads->gamepads[i].available = nk_false;
        }
    }
}

/**
 * Map an nk_gamepad_button value to the corresponding SDL_GamepadButton.
 *
 * @param button An #nk_gamepad_button enum value.
 * @return The matching SDL_GamepadButton, or SDL_GAMEPAD_BUTTON_INVALID if
 *         @p button has no SDL3 equivalent.
 * @internal
 */
SDL_GamepadButton nk_gamepad_sdl3_map_button(int button) {
    switch (button) {
        case NK_GAMEPAD_BUTTON_UP: return SDL_GAMEPAD_BUTTON_DPAD_UP;
        case NK_GAMEPAD_BUTTON_DOWN: return SDL_GAMEPAD_BUTTON_DPAD_DOWN;
        case NK_GAMEPAD_BUTTON_LEFT: return SDL_GAMEPAD_BUTTON_DPAD_LEFT;
        case NK_GAMEPAD_BUTTON_RIGHT: return SDL_GAMEPAD_BUTTON_DPAD_RIGHT;
        case NK_GAMEPAD_BUTTON_A: return SDL_GAMEPAD_BUTTON_SOUTH;
        case NK_GAMEPAD_BUTTON_B: return SDL_GAMEPAD_BUTTON_EAST;
        case NK_GAMEPAD_BUTTON_X: return SDL_GAMEPAD_BUTTON_WEST;
        case NK_GAMEPAD_BUTTON_Y: return SDL_GAMEPAD_BUTTON_NORTH;
        case NK_GAMEPAD_BUTTON_LB: return SDL_GAMEPAD_BUTTON_LEFT_SHOULDER;
        case NK_GAMEPAD_BUTTON_RB: return SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER;
        case NK_GAMEPAD_BUTTON_BACK: return SDL_GAMEPAD_BUTTON_BACK;
        case NK_GAMEPAD_BUTTON_START: return SDL_GAMEPAD_BUTTON_START;
        case NK_GAMEPAD_BUTTON_GUIDE: return SDL_GAMEPAD_BUTTON_GUIDE;
        default: return SDL_GAMEPAD_BUTTON_INVALID;
    }
}

NK_API void nk_gamepad_sdl3_update(struct nk_gamepads* gamepads, void* user_data) {
    NK_UNUSED(user_data);
    for (int num = 0; num < NK_GAMEPAD_MAX; num++) {
        if (gamepads->gamepads[num].data == NULL) {
            continue;
        }

        SDL_Gamepad* gamepad = (SDL_Gamepad*)gamepads->gamepads[num].data;

        // Check to make sure it's still attached.
        if (SDL_GetGamepadID(gamepad) == 0) {
            gamepads->gamepads[num].available = nk_false;
            SDL_CloseGamepad(gamepad);
            gamepads->gamepads[num].data = NULL;
            continue;
        }

        for (int i = NK_GAMEPAD_BUTTON_FIRST; i < NK_GAMEPAD_BUTTON_LAST; i++) {
            if (SDL_GetGamepadButton(gamepad, nk_gamepad_sdl3_map_button(i))) {
                nk_gamepad_button(gamepads, num, (enum nk_gamepad_button)i, nk_true);
            }
        }

        // Axes: SDL3 returns Sint16 in range -32768..32767 (sticks) or 0..32767 (triggers)
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_LEFT_X, SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX) / 32767.0f);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_LEFT_Y, SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY) / 32767.0f);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_RIGHT_X, SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTX) / 32767.0f);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_RIGHT_Y, SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTY) / 32767.0f);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_LEFT_TRIGGER, SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) / 32767.0f);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_RIGHT_TRIGGER, SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) / 32767.0f);
    }
}

NK_API const char* nk_gamepad_sdl3_name(struct nk_gamepads* gamepads, int num, void* user_data) {
    NK_UNUSED(user_data);
    if (gamepads->gamepads[num].data == NULL) {
        return NULL;
    }

    const char* name = SDL_GetGamepadName((SDL_Gamepad*)gamepads->gamepads[num].data);
    if (name == NULL || name[0] == '\0') {
        return gamepads->gamepads[num].name;
    }
    return name;
}

NK_API struct nk_gamepad_input_source nk_gamepad_sdl3_input_source(void* user_data) {
    struct nk_gamepad_input_source source = {
        .user_data = user_data,
        .init = &nk_gamepad_sdl3_init,
        .update = &nk_gamepad_sdl3_update,
        .free = &nk_gamepad_sdl3_free,
        .name = &nk_gamepad_sdl3_name,
        .input_source_name = "SDL3",
        .id = NK_GAMEPAD_INPUT_SOURCE_SDL3,
    };
    return source;
}

#ifdef __cplusplus
}
#endif

#endif
#endif
