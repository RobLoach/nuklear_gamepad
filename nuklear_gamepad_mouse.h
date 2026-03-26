#ifndef NUKLEAR_GAMEPAD_MOUSE_H__
#define NUKLEAR_GAMEPAD_MOUSE_H__

#ifndef NK_GAMEPAD_MAX
#define NK_GAMEPAD_MAX 1
#endif

#ifndef NK_GAMEPAD_MOUSE_SENSITIVITY
/**
 * Default mouse sensitivity: Pixels of delta per frame to reach full axis.
 */
#define NK_GAMEPAD_MOUSE_SENSITIVITY 12.0f
#endif

/**
 * Mouse mapping to gamepad buttons and axes.
 *
 * @see nk_gamepad_mouse_input_source()
 */
struct nk_gamepad_mouse_map {
    enum nk_gamepad_button buttons[NK_BUTTON_MAX]; /** Mapping from Nuklear's mouse enum nk_buttons. */
    float sensitivity; /** Pixels of mouse delta per frame that maps to a full axis deflection of 1.0. @see NK_GAMEPAD_MOUSE_SENSITIVITY */
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Mouse input source for the gamepad.
 *
 * Maps...
 * - Buttons to the gamepad face buttons
 * - Movement to the left stick axes
 * - Scroll to the right stick axes
 *
 * @param user_data [nk_gamepad_mouse_map*] A custom mouse map. If NULL, the default mouse map is used.
 *
 * @return The input source for the mouse.
 */
NK_API struct nk_gamepad_input_source nk_gamepad_mouse_input_source(void* user_data);

/**
 * Initializes the mouse input source.
 *
 * @param gamepads The gamepad system to initialize.
 * @param user_data [nk_gamepad_mouse_map*] A custom mouse map. If NULL, the default mouse map is used.
 *
 * @return True on success, false if gamepads is NULL.
 */
NK_API nk_bool nk_gamepad_mouse_init(struct nk_gamepads* gamepads, void* user_data);

/**
 * Updates the gamepad state from the mouse.
 *
 * @param gamepads The gamepad system to update.
 * @param user_data [nk_gamepad_mouse_map*] A custom mouse map. If NULL, the default mouse map is used.
 */
NK_API void nk_gamepad_mouse_update(struct nk_gamepads* gamepads, void* user_data);

/**
 * Returns the name of the given mouse.
 *
 * @param gamepads The gamepad system.
 * @param num The gamepad slot. Only slot 0 is valid.
 * @param user_data Unused.
 *
 * @return "Mouse" for slot 0, NULL for any other slot.
 */
NK_API const char* nk_gamepad_mouse_name(struct nk_gamepads* gamepads, int num, void* user_data);

#ifdef __cplusplus
}
#endif

#endif

#if defined(NK_GAMEPAD_IMPLEMENTATION) && !defined(NK_GAMEPAD_HEADER_ONLY)
#ifndef NUKLEAR_GAMEPAD_MOUSE_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_MOUSE_IMPLEMENTATION_ONCE

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Default mouse mapping for the gamepad.
 */
static struct nk_gamepad_mouse_map nk_gamepad_mouse_map_default;

NK_API void nk_gamepad_mouse_update(struct nk_gamepads* gamepads, void* user_data) {
    int i;
    struct nk_gamepad_mouse_map* map;
    struct nk_mouse* mouse;
    float dx, dy;

    if (gamepads == NULL || gamepads->ctx == NULL) {
        return;
    }

    map = (user_data == NULL) ? &nk_gamepad_mouse_map_default : (struct nk_gamepad_mouse_map*)user_data;
    mouse = &gamepads->ctx->input.mouse;

    /* Mouse buttons */
    for (i = 0; i < NK_BUTTON_MAX; i++) {
        if (map->buttons[i] != NK_GAMEPAD_BUTTON_INVALID) {
            nk_gamepad_button(gamepads, 0, map->buttons[i], nk_input_is_mouse_down(&gamepads->ctx->input, (enum nk_buttons)i));
        }
    }

    /* Mouse delta maps to left stick axes */
    dx = mouse->delta.x / map->sensitivity;
    dy = mouse->delta.y / map->sensitivity;
    if (dx >  1.0f) dx =  1.0f;
    if (dx < -1.0f) dx = -1.0f;
    if (dy >  1.0f) dy =  1.0f;
    if (dy < -1.0f) dy = -1.0f;
    nk_gamepad_axis(gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X, dx);
    nk_gamepad_axis(gamepads, 0, NK_GAMEPAD_AXIS_LEFT_Y, dy);

    /* When axis reaches max, consider it a D-Pad press. */
    nk_gamepad_button(gamepads, 0, NK_GAMEPAD_BUTTON_LEFT,  dx == -1.0f);
    nk_gamepad_button(gamepads, 0, NK_GAMEPAD_BUTTON_RIGHT, dx ==  1.0f);
    nk_gamepad_button(gamepads, 0, NK_GAMEPAD_BUTTON_UP,    dy == -1.0f);
    nk_gamepad_button(gamepads, 0, NK_GAMEPAD_BUTTON_DOWN,  dy ==  1.0f);

    /* Scroll maps to right stick axes */
    dx = mouse->scroll_delta.x / map->sensitivity;
    dy = mouse->scroll_delta.y / map->sensitivity;
    if (dx >  1.0f) dx =  1.0f;
    if (dx < -1.0f) dx = -1.0f;
    if (dy >  1.0f) dy =  1.0f;
    if (dy < -1.0f) dy = -1.0f;
    nk_gamepad_axis(gamepads, 0, NK_GAMEPAD_AXIS_RIGHT_X, dx);
    nk_gamepad_axis(gamepads, 0, NK_GAMEPAD_AXIS_RIGHT_Y, dy);
}

NK_API nk_bool nk_gamepad_mouse_init(struct nk_gamepads* gamepads, void* user_data) {
    int i;
    if (gamepads == NULL) {
        return nk_false;
    }
    NK_UNUSED(user_data);

    /* Only one mouse is available. */
    gamepads->gamepads[0].available = nk_true;
    for (i = 1; i < NK_GAMEPAD_MAX; i++) {
        gamepads->gamepads[i].available = nk_false;
    }

    /* Initialize the default mouse mapping. */
    nk_zero(&nk_gamepad_mouse_map_default, sizeof(nk_gamepad_mouse_map_default));
    nk_gamepad_mouse_map_default.sensitivity = NK_GAMEPAD_MOUSE_SENSITIVITY;

    /* Initialize all button slots to invalid. */
    for (i = 0; i < NK_BUTTON_MAX; i++) {
        nk_gamepad_mouse_map_default.buttons[i] = NK_GAMEPAD_BUTTON_INVALID;
    }

    /* Mouse buttons */
    nk_gamepad_mouse_map_default.buttons[NK_BUTTON_LEFT]   = NK_GAMEPAD_BUTTON_A;
    nk_gamepad_mouse_map_default.buttons[NK_BUTTON_RIGHT]  = NK_GAMEPAD_BUTTON_B;
    nk_gamepad_mouse_map_default.buttons[NK_BUTTON_MIDDLE] = NK_GAMEPAD_BUTTON_X;
    nk_gamepad_mouse_map_default.buttons[NK_BUTTON_X1] = NK_GAMEPAD_BUTTON_Y;
    nk_gamepad_mouse_map_default.buttons[NK_BUTTON_X2] = NK_GAMEPAD_BUTTON_START;

    return nk_true;
}

NK_API struct nk_gamepad_input_source nk_gamepad_mouse_input_source(void* user_data) {
    struct nk_gamepad_input_source source;
    source.user_data = user_data;
    source.init = nk_gamepad_mouse_init;
    source.update = nk_gamepad_mouse_update;
    source.free = NULL;
    source.name = nk_gamepad_mouse_name;
    source.input_source_name = "Mouse";
    source.id = NK_GAMEPAD_INPUT_SOURCE_MOUSE;
    return source;
}

NK_API const char* nk_gamepad_mouse_name(struct nk_gamepads* gamepads, int num, void* user_data) {
    NK_UNUSED(gamepads);
    NK_UNUSED(user_data);
    if (num == 0) {
        return "Mouse";
    }
    return NULL;
}

#ifdef __cplusplus
}
#endif

#endif
#endif
