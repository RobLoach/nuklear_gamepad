#ifndef NUKLEAR_GAMEPAD_SDL_H__
#define NUKLEAR_GAMEPAD_SDL_H__

#ifdef __cplusplus
extern "C" {
#endif

NK_API void nk_gamepad_sdl_handle_event(struct nk_gamepads* gamepads, SDL_Event *event);
NK_API nk_bool nk_gamepad_sdl_init(struct nk_gamepads* gamepads);
NK_API void nk_gamepad_sdl_free(struct nk_gamepads* gamepads);
NK_API const char* nk_gamepad_sdl_name(struct nk_gamepads* gamepads, int num);

#ifdef __cplusplus
}
#endif

#endif

#if defined(NK_GAMEPAD_IMPLEMENTATION) && !defined(NK_GAMEPAD_HEADER_ONLY)
#ifndef NUKLEAR_GAMEPAD_SDL_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_SDL_IMPLEMENTATION_ONCE

#define NK_GAMEPAD_INIT nk_gamepad_sdl_init
#define NK_GAMEPAD_UPDATE nk_gamepad_sdl_update
#define NK_GAMEPAD_NAME nk_gamepad_sdl_name
#define NK_GAMEPAD_FREE nk_gamepad_sdl_free

#ifdef __cplusplus
extern "C" {
#endif

NK_API void nk_gamepad_sdl_handle_event(struct nk_gamepads* gamepads, SDL_Event *event) {
    switch (event->type) {
        case SDL_CONTROLLERDEVICEADDED: {
            int which = event->cdevice.which;
            if (which < NK_GAMEPAD_MAX && SDL_IsGameController(which)) {
                SDL_GameController* controller = SDL_GameControllerOpen(which);
                if (controller) {
                    gamepads->gamepads[which].data = controller;
                    gamepads->gamepads[which].available = nk_true;
                }
            }
            break;
        }
        case SDL_CONTROLLERDEVICEREMOVED: {
            int which = event->cdevice.which;
            if (which < NK_GAMEPAD_MAX && gamepads->gamepads[which].data) {
                SDL_GameControllerClose(gamepads->gamepads[which].data);
                gamepads->gamepads[which].data = NULL;
                gamepads->gamepads[which].available = nk_false;
            }
            break;
        }
    }
}

NK_API nk_bool nk_gamepad_sdl_init(struct nk_gamepads* gamepads) {
    if (gamepads == NULL) {
        return nk_false;
    }

    for (int i = 0; i < NK_GAMEPAD_MAX; i++) {
        if (SDL_IsGameController(i)) {
            SDL_GameController* controller = SDL_GameControllerOpen(i);
            if (controller != NULL) {
                gamepads->gamepads[i].data = controller;
                gamepads->gamepads[i].available = nk_true;
                continue;
            }
        }

        gamepads->gamepads[i].data = NULL;
        gamepads->gamepads[i].available = nk_false;
    }

    return nk_true;
}

NK_API void nk_gamepad_sdl_free(struct nk_gamepads* gamepads) {
    if (!gamepads) {
        return;
    }

    for (int i = 0; i < NK_GAMEPAD_MAX; i++) {
        if (gamepads->gamepads[i].data != NULL) {
            SDL_GameControllerClose(gamepads->gamepads[i].data);
            gamepads->gamepads[i].data = NULL;
            gamepads->gamepads[i].available = nk_false;
        }
    }
}

int nk_gamepad_sdl_map_button(int button) {
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
    for (int num = 0; num < NK_GAMEPAD_MAX; num++) {
        SDL_GameController* controller = gamepads->gamepads[num].data;
        if (controller == NULL) {
            continue;
        }

        for (int i = NK_GAMEPAD_BUTTON_FIRST; i < NK_GAMEPAD_BUTTON_LAST; i++) {
            if (SDL_GameControllerGetButton(controller, nk_gamepad_sdl_map_button(i))) {
                nk_gamepad_button(gamepads, num, i, nk_true);
            }
        }
    }
}

NK_API const char* nk_gamepad_sdl_name(struct nk_gamepads* gamepads, int num) {
    SDL_GameController* controller = gamepads->gamepads[num].data;
    if (controller == NULL) {
        return NULL;
    }

    const char* name = SDL_GameControllerName(controller);
    if (name == NULL || name[0] == '\0') {
        return gamepads->gamepads[num].name;
    }

    return name;
}

#ifdef __cplusplus
}
#endif

#endif
#endif
