#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NK_ASSERT(cond) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "Assertion failed: %s (%s:%d)\n", #cond, __FILE__, __LINE__); \
            exit(1); \
        } \
    } while (0)

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#include "../vendor/nuklear/nuklear.h"

/* Use a name prefix that is longer than NK_GAMEPAD_NAME_SIZE. */
#define NK_GAMEPAD_NAME_PREFIX "An Extremely Long Gamepad Controller Name Prefix "

#define NK_GAMEPAD_IMPLEMENTATION
#include "../nuklear_gamepad.h"

int main() {
    struct nk_context ctx;
    struct nk_gamepads gamepads;
    int i;
    const char* prefix = NK_GAMEPAD_NAME_PREFIX;
    NK_UNUSED(nk_inv_sqrt); /* Small fix for unused function */
    printf("nuklear_gamepad_test_prefix\n");
    printf("---------------------------\n");

    /* The prefix must be longer than the name buffer for this test. */
    NK_ASSERT((int)strlen(prefix) >= NK_GAMEPAD_NAME_SIZE);

    /* Initialize the Nuklear context */
    nk_init_default(&ctx, 0);

    /* Set up the gamepads. */
    printf("nk_gamepad_init()\n");
    NK_ASSERT(nk_gamepad_init(&gamepads, &ctx, NULL) == nk_true);

    /* Make sure the long prefix was truncated without overflowing the name. */
    printf("truncated controller names\n");
    for (i = 0; i < NK_GAMEPAD_MAX; i++) {
        char expected[NK_GAMEPAD_NAME_SIZE];
        char number[12];
        int number_length;
        int prefix_length;

        /* Build the expected name: truncated prefix, followed by the #. */
        sprintf(number, "%d", i + 1);
        number_length = (int)strlen(number);
        prefix_length = NK_GAMEPAD_NAME_SIZE - number_length - 1;
        memcpy(expected, prefix, (size_t)prefix_length);
        memcpy(expected + prefix_length, number, (size_t)number_length + 1);

        NK_ASSERT(gamepads.gamepads[i].name[NK_GAMEPAD_NAME_SIZE - 1] == '\0');
        NK_ASSERT((int)strlen(gamepads.gamepads[i].name) == NK_GAMEPAD_NAME_SIZE - 1);
        NK_ASSERT(strcmp(gamepads.gamepads[i].name, expected) == 0);
    }

    printf("nk_gamepad_free()\n");
    nk_gamepad_free(&gamepads);

    nk_free(&ctx);

    printf("---------------------------\n");
    printf("nuklear_gamepad_test_prefix: Tests passed!\n");

    return 0;
}
