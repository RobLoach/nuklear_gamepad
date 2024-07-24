#define PNTR_APP_IMPLEMENTATION
#define PNTR_ENABLE_DEFAULT_FONT
#define PNTR_DISABLE_MATH
#include "pntr_app.h"

#define PNTR_NUKLEAR_IMPLEMENTATION
#include "pntr_nuklear.h"

#define NK_GAMEPAD_PNTR
#define NK_GAMEPAD_IMPLEMENTATION
#include "../../nuklear_gamepad.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include "../common/nuklear_gamepad_demo.c"

typedef struct AppData {
    pntr_font* font;
    struct nk_context* ctx;
    struct nk_gamepads gamepads;
} AppData;

bool Init(pntr_app* app) {
    AppData* appData = pntr_load_memory(sizeof(AppData));
    pntr_app_set_userdata(app, appData);

    // Load the default font
    appData->font = pntr_load_font_default();
    appData->ctx = pntr_load_nuklear(appData->font);

    // Initialize the Gamepads, passing in the pntr_app as the user data.
    nk_gamepad_init(&appData->gamepads, appData->ctx, app);

    return true;
}

bool Update(pntr_app* app, pntr_image* screen) {
    AppData* appData = (AppData*)pntr_app_userdata(app);
    struct nk_context* ctx = appData->ctx;

    // Update the pntr input state.
    pntr_nuklear_update(ctx, app);

    // Update the gamepad state
    nk_gamepad_update(&appData->gamepads);

    // Clear the background
    pntr_clear_background(screen, PNTR_BLACK);

    // Update the context
    nuklear_gamepad_demo(ctx, &appData->gamepads);

    // Draw it on the screen
    pntr_draw_nuklear(screen, ctx);

    return true;
}

void Close(pntr_app* app) {
    AppData* appData = (AppData*)pntr_app_userdata(app);

    // Unload the font
    pntr_unload_font(appData->font);
    nk_gamepad_free(&appData->gamepads);
    pntr_unload_nuklear(appData->ctx);

    pntr_unload_memory(appData);
}

pntr_app Main(int argc, char* argv[]) {
    return (pntr_app) {
        .width = WINDOW_WIDTH,
        .height = WINDOW_HEIGHT,
        .title = "nuklear_gamepad",
        .init = Init,
        .update = Update,
        .close = Close,
        .fps = 60
    };
}
