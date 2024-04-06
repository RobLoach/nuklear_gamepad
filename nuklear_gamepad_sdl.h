#ifndef NUKLEAR_GAMEPAD_SDL_H__
#define NUKLEAR_GAMEPAD_SDL_H__

void nk_gamepad_sdl_handle_event(struct nk_gamepads* gamepads, SDL_Event *evt);
void nk_gamepad_sdl_init(struct nk_gamepads* gamepads);
void nk_gamepad_sdl_free(struct nk_gamepads* gamepads);
const char* nk_gamepad_sdl_name(struct nk_gamepads* gamepads, int num);

#endif

#ifdef NK_IMPLEMENTATION
#ifndef NUKLEAR_GAMEPAD_SDL_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_SDL_IMPLEMENTATION_ONCE

void nk_gamepad_sdl_handle_event(struct nk_gamepads* gamepads, SDL_Event *event) {
    switch (event->type) {
        // case SDL_CONTROLLERBUTTONDOWN:
        // case SDL_CONTROLLERBUTTONUP: {
        //     nk_bool pushed = event->type == SDL_CONTROLLERBUTTONDOWN;
        //     int controller = event->cdevice.which;

        //     switch (event->cbutton.button) {
        //         case SDL_CONTROLLER_BUTTON_DPAD_UP:
        //             nk_gamepad_button(gamepads, controller, NK_GAMEPAD_BUTTON_UP, pushed);
        //             break;
        //         case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        //             nk_input_key(ctx, NK_KEY_DOWN, pushed);
        //             break;
        //         case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        //             nk_input_key(ctx, NK_KEY_LEFT, pushed);
        //             break;
        //         case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        //             nk_input_key(ctx, NK_KEY_RIGHT, pushed);
        //             break;
        //         case SDL_CONTROLLER_BUTTON_B:
        //             nk_input_key(ctx, NK_KEY_BACKSPACE, pushed);
        //             break;
        //         case SDL_CONTROLLER_BUTTON_A:
        //             nk_input_key(ctx, NK_KEY_ENTER, pushed);
        //             break;
        //         case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
        //             nk_input_key(ctx, NK_KEY_UP, pushed);
        //             nk_input_key(ctx, NK_KEY_CTRL, pushed);
        //             break;
        //         case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
        //             nk_input_key(ctx, NK_KEY_DOWN, pushed);
        //             nk_input_key(ctx, NK_KEY_CTRL, pushed);
        //             break;
        //     }
        // }

        case SDL_CONTROLLERDEVICEADDED:
        case SDL_CONTROLLERDEVICEREMOVED: {
            nk_gamepad_sdl_init(gamepads);
            break;
        }
    }
}

void nk_gamepad_sdl_init(struct nk_gamepads* gamepads) {
    if (gamepads == NULL) {
        return;
    }
    
    // Reset the state if we have already initialized
    if (gamepads->gamepads != NULL) {
        nk_gamepad_sdl_free(gamepads);
        nk_handle unused;
        nk_mfree(unused, gamepads->gamepads);
        gamepads->gamepads = NULL;
        gamepads->gamepads_count = 0;
    }

    // Initialize the gamepads
    gamepads->gamepads_count = SDL_NumJoysticks();
    nk_handle unused;
    gamepads->gamepads = (struct nk_gamepad*)nk_malloc(unused, NULL, gamepads->gamepads_count * sizeof(struct nk_gamepad));
    nk_zero(gamepads->gamepads, gamepads->gamepads_count * sizeof(struct nk_gamepad));

    for (int i = 0; i < gamepads->gamepads_count; i++) {
        if (SDL_IsGameController(i)) {
            SDL_GameController* controller = SDL_GameControllerOpen(i);
            if (controller) {
                gamepads->gamepads[i].data = controller;
            }
        }
    }
}

void nk_gamepad_sdl_free(struct nk_gamepads* gamepads) {
    if (!gamepads) {
        return;
    }

    for (int i = 0; i < gamepads->gamepads_count; i++) {
        if (gamepads->gamepads[i].data) {
            SDL_GameControllerClose(gamepads->gamepads[i].data);
            gamepads->gamepads[i].data = NULL;
        }
    }
}

void nk_gamepad_sdl_update(struct nk_gamepads* gamepads) {
    int button_mapping[NK_GAMEPAD_BUTTON_MAX] = {
        SDL_CONTROLLER_BUTTON_DPAD_UP, /* NK_GAMEPAD_BUTTON_UP */
        SDL_CONTROLLER_BUTTON_DPAD_DOWN, /* NK_GAMEPAD_BUTTON_DOWN */
        SDL_CONTROLLER_BUTTON_DPAD_LEFT, /* NK_GAMEPAD_BUTTON_LEFT */
        SDL_CONTROLLER_BUTTON_DPAD_RIGHT, /* NK_GAMEPAD_BUTTON_RIGHT */
        SDL_CONTROLLER_BUTTON_A, /* NK_GAMEPAD_BUTTON_A */
        SDL_CONTROLLER_BUTTON_B, /* NK_GAMEPAD_BUTTON_B */
        SDL_CONTROLLER_BUTTON_X, /* NK_GAMEPAD_BUTTON_X */
        SDL_CONTROLLER_BUTTON_Y, /* NK_GAMEPAD_BUTTON_Y */
        SDL_CONTROLLER_BUTTON_LEFTSHOULDER, /* NK_GAMEPAD_BUTTON_LB */
        SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, /* NK_GAMEPAD_BUTTON_RB */
        SDL_CONTROLLER_BUTTON_BACK, /* NK_GAMEPAD_BUTTON_BACK */
        SDL_CONTROLLER_BUTTON_START /* NK_GAMEPAD_BUTTON_START */
    };

    for (int num = 0; num < gamepads->gamepads_count; num++) {
        SDL_GameController* controller = gamepads->gamepads[num].data;
        if (!controller) {
            continue;
        }

        for (int i = 0; i < NK_GAMEPAD_BUTTON_MAX; i++) {
            if (SDL_GameControllerGetButton(controller, button_mapping[i])) {
                nk_gamepad_button(gamepads, num, i, nk_true);
            }
        }
    }
}

const char* nk_gamepad_sdl_name(struct nk_gamepads* gamepads, int num) {
    if (!gamepads || num < 0 || num >= gamepads->gamepads_count) {
        return NULL;
    }

    SDL_GameController* controller = gamepads->gamepads[num].data;
    if (!controller) {
        return NULL;
    }

    const char* name = SDL_GameControllerName(controller);
    if (!name) {
        return "Controller";
    }

    return name;
}

#endif
#endif