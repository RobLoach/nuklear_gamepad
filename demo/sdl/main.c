#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#include <SDL2/SDL.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_COMMAND_USERDATA
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION
#include "../../vendor/nuklear/nuklear.h"
#include "../../vendor/nuklear/demo/sdl_renderer/nuklear_sdl_renderer.h"

#define NK_GAMEPAD_IMPLEMENTATION
#define NK_GAMEPAD_SDL
#include "../../nuklear_gamepad.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include "../common/nuklear_gamepad_demo.c"

int main(int argc, char *argv[]) {
    NK_UNUSED(argc);
    NK_UNUSED(argv);
    /* Platform */
    SDL_Window *win;
    SDL_Renderer *renderer;
    int running = 1;
    int flags = 0;
    float font_scale = 2;
    struct nk_gamepads gamepads;

    /* GUI */
    struct nk_context *ctx;

    /* SDL setup */
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER);

    win = SDL_CreateWindow("nuklear_gamepad_demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);
    if (win == NULL) {
        SDL_Log("Error SDL_CreateWindow %s", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(win, -1, flags);
    if (renderer == NULL) {
        SDL_Log("Error SDL_CreateRenderer %s", SDL_GetError());
        return 1;
    }

    /* GUI */
    ctx = nk_sdl_init(win, renderer);
    {
        struct nk_font_atlas *atlas;
        struct nk_font_config config = nk_font_config(0);
        struct nk_font *font;

        nk_sdl_font_stash_begin(&atlas);
        font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config);
        nk_sdl_font_stash_end();
        nk_style_set_font(ctx, &font->handle);
    }

    /* Initialize the Gamepads */
    nk_gamepad_init(&gamepads, ctx, NULL);

    while (running) {
        SDL_Event evt;

        /* Update the Gamepad state */
        nk_gamepad_update(&gamepads);

        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) goto cleanup;
            if (evt.type == SDL_KEYUP && evt.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = 0;

            /* Allow Gamepads to respond to gamepads being connected/disconnected */
            nk_sdl_handle_event(&evt);

            nk_gamepad_sdl_handle_event(&gamepads, &evt);
        }
        nk_input_end(ctx);

        /* Render the gamepad demo */
        nuklear_gamepad_demo(ctx, &gamepads, NULL);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        nk_sdl_render(NK_ANTI_ALIASING_ON);

        SDL_RenderPresent(renderer);
    }

cleanup:
    nk_gamepad_free(&gamepads);
    nk_sdl_shutdown();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
