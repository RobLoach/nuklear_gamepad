#ifndef NUKLEAR_GAMEPAD_SDL_H__
#define NUKLEAR_GAMEPAD_SDL_H__

NK_API void nk_gamepad_sdl_handle_event(struct nk_gamepads* gamepads, SDL_Event *event);
NK_API nk_bool nk_gamepad_sdl_init(struct nk_gamepads* gamepads);
NK_API void nk_gamepad_sdl_free(struct nk_gamepads* gamepads);
NK_API const char* nk_gamepad_sdl_name(struct nk_gamepads* gamepads, int num);

#endif

#ifdef NK_GAMEPAD_IMPLEMENTATION
#ifndef NUKLEAR_GAMEPAD_SDL_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_SDL_IMPLEMENTATION_ONCE

#ifndef NK_GAMEPAD_MALLOC
    #define NK_GAMEPAD_MALLOC(unused, old, size) SDL_malloc(size)
#endif

#ifndef NK_GAMEPAD_MFREE
    #define NK_GAMEPAD_MFREE(unused, ptr) SDL_free(ptr)
#endif

#define NK_GAMEPAD_INIT nk_gamepad_sdl_init
#define NK_GAMEPAD_UPDATE nk_gamepad_sdl_update
#define NK_GAMEPAD_NAME nk_gamepad_sdl_name
#define NK_GAMEPAD_FREE nk_gamepad_sdl_free

NK_API void nk_gamepad_sdl_handle_event(struct nk_gamepads* gamepads, SDL_Event *event) {
    switch (event->type) {
        case SDL_CONTROLLERDEVICEADDED:
        case SDL_CONTROLLERDEVICEREMOVED: {
            nk_gamepad_sdl_init(gamepads);
            break;
        }
    }
}

NK_API nk_bool nk_gamepad_sdl_init(struct nk_gamepads* gamepads) {
    if (gamepads == NULL) {
        return nk_false;
    }

    // Reset the state if we have already initialized
    if (gamepads->gamepads != NULL) {
        nk_gamepad_sdl_free(gamepads);
        nk_handle unused;
        NK_UNUSED(unused);
        NK_GAMEPAD_MFREE(unused, gamepads->gamepads);
        gamepads->gamepads = NULL;
        gamepads->gamepads_count = 0;
    }

    // Initialize the gamepads
    if (nk_gamepad_init_gamepads(gamepads, SDL_NumJoysticks()) == nk_false) {
        return nk_false;
    }

    for (int i = 0; i < gamepads->gamepads_count; i++) {
        if (SDL_IsGameController(i)) {
            SDL_GameController* controller = SDL_GameControllerOpen(i);
            if (controller) {
                gamepads->gamepads[i].data = controller;
            }
        }
    }

    return nk_true;
}

NK_API void nk_gamepad_sdl_free(struct nk_gamepads* gamepads) {
    if (!gamepads) {
        return;
    }

    for (int i = 0; i < gamepads->gamepads_count; i++) {
        if (gamepads->gamepads[i].data) {
            SDL_GameControllerClose((SDL_GameController*)gamepads->gamepads[i].data);
            gamepads->gamepads[i].data = NULL;
        }
    }
}

SDL_GameControllerButton nk_gamepad_sdl_map_button(int button) {
    switch (button) {
        case NK_GAMEPAD_BUTTON_UP: return SDL_CONTROLLER_BUTTON_DPAD_UP;
        case NK_GAMEPAD_BUTTON_DOWN: return SDL_CONTROLLER_BUTTON_DPAD_DOWN;
        case NK_GAMEPAD_BUTTON_LEFT: return SDL_CONTROLLER_BUTTON_DPAD_LEFT;
        case NK_GAMEPAD_BUTTON_RIGHT: return SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
        case NK_GAMEPAD_BUTTON_A: return SDL_CONTROLLER_BUTTON_A;
        case NK_GAMEPAD_BUTTON_B: return SDL_CONTROLLER_BUTTON_B;
        case NK_GAMEPAD_BUTTON_X: return SDL_CONTROLLER_BUTTON_X;
        case NK_GAMEPAD_BUTTON_Y: return SDL_CONTROLLER_BUTTON_Y;
        case NK_GAMEPAD_BUTTON_LB: return SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
        case NK_GAMEPAD_BUTTON_RB: return SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
        case NK_GAMEPAD_BUTTON_BACK: return SDL_CONTROLLER_BUTTON_BACK;
        case NK_GAMEPAD_BUTTON_START: return SDL_CONTROLLER_BUTTON_START;
        default: return SDL_CONTROLLER_BUTTON_INVALID;
    }
}

NK_API void nk_gamepad_sdl_update(struct nk_gamepads* gamepads) {
    for (int num = 0; num < gamepads->gamepads_count; num++) {
        SDL_GameController* controller = (SDL_GameController*)gamepads->gamepads[num].data;
        if (!controller) {
            continue;
        }

        for (int i = NK_GAMEPAD_BUTTON_FIRST; i < NK_GAMEPAD_BUTTON_LAST; i++) {
            if (SDL_GameControllerGetButton(controller, nk_gamepad_sdl_map_button(i))) {
                nk_gamepad_button(gamepads, num, (enum nk_gamepad_button)i, nk_true);
            }
        }
    }
}

NK_API const char* nk_gamepad_sdl_name(struct nk_gamepads* gamepads, int num) {
    if (!gamepads || num < 0 || num >= gamepads->gamepads_count) {
        return NULL;
    }

    SDL_GameController* controller = (SDL_GameController*)gamepads->gamepads[num].data;
    if (!controller) {
        return gamepads->gamepads[num].name;
    }

    const char* name = SDL_GameControllerName(controller);
    if (!name) {
        return gamepads->gamepads[num].name;
    }

    return name;
}

#endif
#endif
