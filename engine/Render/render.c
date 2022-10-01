//
// Created by vic485 on 22/10/01.
//

#include "../render.h"
#include "../util.h"

#include <glad/glad.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define RENDER_WIDTH 640
#define RENDER_HEIGHT 360
#define RENDER_SCALE 2

SDL_Window *renderInitWindow() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) ERROR_EXIT("Failed to initialize SDL: %s", SDL_GetError())

    SDL_Window *window = SDL_CreateWindow("くそゲーム", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                          WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window) ERROR_EXIT("Failed to create SDL window: %s", SDL_GetError())

    SDL_GL_CreateContext(window);
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) ERROR_EXIT("Failed to load GL: %s", SDL_GetError())

    return window;
}

SDL_Window *renderInit() {
    SDL_Window *window = renderInitWindow();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return window;
}

void renderBegin() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderEnd(SDL_Window *window) {
    SDL_GL_SwapWindow(window);
}
