#include "engine_buffers.h"

#include <glad/glad.h>

#include <stdarg.h>

static inline void engineInitVertexAttribArrays(void)
{
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void engineCreateVAO(unsigned int* vao)
{
	glGenVertexArrays(1, vao);
}

void engineCreateVBO(unsigned int* vbo)
{
	glGenBuffers(1, vbo);
}

void engineDeleteVAO(unsigned int* vao)
{
	glBindVertexArray(0);
	glDeleteVertexArrays(1, vao);
}

void engineDeleteVBO(unsigned int* vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, vbo);
}

void engineInitVBOVAOPair(EngineContext* ctx, size_t vertBytes, float* vertices)
{
	unsigned int vbo;

	engineCreateVAO(&ctx->vao);
	engineCreateVBO(&vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	engineBindVAO(ctx->vao);

	glBufferData(GL_ARRAY_BUFFER, vertBytes, vertices, GL_STATIC_DRAW);

	engineInitVertexAttribArrays();

	engineBindVAO(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);
}

void engineBindVAO(unsigned int vao)
{
	glBindVertexArray(vao);
}