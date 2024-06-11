/**
 * nuklear_gamepad: Gamepad support for Nuklear GUI
 * https://github.com/robloach/nuklear_gamepad
 *
 * @file nuklear_gamepad.h
 *
 * License:
 *
 * Copyright (c) 2024 Rob Loach (@RobLoach, https://robloach.net)
 *
 * This software is provided "as-is", without any express or implied warranty. In no event
 * will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim that you
 * wrote the original software. If you use this software in a product, an acknowledgment
 * in the product documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented
 * as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

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
    char name[32];
};

struct nk_gamepads {
    struct nk_gamepad* gamepads;
    int gamepads_count;
    struct nk_context* ctx;
    void* user_data;
};

NK_API struct nk_gamepads* nk_gamepad_init(struct nk_context* ctx, void* user_data);
NK_API void nk_gamepad_free(struct nk_gamepads* gamepads);
NK_API void nk_gamepad_init_gamepads(struct nk_gamepads* gamepads, int num);
NK_API nk_bool nk_gamepad_is_button_down(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button);
NK_API nk_bool nk_gamepad_is_button_pressed(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button);
NK_API nk_bool nk_gamepad_is_button_released(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button);
NK_API void nk_gamepad_button(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button, nk_bool down);
NK_API int nk_gamepad_count(struct nk_gamepads* gamepads);
NK_API const char* nk_gamepad_name(struct nk_gamepads* gamepads, int num);

#define NK_GAMEPAD_BUTTON_FLAG(button) (1 << (button))

#endif

#ifdef NK_GAMEPAD_IMPLEMENTATION
#ifndef NK_GAMEPAD_IMPLEMENTATION_ONCE
#define NK_GAMEPAD_IMPLEMENTATION_ONCE

#if !defined(NK_GAMEPAD_SDL) && !defined(NK_GAMEPAD_GLFW) && !defined(NK_GAMEPAD_RAYLIB) && !defined(NK_GAMEPAD_PNTR) && !defined(NK_GAMEPAD_NONE)
#if defined(NK_SDL_RENDERER_IMPLEMENTATION) || defined(NK_SDL_GL2_IMPLEMENTATION) || defined(NK_SDL_GL3_IMPLEMENTATION) || defined(NK_SDL_GLES2_IMPLEMENTATION)
    #define NK_GAMEPAD_SDL
#elif defined(NK_GLFW_RENDERER_IMPLEMENTATION) || defined(NK_GLFW_GL2_IMPLEMENTATION) || defined(NK_GLFW_GL3_IMPLEMENTATION) || defined(GLFW_INCLUDE_VULKAN)
    #define NK_GAMEPAD_GLFW
#elif defined(RAYLIB_NUKLEAR_IMPLEMENTATION) || defined(NK_RAYLIB_IMPLEMENTATION)
    #define NK_GAMEPAD_RAYLIB
#elif defined(PNTR_NUKLEAR_IMPLEMENTATION)
    #define NK_GAMEPAD_PNTR
#endif
#endif

#ifndef NK_GAMEPAD_MFREE
#ifdef NK_GAMEPAD_SDL
#define NK_GAMEPAD_MFREE(unused, ptr) SDL_free(ptr)
#elif defined(NK_GAMEPAD_PNTR)
#define NK_GAMEPAD_MFREE(unused, ptr) pntr_unload_memory(ptr)
#elif defined(NK_GAMEPAD_GLFW)
// GLFW: Defer to NK_INCLUDE_DEFAULT_ALLOCATOR
#elif defined(NK_GAMEPAD_RAYLIB)
#define NK_GAMEPAD_MFREE(unused, ptr) MemFree(ptr)
#endif
#endif

#if !defined(NK_GAMEPAD_MFREE) && defined(NK_INCLUDE_DEFAULT_ALLOCATOR)
#define NK_GAMEPAD_MFREE(unused, ptr) nk_mfree(unused, ptr)
#endif

#ifndef NK_GAMEPAD_MALLOC
#ifdef NK_GAMEPAD_SDL
#define NK_GAMEPAD_MALLOC(unused, old, size) SDL_malloc(size)
#elif defined(NK_GAMEPAD_PNTR)
#define NK_GAMEPAD_MALLOC(unused, old, size) pntr_load_memory(size)
#elif defined(NK_GAMEPAD_GLFW)
// GLFW: Defer to NK_INCLUDE_DEFAULT_ALLOCATOR
#elif defined(NK_GAMEPAD_RAYLIB)
#define NK_GAMEPAD_MALLOC(unused, old, size) MemAlloc(size)
#endif
#endif

#if !defined(NK_GAMEPAD_MALLOC) && defined(NK_INCLUDE_DEFAULT_ALLOCATOR)
#define NK_GAMEPAD_MALLOC(unused, old, size) nk_malloc(unused, old, size)
#endif

#ifdef NK_GAMEPAD_SDL
#include "nuklear_gamepad_sdl.h"
#elif defined(NK_GAMEPAD_GLFW)
#include "nuklear_gamepad_glfw.h"
#elif defined(NK_GAMEPAD_RAYLIB)
#include "nuklear_gamepad_raylib.h"
#elif defined(NK_GAMEPAD_PNTR)
#include "nuklear_gamepad_pntr.h"
#endif

NK_API struct nk_gamepads* nk_gamepad_init(struct nk_context* ctx, void* user_data) {
    nk_handle unused = {0};
    (void)unused;
    struct nk_gamepads* gamepads = (struct nk_gamepads*)NK_GAMEPAD_MALLOC(unused, NULL, sizeof(struct nk_gamepads));
    nk_zero(gamepads, sizeof(struct nk_gamepads));
    gamepads->ctx = ctx;
    gamepads->user_data = user_data;

    #ifdef NK_GAMEPAD_INIT
    NK_GAMEPAD_INIT(gamepads);
    #endif

    return gamepads;
}

NK_API void nk_gamepad_free(struct nk_gamepads* gamepads) {
    if (gamepads == NULL) {
        return;
    }

    #ifdef NK_GAMEPAD_FREE
    NK_GAMEPAD_FREE(gamepads);
    #endif

    nk_handle unused = {0};
    (void)unused;
    if (gamepads->gamepads != NULL) {
        NK_GAMEPAD_MFREE(unused, gamepads->gamepads);
    }

    NK_GAMEPAD_MFREE(unused, gamepads);
}

NK_API void nk_gamepad_init_gamepads(struct nk_gamepads* gamepads, int num) {
    if (gamepads == NULL || num <= 0) {
        return;
    }

    // Clean up if needed.
    if (gamepads->gamepads != NULL) {
        nk_handle unused = {0};
        (void)unused;
        NK_GAMEPAD_MFREE(unused, gamepads->gamepads);
        gamepads->gamepads = NULL;
        gamepads->gamepads_count = 0;
    }

    // Initialize the new gamepad
    nk_handle unused = {0};
    (void)unused;
    gamepads->gamepads = (struct nk_gamepad*)NK_GAMEPAD_MALLOC(unused, NULL, num * sizeof(struct nk_gamepad));
    nk_zero(gamepads->gamepads, num * sizeof(struct nk_gamepad));
    gamepads->gamepads_count = num;

    // Set up the initial name for the gamepads
    for (int i = 0; i < num; i++) {
        NK_MEMCPY((void*)gamepads->gamepads[i].name, "Controller ", 11);
        nk_itoa(gamepads->gamepads[i].name + 11, i + 1);
    }
}

NK_API void nk_gamepad_button(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button, nk_bool down) {
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

NK_API void nk_gamepad_update(struct nk_gamepads* gamepads) {
    if (gamepads == NULL) {
        return;
    }

    for (int i = 0; i < gamepads->gamepads_count; i++) {
        gamepads->gamepads[i].buttons_prev = gamepads->gamepads[i].buttons;
        gamepads->gamepads[i].buttons = 0;
    }

    #ifdef NK_GAMEPAD_UPDATE
    NK_GAMEPAD_UPDATE(gamepads);
    #endif
}

/**
 * Check whether or not the given gamepad button is currently down.
 *
 * @param gamepads The gamepads to check.
 * @param num The gamepad number to check. If -1 all gamepads are checked.
 * @param button The button to check.
 */
NK_API nk_bool nk_gamepad_is_button_down(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button) {
    if (gamepads == NULL) {
        return nk_false;
    }

    if (num <= -1) {
        for (int i = 0; i < gamepads->gamepads_count; i++) {
            if ((gamepads->gamepads[i].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) != 0) {
                return nk_true;
            }
        }
        return nk_false;
    }

    if (num >= gamepads->gamepads_count) {
        return nk_false;
    }

    return (gamepads->gamepads[num].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) != 0;
}

/**
 * Check whether or not the given gamepad button was pressed.
 *
 * @param gamepads The gamepads to check.
 * @param num The gamepad number to check. If -1 all gamepads are checked.
 * @param button The button to check.
 */
NK_API nk_bool nk_gamepad_is_button_pressed(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button) {
    if (gamepads == NULL) {
        return nk_false;
    }

    if (num <= -1) {
        for (int i = 0; i < gamepads->gamepads_count; i++) {
            if ((gamepads->gamepads[i].buttons_prev & NK_GAMEPAD_BUTTON_FLAG(button)) == 0 &&
			    (gamepads->gamepads[i].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) != 0) {
                return nk_true;
            }
        }
        return nk_false;
    }

    if (num >= gamepads->gamepads_count) {
        return nk_false;
    }

    return ((gamepads->gamepads[num].buttons_prev & NK_GAMEPAD_BUTTON_FLAG(button)) == 0 &&
			(gamepads->gamepads[num].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) != 0);
}

/**
 * Check whether or not the given gamepad button was released.
 *
 * @param gamepads The gamepads to check.
 * @param num The gamepad number to check. If -1 all gamepads are checked.
 * @param button The button to check.
 */
NK_API nk_bool nk_gamepad_is_button_released(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button) {
    if (gamepads == NULL) {
        return nk_false;
    }

    if (num <= -1) {
        for (int i = 0; i < gamepads->gamepads_count; i++) {
            if ((gamepads->gamepads[i].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) == 0 &&
			    (gamepads->gamepads[i].buttons_prev & NK_GAMEPAD_BUTTON_FLAG(button)) != 0) {
                return nk_true;
            }
        }
        return nk_false;
    }

    if (num >= gamepads->gamepads_count) {
        return nk_false;
    }

    return ((gamepads->gamepads[num].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) == 0 &&
			(gamepads->gamepads[num].buttons_prev & NK_GAMEPAD_BUTTON_FLAG(button)) != 0);
}

NK_API int nk_gamepad_count(struct nk_gamepads* gamepads) {
    if (!gamepads) {
        return 0;
    }

    return gamepads->gamepads_count;
}

NK_API const char* nk_gamepad_name(struct nk_gamepads* gamepads, int num) {
    if (!gamepads || num < 0 || num >= gamepads->gamepads_count) {
        return NULL;
    }

    #ifdef NK_GAMEPAD_NAME
    return NK_GAMEPAD_NAME(gamepads, num);
    #else
    return gamepads->gamepads[num].name;
    #endif
}

#endif  // NK_GAMEPAD_IMPLEMENTATION_ONCE
#endif  // NK_GAMEPAD_IMPLEMENTATION
