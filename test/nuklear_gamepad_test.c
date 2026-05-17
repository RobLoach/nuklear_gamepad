#include <assert.h>
#include <stdio.h>
#include <string.h>

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#include "../vendor/nuklear/nuklear.h"

#define NK_GAMEPAD_IMPLEMENTATION
#define NK_GAMEPAD_KEYBOARD
#define NK_GAMEPAD_MOUSE
#include "../nuklear_gamepad.h"

int main() {
    struct nk_context ctx;
    struct nk_gamepads gamepads;
    NK_UNUSED(nk_inv_sqrt); /* Small fix for unused function */
    printf("nuklear_gamepad_test\n");
    printf("--------------------\n");

    /* NK_GAMEPAD_BUTTON is used with int flags, so we need it to be <32 */
    assert(NK_GAMEPAD_BUTTON_LAST < (32 - 1));

    /* Initialize the Nuklear context */
    nk_init_default(&ctx, 0);

    /* Set up the gamepads. */
    printf("nk_gamepad_init()\n");
    nk_gamepad_init(&gamepads, &ctx, NULL);

    printf("nk_gamepad_count()\n");
    assert(nk_gamepad_count(&gamepads) == NK_GAMEPAD_MAX);

    printf("nk_gamepad_is_available()\n");
    {
        assert(nk_gamepad_is_available(&gamepads, 0) == nk_true);
        assert(nk_gamepad_is_available(&gamepads, 1) == nk_false);
        assert(nk_gamepad_is_available(&gamepads, -1) == nk_true);
    }

    printf("nk_gamepad_set_available()\n");
    {
        nk_gamepad_set_available(&gamepads, 0, nk_true);
        assert(nk_gamepad_is_available(&gamepads, 0) == nk_true);
        assert(nk_gamepad_is_available(&gamepads, 1) == nk_false);
        nk_gamepad_set_available(&gamepads, -1, nk_true);
        assert(nk_gamepad_is_available(&gamepads, 0) == nk_true);
        assert(nk_gamepad_is_available(&gamepads, 1) == nk_true);
    }

    printf("nk_gamepad_name()\n");
    {
        const char* controller_name = nk_gamepad_name(&gamepads, 0);
        assert(controller_name != NULL);
        assert(strcmp(controller_name, "Keyboard") == 0);
    }

    {
        const char* controller_name = nk_gamepad_name(&gamepads, 1);
        assert(controller_name == NULL);
    }

    /* Update the state of the gamepads. */
    printf("nk_gamepad_update()\n");
    nk_gamepad_update(&gamepads);

    /* Make sure the buttons are not pushed. */
    assert(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_A) == nk_false);
    assert(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_B) == nk_false);

    /* Manually state that a button was pushed. */
    printf("nk_gamepad_button()\n");
    nk_gamepad_button(&gamepads, 0, NK_GAMEPAD_BUTTON_A, nk_true);
    assert(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_A) == nk_true);
    assert(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_B) == nk_false);
    assert(nk_gamepad_is_button_down(&gamepads, -1, NK_GAMEPAD_BUTTON_A) == nk_true);
    assert(nk_gamepad_is_button_down(&gamepads, -1, NK_GAMEPAD_BUTTON_B) == nk_false);

    /* nk_gamepad_any_button_pressed() */
    {
        int num = 9999;
        enum nk_gamepad_button button = NK_GAMEPAD_BUTTON_B;
        assert(nk_gamepad_any_button_pressed(&gamepads, -1, &num, &button) == nk_true);
        assert(num == 0);
        assert(button == NK_GAMEPAD_BUTTON_A);
    }

    /* Update the state so that the release checks can be processed */
    nk_gamepad_update(&gamepads);

    /* nk_gamepad_any_button_released() */
    {
        int num = 9999;
        enum nk_gamepad_button button = NK_GAMEPAD_BUTTON_B;
        assert(!nk_gamepad_is_button_down(&gamepads, -1, NK_GAMEPAD_BUTTON_A));
        assert(nk_gamepad_is_button_released(&gamepads, -1, NK_GAMEPAD_BUTTON_A));
        assert(!nk_gamepad_is_button_released(&gamepads, -1, NK_GAMEPAD_BUTTON_B));
        assert(nk_gamepad_any_button_released(&gamepads, -1, &num, &button) == nk_true);
        assert(num == 0);
        assert(button == NK_GAMEPAD_BUTTON_A);
    }

    /* nk_gamepad_button_name() */
    assert(strcmp(nk_gamepad_button_name(NULL, NK_GAMEPAD_BUTTON_B), "B") == 0);
    assert(strcmp(nk_gamepad_button_name(NULL, NK_GAMEPAD_BUTTON_START), "Start") == 0);
    assert(strcmp(nk_gamepad_button_name(&gamepads, NK_GAMEPAD_BUTTON_UP), "Up") == 0);
    assert(strcmp(nk_gamepad_button_name(&gamepads, NK_GAMEPAD_BUTTON_A), "Z") == 0);

    /* nk_gamepad_get_axis_deadzone() */
    printf("nk_gamepad_get_axis_deadzone()\n");
    {
        float pos;
        float neg;
        nk_gamepad_set_available(&gamepads, 0, nk_true);

        /* Inside deadzone returns 0 */
        nk_gamepad_axis(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X, 0.1f);
        assert(nk_gamepad_get_axis_deadzone(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X, 0.15f) == 0.0f);

        /* Deadzone applies symmetrically */
        nk_gamepad_axis(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X, 0.5f);
        pos = nk_gamepad_get_axis_deadzone(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X, 0.15f);
        nk_gamepad_axis(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X, -0.5f);
        neg = nk_gamepad_get_axis_deadzone(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X, 0.15f);
        assert(pos > 0.0f);
        assert(neg < 0.0f);
        assert((pos + neg) < 0.001f && (pos + neg) > -0.001f);

        /* Saturates at 1.0 */
        nk_gamepad_axis(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X, 1.0f);
        pos = nk_gamepad_get_axis_deadzone(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X, 0.15f);
        assert(pos <= 1.0f);
        assert(pos > 0.99f);

        nk_gamepad_axis(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X, -1.0f);
        neg = nk_gamepad_get_axis_deadzone(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X, 0.15f);
        assert(neg >= -1.0f);
        assert(neg < -0.99f);
    }

    /* nk_gamepad_get_stick() */
    printf("nk_gamepad_get_stick()\n");
    {
        float x;
        float y;
        nk_gamepad_set_available(&gamepads, 0, nk_true);

        /* Inside deadzone returns (0, 0) */
        nk_gamepad_axis(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X, 0.1f);
        nk_gamepad_axis(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_Y, 0.1f);
        assert(nk_gamepad_get_stick(&gamepads, 0, 0, 0.15f, &x, &y) == nk_true);
        assert(x == 0.0f);
        assert(y == 0.0f);

        /* Full deflection saturates at 1.0 */
        nk_gamepad_axis(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X, 1.0f);
        nk_gamepad_axis(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_Y, 0.0f);
        assert(nk_gamepad_get_stick(&gamepads, 0, 0, 0.15f, &x, &y) == nk_true);
        assert(x > 0.99f && x <= 1.0f);
        assert(y == 0.0f);

        /* Symmetry: (-1, 0) gives (-1, 0) */
        nk_gamepad_axis(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X, -1.0f);
        nk_gamepad_axis(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_Y, 0.0f);
        assert(nk_gamepad_get_stick(&gamepads, 0, 0, 0.15f, &x, &y) == nk_true);
        assert(x < -0.99f && x >= -1.0f);
        assert(y == 0.0f);

        /* NULL gamepads returns false */
        assert(nk_gamepad_get_stick(NULL, 0, 0, 0.15f, &x, &y) == nk_false);
    }

    printf("nk_gamepad_free()\n");
    nk_gamepad_free(&gamepads);

    nk_free(&ctx);

    printf("--------------------\n");
    printf("nuklear_gamepad_test: Tests passed!\n");

    return 0;
}
