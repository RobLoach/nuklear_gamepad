#ifndef NUKLEAR_GAMEPAD_H__
#define NUKLEAR_GAMEPAD_H__

enum nk_gamepad_button {
    NK_GAMEPAD_BUTTON_UP,
    NK_GAMEPAD_BUTTON_DOWN,
    NK_GAMEPAD_BUTTON_LEFT,
    NK_GAMEPAD_BUTTON_RIGHT,
    NK_GAMEPAD_BUTTON_A,
    NK_GAMEPAD_BUTTON_B,
    NK_GAMEPAD_BUTTON_X,
    NK_GAMEPAD_BUTTON_Y,
    NK_GAMEPAD_BUTTON_LB,
    NK_GAMEPAD_BUTTON_RB,
    NK_GAMEPAD_BUTTON_BACK,
    NK_GAMEPAD_BUTTON_START,
    NK_GAMEPAD_BUTTON_MAX
};

struct nk_gamepad {
    int buttons;
    int buttons_prev;
    void* data;
};

struct nk_gamepads {
    struct nk_gamepad* gamepads;
    int gamepads_count;
    struct nk_context* ctx;
};

struct nk_gamepads* nk_gamepad_init(struct nk_context* ctx);
void nk_gamepad_free(struct nk_gamepads* gamepads);
nk_bool nk_gamepad_is_button_down(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button);
nk_bool nk_gamepad_is_button_pressed(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button);
nk_bool nk_gamepad_is_button_released(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button);
void nk_gamepad_button(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button, nk_bool down);
int nk_gamepad_count(struct nk_gamepads* gamepads);

#define NK_GAMEPAD_BUTTON_FLAG(button) (1 << (button))

#endif

#ifdef NK_IMPLEMENTATION
#ifndef NK_GAMEPAD_IMPLEMENTATION_ONCE
#define NK_GAMEPAD_IMPLEMENTATION_ONCE

#ifdef NK_GAMEPAD_SDL
#include "nuklear_gamepad_sdl.h"
#endif

struct nk_gamepads* nk_gamepad_init(struct nk_context* ctx) {
    nk_handle unused;
    struct nk_gamepads* gamepads = (struct nk_gamepads*)nk_malloc(unused, NULL, sizeof(struct nk_gamepads));
    nk_zero(gamepads, sizeof(struct nk_gamepads));
    gamepads->ctx = ctx;

    #ifdef NK_GAMEPAD_SDL
    nk_gamepad_sdl_init(gamepads);
    #endif

    return gamepads;
}

void nk_gamepad_free(struct nk_gamepads* gamepads) {
    if (gamepads == NULL) {
        return;
    }

    #ifdef NK_GAMEPAD_SDL
    nk_gamepad_sdl_free(gamepads);
    #endif

    nk_handle unused;
    if (gamepads->gamepads != NULL) {
        nk_mfree(unused, gamepads->gamepads);
    }
    nk_mfree(unused, gamepads);
}

void nk_gamepad_button(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button, nk_bool down) {
    if (gamepads == NULL || num < 0 || num >= gamepads->gamepads_count) {
        return;
    }

    if (down) {
        gamepads->gamepads[num].buttons |= NK_GAMEPAD_BUTTON_FLAG(button);
    }
    else {
        gamepads->gamepads[num].buttons &= ~NK_GAMEPAD_BUTTON_FLAG(button);
    }
}

void nk_gamepad_update(struct nk_gamepads* gamepads) {
    if (gamepads == NULL) {
        return;
    }

    for (int i = 0; i < gamepads->gamepads_count; i++) {
        gamepads->gamepads[i].buttons_prev = gamepads->gamepads[i].buttons;
        gamepads->gamepads[i].buttons = 0;
    }

    #ifdef NK_GAMEPAD_SDL
    nk_gamepad_sdl_update(gamepads);
    #endif
}

nk_bool nk_gamepad_is_button_down(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button) {
    if (gamepads == NULL || num < 0 || num >= gamepads->gamepads_count) {
        return nk_false;
    }

    return (gamepads->gamepads[num].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) != 0/* ? nk_true : nk_false*/;
}

nk_bool nk_gamepad_is_button_pressed(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button) {
    if (gamepads == NULL || num < 0 || num >= gamepads->gamepads_count) {
        return nk_false;
    }
    
    return ((gamepads->gamepads[num].buttons_prev & NK_GAMEPAD_BUTTON_FLAG(button)) == 0 &&
			(gamepads->gamepads[num].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) != 0)/* ? nk_true : nk_false*/;
}

nk_bool nk_gamepad_is_button_released(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button) {
    if (gamepads == NULL || num < 0 || num >= gamepads->gamepads_count) {
        return nk_false;
    }

    return ((gamepads->gamepads[num].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) == 0 &&
			(gamepads->gamepads[num].buttons_prev & NK_GAMEPAD_BUTTON_FLAG(button)) != 0)/* ? nk_true : nk_false*/;
}

int nk_gamepad_count(struct nk_gamepads* gamepads) {
    if (!gamepads) {
        return 0;
    }

    return gamepads->gamepads_count;
}

const char* nk_gamepad_name(struct nk_gamepads* gamepads, int num) {
    if (!gamepads || num < 0 || num >= gamepads->gamepads_count) {
        return NULL;
    }

    #ifdef NK_GAMEPAD_SDL
    return nk_gamepad_sdl_name(gamepads, num);
    #else
    return NULL;
    #endif
}

#endif
#endif