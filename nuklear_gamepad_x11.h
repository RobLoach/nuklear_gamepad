#ifndef NUKLEAR_GAMEPAD_X11_H__
#define NUKLEAR_GAMEPAD_X11_H__

#ifndef NK_GAMEPAD_MAX
#define NK_GAMEPAD_MAX 4
#endif

#ifdef __cplusplus
extern "C" {
#endif

NK_API nk_bool nk_gamepad_x11_init(struct nk_gamepads* gamepads, void* user_data);
NK_API void nk_gamepad_x11_update(struct nk_gamepads* gamepads, void* user_data);
NK_API void nk_gamepad_x11_free(struct nk_gamepads* gamepads, void* user_data);
NK_API const char* nk_gamepad_x11_name(struct nk_gamepads* gamepads, int num, void* user_data);
NK_API struct nk_gamepad_input_source nk_gamepad_x11_input_source(void* user_data);

#ifdef __cplusplus
}
#endif

#endif

#if defined(NK_GAMEPAD_IMPLEMENTATION) && !defined(NK_GAMEPAD_HEADER_ONLY)
#ifndef NUKLEAR_GAMEPAD_X11_IMPLEMENTATION_ONCE
#define NUKLEAR_GAMEPAD_X11_IMPLEMENTATION_ONCE

#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <string.h>
#include <sys/ioctl.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Maps a Linux joystick button number (Xbox layout) to an nk_gamepad_button. */
static enum nk_gamepad_button nk_gamepad_x11_map_button(int btn) {
    switch (btn) {
        case 0:  return NK_GAMEPAD_BUTTON_A;
        case 1:  return NK_GAMEPAD_BUTTON_B;
        case 2:  return NK_GAMEPAD_BUTTON_X;
        case 3:  return NK_GAMEPAD_BUTTON_Y;
        case 4:  return NK_GAMEPAD_BUTTON_LB;
        case 5:  return NK_GAMEPAD_BUTTON_RB;
        case 6:  return NK_GAMEPAD_BUTTON_BACK;
        case 7:  return NK_GAMEPAD_BUTTON_START;
        case 8:  return NK_GAMEPAD_BUTTON_GUIDE;
        default: return NK_GAMEPAD_BUTTON_INVALID;
    }
}

/* Maps a Linux joystick axis number (Xbox layout) to an nk_gamepad_axis. */
static enum nk_gamepad_axis nk_gamepad_x11_map_axis(int axis) {
    switch (axis) {
        case 0: return NK_GAMEPAD_AXIS_LEFT_X;
        case 1: return NK_GAMEPAD_AXIS_LEFT_Y;
        case 2: return NK_GAMEPAD_AXIS_LEFT_TRIGGER;
        case 3: return NK_GAMEPAD_AXIS_RIGHT_X;
        case 4: return NK_GAMEPAD_AXIS_RIGHT_Y;
        case 5: return NK_GAMEPAD_AXIS_RIGHT_TRIGGER;
        default: return NK_GAMEPAD_AXIS_INVALID;
    }
}

NK_API nk_bool nk_gamepad_x11_init(struct nk_gamepads* gamepads, void* user_data) {
    int i;
    char path[32];
    NK_UNUSED(user_data);
    if (gamepads == NULL) {
        return nk_false;
    }

    for (i = 0; i < NK_GAMEPAD_MAX; i++) {
        int fd;
        snprintf(path, sizeof(path), "/dev/input/js%d", i);
        fd = open(path, O_RDONLY | O_NONBLOCK);
        if (fd < 0) {
            gamepads->gamepads[i].data = (void*)(intptr_t)-1;
            gamepads->gamepads[i].available = nk_false;
            continue;
        }

        gamepads->gamepads[i].data = (void*)(intptr_t)fd;
        gamepads->gamepads[i].available = nk_true;

        /* Read joystick name via ioctl */
        ioctl(fd, JSIOCGNAME(NK_GAMEPAD_NAME_SIZE - 1), gamepads->gamepads[i].name);
    }

    return nk_true;
}

NK_API void nk_gamepad_x11_update(struct nk_gamepads* gamepads, void* user_data) {
    int i;
    struct js_event event;
    NK_UNUSED(user_data);
    if (gamepads == NULL) {
        return;
    }

    for (i = 0; i < NK_GAMEPAD_MAX; i++) {
        int fd = (int)(intptr_t)gamepads->gamepads[i].data;
        if (fd < 0) {
            continue;
        }

        /* Drain all pending events */
        while (read(fd, &event, sizeof(event)) == sizeof(event)) {
            switch (event.type & ~JS_EVENT_INIT) {
                case JS_EVENT_BUTTON: {
                    enum nk_gamepad_button btn = nk_gamepad_x11_map_button(event.number);
                    if (btn != NK_GAMEPAD_BUTTON_INVALID) {
                        nk_gamepad_button(gamepads, i, btn, event.value != 0);
                    }
                    break;
                }
                case JS_EVENT_AXIS: {
                    enum nk_gamepad_axis ax = nk_gamepad_x11_map_axis(event.number);
                    if (ax != NK_GAMEPAD_AXIS_INVALID) {
                        float value = event.value / 32767.0f;
                        if (value >  1.0f) value =  1.0f;
                        if (value < -1.0f) value = -1.0f;
                        /* Triggers are reported as -1..1; remap to 0..1 */
                        if (ax == NK_GAMEPAD_AXIS_LEFT_TRIGGER || ax == NK_GAMEPAD_AXIS_RIGHT_TRIGGER) {
                            value = (value + 1.0f) * 0.5f;
                        }
                        nk_gamepad_axis(gamepads, i, ax, value);

                        /* D-pad via hat axes (axis 6 = X, axis 7 = Y) */
                        if (event.number == 6) {
                            nk_gamepad_button(gamepads, i, NK_GAMEPAD_BUTTON_LEFT,  event.value < 0);
                            nk_gamepad_button(gamepads, i, NK_GAMEPAD_BUTTON_RIGHT, event.value > 0);
                        } else if (event.number == 7) {
                            nk_gamepad_button(gamepads, i, NK_GAMEPAD_BUTTON_UP,   event.value < 0);
                            nk_gamepad_button(gamepads, i, NK_GAMEPAD_BUTTON_DOWN, event.value > 0);
                        }
                    } else if (event.number == 6) {
                        nk_gamepad_button(gamepads, i, NK_GAMEPAD_BUTTON_LEFT,  event.value < 0);
                        nk_gamepad_button(gamepads, i, NK_GAMEPAD_BUTTON_RIGHT, event.value > 0);
                    } else if (event.number == 7) {
                        nk_gamepad_button(gamepads, i, NK_GAMEPAD_BUTTON_UP,   event.value < 0);
                        nk_gamepad_button(gamepads, i, NK_GAMEPAD_BUTTON_DOWN, event.value > 0);
                    }
                    break;
                }
            }
        }
    }
}

NK_API void nk_gamepad_x11_free(struct nk_gamepads* gamepads, void* user_data) {
    int i;
    NK_UNUSED(user_data);
    if (gamepads == NULL) {
        return;
    }

    for (i = 0; i < NK_GAMEPAD_MAX; i++) {
        int fd = (int)(intptr_t)gamepads->gamepads[i].data;
        if (fd >= 0) {
            close(fd);
        }
        gamepads->gamepads[i].data = (void*)(intptr_t)-1;
        gamepads->gamepads[i].available = nk_false;
    }
}

NK_API const char* nk_gamepad_x11_name(struct nk_gamepads* gamepads, int num, void* user_data) {
    NK_UNUSED(user_data);
    if (num < 0 || num >= NK_GAMEPAD_MAX || !gamepads->gamepads[num].available) {
        return NULL;
    }
    return gamepads->gamepads[num].name;
}

NK_API struct nk_gamepad_input_source nk_gamepad_x11_input_source(void* user_data) {
    struct nk_gamepad_input_source source;
    source.user_data = user_data;
    source.init = &nk_gamepad_x11_init;
    source.update = &nk_gamepad_x11_update;
    source.free = &nk_gamepad_x11_free;
    source.name = &nk_gamepad_x11_name;
    source.button_name = NULL;
    source.input_source_name = "X11";
    source.id = NK_GAMEPAD_INPUT_SOURCE_X11;
    return source;
}

#ifdef __cplusplus
}
#endif

#endif
#endif
