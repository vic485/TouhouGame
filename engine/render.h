//
// Created by vic485 on 22/10/01.
//

#ifndef TOUHOUGAME_RENDER_H
#define TOUHOUGAME_RENDER_H

#include "types.h"
#include <linmath.h>

#ifdef _WIN32
#include <SDL.h>
#else

#include <SDL2/SDL.h>

#endif // _WIN32

typedef struct batch_vertex {
    vec2 position;
    vec2 uvs;
    vec4 color;
} BatchVertex;

typedef struct sprite_sheet {
    f32 width;
    f32 height;
    f32 cellWidth;
    f32 cellHeight;
    u32 textureId;
} SpriteSheet;

#define MAX_BATCH_QUADS 10000
#define MAX_BATCH_VERTICES 40000
#define MAX_BATCH_ELEMENTS 60000

SDL_Window *renderInit();

void renderBegin();

void renderEnd(SDL_Window *window, u32 batchTextureId);

void render_spriteSheetInit(SpriteSheet *spriteSheet, const char *path, f32 cellWidth, f32 cellHeight);

void render_spriteSheetFrame(SpriteSheet *spriteSheet, f32 row, f32 column, vec2 position);

#endif //TOUHOUGAME_RENDER_H
