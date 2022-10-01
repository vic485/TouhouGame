//
// Created by vic485 on 22/10/01.
//

#ifndef TOUHOUGAME_RENDER_H
#define TOUHOUGAME_RENDER_H

#include "types.h"

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif // _WIN32

SDL_Window *renderInit();
void renderBegin();
void renderEnd(SDL_Window *window);

#endif //TOUHOUGAME_RENDER_H
