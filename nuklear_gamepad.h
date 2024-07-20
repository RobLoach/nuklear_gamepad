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

#ifdef __cplusplus
extern "C" {
#endif

enum nk_gamepad_button {
    NK_GAMEPAD_BUTTON_INVALID = -1,
    NK_GAMEPAD_BUTTON_FIRST = 0,
    NK_GAMEPAD_BUTTON_UP = 0,
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
    NK_GAMEPAD_BUTTON_LAST
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

/**
 * Initialize the Nuklear Gamepad system.
 *
 * @param ctx The Nuklear context.
 * @param user_data Any user data to pass through to the gamepad system.
 *
 * @return The gamepad system, or NULL on failure.
 * @see nk_gamepad_free()
 */
NK_API struct nk_gamepads* nk_gamepad_init(struct nk_context* ctx, void* user_data);

/**
 * Disconnect all the gamepads and frees associated memory.
 *
 * @param gamepads The gamepad system to free up.
 * @see nk_gamepad_init()
 */
NK_API void nk_gamepad_free(struct nk_gamepads* gamepads);

/**
 * Initializes a number of gamepads within the given gamepad system.
 *
 * @param gamepads The gamepad system.
 * @param num The amount of gamepads to initialize.
 *
 * @return True if the gamepads were initialized properly, false otherwise.
 */
NK_API nk_bool nk_gamepad_init_gamepads(struct nk_gamepads* gamepads, int num);

/**
 * Updates the state of all the gamepads.
 *
 * @param gamepads The gamepad system to update.
 */
NK_API void nk_gamepad_update(struct nk_gamepads* gamepads);

/**
 * Checks whether the specified button of the given gamepad is currently down.
 *
 * @param gamepads The associated gamepad system.
 * @param num Which gamepad to check. -1 will check for any available gamepad.
 * @param button The button to check.
 *
 * @return True if the button is down, false otherwise.
 *
 * @see nk_gamepad_is_button_pressed()
 */
NK_API nk_bool nk_gamepad_is_button_down(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button);

/**
 * Checks whether the specified button of the given gamepad was just pressed.
 *
 * @param gamepads The associated gamepad system.
 * @param num Which gamepad to check. -1 will check for any available gamepad.
 * @param button The button to check.
 *
 * @return True if the button was just pressed, false otherwise.
 */
NK_API nk_bool nk_gamepad_is_button_pressed(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button);

/**
 * Checks whether the specified button of the given gamepad was just released.
 *
 * @param gamepads The associated gamepad system.
 * @param num Which gamepad to check. -1 will check for any available gamepad.
 * @param button The button to check.
 *
 * @return True if the button was just released, false otherwise.
 *
 * @see nk_gamepad_is_button_pressed()
 */
NK_API nk_bool nk_gamepad_is_button_released(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button);

/**
 * Invoke a button press or release for the specified gamepad.
 *
 * @param gamepads The associated gamepad system.
 * @param num The gamepad number.
 * @param button The button to invoke.
 * @param down True to indicate a button press, false to indicate a button release.
 */
NK_API void nk_gamepad_button(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button, nk_bool down);

/**
 * Returns the amount of gamepads currently available.
 *
 * @param gamepads The associated gamepad system.
 *
 * @return The amount of gamepads currently available.
 */
NK_API int nk_gamepad_count(struct nk_gamepads* gamepads);

/**
 * Returns the name of the specified gamepad.
 *
 * @param gamepads The associated gamepad system.
 * @param num The gamepad number.
 *
 * @return The name of the gamepad.
 */
NK_API const char* nk_gamepad_name(struct nk_gamepads* gamepads, int num);

/**
 * Get the user data for the associated gamepad system.
 *
 * @param gamepads The associated gamepad system.
 *
 * @return The user data for the gamepad system.
 */
NK_API void* nk_gamepad_user_data(struct nk_gamepads* gamepads);

/**
 * Get the first gamepad button that is found to be pressed.
 *
 * @param gamepads The associated gamepad system.
 * @param num The gamepad number to check against. Provide -1 to check all gamepads.
 * @param out_num The gamepad number that the button is pressed on.
 * @param out_button The button that was pressed.
 *
 * @return True or False depending on whether or not a button was pressed.
 *
 * @code
 * int num;
 * enum nk_gamepad_button button;
 * if (nk_gamepad_any_button_pressed(gamepads, -1, &num, &button)) {
 *   printf("Gamepad: %d. Button: %d\n", num, button);
 * }
 * @endcode
 */
NK_API nk_bool nk_gamepad_any_button_pressed(struct nk_gamepads* gamepads, int num, int* out_num, enum nk_gamepad_button* out_button);

#define NK_GAMEPAD_BUTTON_FLAG(button) (1 << (button))

#ifdef __cplusplus
}
#endif

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

#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
    #ifndef NK_GAMEPAD_MALLOC
        #define NK_GAMEPAD_MALLOC(unused, old, size) nk_malloc(unused, old, size)
    #endif
    #ifndef NK_GAMEPAD_MFREE
        #define NK_GAMEPAD_MFREE(unused, ptr) nk_mfree(unused, ptr)
    #endif
#elif defined(NK_GAMEPAD_NONE)
    #ifndef NK_GAMEPAD_MALLOC
        #include <stdlib.h>
        #define NK_GAMEPAD_MALLOC(unused, old, size) malloc(size)
    #endif
    #ifndef NK_GAMEPAD_MFREE
        #include <stdlib.h>
        #define NK_GAMEPAD_MFREE(unused, ptr) free(ptr)
    #endif
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
    NK_UNUSED(unused);
    struct nk_gamepads* gamepads = (struct nk_gamepads*)NK_GAMEPAD_MALLOC(unused, NULL, sizeof(struct nk_gamepads));
    if (gamepads == NULL) {
        return NULL;
    }

    nk_zero(gamepads, sizeof(struct nk_gamepads));
    gamepads->ctx = ctx;
    gamepads->user_data = user_data;

    #ifdef NK_GAMEPAD_INIT
        if (NK_GAMEPAD_INIT(gamepads) == nk_false) {
            NK_GAMEPAD_MFREE(unused, gamepads);
            return NULL;
        }
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
    NK_UNUSED(unused);
    if (gamepads->gamepads != NULL) {
        NK_GAMEPAD_MFREE(unused, gamepads->gamepads);
    }

    NK_GAMEPAD_MFREE(unused, gamepads);
}

NK_API nk_bool nk_gamepad_init_gamepads(struct nk_gamepads* gamepads, int num) {
    if (gamepads == NULL || num <= 0) {
        return nk_false;
    }

    // Clean up if needed.
    nk_handle unused = {0};
    NK_UNUSED(unused);
    if (gamepads->gamepads != NULL) {
        NK_GAMEPAD_MFREE(unused, gamepads->gamepads);
        gamepads->gamepads = NULL;
        gamepads->gamepads_count = 0;
    }

    // Initialize the new gamepad
    gamepads->gamepads = (struct nk_gamepad*)NK_GAMEPAD_MALLOC(unused, NULL, num * sizeof(struct nk_gamepad));
    if (gamepads->gamepads == NULL) {
        return nk_false;
    }

    // Clear the memory
    nk_zero(gamepads->gamepads, num * sizeof(struct nk_gamepad));
    gamepads->gamepads_count = num;

    // Set up the initial name for the gamepads
    for (int i = 0; i < num; i++) {
        NK_MEMCPY((void*)gamepads->gamepads[i].name, "Controller ", 11);
        nk_itoa(gamepads->gamepads[i].name + 11, i + 1);
    }

    return nk_true;
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
    if (gamepads == NULL) {
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

NK_API void* nk_gamepad_user_data(struct nk_gamepads* gamepads) {
    if (gamepads == NULL) {
        return NULL;
    }

    return gamepads->user_data;
}

NK_API nk_bool nk_gamepad_any_button_pressed(struct nk_gamepads* gamepads, int num, int* out_num, enum nk_gamepad_button* out_button) {
    if (gamepads == NULL || num > gamepads->gamepads_count) {
        return nk_false;
    }

    if (num <= -1) {
        for (num = 0; num < gamepads->gamepads_count; num++) {
            for (int i = NK_GAMEPAD_BUTTON_FIRST; i < NK_GAMEPAD_BUTTON_LAST; i++) {
                if (nk_gamepad_is_button_pressed(gamepads, num, (enum nk_gamepad_button)i)) {
                    if (out_num != NULL) {
                        *out_num = num;
                    }
                    if (out_button != NULL) {
                        *out_button = (enum nk_gamepad_button)i;
                    }
                    return nk_true;
                }
            }
        }

        return nk_false;
    }

    for (int button = NK_GAMEPAD_BUTTON_FIRST; button < NK_GAMEPAD_BUTTON_LAST; button++) {
        if (nk_gamepad_is_button_pressed(gamepads, num, (enum nk_gamepad_button)button)) {
            if (out_num != NULL) {
                *out_num = num;
            }
            if (out_button != NULL) {
                *out_button = (enum nk_gamepad_button)button;
            }
            return nk_true;
        }
    }

    return nk_false;
}

#endif  // NK_GAMEPAD_IMPLEMENTATION_ONCE
#endif  // NK_GAMEPAD_IMPLEMENTATION
