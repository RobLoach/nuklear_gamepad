#ifndef NUKLEAR_GAMEPAD_SDL3_H__
#define NUKLEAR_GAMEPAD_SDL3_H__

#ifdef __cplusplus
extern "C" {
#endif

NK_API void nk_gamepad_sdl3_handle_event(struct nk_gamepads* gamepads, SDL_Event *event);
NK_API nk_bool nk_gamepad_sdl3_init(struct nk_gamepads* gamepad, void* user_data);
NK_API void nk_gamepad_sdl3_update(struct nk_gamepads* gamepads, void* user_data);
NK_API void nk_gamepad_sdl3_free(struct nk_gamepads* gamepads, void* user_data);
NK_API const char* nk_gamepad_sdl3_name(struct nk_gamepads* gamepads, int num, void* user_data);
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
            int which = event->gamepad.which;
            if (which < NK_GAMEPAD_MAX && SDL_IsGamepad(which)) {
                SDL_Gamepad* gamepad = SDL_OpenGamepad(which);
                if (gamepad) {
                    gamepads->gamepads[which].data = gamepad;
                    gamepads->gamepads[which].available = nk_true;
                }
            }
            break;
        }
        case SDL_EVENT_GAMEPAD_REMOVED: {
            int which = event->gamepad.which;
            if (which < NK_GAMEPAD_MAX && gamepads->gamepads[which].data) {
                SDL_CloseGamepad(gamepads->gamepads[which].data);
                gamepads->gamepads[which].data = NULL;
                gamepads->gamepads[which].available = nk_false;
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

    for (int i = 0; i < NK_GAMEPAD_MAX; i++) {
        if (SDL_IsGamepad(i)) {
            SDL_Gamepad* gamepad = SDL_OpenGamepad(i);
            if (gamepad != NULL) {
                gamepads->gamepads[i].data = gamepad;
                gamepads->gamepads[i].available = nk_true;
                continue;
            }
        }
        gamepads->gamepads[i].data = NULL;
        gamepads->gamepads[i].available = nk_false;
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

SDL_GamepadButton nk_gamepad_sdl3_map_button(int button) {
    switch (button) {
        case NK_GAMEPAD_BUTTON_UP: return SDL_GAMEPAD_BUTTON_DPAD_UP;
        case NK_GAMEPAD_BUTTON_DOWN: return SDL_GAMEPAD_BUTTON_DPAD_DOWN;
        case NK_GAMEPAD_BUTTON_LEFT: return SDL_GAMEPAD_BUTTON_DPAD_LEFT;
        case NK_GAMEPAD_BUTTON_RIGHT: return SDL_GAMEPAD_BUTTON_DPAD_RIGHT;
        case NK_GAMEPAD_BUTTON_A: return SDL_GAMEPAD_BUTTON_A;
        case NK_GAMEPAD_BUTTON_B: return SDL_GAMEPAD_BUTTON_B;
        case NK_GAMEPAD_BUTTON_X: return SDL_GAMEPAD_BUTTON_X;
        case NK_GAMEPAD_BUTTON_Y: return SDL_GAMEPAD_BUTTON_Y;
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
