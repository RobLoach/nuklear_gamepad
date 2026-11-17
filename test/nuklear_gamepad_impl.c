/*
 * Builds the nuklear_gamepad implementation in a separate translation unit
 * from nuklear's NK_IMPLEMENTATION, to ensure it does not depend on nuklear
 * internals.
 */
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include "../vendor/nuklear/nuklear.h"

#define NK_GAMEPAD_IMPLEMENTATION
#define NK_GAMEPAD_KEYBOARD
#define NK_GAMEPAD_MOUSE
#include "../nuklear_gamepad.h"
