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
 * This is a no-op unless the SDL3 input source is the active input source of
 * the given gamepads context, so it remains safe to call unconditionally from
 * the event loop after nk_gamepad_set_input_source() switched to another one.
 *
 * @param gamepads The gamepads context to update.
 * @param event The SDL_Event to process.
 */
NK_API void nk_gamepad_sdl3_handle_event(struct nk_gamepads* gamepads, SDL_Event *event);

/**
 * Initialize the SDL3 gamepad backend.
 *
 * Initializes SDL's gamepad subsystem with SDL_InitSubSystem(), so the
 * application does not have to do so itself. The reference count is released
 * again by nk_gamepad_sdl3_free().
 *
 * @param gamepads The gamepads context to initialize.
 * @param user_data User data from the input source; unused by this backend.
 * @return nk_true on success, nk_false on failure.
 */
NK_API nk_bool nk_gamepad_sdl3_init(struct nk_gamepads* gamepads, void* user_data);

/**
 * Update button and axis state for all connected SDL3 gamepads.
 *
 * @param gamepads The gamepads context to update.
 * @param user_data User data from the input source; unused by this backend.
 */
NK_API void nk_gamepad_sdl3_update(struct nk_gamepads* gamepads, void* user_data);

/**
 * Close all open SDL3 gamepad handles.
 *
 * Called automatically by nk_gamepad_free() when using the SDL3 input source.
 * Also releases the SDL gamepad subsystem reference taken by
 * nk_gamepad_sdl3_init().
 *
 * @param gamepads  The gamepads context to free.
 * @param user_data User data from the input source; unused by this backend.
 */
NK_API void nk_gamepad_sdl3_free(struct nk_gamepads* gamepads, void* user_data);

/**
 * Retrieve the name of the given SDL3 gamepad.
 *
 * The returned string is a copy owned by the gamepads context, truncated to
 * NK_GAMEPAD_NAME_SIZE, so it stays valid after the gamepad is closed.
 *
 * @param gamepads The gamepads context.
 * @param num Zero-indexed gamepad slot.
 * @param user_data User data from the input source; unused by this backend.
 * @return A pointer to the gamepad name string, or NULL if the slot is empty.
 */
NK_API const char* nk_gamepad_sdl3_name(struct nk_gamepads* gamepads, int num, void* user_data);

/**
 * Get the human-readable name of a button for the connected SDL3 gamepad.
 *
 * Uses SDL_GetGamepadButtonLabel to return controller-specific face button
 * labels (e.g. "Cross", "Circle", "Square", "Triangle" for PlayStation
 * controllers), falling back to generic names for all other buttons.
 *
 * The nk_gamepad_button_name_fn signature carries no gamepad index, so the
 * labels are taken from the first connected gamepad. In a mixed-controller
 * setup the labels follow whichever pad occupies the lowest slot.
 *
 * @param gamepads  The gamepads context.
 * @param button    The button to name.
 * @param user_data User data from the input source; unused by this backend.
 * @return A pointer to the button name string, or NULL if invalid.
 */
NK_API const char* nk_gamepad_sdl3_button_name(struct nk_gamepads* gamepads, enum nk_gamepad_button button, void* user_data);

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

/**
 * Clear the cached button and axis state of a gamepad slot.
 *
 * Without this, unplugging a controller while a button is held leaves that
 * bit set in `buttons`, so re-connecting into the same slot reports a
 * button release the user never performed.
 *
 * @param gamepads The gamepads context.
 * @param num The zero-indexed gamepad slot to reset.
 * @internal
 */
static void nk_gamepad_sdl3_reset_state(struct nk_gamepads* gamepads, int num) {
    gamepads->gamepads[num].buttons = 0;
    gamepads->gamepads[num].buttons_prev = 0;
    nk_zero(gamepads->gamepads[num].axes, sizeof(gamepads->gamepads[num].axes));
}

/**
 * Copy the SDL3 gamepad name into the slot's durable name buffer.
 *
 * SDL owns the string returned by SDL_GetGamepadName() and only keeps it
 * alive while the gamepad is open, so it is copied into the gamepad's own
 * NK_GAMEPAD_NAME_SIZE buffer. The default "Controller #" name is kept
 * when SDL has no name to report. Longer names are truncated.
 *
 * @param gamepads The gamepads context.
 * @param num The zero-indexed gamepad slot to name.
 * @internal
 */
static void nk_gamepad_sdl3_copy_name(struct nk_gamepads* gamepads, int num) {
    const char* name;
    int i;

    name = SDL_GetGamepadName((SDL_Gamepad*)gamepads->gamepads[num].data);
    if (name == NULL || name[0] == '\0') {
        return;
    }

    for (i = 0; i < NK_GAMEPAD_NAME_SIZE - 1 && name[i] != '\0'; i++) {
        gamepads->gamepads[num].name[i] = name[i];
    }
    gamepads->gamepads[num].name[i] = '\0';
}

NK_API void nk_gamepad_sdl3_handle_event(struct nk_gamepads* gamepads, SDL_Event *event) {
    if (gamepads == NULL || event == NULL) {
        return;
    }

    /* Only act when the SDL3 input source owns the gamepad slots. Otherwise an
     * SDL_Gamepad* would be stored in a slot belonging to another input source,
     * which would never close it. */
    if (gamepads->input_source.id != NK_GAMEPAD_INPUT_SOURCE_SDL3) {
        return;
    }

    switch (event->type) {
        case SDL_EVENT_GAMEPAD_ADDED: {
            SDL_JoystickID which = event->gdevice.which;
            if (SDL_IsGamepad(which)) {
                int first_free = -1;
                int i;
                for (i = 0; i < NK_GAMEPAD_MAX; i++) {
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
                        nk_gamepad_sdl3_reset_state(gamepads, first_free);
                        nk_gamepad_sdl3_copy_name(gamepads, first_free);
                    }
                }
            }
            break;
        }
        case SDL_EVENT_GAMEPAD_REMOVED: {
            int i;
            for (i = 0; i < NK_GAMEPAD_MAX; i++) {
                if (gamepads->gamepads[i].data != NULL &&
                    SDL_GetGamepadID((SDL_Gamepad*)gamepads->gamepads[i].data) == event->gdevice.which) {
                    SDL_CloseGamepad((SDL_Gamepad*)gamepads->gamepads[i].data);
                    gamepads->gamepads[i].data = NULL;
                    gamepads->gamepads[i].available = nk_false;
                    nk_gamepad_sdl3_reset_state(gamepads, i);
                    break;
                }
            }
            break;
        }
    }
}

NK_API nk_bool nk_gamepad_sdl3_init(struct nk_gamepads* gamepads, void* user_data) {
    int count;
    SDL_JoystickID* joysticks;
    NK_UNUSED(user_data);
    if (gamepads == NULL) {
        return nk_false;
    }

    /* SDL_GetGamepads() silently returns NULL when the gamepad subsystem was
     * never initialized. SDL_InitSubSystem() is reference counted, and is
     * released again by nk_gamepad_sdl3_free(). */
    if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD)) {
        return nk_false;
    }

    count = 0;
    joysticks = SDL_GetGamepads(&count);
    if (joysticks) {
        int i;
        for (i = 0; i < count && i < NK_GAMEPAD_MAX; i++) {
            SDL_Gamepad* gamepad = SDL_OpenGamepad(joysticks[i]);
            if (gamepad != NULL) {
                gamepads->gamepads[i].data = gamepad;
                gamepads->gamepads[i].available = nk_true;
                nk_gamepad_sdl3_reset_state(gamepads, i);
                nk_gamepad_sdl3_copy_name(gamepads, i);
            }
        }
        SDL_free(joysticks);
    }

    return nk_true;
}

NK_API void nk_gamepad_sdl3_free(struct nk_gamepads* gamepads, void* user_data) {
    int i;
    NK_UNUSED(user_data);
    if (!gamepads) {
        return;
    }

    for (i = 0; i < NK_GAMEPAD_MAX; i++) {
        if (gamepads->gamepads[i].data != NULL) {
            SDL_CloseGamepad((SDL_Gamepad*)gamepads->gamepads[i].data);
            gamepads->gamepads[i].data = NULL;
            gamepads->gamepads[i].available = nk_false;
        }
    }

    /* Reference counted, so this only pairs with the SDL_InitSubSystem() call
     * made by nk_gamepad_sdl3_init() and will not shut the subsystem down
     * under an application that initialized it itself. */
    SDL_QuitSubSystem(SDL_INIT_GAMEPAD);
}

/**
 * Map an nk_gamepad_button value to the corresponding SDL_GamepadButton.
 *
 * @param button An #nk_gamepad_button enum value.
 * @return The matching SDL_GamepadButton, or SDL_GAMEPAD_BUTTON_INVALID if
 *         @p button has no SDL3 equivalent.
 * @internal
 */
static SDL_GamepadButton nk_gamepad_sdl3_map_button(int button) {
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
        case NK_GAMEPAD_BUTTON_L3: return SDL_GAMEPAD_BUTTON_LEFT_STICK;
        case NK_GAMEPAD_BUTTON_R3: return SDL_GAMEPAD_BUTTON_RIGHT_STICK;
        default: return SDL_GAMEPAD_BUTTON_INVALID;
    }
}

NK_API void nk_gamepad_sdl3_update(struct nk_gamepads* gamepads, void* user_data) {
    int num;
    int i;
    SDL_Gamepad* gamepad;
    NK_UNUSED(user_data);
    if (gamepads == NULL) {
        return;
    }

    /* SDL3 only refreshes gamepad state while pumping events, so an application
     * that does not pump SDL events on a given frame, or that called
     * SDL_SetGamepadEventsEnabled(false), would otherwise read stale input. */
    SDL_UpdateGamepads();

    for (num = 0; num < NK_GAMEPAD_MAX; num++) {
        if (gamepads->gamepads[num].data == NULL) {
            continue;
        }

        gamepad = (SDL_Gamepad*)gamepads->gamepads[num].data;

        /* Check to make sure it's still attached. */
        if (!SDL_GamepadConnected(gamepad)) {
            gamepads->gamepads[num].available = nk_false;
            SDL_CloseGamepad(gamepad);
            gamepads->gamepads[num].data = NULL;
            nk_gamepad_sdl3_reset_state(gamepads, num);
            continue;
        }

        for (i = NK_GAMEPAD_BUTTON_FIRST; i < NK_GAMEPAD_BUTTON_LAST; i++) {
            if (SDL_GetGamepadButton(gamepad, nk_gamepad_sdl3_map_button(i))) {
                nk_gamepad_button(gamepads, num, (enum nk_gamepad_button)i, nk_true);
            }
        }

        /* Axes: SDL3 returns Sint16 in range -32768..32767 (sticks) or 0..32767 (triggers) */
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_LEFT_X, SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX) / 32767.0f);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_LEFT_Y, SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY) / 32767.0f);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_RIGHT_X, SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTX) / 32767.0f);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_RIGHT_Y, SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTY) / 32767.0f);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_LEFT_TRIGGER, SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) / 32767.0f);
        nk_gamepad_axis(gamepads, num, NK_GAMEPAD_AXIS_RIGHT_TRIGGER, SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) / 32767.0f);

        /* L2 / R2 */
        nk_gamepad_button(gamepads, num, NK_GAMEPAD_BUTTON_L2, SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) > (16383 + 100) ? nk_true : nk_false);
        nk_gamepad_button(gamepads, num, NK_GAMEPAD_BUTTON_R2, SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) > (16383 + 100) ? nk_true : nk_false);
    }
}

NK_API const char* nk_gamepad_sdl3_name(struct nk_gamepads* gamepads, int num, void* user_data) {
    NK_UNUSED(user_data);
    if (gamepads == NULL || num < 0 || num >= NK_GAMEPAD_MAX) {
        return NULL;
    }

    if (gamepads->gamepads[num].data == NULL) {
        return NULL;
    }

    return gamepads->gamepads[num].name;
}

NK_API const char* nk_gamepad_sdl3_button_name(struct nk_gamepads* gamepads, enum nk_gamepad_button button, void* user_data) {
    SDL_GamepadButton sdl_button;
    NK_UNUSED(user_data);

    sdl_button = nk_gamepad_sdl3_map_button(button);
    if (sdl_button != SDL_GAMEPAD_BUTTON_INVALID && gamepads != NULL) {
        int i;
        for (i = 0; i < NK_GAMEPAD_MAX; i++) {
            if (gamepads->gamepads[i].data != NULL) {
                SDL_GamepadButtonLabel label = SDL_GetGamepadButtonLabel(
                    (SDL_Gamepad*)gamepads->gamepads[i].data, sdl_button);
                switch (label) {
                    case SDL_GAMEPAD_BUTTON_LABEL_A:        return "A";
                    case SDL_GAMEPAD_BUTTON_LABEL_B:        return "B";
                    case SDL_GAMEPAD_BUTTON_LABEL_X:        return "X";
                    case SDL_GAMEPAD_BUTTON_LABEL_Y:        return "Y";
                    case SDL_GAMEPAD_BUTTON_LABEL_CROSS:    return "Cross";
                    case SDL_GAMEPAD_BUTTON_LABEL_CIRCLE:   return "Circle";
                    case SDL_GAMEPAD_BUTTON_LABEL_SQUARE:   return "Square";
                    case SDL_GAMEPAD_BUTTON_LABEL_TRIANGLE: return "Triangle";
                    default: break;
                }
                break;
            }
        }
    }

    return nk_gamepad_button_name(NULL, button);
}

NK_API struct nk_gamepad_input_source nk_gamepad_sdl3_input_source(void* user_data) {
    struct nk_gamepad_input_source source;
    source.user_data = user_data;
    source.init = &nk_gamepad_sdl3_init;
    source.update = &nk_gamepad_sdl3_update;
    source.free = &nk_gamepad_sdl3_free;
    source.name = &nk_gamepad_sdl3_name;
    source.button_name = &nk_gamepad_sdl3_button_name;
    source.axis_name = NULL;
    source.input_source_name = "SDL3";
    source.id = NK_GAMEPAD_INPUT_SOURCE_SDL3;
    return source;
}

#ifdef __cplusplus
}
#endif

#endif
#endif
