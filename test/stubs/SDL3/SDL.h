/**
 * @file SDL.h
 *
 * !!! THIS IS NOT A REAL SDL3 HEADER. !!!
 *
 * This is a headless test double used only by test/nuklear_gamepad_sdl3_test.c
 * so that nuklear_gamepad_sdl3.h can be compiled and exercised on CI without
 * linking against, or having any hardware for, SDL3.
 *
 * The declarations mirror the shapes of the real SDL3 API so that the backend
 * is compiled against a faithful signature set. The one deliberate difference
 * is that where real SDL3 uses the C23 `bool` type, this stub uses `int`,
 * because the test suite is built as strict C90.
 *
 * Only the gamepad API surface used by nuklear_gamepad_sdl3.h is declared, and
 * only prototypes are declared here. The test program provides the definitions
 * in its own translation unit.
 */
#ifndef NUKLEAR_GAMEPAD_TEST_STUB_SDL3_SDL_H__
#define NUKLEAR_GAMEPAD_TEST_STUB_SDL3_SDL_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int Uint32;
typedef short Sint16;
typedef Uint32 SDL_JoystickID;
typedef Uint32 SDL_InitFlags;

#define SDL_INIT_GAMEPAD 0x00002000u

/* Opaque gamepad handle. */
typedef struct SDL_Gamepad SDL_Gamepad;

typedef enum SDL_GamepadButton {
    SDL_GAMEPAD_BUTTON_INVALID = -1,
    SDL_GAMEPAD_BUTTON_SOUTH,
    SDL_GAMEPAD_BUTTON_EAST,
    SDL_GAMEPAD_BUTTON_WEST,
    SDL_GAMEPAD_BUTTON_NORTH,
    SDL_GAMEPAD_BUTTON_BACK,
    SDL_GAMEPAD_BUTTON_GUIDE,
    SDL_GAMEPAD_BUTTON_START,
    SDL_GAMEPAD_BUTTON_LEFT_STICK,
    SDL_GAMEPAD_BUTTON_RIGHT_STICK,
    SDL_GAMEPAD_BUTTON_LEFT_SHOULDER,
    SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER,
    SDL_GAMEPAD_BUTTON_DPAD_UP,
    SDL_GAMEPAD_BUTTON_DPAD_DOWN,
    SDL_GAMEPAD_BUTTON_DPAD_LEFT,
    SDL_GAMEPAD_BUTTON_DPAD_RIGHT,
    SDL_GAMEPAD_BUTTON_COUNT
} SDL_GamepadButton;

typedef enum SDL_GamepadAxis {
    SDL_GAMEPAD_AXIS_INVALID = -1,
    SDL_GAMEPAD_AXIS_LEFTX,
    SDL_GAMEPAD_AXIS_LEFTY,
    SDL_GAMEPAD_AXIS_RIGHTX,
    SDL_GAMEPAD_AXIS_RIGHTY,
    SDL_GAMEPAD_AXIS_LEFT_TRIGGER,
    SDL_GAMEPAD_AXIS_RIGHT_TRIGGER,
    SDL_GAMEPAD_AXIS_COUNT
} SDL_GamepadAxis;

typedef enum SDL_GamepadButtonLabel {
    SDL_GAMEPAD_BUTTON_LABEL_UNKNOWN,
    SDL_GAMEPAD_BUTTON_LABEL_A,
    SDL_GAMEPAD_BUTTON_LABEL_B,
    SDL_GAMEPAD_BUTTON_LABEL_X,
    SDL_GAMEPAD_BUTTON_LABEL_Y,
    SDL_GAMEPAD_BUTTON_LABEL_CROSS,
    SDL_GAMEPAD_BUTTON_LABEL_CIRCLE,
    SDL_GAMEPAD_BUTTON_LABEL_SQUARE,
    SDL_GAMEPAD_BUTTON_LABEL_TRIANGLE
} SDL_GamepadButtonLabel;

#define SDL_EVENT_GAMEPAD_ADDED 0x653u
#define SDL_EVENT_GAMEPAD_REMOVED 0x654u

typedef struct SDL_GamepadDeviceEvent {
    Uint32 type;
    SDL_JoystickID which;
} SDL_GamepadDeviceEvent;

typedef union SDL_Event {
    Uint32 type;
    SDL_GamepadDeviceEvent gdevice;
} SDL_Event;

extern int SDL_InitSubSystem(SDL_InitFlags flags);
extern void SDL_QuitSubSystem(SDL_InitFlags flags);
extern int SDL_IsGamepad(SDL_JoystickID instance_id);
extern SDL_Gamepad* SDL_OpenGamepad(SDL_JoystickID instance_id);
extern void SDL_CloseGamepad(SDL_Gamepad* gamepad);
extern SDL_JoystickID* SDL_GetGamepads(int* count);
extern SDL_JoystickID SDL_GetGamepadID(SDL_Gamepad* gamepad);
extern int SDL_GamepadConnected(SDL_Gamepad* gamepad);
extern int SDL_GetGamepadButton(SDL_Gamepad* gamepad, SDL_GamepadButton button);
extern Sint16 SDL_GetGamepadAxis(SDL_Gamepad* gamepad, SDL_GamepadAxis axis);
extern const char* SDL_GetGamepadName(SDL_Gamepad* gamepad);
extern SDL_GamepadButtonLabel SDL_GetGamepadButtonLabel(SDL_Gamepad* gamepad, SDL_GamepadButton button);
extern void SDL_UpdateGamepads(void);
extern void SDL_free(void* mem);

#ifdef __cplusplus
}
#endif

#endif  /* NUKLEAR_GAMEPAD_TEST_STUB_SDL3_SDL_H__ */
