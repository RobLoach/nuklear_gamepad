#ifndef NUKLEAR_GAMEPAD_SDL_H__
#define NUKLEAR_GAMEPAD_SDL_H__

void nk_gamepad_sdl_handle_event(struct nk_context* ctx, SDL_Event *evt);
void nk_gamepad_sdl_init(struct nk_context* ctx);

#endif

#ifdef NK_IMPLEMENTATION
#ifndef NUKLEAR_GAMEPAD_SDL_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_SDL_IMPLEMENTATION_ONCE

void nk_gamepad_sdl_handle_event(struct nk_context* ctx, SDL_Event *evt) {
    switch (event.type) {
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            switch (evt.cbutton.button) {
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    nk_input_key(ctx, NK_KEY_UP, evt.type == SDL_CONTROLLERBUTTONDOWN);
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    nk_input_key(ctx, NK_KEY_DOWN, evt.type == SDL_CONTROLLERBUTTONDOWN);
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    nk_input_key(ctx, NK_KEY_LEFT, evt.type == SDL_CONTROLLERBUTTONDOWN);
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    nk_input_key(ctx, NK_KEY_RIGHT, evt.type == SDL_CONTROLLERBUTTONDOWN);
                    break;
                case SDL_CONTROLLER_BUTTON_B:
                    nk_input_key(ctx, NK_KEY_BACKSPACE, evt.type == SDL_CONTROLLERBUTTONDOWN);
                    break;
                case SDL_CONTROLLER_BUTTON_A:
                    nk_input_key(ctx, NK_KEY_ENTER, evt.type == SDL_CONTROLLERBUTTONDOWN);
                    break;
                case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                    nk_input_key(ctx, NK_KEY_UP, evt.type == SDL_CONTROLLERBUTTONDOWN);
                    nk_input_key(ctx, NK_KEY_CTRL, evt.type == SDL_CONTROLLERBUTTONDOWN);
                    break;
                case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                    nk_input_key(ctx, NK_KEY_DOWN, evt.type == SDL_CONTROLLERBUTTONDOWN);
                    nk_input_key(ctx, NK_KEY_CTRL, evt.type == SDL_CONTROLLERBUTTONDOWN);
                    break;
            }
    }
}

#endif
#endif