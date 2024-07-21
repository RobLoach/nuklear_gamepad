#include <assert.h>
#include <stdio.h>
#include <string.h>

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#include "../vendor/nuklear/nuklear.h"

#define NK_GAMEPAD_IMPLEMENTATION
#include "../nuklear_gamepad.h"

int main() {
    printf("nuklear_gamepad_test\n");
    printf("--------------------\n");

    // Initialize the Nuklear context
    struct nk_context ctx;
    nk_init_default(&ctx, 0);

    // Set up the gamepads.
    printf("nk_gamepad_init()\n");
    struct nk_gamepads gamepads;
    nk_gamepad_init(&gamepads, &ctx, NULL);

    printf("nk_gamepad_count()\n");
    assert(nk_gamepad_count(&gamepads) == NK_GAMEPAD_MAX);

    printf("nk_gamepad_is_available()\n");
    {
        assert(nk_gamepad_is_available(&gamepads, 0) == nk_true);
        assert(nk_gamepad_is_available(&gamepads, 1) == nk_true);
        assert(nk_gamepad_is_available(&gamepads, -1) == nk_true);
    }

    printf("nk_gamepad_name()\n");
    {
        const char* controller_name = nk_gamepad_name(&gamepads, 0);
        assert(controller_name != NULL);
        assert(strcmp(controller_name, "Controller 1") == 0);
    }

    {
        const char* controller_name = nk_gamepad_name(&gamepads, 1);
        assert(controller_name != NULL);
        assert(strcmp(controller_name, "Controller 2") == 0);
    }

    // Update the state of the gamepads.
    printf("nk_gamepad_update()\n");
    nk_gamepad_update(&gamepads);

    // Make sure the buttons are not pushed.
    assert(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_A) == nk_false);
    assert(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_B) == nk_false);

    // Manually state that a button was pushed.
    printf("nk_gamepad_button()\n");
    nk_gamepad_button(&gamepads, 0, NK_GAMEPAD_BUTTON_A, nk_true);
    assert(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_A) == nk_true);
    assert(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_B) == nk_false);

    // nk_gamepad_any_button_pressed()
    int num = 9999;
    enum nk_gamepad_button button = NK_GAMEPAD_BUTTON_B;
    assert(nk_gamepad_any_button_pressed(&gamepads, -1, &num, &button) == nk_true);
    assert(num == 0);
    assert(button == NK_GAMEPAD_BUTTON_A);

    printf("nk_gamepad_free()\n");
    nk_gamepad_free(&gamepads);

    nk_free(&ctx);

    printf("--------------------\n");
    printf("nuklear_gamepad_test: Tests passed!\n");

    return 0;
}
