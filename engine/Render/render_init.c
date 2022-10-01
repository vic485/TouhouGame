//
// Created by vic485 on 22/10/01.
//

#include "render_internal.h"
#include "../util.h"
#include "../IO/io.h"

#include <glad/glad.h>

SDL_Window *renderInitWindow(u32 width, u32 height) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) ERROR_EXIT("Failed to initialize SDL: %s", SDL_GetError())

    SDL_Window *window = SDL_CreateWindow("くそゲーム", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int) width,
                                          (int) height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window) ERROR_EXIT("Failed to create SDL window: %s", SDL_GetError())

    SDL_GL_CreateContext(window);
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) ERROR_EXIT("Failed to load GL: %s", SDL_GetError())

    return window;
}

void renderInitShaders(u32 *shaderDefault, u32 *shaderBatch, f32 renderWidth, f32 renderHeight) {
    mat4x4 projection;
    *shaderDefault = renderShaderCreate("./Assets/shaders/default.vert", "./Assets/shaders/default.frag");
    *shaderBatch = renderShaderCreate("./Assets/shaders/batch_quad.vert", "./Assets/shaders/batch_quad.frag");

    mat4x4_ortho(projection, 0, renderWidth, 0, renderHeight, -2, 2);

    glUseProgram(*shaderDefault);
    glUniformMatrix4fv(
            glGetUniformLocation(*shaderDefault, "projection"),
            1,
            GL_FALSE,
            &projection[0][0]
    );

    glUseProgram(*shaderBatch);
    glUniformMatrix4fv(
            glGetUniformLocation(*shaderBatch, "projection"),
            1,
            GL_FALSE,
            &projection[0][0]
    );
}

u32 renderShaderCreate(const char *pathVert, const char *pathFrag) {
    int success;
    char log[512];

    File fileVertex = io_fileRead(pathVert);
    if (!fileVertex.isValid) ERROR_EXIT("Error reading shader: %s\n", pathVert)

    u32 shaderVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderVertex, 1, (const char *const *) &fileVertex, NULL);
    glCompileShader(shaderVertex);
    glGetShaderiv(shaderVertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderVertex, 512, NULL, log);
        ERROR_EXIT("Error compiling vertex shader. %s\n", log)
    }

    File fileFragment = io_fileRead(pathFrag);
    if (!fileFragment.isValid) ERROR_EXIT("Error reading shader: %s\n", pathFrag)

    u32 shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderFragment, 1, (const char *const *) &fileFragment, NULL);
    glCompileShader(shaderFragment);
    glGetShaderiv(shaderFragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderFragment, 512, NULL, log);
        ERROR_EXIT("Error compiling fragment shader. %s\n", log)
    }

    u32 shader = glCreateProgram();
    glAttachShader(shader, shaderVertex);
    glAttachShader(shader, shaderFragment);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, NULL, log);
        ERROR_EXIT("Error linking shader. %s\n", log)
    }

    free(fileVertex.data);
    free(fileFragment.data);

    return shader;
}

void renderInitBatchQuads(u32 *vao, u32 *vbo, u32 *ebo) {
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    u32 indices[MAX_BATCH_ELEMENTS];
    for (u32 i = 0, offset = 0; i < MAX_BATCH_ELEMENTS; i += 6, offset += 4) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
    }

    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_BATCH_VERTICES * sizeof(BatchVertex), NULL, GL_DYNAMIC_DRAW);

    // [x, y], [u, v], [r, g, b, a]
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), (void *) offsetof(BatchVertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), (void *) offsetof(BatchVertex, uvs));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), (void *) offsetof(BatchVertex, color));

    glGenBuffers(1, ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_BATCH_ELEMENTS * sizeof(u32), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
