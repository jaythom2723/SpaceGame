#ifndef ENGINE_BUFFERS_H
#define ENGINE_BUFFERS_H

#include "engine_defs.h"

#include <stdlib.h>

void engineCreateVAO(unsigned int* vao);
void engineCreateVBO(unsigned int* vbo);

void engineDeleteVAO(unsigned int* vao);
void engineDeleteVBO(unsigned int* vbo);

void engineInitVBOVAOPair(EngineContext* ctx, size_t vertBytes, float* vertices);

void engineBindVAO(unsigned int vao);

#endif // ENGINE_BUFFERS_H