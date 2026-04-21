#ifndef ENGINE_BUFFERS_H
#define ENGINE_BUFFERS_H

#include "engine_defs.h"

#include <stdlib.h>

/**
 * \brief Create a Vertex Array Object (VAO).
 * 
 * \param vao A pointer to an uninitialized vao
 */
void engineCreateVAO(unsigned int* vao);

/**
 * \brief Create a Vertex Buffer Object (VBO).
 * 
 * \param vbo A pointer to an uninitialized vbo
 */
void engineCreateVBO(unsigned int* vbo);

/**
 * \brief Delete a VAO from OpenGL.
 * 
 * \param vao The vao to delete
 */
void engineDeleteVAO(unsigned int* vao);

/**
 * \brief Delete a VBO from OpenGL.
 * 
 * \param vbo The vbo to delete
 */
void engineDeleteVBO(unsigned int* vbo);

/**
 * \brief Initialize render data required for the vao and vbo.
 * 
 * \param ctx A valid context
 * \param vertBytes The size of our vertex array in bytes
 * \param vertices The vertices array
 */
void engineInitVBOVAOPair(EngineContext* ctx, size_t vertBytes, float* vertices);

/**
 * \brief Bind an OpenGL VAO.
 * 
 * \param vao The vao to bind (or zero)
 */
void engineBindVAO(unsigned int vao);

#endif // ENGINE_BUFFERS_H


