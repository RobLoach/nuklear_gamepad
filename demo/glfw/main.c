/* nuklear - 1.32.0 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "../../vendor/nuklear/nuklear.h"
#include "../../vendor/nuklear/demo/glfw_opengl3/nuklear_glfw_gl3.h"

#define NK_GAMEPAD_GLFW
#include "../../nuklear_gamepad.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include "../common/nuklear_gamepad_demo.c"

/* ===============================================================
 *
 *                          DEMO
 *
 * ===============================================================*/
static void error_callback(int e, const char *d)
{printf("Error %d: %s\n", e, d);}

int main(void)
{
    /* Platform */
    struct nk_glfw glfw = {0};
    static GLFWwindow *win;
    int width = 0, height = 0;
    struct nk_context *ctx;
    float font_scale = 2;

    /* GLFW */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "nuklear_gamepad", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwGetWindowSize(win, &width, &height);

    /* OpenGL */
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to setup GLEW\n");
        exit(1);
    }

    ctx = nk_glfw3_init(&glfw, win, NK_GLFW3_INSTALL_CALLBACKS);
    {
        struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&glfw, &atlas);
        struct nk_font_config config = nk_font_config(0);
        struct nk_font *font;
        font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config);
        nk_glfw3_font_stash_end(&glfw);
        nk_style_set_font(ctx, &font->handle);
    }

    /* Initialize the Gamepads */
    struct nk_gamepads* gamepads = nk_gamepad_init(ctx);

    while (!glfwWindowShouldClose(win))
    {
        /* Input */
        glfwPollEvents();

        /* Update the Gamepad state */
        nk_gamepad_update(gamepads);

        nk_glfw3_new_frame(&glfw);

        /* Render the gamepad demo */
        nuklear_gamepad_demo(ctx, gamepads);

        /* ----------------------------------------- */

        /* Draw */
        glfwGetWindowSize(win, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(win);
    }
    nk_glfw3_shutdown(&glfw);
    glfwTerminate();
    return 0;
}
