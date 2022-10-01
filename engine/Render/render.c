//
// Created by vic485 on 22/10/01.
//

#include "../render.h"
#include "render_internal.h"
#include "../ArrayList/ArrayList.h"
#include "../util.h"

#include <glad/glad.h>
#include <qoi.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define RENDER_WIDTH 640
#define RENDER_HEIGHT 360
#define RENDER_SCALE 2

static u32 shaderDefault;
static u32 vaoBatch;
static u32 vboBatch;
static u32 eboBatch;
static u32 shaderBatch;
static ArrayList *listBatch;

SDL_Window *renderInit() {
    SDL_Window *window = renderInitWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

    renderInitBatchQuads(&vaoBatch, &vboBatch, &eboBatch);
    renderInitShaders(&shaderDefault, &shaderBatch, RENDER_WIDTH, RENDER_HEIGHT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    listBatch = arrayListCreate(sizeof(BatchVertex), 8);

    return window;
}

void renderBegin() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    listBatch->length = 0;
}

static void renderBatch(BatchVertex *vertices, usize count, u32 textureId) {
    glBindBuffer(GL_ARRAY_BUFFER, vboBatch);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (long) (count * sizeof(BatchVertex)), vertices);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glUseProgram(shaderBatch);
    glBindVertexArray(vaoBatch);

    glDrawElements(GL_TRIANGLES, (int) (count >> 2) * 6, GL_UNSIGNED_INT, NULL);
}

static void appendQuad(vec2 position, vec2 size, vec4 textureCoordinates, vec4 color) {
    vec4 uvs = {0, 0, 1, 1};

    if (textureCoordinates != NULL) {
        memcpy(uvs, textureCoordinates, sizeof(vec4));
    }

    arrayListAppend(listBatch, &(BatchVertex) {
            .position = {position[0], position[1]},
            .uvs = {uvs[0], uvs[1]},
            .color = {color[0], color[1], color[2], color[3]},
    });

    arrayListAppend(listBatch, &(BatchVertex) {
            .position = {position[0] + size[0], position[1]},
            .uvs = {uvs[2], uvs[1]},
            .color = {color[0], color[1], color[2], color[3]},
    });

    arrayListAppend(listBatch, &(BatchVertex) {
            .position = {position[0] + size[0], position[1] + size[1]},
            .uvs = {uvs[2], uvs[3]},
            .color = {color[0], color[1], color[2], color[3]},
    });

    arrayListAppend(listBatch, &(BatchVertex) {
            .position = {position[0], position[1] + size[1]},
            .uvs = {uvs[0], uvs[3]},
            .color = {color[0], color[1], color[2], color[3]},
    });
}

void renderEnd(SDL_Window *window, u32 batchTextureId) {
    renderBatch(listBatch->items, listBatch->length, batchTextureId);
    SDL_GL_SwapWindow(window);
}

void render_spriteSheetInit(SpriteSheet *spriteSheet, const char *path, f32 cellWidth, f32 cellHeight) {
    glGenTextures(1, &spriteSheet->textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, spriteSheet->textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    qoi_desc desc;
    void *image = qoi_read(path, &desc, 4);
    if (!image) ERROR_EXIT("Failed to load image: %s", path)

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int) desc.width, (int) desc.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    spriteSheet->width = (f32) desc.width;
    spriteSheet->height = (f32) desc.height;
    spriteSheet->cellWidth = cellWidth;
    spriteSheet->cellHeight = cellHeight;
}

static void
calculateSpriteSheetTextureCoords(vec4 result, f32 row, f32 column, f32 textureWidth, f32 textureHeight, f32 cellWidth,
                                  f32 cellHeight) {
    f32 w = 1.0f / (textureWidth / cellWidth);
    f32 h = 1.0f / (textureHeight / cellHeight);
    f32 x = column * w;
    f32 y = row * h;
    result[0] = x;
    //result[1] = y;
    result[2] = x + w;
    //result[3] = y + h;
    // Flip image vertically, so it displays properly
    result[1] = y + h;
    result[3] = y;
}

void render_spriteSheetFrame(SpriteSheet *spriteSheet, f32 row, f32 column, vec2 position) {
    vec4 uvs;
    calculateSpriteSheetTextureCoords(uvs, row, column, spriteSheet->width, spriteSheet->height, spriteSheet->cellWidth,
                                      spriteSheet->cellHeight);

    vec2 size = {spriteSheet->cellWidth, spriteSheet->cellHeight};
    vec2 bottomLeft = {position[0] - size[0] * 0.5f, position[1] - size[1] * 0.5f};
    appendQuad(bottomLeft, size, uvs, (vec4) {1, 1, 1, 1});
}
