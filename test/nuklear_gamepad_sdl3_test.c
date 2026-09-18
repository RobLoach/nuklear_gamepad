#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The SDL3 backend expects the consumer to have included SDL3 first, matching
 * the include contract of the SDL2, GLFW and raylib backends. Here that
 * resolves to test/stubs/SDL3/SDL.h, a headless test double. */
#include <SDL3/SDL.h>

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

/*
 * The fake SDL3 device table.
 *
 * This implements the prototypes declared by the SDL3 stub header, so the SDL3
 * backend can be exercised without any real SDL3 library or hardware.
 */

#define NK_GAMEPAD_TEST_FAKE_MAX 8
#define NK_GAMEPAD_TEST_FAKE_NAME_MAX 64

struct nk_gamepad_test_fake {
    SDL_JoystickID id;
    int present;
    int connected;
    int open;
    int has_name;
    char name[NK_GAMEPAD_TEST_FAKE_NAME_MAX];
    int buttons[SDL_GAMEPAD_BUTTON_COUNT];
    Sint16 axes[SDL_GAMEPAD_AXIS_COUNT];
    SDL_GamepadButtonLabel labels[SDL_GAMEPAD_BUTTON_COUNT];
};

static struct nk_gamepad_test_fake fake_devices[NK_GAMEPAD_TEST_FAKE_MAX];
static int fake_device_count = 0;
static int fake_open_count = 0;
static int fake_close_count = 0;
static int fake_init_refcount = 0;
static int fake_update_count = 0;
static int fake_init_should_fail = 0;

static void fake_reset(void) {
    memset(fake_devices, 0, sizeof(fake_devices));
    fake_device_count = 0;
    fake_open_count = 0;
    fake_close_count = 0;
    fake_init_refcount = 0;
    fake_update_count = 0;
    fake_init_should_fail = 0;
}

static int fake_add_device(SDL_JoystickID id, const char* name) {
    int index = fake_device_count;
    NK_ASSERT(index < NK_GAMEPAD_TEST_FAKE_MAX);
    fake_devices[index].id = id;
    fake_devices[index].present = 1;
    fake_devices[index].connected = 1;
    if (name != NULL) {
        NK_ASSERT(strlen(name) < (size_t)NK_GAMEPAD_TEST_FAKE_NAME_MAX);
        strcpy(fake_devices[index].name, name);
        fake_devices[index].has_name = 1;
    }
    fake_device_count++;
    return index;
}

static struct nk_gamepad_test_fake* fake_find(SDL_JoystickID id) {
    int i;
    for (i = 0; i < fake_device_count; i++) {
        if (fake_devices[i].present && fake_devices[i].id == id) {
            return &fake_devices[i];
        }
    }
    return NULL;
}

int SDL_InitSubSystem(SDL_InitFlags flags) {
    NK_UNUSED(flags);
    if (fake_init_should_fail) {
        return 0;
    }
    fake_init_refcount++;
    return 1;
}

void SDL_QuitSubSystem(SDL_InitFlags flags) {
    NK_UNUSED(flags);
    if (fake_init_refcount > 0) {
        fake_init_refcount--;
    }
}

int SDL_IsGamepad(SDL_JoystickID instance_id) {
    return fake_find(instance_id) != NULL;
}

SDL_Gamepad* SDL_OpenGamepad(SDL_JoystickID instance_id) {
    struct nk_gamepad_test_fake* device = fake_find(instance_id);
    if (device == NULL) {
        return NULL;
    }
    device->open = 1;
    fake_open_count++;
    return (SDL_Gamepad*)device;
}

void SDL_CloseGamepad(SDL_Gamepad* gamepad) {
    struct nk_gamepad_test_fake* device = (struct nk_gamepad_test_fake*)gamepad;
    if (device == NULL) {
        return;
    }
    device->open = 0;
    fake_close_count++;

    /* SDL owns the name string and only keeps it alive while the gamepad is
     * open. Scribble over it so that any backend holding on to SDL's pointer
     * is caught rather than silently working. */
    memset(device->name, '?', sizeof(device->name) - 1);
    device->name[sizeof(device->name) - 1] = '\0';
}

SDL_JoystickID* SDL_GetGamepads(int* count) {
    SDL_JoystickID* out;
    int i;

    out = (SDL_JoystickID*)malloc(sizeof(SDL_JoystickID) * (size_t)(fake_device_count + 1));
    if (out == NULL) {
        if (count != NULL) {
            *count = 0;
        }
        return NULL;
    }

    for (i = 0; i < fake_device_count; i++) {
        out[i] = fake_devices[i].id;
    }
    out[fake_device_count] = 0;

    if (count != NULL) {
        *count = fake_device_count;
    }
    return out;
}

SDL_JoystickID SDL_GetGamepadID(SDL_Gamepad* gamepad) {
    struct nk_gamepad_test_fake* device = (struct nk_gamepad_test_fake*)gamepad;
    if (device == NULL) {
        return 0;
    }
    return device->id;
}

int SDL_GamepadConnected(SDL_Gamepad* gamepad) {
    struct nk_gamepad_test_fake* device = (struct nk_gamepad_test_fake*)gamepad;
    if (device == NULL) {
        return 0;
    }
    return device->connected;
}

int SDL_GetGamepadButton(SDL_Gamepad* gamepad, SDL_GamepadButton button) {
    struct nk_gamepad_test_fake* device = (struct nk_gamepad_test_fake*)gamepad;
    if (device == NULL || button < 0 || button >= SDL_GAMEPAD_BUTTON_COUNT) {
        return 0;
    }
    return device->buttons[button];
}

Sint16 SDL_GetGamepadAxis(SDL_Gamepad* gamepad, SDL_GamepadAxis axis) {
    struct nk_gamepad_test_fake* device = (struct nk_gamepad_test_fake*)gamepad;
    if (device == NULL || axis < 0 || axis >= SDL_GAMEPAD_AXIS_COUNT) {
        return 0;
    }
    return device->axes[axis];
}

const char* SDL_GetGamepadName(SDL_Gamepad* gamepad) {
    struct nk_gamepad_test_fake* device = (struct nk_gamepad_test_fake*)gamepad;
    if (device == NULL || device->open == 0 || device->has_name == 0) {
        /* SDL only keeps the name alive while the gamepad is open. */
        return NULL;
    }
    return device->name;
}

SDL_GamepadButtonLabel SDL_GetGamepadButtonLabel(SDL_Gamepad* gamepad, SDL_GamepadButton button) {
    struct nk_gamepad_test_fake* device = (struct nk_gamepad_test_fake*)gamepad;
    if (device == NULL || button < 0 || button >= SDL_GAMEPAD_BUTTON_COUNT) {
        return SDL_GAMEPAD_BUTTON_LABEL_UNKNOWN;
    }
    return device->labels[button];
}

void SDL_UpdateGamepads(void) {
    fake_update_count++;
}

void SDL_free(void* mem) {
    free(mem);
}

#define NK_GAMEPAD_IMPLEMENTATION
#define NK_GAMEPAD_SDL3
#include "../nuklear_gamepad.h"

int main() {
    struct nk_context ctx;
    NK_UNUSED(nk_inv_sqrt); /* Small fix for unused function */
    printf("nuklear_gamepad_sdl3_test\n");
    printf("-------------------------\n");

    nk_init_default(&ctx, 0);

    printf("nk_gamepad_sdl3_input_source()\n");
    {
        int user_data = 42;
        struct nk_gamepad_input_source source = nk_gamepad_sdl3_input_source(&user_data);
        NK_ASSERT(source.id == NK_GAMEPAD_INPUT_SOURCE_SDL3);
        NK_ASSERT(source.input_source_name != NULL);
        NK_ASSERT(strcmp(source.input_source_name, "SDL3") == 0);
        NK_ASSERT(source.init != NULL);
        NK_ASSERT(source.update != NULL);
        NK_ASSERT(source.free != NULL);
        NK_ASSERT(source.name != NULL);
        NK_ASSERT(source.button_name != NULL);
        NK_ASSERT(source.axis_name == NULL);
        NK_ASSERT(source.user_data == (void*)&user_data);
    }

    printf("nk_gamepad_init_with_source()\n");
    {
        struct nk_gamepads gamepads;
        const char* saved_name;
        SDL_Event event;

        fake_reset();
        fake_add_device(1, "Fake Pad One");
        fake_add_device(2, "Fake Pad Two");

        NK_ASSERT(nk_gamepad_init_with_source(&gamepads, &ctx, nk_gamepad_sdl3_input_source(NULL)) == nk_true);
        NK_ASSERT(fake_init_refcount == 1);
        NK_ASSERT(fake_open_count == 2);
        NK_ASSERT(nk_gamepad_is_available(&gamepads, 0) == nk_true);
        NK_ASSERT(nk_gamepad_is_available(&gamepads, 1) == nk_true);
        NK_ASSERT(nk_gamepad_is_available(&gamepads, 2) == nk_false);
        NK_ASSERT(strcmp(nk_gamepad_name(&gamepads, 0), "Fake Pad One") == 0);
        NK_ASSERT(strcmp(nk_gamepad_name(&gamepads, 1), "Fake Pad Two") == 0);

        printf("nk_gamepad_sdl3_name() guards\n");
        NK_ASSERT(nk_gamepad_sdl3_name(NULL, 0, NULL) == NULL);
        NK_ASSERT(nk_gamepad_sdl3_name(&gamepads, -1, NULL) == NULL);
        NK_ASSERT(nk_gamepad_sdl3_name(&gamepads, NK_GAMEPAD_MAX, NULL) == NULL);
        NK_ASSERT(nk_gamepad_sdl3_name(&gamepads, 2, NULL) == NULL);

        /* The name is a durable copy, so it outlives the SDL gamepad handle. */
        printf("gamepad name survives a disconnect\n");
        saved_name = nk_gamepad_name(&gamepads, 0);
        NK_ASSERT(saved_name != NULL);
        event.type = SDL_EVENT_GAMEPAD_REMOVED;
        event.gdevice.which = 1;
        nk_gamepad_sdl3_handle_event(&gamepads, &event);
        NK_ASSERT(fake_devices[0].open == 0);
        NK_ASSERT(SDL_GetGamepadName((SDL_Gamepad*)&fake_devices[0]) == NULL);
        NK_ASSERT(strcmp(saved_name, "Fake Pad One") == 0);

        printf("nk_gamepad_free()\n");
        nk_gamepad_free(&gamepads);
        NK_ASSERT(fake_close_count == 2);
        NK_ASSERT(fake_init_refcount == 0);
    }

    printf("gamepad name fallback and truncation\n");
    {
        struct nk_gamepads gamepads;
        const char* long_name = "A Very Long Gamepad Name That Exceeds The Buffer";
        const char* name;

        fake_reset();
        fake_add_device(1, NULL);
        fake_add_device(2, "");
        fake_add_device(3, long_name);

        NK_ASSERT(nk_gamepad_init_with_source(&gamepads, &ctx, nk_gamepad_sdl3_input_source(NULL)) == nk_true);

        /* SDL reported no name, so the default "Controller N" name is kept. */
        NK_ASSERT(nk_gamepad_name(&gamepads, 0) != NULL);
        NK_ASSERT(strcmp(nk_gamepad_name(&gamepads, 0), "Controller 1") == 0);
        NK_ASSERT(nk_gamepad_name(&gamepads, 1) != NULL);
        NK_ASSERT(strcmp(nk_gamepad_name(&gamepads, 1), "Controller 2") == 0);

        name = nk_gamepad_name(&gamepads, 2);
        NK_ASSERT(name != NULL);
        NK_ASSERT(strlen(name) == (size_t)(NK_GAMEPAD_NAME_SIZE - 1));
        NK_ASSERT(strncmp(name, long_name, (size_t)(NK_GAMEPAD_NAME_SIZE - 1)) == 0);
        NK_ASSERT(name[NK_GAMEPAD_NAME_SIZE - 1] == '\0');

        nk_gamepad_free(&gamepads);
    }

    printf("button mapping\n");
    {
        struct nk_gamepads gamepads;
        int updates;
        int b;

        fake_reset();
        fake_add_device(1, "Button Pad");
        NK_ASSERT(nk_gamepad_init_with_source(&gamepads, &ctx, nk_gamepad_sdl3_input_source(NULL)) == nk_true);

        for (b = 0; b < SDL_GAMEPAD_BUTTON_COUNT; b++) {
            fake_devices[0].buttons[b] = 1;
        }

        updates = fake_update_count;
        nk_gamepad_update(&gamepads);

        /* nk_gamepad_sdl3_update() must pump SDL's gamepad state itself. */
        NK_ASSERT(fake_update_count == updates + 1);

        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_A) == nk_true);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_B) == nk_true);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_X) == nk_true);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_Y) == nk_true);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_UP) == nk_true);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_LB) == nk_true);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_START) == nk_true);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_L3) == nk_true);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_R3) == nk_true);

        /* L2 and R2 have no SDL3 button, so the button loop must not set them. */
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_L2) == nk_false);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_R2) == nk_false);

        /* A single button maps to exactly one nk_gamepad button. */
        memset(fake_devices[0].buttons, 0, sizeof(fake_devices[0].buttons));
        fake_devices[0].buttons[SDL_GAMEPAD_BUTTON_EAST] = 1;
        nk_gamepad_update(&gamepads);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_B) == nk_true);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_A) == nk_false);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_X) == nk_false);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_Y) == nk_false);

        nk_gamepad_free(&gamepads);
    }

    printf("axis mapping\n");
    {
        struct nk_gamepads gamepads;
        float expected;

        fake_reset();
        fake_add_device(1, "Axis Pad");
        NK_ASSERT(nk_gamepad_init_with_source(&gamepads, &ctx, nk_gamepad_sdl3_input_source(NULL)) == nk_true);

        fake_devices[0].axes[SDL_GAMEPAD_AXIS_LEFTX] = 16383;
        fake_devices[0].axes[SDL_GAMEPAD_AXIS_LEFTY] = -16383;
        fake_devices[0].axes[SDL_GAMEPAD_AXIS_RIGHTX] = 32767;
        fake_devices[0].axes[SDL_GAMEPAD_AXIS_RIGHTY] = -32767;
        fake_devices[0].axes[SDL_GAMEPAD_AXIS_LEFT_TRIGGER] = 8000;
        fake_devices[0].axes[SDL_GAMEPAD_AXIS_RIGHT_TRIGGER] = 12000;
        nk_gamepad_update(&gamepads);

        expected = 16383 / 32767.0f;
        NK_ASSERT(nk_gamepad_get_axis(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_X) == expected);
        expected = -16383 / 32767.0f;
        NK_ASSERT(nk_gamepad_get_axis(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_Y) == expected);
        expected = 32767 / 32767.0f;
        NK_ASSERT(nk_gamepad_get_axis(&gamepads, 0, NK_GAMEPAD_AXIS_RIGHT_X) == expected);
        expected = -32767 / 32767.0f;
        NK_ASSERT(nk_gamepad_get_axis(&gamepads, 0, NK_GAMEPAD_AXIS_RIGHT_Y) == expected);
        expected = 8000 / 32767.0f;
        NK_ASSERT(nk_gamepad_get_axis(&gamepads, 0, NK_GAMEPAD_AXIS_LEFT_TRIGGER) == expected);
        expected = 12000 / 32767.0f;
        NK_ASSERT(nk_gamepad_get_axis(&gamepads, 0, NK_GAMEPAD_AXIS_RIGHT_TRIGGER) == expected);

        /* Below the trigger threshold, so L2 and R2 stay up. */
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_L2) == nk_false);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_R2) == nk_false);

        printf("trigger to button conversion\n");
        fake_devices[0].axes[SDL_GAMEPAD_AXIS_LEFT_TRIGGER] = 16383 + 101;
        fake_devices[0].axes[SDL_GAMEPAD_AXIS_RIGHT_TRIGGER] = 16383 + 100;
        nk_gamepad_update(&gamepads);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_L2) == nk_true);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_R2) == nk_false);

        nk_gamepad_free(&gamepads);
    }

    printf("nk_gamepad_sdl3_handle_event()\n");
    {
        struct nk_gamepads gamepads;
        SDL_Event event;

        fake_reset();
        NK_ASSERT(nk_gamepad_init_with_source(&gamepads, &ctx, nk_gamepad_sdl3_input_source(NULL)) == nk_true);
        NK_ASSERT(fake_open_count == 0);

        event.type = SDL_EVENT_GAMEPAD_ADDED;
        event.gdevice.which = 1;

        /* NULL arguments are no-ops. */
        nk_gamepad_sdl3_handle_event(NULL, &event);
        nk_gamepad_sdl3_handle_event(&gamepads, NULL);
        NK_ASSERT(fake_open_count == 0);

        /* An unknown instance id is ignored. */
        nk_gamepad_sdl3_handle_event(&gamepads, &event);
        NK_ASSERT(fake_open_count == 0);
        NK_ASSERT(nk_gamepad_is_available(&gamepads, 0) == nk_false);

        /* A real device is opened into the first free slot. */
        fake_add_device(1, "Hotplug Pad");
        nk_gamepad_sdl3_handle_event(&gamepads, &event);
        NK_ASSERT(fake_open_count == 1);
        NK_ASSERT(gamepads.gamepads[0].data != NULL);
        NK_ASSERT(nk_gamepad_is_available(&gamepads, 0) == nk_true);
        NK_ASSERT(strcmp(nk_gamepad_name(&gamepads, 0), "Hotplug Pad") == 0);

        /* A duplicate ADDED does not open a second handle. */
        nk_gamepad_sdl3_handle_event(&gamepads, &event);
        NK_ASSERT(fake_open_count == 1);
        NK_ASSERT(gamepads.gamepads[1].data == NULL);
        NK_ASSERT(nk_gamepad_is_available(&gamepads, 1) == nk_false);

        /* REMOVED closes the handle and clears the slot. */
        event.type = SDL_EVENT_GAMEPAD_REMOVED;
        nk_gamepad_sdl3_handle_event(&gamepads, &event);
        NK_ASSERT(fake_close_count == 1);
        NK_ASSERT(gamepads.gamepads[0].data == NULL);
        NK_ASSERT(nk_gamepad_is_available(&gamepads, 0) == nk_false);

        nk_gamepad_free(&gamepads);
    }

    printf("hot-plug clears stale button state\n");
    {
        struct nk_gamepads gamepads;
        SDL_Event event;

        fake_reset();
        fake_add_device(1, "Hotplug Pad");
        NK_ASSERT(nk_gamepad_init_with_source(&gamepads, &ctx, nk_gamepad_sdl3_input_source(NULL)) == nk_true);

        /* Hold a button, then yank the controller out while it is held. */
        fake_devices[0].buttons[SDL_GAMEPAD_BUTTON_SOUTH] = 1;
        nk_gamepad_update(&gamepads);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_A) == nk_true);

        event.type = SDL_EVENT_GAMEPAD_REMOVED;
        event.gdevice.which = 1;
        nk_gamepad_sdl3_handle_event(&gamepads, &event);

        /* Plug the same controller back in with nothing pressed. */
        fake_devices[0].buttons[SDL_GAMEPAD_BUTTON_SOUTH] = 0;
        event.type = SDL_EVENT_GAMEPAD_ADDED;
        nk_gamepad_sdl3_handle_event(&gamepads, &event);
        NK_ASSERT(nk_gamepad_is_available(&gamepads, 0) == nk_true);

        /* No phantom release should ever be reported. */
        nk_gamepad_update(&gamepads);
        NK_ASSERT(nk_gamepad_is_button_down(&gamepads, 0, NK_GAMEPAD_BUTTON_A) == nk_false);
        NK_ASSERT(nk_gamepad_is_button_released(&gamepads, 0, NK_GAMEPAD_BUTTON_A) == nk_false);
        nk_gamepad_update(&gamepads);
        NK_ASSERT(nk_gamepad_is_button_released(&gamepads, 0, NK_GAMEPAD_BUTTON_A) == nk_false);

        nk_gamepad_free(&gamepads);
    }

    printf("handle_event is ignored for other input sources\n");
    {
        struct nk_gamepads gamepads;
        SDL_Event event;

        fake_reset();
        fake_add_device(1, "Ignored Pad");
        NK_ASSERT(nk_gamepad_init_with_source(&gamepads, &ctx, nk_gamepad_none_input_source(NULL)) == nk_true);

        event.type = SDL_EVENT_GAMEPAD_ADDED;
        event.gdevice.which = 1;
        nk_gamepad_sdl3_handle_event(&gamepads, &event);

        NK_ASSERT(fake_open_count == 0);
        NK_ASSERT(gamepads.gamepads[0].data == NULL);
        NK_ASSERT(nk_gamepad_is_available(&gamepads, 0) == nk_false);

        nk_gamepad_free(&gamepads);
    }

    printf("nk_gamepad_sdl3_button_name()\n");
    {
        struct nk_gamepads gamepads;

        fake_reset();
        fake_add_device(1, "PlayStation Pad");
        fake_devices[0].labels[SDL_GAMEPAD_BUTTON_SOUTH] = SDL_GAMEPAD_BUTTON_LABEL_CROSS;
        fake_devices[0].labels[SDL_GAMEPAD_BUTTON_EAST] = SDL_GAMEPAD_BUTTON_LABEL_CIRCLE;
        fake_devices[0].labels[SDL_GAMEPAD_BUTTON_WEST] = SDL_GAMEPAD_BUTTON_LABEL_SQUARE;
        fake_devices[0].labels[SDL_GAMEPAD_BUTTON_NORTH] = SDL_GAMEPAD_BUTTON_LABEL_TRIANGLE;
        NK_ASSERT(nk_gamepad_init_with_source(&gamepads, &ctx, nk_gamepad_sdl3_input_source(NULL)) == nk_true);

        NK_ASSERT(strcmp(nk_gamepad_button_name(&gamepads, NK_GAMEPAD_BUTTON_A), "Cross") == 0);
        NK_ASSERT(strcmp(nk_gamepad_button_name(&gamepads, NK_GAMEPAD_BUTTON_B), "Circle") == 0);
        NK_ASSERT(strcmp(nk_gamepad_button_name(&gamepads, NK_GAMEPAD_BUTTON_X), "Square") == 0);
        NK_ASSERT(strcmp(nk_gamepad_button_name(&gamepads, NK_GAMEPAD_BUTTON_Y), "Triangle") == 0);

        /* SDL has no label for this button, so the built-in name is used. */
        NK_ASSERT(strcmp(nk_gamepad_button_name(&gamepads, NK_GAMEPAD_BUTTON_START), "Start") == 0);

        /* L2 has no SDL3 button at all, so it falls back too. */
        NK_ASSERT(strcmp(nk_gamepad_button_name(&gamepads, NK_GAMEPAD_BUTTON_L2), "L2") == 0);

        nk_gamepad_free(&gamepads);
    }

    {
        struct nk_gamepads gamepads;

        fake_reset();
        NK_ASSERT(nk_gamepad_init_with_source(&gamepads, &ctx, nk_gamepad_sdl3_input_source(NULL)) == nk_true);

        /* With no gamepads connected, the built-in names are used. */
        NK_ASSERT(strcmp(nk_gamepad_button_name(&gamepads, NK_GAMEPAD_BUTTON_A), "A") == 0);
        NK_ASSERT(strcmp(nk_gamepad_button_name(&gamepads, NK_GAMEPAD_BUTTON_UP), "Up") == 0);

        nk_gamepad_free(&gamepads);
    }

    printf("SDL_InitSubSystem() failure\n");
    {
        struct nk_gamepads gamepads;

        fake_reset();
        fake_add_device(1, "Unreachable Pad");
        fake_init_should_fail = 1;

        NK_ASSERT(nk_gamepad_init_with_source(&gamepads, &ctx, nk_gamepad_sdl3_input_source(NULL)) == nk_false);
        NK_ASSERT(fake_open_count == 0);
    }

    printf("NULL safety\n");
    {
        fake_reset();
        NK_ASSERT(nk_gamepad_sdl3_init(NULL, NULL) == nk_false);
        nk_gamepad_sdl3_update(NULL, NULL);
        nk_gamepad_sdl3_free(NULL, NULL);
        NK_ASSERT(fake_update_count == 0);
        NK_ASSERT(fake_init_refcount == 0);
    }

    nk_free(&ctx);

    printf("-------------------------\n");
    printf("nuklear_gamepad_sdl3_test: Tests passed!\n");

    return 0;
}
