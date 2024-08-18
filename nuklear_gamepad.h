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

#ifndef NK_GAMEPAD_MAX
/**
 * The maximum amount of gamepads that can be supported.
 */
#define NK_GAMEPAD_MAX 4
#endif  // NK_GAMEPAD_MAX

#ifndef NK_GAMEPAD_NAME_PREFIX
/**
 * The prefix to use for the default gamepad names.
 */
#define NK_GAMEPAD_NAME_PREFIX "Controller "
#endif  // NK_GAMEPAD_NAME_PREFIX

#ifndef NK_GAMEPAD_NAME_SIZE
/**
 * The maximum string size of gamepad names.
 */
#define NK_GAMEPAD_NAME_SIZE 16
#endif  // NK_GAMEPAD_NAME_SIZE

/**
 * Create a flag for the specified button.
 * @internal
 */
#define NK_GAMEPAD_BUTTON_FLAG(button) (1 << (button))

enum nk_gamepad_input_source_type {
    NK_GAMEPAD_INPUT_SOURCE_UNKNOWN = 0,
    NK_GAMEPAD_INPUT_SOURCE_NONE, // A gamepad input source which provides no callbacks. @see nk_gamepad_none_input_source()
    NK_GAMEPAD_INPUT_SOURCE_SDL, // A gamepad input source which uses SDL to retrieve its input. @see nk_gamepad_sdl_input_source()
    NK_GAMEPAD_INPUT_SOURCE_GLFW, // A gamepad input source which uses GLFW to retrieve its input. @see nk_gamepad_glfw_input_source()
    NK_GAMEPAD_INPUT_SOURCE_RAYLIB, // A gamepad input source which uses raylib to retrieve its input. @see nk_gamepad_raylib_input_source()
    NK_GAMEPAD_INPUT_SOURCE_PNTR, // A gamepad input source which uses pntr to retrieve its input. @see nk_gamepad_pntr_input_source()
    NK_GAMEPAD_INPUT_SOURCE_KEYBOARD, // A gamepad input source which uses nuklear's keyboard interface to retrieve its input. @see nuklear_gamepad_keyboard_input_source()
};

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
    NK_GAMEPAD_BUTTON_GUIDE,
    NK_GAMEPAD_BUTTON_LAST
};

struct nk_gamepads;

typedef nk_bool (*nk_gamepad_init_fn)(struct nk_gamepads *gamepads, void* user_data);
typedef void (*nk_gamepad_update_fn)(struct nk_gamepads *gamepads, void* user_data);
typedef void (*nk_gamepad_free_fn)(struct nk_gamepads *gamepads, void* user_data);
typedef const char *(*nk_gamepad_name_fn)(struct nk_gamepads *gamepads, int num, void* user_data);

/**
 * A function to retrieve an input source definition.
 *
 * @param user_data Any user data to pass through to the input source.
 * @return The input source.
 *
 * @see nuklear_gamepad_none_input_source()
 */
typedef struct nk_gamepad_input_source (*nk_gamepad_input_source_fn)(void* user_data);

struct nk_gamepad_input_source {
    void* user_data;
    nk_gamepad_init_fn init;
    nk_gamepad_update_fn update;
    nk_gamepad_free_fn free;
    nk_gamepad_name_fn name; // Callback to get the name of a plugged in controller.
    const char* input_source_name; // The human-readable name of the input source.
    enum nk_gamepad_input_source_type id; // A unique identifier of the input source.
};

struct nk_gamepad {
    nk_bool available;
    unsigned int buttons;
    unsigned int buttons_prev;
    char name[NK_GAMEPAD_NAME_SIZE];
    void* data;
};

struct nk_gamepads {
    struct nk_gamepad gamepads[NK_GAMEPAD_MAX];
    struct nk_context* ctx;
    struct nk_gamepad_input_source input_source;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize a Nuklear Gamepad system with a default input source.
 *
 * @param gamepads The gamepad system to initialize.
 * @param ctx The Nuklear context.
 * @param user_data Any user data to pass through to the default input source.
 * @return True if the gamepads were initialized properly, false otherwise.
 *
 * @see NK_GAMEPAD_DEFAULT_INPUT_SOURCE
 */
NK_API nk_bool nk_gamepad_init(struct nk_gamepads* gamepads, struct nk_context* ctx, void* user_data);

/**
 * Initialize a Nuklear Gamepad system with a custom input source.
 *
 * @param gamepads The gamepad system to initialize.
 * @param ctx The Nuklear context.
 * @param input_source The input source to read gamepad state from.
 *
 * @return True if the gamepads were initialized properly, false otherwise.
 */
NK_API nk_bool nk_gamepad_init_with_source(struct nk_gamepads* gamepads, struct nk_context* ctx, struct nk_gamepad_input_source input_source);

/**
 * Disconnect all the gamepads and frees associated memory.
 *
 * @param gamepads The gamepad system to free up.
 * @see nk_gamepad_init()
 */
NK_API void nk_gamepad_free(struct nk_gamepads* gamepads);

/**
 * Updates the state of all the gamepads.
 *
 * @param gamepads The gamepad system to update.
 */
NK_API void nk_gamepad_update(struct nk_gamepads* gamepads);

/**
 * Check whether or not a gamepad is available.
 *
 * @param gamepads The gamepad system.
 * @param num Which gamepad to check is available or not. -1 will check if any gamepad is available.
 *
 * @return True if the gamepads is available, false otherwise.
 */
NK_API nk_bool nk_gamepad_is_available(struct nk_gamepads* gamepads, int num);

/**
 * Set whether or not the given gamepad number is available.
 *
 * @param gamepads The gamepad system.
 * @param num Which gamepad to set as available or not. -1 will set all gamepads.
 * @param available True or false depending on whether or not the gamepad is available.
 */
NK_API void nk_gamepad_set_available(struct nk_gamepads* gamepads, int num, nk_bool available);

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

/**
 * Invoke a button press or release event for the specified gamepad.
 *
 * @param gamepads The associated gamepad system.
 * @param num The gamepad number.
 * @param button The button to invoke.
 * @param down True to indicate a button press, false to indicate a button release.
 */
NK_API void nk_gamepad_button(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button, nk_bool down);

/**
 * Returns the amount of gamepads that could become available.
 *
 * @param gamepads The associated gamepad system.
 *
 * @return The amount of gamepads.
 *
 * @see NK_GAMEPAD_MAX
 */
NK_API int nk_gamepad_count(struct nk_gamepads* gamepads);

/**
 * Get the name of the specified gamepad.
 *
 * @param gamepads The associated gamepad system.
 * @param num The gamepad number.
 *
 * @return The name of the gamepad.
 *
 * @see NK_GAMEPAD_NAME
 */
NK_API const char* nk_gamepad_name(struct nk_gamepads* gamepads, int num);

/**
 * Get the input source of the specified gamepad.
 *
 * @param gamepads The associated gamepad system.
 *
 * @return A pointer to the specified gamepad's input source.
 */
NK_API struct nk_gamepad_input_source* nk_gamepad_input_source(struct nk_gamepads* gamepads);

/**
 * Set the input source of the gamepad system.
 *
 * @param gamepads The associated gamepad system.
 * @param input_source The input source to set.
 * @return True if the input source was set, false otherwise.
 */
NK_API nk_bool nk_gamepad_set_input_source(struct nk_gamepads* gamepads, struct nk_gamepad_input_source input_source);

/**
 * An null-terminated array of the available compiled gamepad input sources.
 */
NK_API nk_gamepad_input_source_fn nk_gamepad_input_sources[];

#ifdef __cplusplus
}
#endif

#endif

#if defined(NK_GAMEPAD_IMPLEMENTATION) && !defined(NK_GAMEPAD_HEADER_ONLY)
#ifndef NK_GAMEPAD_IMPLEMENTATION_ONCE
#define NK_GAMEPAD_IMPLEMENTATION_ONCE

// Platform detection.
#if !defined(NK_GAMEPAD_SDL) && !defined(NK_GAMEPAD_GLFW) && !defined(NK_GAMEPAD_RAYLIB) && !defined(NK_GAMEPAD_PNTR) && !defined(NK_GAMEPAD_KEYBOARD) && !defined(NK_GAMEPAD_NONE)
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

// Include all the enabled platform-specific implementations.
#ifdef NK_GAMEPAD_SDL
#include "nuklear_gamepad_sdl.h"
#endif
#ifdef NK_GAMEPAD_GLFW
#include "nuklear_gamepad_glfw.h"
#endif
#ifdef NK_GAMEPAD_RAYLIB
#include "nuklear_gamepad_raylib.h"
#endif
#ifdef NK_GAMEPAD_PNTR
#include "nuklear_gamepad_pntr.h"
#endif
#ifdef NK_GAMEPAD_KEYBOARD
#include "nuklear_gamepad_keyboard.h"
#endif

// None Gamepad Source
#include "nuklear_gamepad_none.h"

nk_gamepad_input_source_fn nk_gamepad_input_sources[] = {
#ifdef NK_GAMEPAD_SDL
    &nk_gamepad_sdl_input_source,
#endif
#ifdef NK_GAMEPAD_GLFW
    &nk_gamepad_glfw_input_source,
#endif
#ifdef NK_GAMEPAD_RAYLIB
    &nk_gamepad_raylib_input_source,
#endif
#ifdef NK_GAMEPAD_PNTR
    &nk_gamepad_pntr_input_source,
#endif
#ifdef NK_GAMEPAD_KEYBOARD
    &nk_gamepad_keyboard_input_source,
#endif

    // Dummy Gamepad Source
    &nk_gamepad_none_input_source,

    // Null terminator.
    NULL,
};

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NK_GAMEPAD_DEFAULT_INPUT_SOURCE
/**
 * The default input source to use for the gamepad when using nk_gamepad_init().
 *
 * When not explicitly set, will be the first available input source.
 *
 * @see nk_gamepad_init()
 * @see nk_gamepad_input_sources
 */
#define NK_GAMEPAD_DEFAULT_INPUT_SOURCE nk_gamepad_input_sources[0]
#endif

NK_API nk_bool nk_gamepad_init(struct nk_gamepads* gamepads, struct nk_context* ctx, void* user_data) {
    return nk_gamepad_init_with_source(gamepads, ctx, NK_GAMEPAD_DEFAULT_INPUT_SOURCE(user_data));
}

NK_API nk_bool nk_gamepad_init_with_source(struct nk_gamepads* gamepads, struct nk_context* ctx, struct nk_gamepad_input_source input_source) {
    if (gamepads == NULL) {
        return nk_false;
    }

    // Initialize the gamepads as a default state.
    nk_zero(gamepads, sizeof(struct nk_gamepads));
    gamepads->ctx = ctx;
    gamepads->input_source = input_source;

    // Set the default state for all gamepads.
    for (int i = 0; i < NK_GAMEPAD_MAX; i++) {
        // Name
        int j;
        const char* name_prefix = NK_GAMEPAD_NAME_PREFIX;
        for (j = 0; j < nk_strlen(name_prefix); j++) {
            gamepads->gamepads[i].name[j] = name_prefix[j];
        }

        // Add the # of the controller to the name.
        nk_itoa(&gamepads->gamepads[i].name[j], (long)(i + 1));
    }

    // Run the init() callback, or update() if it's not available.
    if (input_source.init != NULL) {
        if (input_source.init(gamepads, input_source.user_data) == nk_false) {
            nk_zero(gamepads, sizeof(struct nk_gamepads));
            return nk_false;
        }
    }
    else if (gamepads->input_source.update != NULL) {
        gamepads->input_source.update(gamepads, gamepads->input_source.user_data);
    }

    // Set all the states as the same as their previous states so that they don't trigger any events.
    for (int i = 0; i < NK_GAMEPAD_MAX; i++) {
        gamepads->gamepads[i].buttons_prev = gamepads->gamepads[i].buttons;
    }

    return nk_true;
}

NK_API void nk_gamepad_free(struct nk_gamepads* gamepads) {
    if (gamepads == NULL) {
        return;
    }

    // Tell the runner that we are freeing the gamepads.
    if (gamepads->input_source.free != NULL) {
        gamepads->input_source.free(gamepads, gamepads->input_source.user_data);
    }

    // Reset the default state of the gamepad data.
    nk_zero(gamepads, sizeof(struct nk_gamepads));
}

NK_API void nk_gamepad_button(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button, nk_bool down) {
    if (gamepads == NULL || num < 0 || num >= NK_GAMEPAD_MAX || gamepads->gamepads[num].available == nk_false) {
        return;
    }

    if (down) {
        gamepads->gamepads[num].buttons |= NK_GAMEPAD_BUTTON_FLAG(button);
    }
    else {
        gamepads->gamepads[num].buttons &= (unsigned int)~NK_GAMEPAD_BUTTON_FLAG(button);
    }
}

NK_API void nk_gamepad_update(struct nk_gamepads* gamepads) {
    if (gamepads == NULL) {
        return;
    }

    for (int i = 0; i < NK_GAMEPAD_MAX; i++) {
        if (gamepads->gamepads[i].available == nk_false) {
            continue;
        }
        gamepads->gamepads[i].buttons_prev = gamepads->gamepads[i].buttons;
        gamepads->gamepads[i].buttons = 0;
    }

    if (gamepads->input_source.update != NULL) {
        gamepads->input_source.update(gamepads, gamepads->input_source.user_data);
    }
}

NK_API nk_bool nk_gamepad_is_button_down(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button) {
    if (gamepads == NULL) {
        return nk_false;
    }

    if (num < 0) {
        for (int i = 0; i < NK_GAMEPAD_MAX; i++) {
            if (gamepads->gamepads[i].available == nk_false) {
                continue;
            }

            if ((gamepads->gamepads[i].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) != 0) {
                return nk_true;
            }
        }
        return nk_false;
    }

    if (num >= NK_GAMEPAD_MAX || gamepads->gamepads[num].available == nk_false) {
        return nk_false;
    }

    return (gamepads->gamepads[num].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) != 0;
}

NK_API nk_bool nk_gamepad_is_button_pressed(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button) {
    if (gamepads == NULL) {
        return nk_false;
    }

    if (num < 0) {
        for (int i = 0; i < NK_GAMEPAD_MAX; i++) {
            if (gamepads->gamepads[i].available == nk_false) {
                continue;
            }

            if ((gamepads->gamepads[i].buttons_prev & NK_GAMEPAD_BUTTON_FLAG(button)) == 0 &&
			    (gamepads->gamepads[i].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) != 0) {
                return nk_true;
            }
        }
        return nk_false;
    }

    if (num >= NK_GAMEPAD_MAX || gamepads->gamepads[num].available == nk_false) {
        return nk_false;
    }

    return ((gamepads->gamepads[num].buttons_prev & NK_GAMEPAD_BUTTON_FLAG(button)) == 0 &&
			(gamepads->gamepads[num].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) != 0);
}

NK_API nk_bool nk_gamepad_is_button_released(struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button) {
    if (gamepads == NULL) {
        return nk_false;
    }

    if (num < 0) {
        for (int i = 0; i < NK_GAMEPAD_MAX; i++) {
            if (gamepads->gamepads[i].available == nk_false) {
                continue;
            }

            if ((gamepads->gamepads[i].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) == 0 &&
			    (gamepads->gamepads[i].buttons_prev & NK_GAMEPAD_BUTTON_FLAG(button)) != 0) {
                return nk_true;
            }
        }
        return nk_false;
    }

    if (num >= NK_GAMEPAD_MAX || gamepads->gamepads[num].available == nk_false) {
        return nk_false;
    }

    return ((gamepads->gamepads[num].buttons & NK_GAMEPAD_BUTTON_FLAG(button)) == 0 &&
			(gamepads->gamepads[num].buttons_prev & NK_GAMEPAD_BUTTON_FLAG(button)) != 0);
}

NK_API int nk_gamepad_count(struct nk_gamepads* gamepads) {
    if (gamepads == NULL) {
        return 0;
    }

    return NK_GAMEPAD_MAX;
}

NK_API void nk_gamepad_set_available(struct nk_gamepads* gamepads, int num, nk_bool available) {
    if (gamepads == NULL) {
        return;
    }

    if (num < 0) {
        for (int i = 0; i < NK_GAMEPAD_MAX; i++) {
            gamepads->gamepads[i].available = available;
        }
    } else if (num < NK_GAMEPAD_MAX) {
        gamepads->gamepads[num].available = available;
    }
}

NK_API const char* nk_gamepad_name(struct nk_gamepads* gamepads, int num) {
    if (!gamepads || num < 0 || num >= NK_GAMEPAD_MAX || gamepads->gamepads[num].available == nk_false) {
        return NULL;
    }

    if (gamepads->input_source.name) {
        return gamepads->input_source.name(gamepads, num, gamepads->input_source.user_data);
    } else {
        return gamepads->gamepads[num].name;
    }
}

NK_API struct nk_gamepad_input_source* nk_gamepad_input_source(struct nk_gamepads* gamepads) {
    if (gamepads == NULL) {
        return NULL;
    }

    return &gamepads->input_source;
}

NK_API nk_bool nk_gamepad_any_button_pressed(struct nk_gamepads* gamepads, int num, int* out_num, enum nk_gamepad_button* out_button) {
    if (gamepads == NULL || num >= NK_GAMEPAD_MAX) {
        return nk_false;
    }

    if (num < 0) {
        for (num = 0; num < NK_GAMEPAD_MAX; num++) {
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

    if (gamepads->gamepads[num].available == nk_false) {
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

NK_API nk_bool nk_gamepad_is_available(struct nk_gamepads* gamepads, int num) {
    if (gamepads == NULL) {
        return nk_false;
    }

    if (num < 0) {
        for (int i = 0; i < NK_GAMEPAD_MAX; i++) {
            if (gamepads->gamepads[i].available) {
                return nk_true;
            }
        }
        return nk_false;
    }

    if (num >= NK_GAMEPAD_MAX) {
        return nk_false;
    }

    return gamepads->gamepads[num].available;
}

NK_API nk_bool nk_gamepad_set_input_source(struct nk_gamepads* gamepads, struct nk_gamepad_input_source input_source) {
    if (gamepads == NULL) {
        return nk_false;
    }

    // Initialize the new gamepad system.
    struct nk_gamepads new_gamepads;
    if (nk_gamepad_init_with_source(&new_gamepads, gamepads->ctx, input_source) == nk_false) {
        return nk_false;
    }

    // Since it was successful, free the old gamepad system.
    nk_gamepad_free(gamepads);

    // Copy the new gamepad system over the old one.
    NK_MEMCPY(gamepads, &new_gamepads, sizeof(struct nk_gamepads));

    return nk_true;
}

#ifdef __cplusplus
}
#endif

#endif  // NK_GAMEPAD_IMPLEMENTATION_ONCE
#endif  // NK_GAMEPAD_IMPLEMENTATION
