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

typedef struct nk_gamepad {
    nk_bool buttons_last[NK_GAMEPAD_BUTTON_MAX];
    nk_bool buttons_current[NK_GAMEPAD_BUTTON_MAX];
} nk_gamepad_system;

typedef struct nk_gamepad_system {
    nk_gamepad* gamepads;
    int gamepads_count;
    void* user_data;
} nk_gamepad_system;

nk_gamepad_system* nk_gamepad_init(struct nk_context* ctx);
void nk_gamepad_free(nk_gamepad_system* system);
nk_bool nk_gamepad_is_button_down(struct nk_gamepad_system* system, int num, nk_gamepad_button button);
nk_bool nk_gamepad_is_button_pressed(struct nk_gamepad_system* system, int num, nk_gamepad_button button);
nk_bool nk_gamepad_is_button_released(struct nk_gamepad_system* system, int num, nk_gamepad_button button);

#endif

#ifdef NK_IMPLEMENTATION
#ifndef NUKLEAR_GAMEPAD_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_IMPLEMENTATION_ONCE

nk_gamepad_system* nk_gamepad_init(struct nk_context* ctx) {
    nk_gamepad_system* gamepad = (nk_gamepad_system*)nk_malloc(sizeof(nk_gamepad_system));
    nk_zero(gamepad, sizeof(nk_gamepad_system));
    return gamepad;
}

#endif
#endif