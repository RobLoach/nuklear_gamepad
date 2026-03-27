/* nuklear - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#include <SDL3/SDL.h>

/* This demo uses "main callbacks" which are new in SDL3
 * Those provide highly portable entry point and event loop for the app
 * see: https://wiki.libsdl.org/SDL3/README-main-functions
 * */
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_STANDARD_IO

#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

/*#define NK_INCLUDE_DEFAULT_ALLOCATOR*/

#define NK_INCLUDE_COMMAND_USERDATA
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT


#ifndef NK_INCLUDE_FIXED_TYPES
    #define NK_INT8              Sint8
    #define NK_UINT8             Uint8
    #define NK_INT16             Sint16
    #define NK_UINT16            Uint16
    #define NK_INT32             Sint32
    #define NK_UINT32            Uint32
    /* SDL guarantees 'uintptr_t' typedef */
    #define NK_SIZE_TYPE         uintptr_t
    #define NK_POINTER_TYPE      uintptr_t
#endif

#ifndef NK_INCLUDE_STANDARD_BOOL
    #define NK_BOOL               bool
#endif

#define NK_ASSERT(condition)      SDL_assert(condition)
#define NK_STATIC_ASSERT(exp)     SDL_COMPILE_TIME_ASSERT(, exp)
#define NK_MEMSET(dst, c, len)    SDL_memset(dst, c, len)
#define NK_MEMCPY(dst, src, len)  SDL_memcpy(dst, src, len)
#define NK_VSNPRINTF(s, n, f, a)  SDL_vsnprintf(s, n, f, a)
#define NK_STRTOD(str, endptr)    SDL_strtod(str, endptr)

static char* nk_sdl_dtoa(char *str, double d);
#define NK_DTOA(str, d) nk_sdl_dtoa(str, d)

#define NK_INV_SQRT(f)            (1.0f / SDL_sqrtf(f))
#define NK_SIN(f)                 SDL_sinf(f)
#define NK_COS(f)                 SDL_cosf(f)

#define STBTT_ifloor(x)       ((int)SDL_floor(x))
#define STBTT_iceil(x)        ((int)SDL_ceil(x))
#define STBTT_sqrt(x)         SDL_sqrt(x)
#define STBTT_pow(x,y)        SDL_pow(x,y)
#define STBTT_fmod(x,y)       SDL_fmod(x,y)
#define STBTT_cos(x)          SDL_cosf(x)
#define STBTT_acos(x)         SDL_acos(x)
#define STBTT_fabs(x)         SDL_fabs(x)
#define STBTT_assert(x)       SDL_assert(x)
#define STBTT_strlen(x)       SDL_strlen(x)
#define STBTT_memcpy          SDL_memcpy
#define STBTT_memset          SDL_memset
#define stbtt_uint8           Uint8
#define stbtt_int8            Sint8
#define stbtt_uint16          Uint16
#define stbtt_int16           Sint16
#define stbtt_uint32          Uint32
#define stbtt_int32           Sint32
#define STBRP_SORT            SDL_qsort
#define STBRP_ASSERT          SDL_assert

#define NK_IMPLEMENTATION
#include "../../vendor/nuklear/nuklear.h"
#define NK_SDL3_RENDERER_IMPLEMENTATION
#include "../../vendor/nuklear/demo/sdl3_renderer/nuklear_sdl3_renderer.h"

// Nuklear Gamepad
#define NK_GAMEPAD_SDL3
#define NK_GAMEPAD_KEYBOARD
#define NK_GAMEPAD_IMPLEMENTATION
#include "../../nuklear_gamepad.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include "../common/nuklear_gamepad_demo.c"

struct nk_sdl_app {
    SDL_Window* window;
    SDL_Renderer* renderer;
    struct nk_context * ctx;
    enum nk_anti_aliasing AA;
    struct nk_gamepads gamepads;
};

static SDL_AppResult
nk_sdl_fail()
{
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error: %s", SDL_GetError());
    return SDL_APP_FAILURE;
}

SDL_AppResult
SDL_AppInit(void** appstate, int argc, char* argv[])
{
    struct nk_sdl_app* app;
    struct nk_context* ctx;
    float font_scale;
    NK_UNUSED(argc);
    NK_UNUSED(argv);

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD)) {
        return nk_sdl_fail();
    }

    app = SDL_malloc(sizeof(*app));
    if (app == NULL) {
        return nk_sdl_fail();
    }

    if (!SDL_CreateWindowAndRenderer("nuklear_gamepad_demo", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &app->window, &app->renderer)) {
        SDL_free(app);
        return nk_sdl_fail();
    }
    *appstate = app;

    if (!SDL_SetRenderVSync(app->renderer, 1)) {
        SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "SDL_SetRenderVSync failed: %s", SDL_GetError());
    }

    font_scale = 1;
    {
        const float scale = SDL_GetWindowDisplayScale(app->window);
        SDL_SetRenderScale(app->renderer, scale, scale);
        font_scale = scale;
    }

    ctx = nk_sdl_init(app->window, app->renderer, nk_sdl_allocator());
    app->ctx = ctx;

    {
        struct nk_font_atlas *atlas;
        struct nk_font_config config = nk_font_config(0);
        struct nk_font *font;

        atlas = nk_sdl_font_stash_begin(ctx);
        font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config);
        nk_sdl_font_stash_end(ctx);

        font->handle.height /= font_scale;
        nk_style_set_font(ctx, &font->handle);

        app->AA = NK_ANTI_ALIASING_ON;
    }

    /* Initialize the Gamepads */
    nk_gamepad_init(&app->gamepads, ctx, NULL);

    nk_input_begin(ctx);

    return SDL_APP_CONTINUE;
}

SDL_AppResult
SDL_AppEvent(void *appstate, SDL_Event* event)
{
    struct nk_sdl_app* app = (struct nk_sdl_app*)appstate;

    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
    }

    SDL_ConvertEventToRenderCoordinates(app->renderer, event);

    nk_sdl_handle_event(app->ctx, event);

    // Handle the Gamepad event.
    nk_gamepad_sdl3_handle_event(&app->gamepads, event);

    return SDL_APP_CONTINUE;
}

SDL_AppResult
SDL_AppIterate(void *appstate)
{
    struct nk_sdl_app* app = (struct nk_sdl_app*)appstate;
    struct nk_context* ctx = app->ctx;

    // Update the Gamepad state.
    nk_gamepad_update(&app->gamepads);

    nk_input_end(ctx);

    // Render the gamepad demo.
    nuklear_gamepad_demo(ctx, &app->gamepads, NULL);

    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    nk_sdl_render(ctx, app->AA);
    nk_sdl_update_TextInput(ctx);

    SDL_RenderPresent(app->renderer);

    nk_input_begin(ctx);
    return SDL_APP_CONTINUE;
}

void
SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    struct nk_sdl_app* app = (struct nk_sdl_app*)appstate;
    NK_UNUSED(result);

    if (app) {
        nk_input_end(app->ctx);

        // Free the Gamepads
        nk_gamepad_free(&app->gamepads);

        nk_sdl_shutdown(app->ctx);
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        SDL_free(app);
    }
}

static char*
nk_sdl_dtoa(char *str, double d)
{
    NK_ASSERT(str);
    if (!str) return NULL;
    (void)SDL_snprintf(str, 99999, "%.17g", d);
    return str;
}
