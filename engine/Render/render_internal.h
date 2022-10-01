//
// Created by vic485 on 22/10/01.
//

#ifndef TOUHOUGAME_RENDER_INTERNAL_H
#define TOUHOUGAME_RENDER_INTERNAL_H

#include "../render.h"

SDL_Window *renderInitWindow(u32 width, u32 height);

void renderInitShaders(u32 *shaderDefault, u32 *shaderBatch, f32 renderWidth, f32 renderHeight);

u32 renderShaderCreate(const char *pathVert, const char *pathFrag);

void renderInitBatchQuads(u32 *vao, u32 *vbo, u32 *ebo);

#endif //TOUHOUGAME_RENDER_INTERNAL_H
