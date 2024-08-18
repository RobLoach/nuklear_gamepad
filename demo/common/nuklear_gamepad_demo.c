void nuklear_gamepad_button_style(struct nk_context* ctx, struct nk_gamepads* gamepads, int num, enum nk_gamepad_button button) {
    // Apply a style to the button if it is pressed.
    if (nk_gamepad_is_button_down(gamepads, num, button)) {
        nk_widget_disable_begin(ctx);
    }
    else {
        nk_widget_disable_end(ctx);
    }
}

void nuklear_gamepad_demo(struct nk_context* ctx, struct nk_gamepads* gamepads, void* user_data) {
    int padding = 25;

    // Switch Gamepad Input Source
    if (nk_begin(ctx, "Source", nk_rect(WINDOW_WIDTH - 200, 0, 200, 200), NK_WINDOW_MINIMIZABLE | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_MOVABLE | NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
        nk_layout_row_dynamic(ctx, 0, 1);

        // Loop through the list of input sources
        nk_gamepad_input_source_fn* input_source = nk_gamepad_input_sources;
        while (*input_source != NULL) {
            // Get the initial meta data for the source
            struct nk_gamepad_input_source source = (*input_source)(NULL);

            // Display a radio button for the source
            if (nk_option_label(ctx, source.input_source_name, gamepads->input_source.id == source.id)) {
                // Only trigger the change if the source is different
                if (gamepads->input_source.id != source.id) {
                    void* source_user_data = source.id == NK_GAMEPAD_INPUT_SOURCE_KEYBOARD ? NULL : user_data;
                    nk_gamepad_set_input_source(gamepads, (*input_source)(source_user_data));
                }
            }
            input_source++;
        }
    }
    nk_end(ctx);

    // Make a window for each gamepad
    int count = 0;
    for (int i = 0; i < nk_gamepad_count(gamepads); i++) {
        if (nk_gamepad_is_available(gamepads, i) == nk_false) {
            continue;
        }
        count++;

        // Make a unique name for the window.
        char name[32];
        name[0] = i + 95;
        name[1] = '\0';

        struct nk_rect window_bounds = nk_rect(
            padding + (padding * i * 5), padding + (padding * i * 5),
            WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - padding * 2);

        // Make a window for each controller
        if (nk_begin_titled(ctx, name, nk_gamepad_name(gamepads, i), window_bounds,
            NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_NO_SCROLLBAR))
        {
            nk_layout_row_dynamic(ctx, 0, 7);

            // First Row
            nk_spacing(ctx, 1);
            nuklear_gamepad_button_style(ctx, gamepads, i, NK_GAMEPAD_BUTTON_UP);
            nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_UP);
            nk_spacing(ctx, 1);
            nk_spacing(ctx, 1);
            nk_spacing(ctx, 1);
            nuklear_gamepad_button_style(ctx, gamepads, i, NK_GAMEPAD_BUTTON_Y);
            nk_button_label(ctx, "Y");
            nk_spacing(ctx, 1);

            // Second Row
            nuklear_gamepad_button_style(ctx, gamepads, i, NK_GAMEPAD_BUTTON_LEFT);
            nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT);
            nk_spacing(ctx, 1);
            nuklear_gamepad_button_style(ctx, gamepads, i, NK_GAMEPAD_BUTTON_RIGHT);
            nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT);
            nk_spacing(ctx, 1);
            nuklear_gamepad_button_style(ctx, gamepads, i, NK_GAMEPAD_BUTTON_X);
            nk_button_label(ctx, "X");
            nk_spacing(ctx, 1);
            nuklear_gamepad_button_style(ctx, gamepads, i, NK_GAMEPAD_BUTTON_B);
            nk_button_label(ctx, "B");

            // Third Row
            nk_spacing(ctx, 1);
            nuklear_gamepad_button_style(ctx, gamepads, i, NK_GAMEPAD_BUTTON_DOWN);
            nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_DOWN);
            nk_spacing(ctx, 1);
            nk_spacing(ctx, 1);
            nk_spacing(ctx, 1);
            nuklear_gamepad_button_style(ctx, gamepads, i, NK_GAMEPAD_BUTTON_A);
            nk_button_label(ctx, "A");

            // Start/Back buttons
            nk_layout_row_dynamic(ctx, 0, 5);
            nuklear_gamepad_button_style(ctx, gamepads, i, NK_GAMEPAD_BUTTON_BACK);
            nk_button_symbol(ctx, NK_SYMBOL_MINUS);
            nk_spacing(ctx, 1);
            nuklear_gamepad_button_style(ctx, gamepads, i, NK_GAMEPAD_BUTTON_GUIDE);
            nk_button_symbol(ctx, NK_SYMBOL_RECT_SOLID);
            nk_spacing(ctx, 1);
            nuklear_gamepad_button_style(ctx, gamepads, i, NK_GAMEPAD_BUTTON_START);
            nk_button_symbol(ctx, NK_SYMBOL_PLUS);

            // Finish applying any disable styles.
            nk_widget_disable_end(ctx);
        }
        nk_end(ctx);
    }

    if (count == 0) {
        struct nk_rect window_bounds = nk_rect(padding * 5, padding, WINDOW_WIDTH - padding * 18, WINDOW_HEIGHT - padding * 18);
        if (nk_begin(ctx, "nuklear_gamepad", window_bounds, NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, 0, 1);
            nk_label(ctx, "No Gamepads Detected", NK_TEXT_CENTERED);
        }
        nk_end(ctx);
    }
}
