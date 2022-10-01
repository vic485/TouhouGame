#include <stdbool.h>

#include "engine/render.h"
#include "engine/util.h"

int main(int argc, char **argv) {
    FILE *logFile = fopen("log.txt", "w");
    log_add_fp(logFile, LOG_DEBUG);

    SDL_Window *window = renderInit();

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            } else if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        renderBegin();
        // Do rendering
        renderEnd(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    fclose(logFile);
    return 0;
}
