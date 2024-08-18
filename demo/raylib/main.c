/**********************************************************************************************
*
*   raylib-nuklear-example - Example of using Nuklear with Raylib.
*
*   LICENSE: zlib/libpng
*
*   nuklear_raylib is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   Copyright (c) 2020 Rob Loach (@RobLoach)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib-nuklear.h"

#define NK_GAMEPAD_RAYLIB
#define NK_GAMEPAD_KEYBOARD
#define NK_GAMEPAD_IMPLEMENTATION
#include "../../nuklear_gamepad.h"

#include "../common/nuklear_gamepad_demo.c"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "[raylib-nuklear] example");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    /* GUI */
    struct nk_context *ctx = InitNuklear(20);

    /* Initialize the Gamepads */
    struct nk_gamepads gamepads;
    nk_gamepad_init(&gamepads, ctx, NULL);
    //nk_gamepad_init_with_source(&gamepads, ctx, nk_gamepad_keyboard_input_source(NULL));

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        UpdateNuklear(ctx);

        // Update the gamepad state
        nk_gamepad_update(&gamepads);

        // Render the gamepad demo
        nuklear_gamepad_demo(ctx, &gamepads, NULL);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawNuklear(ctx);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    nk_gamepad_free(&gamepads);

    UnloadNuklear(ctx);     // Unload the Nuklear GUI
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
