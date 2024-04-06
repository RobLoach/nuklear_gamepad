#include <stdio.h>

void nuklear_gamepad_button_style(struct nk_context* ctx, struct nk_gamepads* gamepads, enum nk_gamepad_button button) {
    if (nk_gamepad_is_button_down(gamepads, 0, button)) {
        nk_widget_disable_begin(ctx);
    }
    else {
        nk_widget_disable_end(ctx);
    }
}

void nuklear_gamepad_demo(struct nk_context* ctx, struct nk_gamepads* gamepads) {
    int padding = 25;

    int count = nk_gamepad_count(gamepads);
    if (count == 0) {
        if (nk_begin(ctx, "nuklear_gamepad", nk_rect(padding, padding, WINDOW_WIDTH - padding * 2, WINDOW_HEIGHT - padding * 2),
            NK_WINDOW_BORDER | NK_WINDOW_TITLE))
        {
            nk_layout_row_dynamic(ctx, 0, 1);
            nk_label(ctx, "No Gamepads Detected", NK_TEXT_CENTERED);
        }
        nk_end(ctx);

        return;
    }

    // for (int i = 0; i < count; i++) {
    //     if (nk_gamepad_is_button_down(gamepads, i, NK_GAMEPAD_BUTTON_A)) {
    //         printf("Gamepad %d: A button is down\n", i);
    //     }
    // }

    
    /* GUI */
    if (nk_begin(ctx, "nuklear_gamepad", nk_rect(padding, padding, WINDOW_WIDTH - padding * 2, WINDOW_HEIGHT - padding * 2),
        NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE))
    {
        enum {EASY, HARD};
        static int op = EASY;
        static int property = 20;

        nk_layout_row_dynamic(ctx, 0, 1);
        nk_labelf(ctx, NK_TEXT_LEFT, "Number of Gamepads: %d", nk_gamepad_count(gamepads));

        nk_layout_row_dynamic(ctx, 0, 1);
        nk_layout_row_dynamic(ctx, 0, 7);

        // First Row
        nk_spacing(ctx, 1);
        nuklear_gamepad_button_style(ctx, gamepads, NK_GAMEPAD_BUTTON_UP);
        nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_UP);
        nk_spacing(ctx, 1);
        nk_spacing(ctx, 1);
        nk_spacing(ctx, 1);
        nuklear_gamepad_button_style(ctx, gamepads, NK_GAMEPAD_BUTTON_Y);
        nk_button_label(ctx, "Y");
        nk_spacing(ctx, 1);

        // Second Row
        nuklear_gamepad_button_style(ctx, gamepads, NK_GAMEPAD_BUTTON_LEFT);
        nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT);
        nk_spacing(ctx, 1);
        nuklear_gamepad_button_style(ctx, gamepads, NK_GAMEPAD_BUTTON_RIGHT);
        nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT);
        nk_spacing(ctx, 1);
        nuklear_gamepad_button_style(ctx, gamepads, NK_GAMEPAD_BUTTON_X);
        nk_button_label(ctx, "X");
        nk_spacing(ctx, 1);
        nuklear_gamepad_button_style(ctx, gamepads, NK_GAMEPAD_BUTTON_B);
        nk_button_label(ctx, "B");

        // Third Row
        nk_spacing(ctx, 1);
        nuklear_gamepad_button_style(ctx, gamepads, NK_GAMEPAD_BUTTON_DOWN);
        nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_DOWN);
        nk_spacing(ctx, 1);
        nk_spacing(ctx, 1);
        nk_spacing(ctx, 1);
        nuklear_gamepad_button_style(ctx, gamepads, NK_GAMEPAD_BUTTON_A);
        nk_button_label(ctx, "A");

        // Start/Back buttons
        nk_layout_row_dynamic(ctx, 0, 1);
        nk_layout_row_dynamic(ctx, 0, 5);
        nk_spacing(ctx, 1);
        nuklear_gamepad_button_style(ctx, gamepads, NK_GAMEPAD_BUTTON_BACK);
        nk_button_label(ctx, "Back");
        nk_spacing(ctx, 1);
        nuklear_gamepad_button_style(ctx, gamepads, NK_GAMEPAD_BUTTON_START);
        nk_button_label(ctx, "Start");

        // Finish applying the styles.
        nk_widget_disable_end(ctx);
    }
    nk_end(ctx);
}