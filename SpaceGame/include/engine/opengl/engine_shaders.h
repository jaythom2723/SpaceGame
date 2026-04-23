#ifndef SHADER_H
#define SHADER_H

#include <cglm/cglm.h>

#include "engine_defs.h"

/**
 * \brief Create an OpenGL shader.
 * 
 * \param shaderType The type of shader to create
 * 
 * \return Returns the shader created by the engine
 */
EngineShader engineCreateShader(int shaderType);

/**
 * \brief Compile a GL shader via a file.
 * 
 * \param shader A valid shader
 * \param path The path to the shader file
 * 
 * \return Returns EFALSE if unsuccessful, ETRUE if successful
 */
const EBOOL engineCompileShader(EngineShader* shader, const char* path);

/**
 * \brief Delete a GL shader.
 * 
 * \param shader A pointer to a valid shader
 */
void engineDeleteShader(EngineShader* shader);

/**
 * \brief Create an OpenGL shader program.
 * 
 * \param ctx A valid context
 */
void engineCreateProgram(EngineContext* ctx);

/**
 * \brief Attach shaders and link the shader program.
 * 
 * \param ctx A valid context
 * \param vertex The vertex shader (required)
 * \param fragment The fragment shader (required)
 * \param geometry The geometry shader (or NULL)
 * 
 * \return Returns EFALSE if unsuccessful, ETRUE if successful
 */
const EBOOL engineAttachAndLink(EngineContext* ctx, EngineShader* vertex, EngineShader* fragment, EngineShader* geometry);

/**
 * \brief Delete the shader program.
 * 
 * \param ctx A valid context
 */
void engineDeleteProgram(EngineContext* ctx);

/**
 * \brief Begin using the engine's shader program.
 * 
 * \param ctx A valid context
 */
void engineUseShader(EngineContext* ctx);

/**
 * \brief Set a Shader Uniform of type Float.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param value The value to set
 */
void engineSetFloat(EngineContext* ctx, const char* name, float value);

/**
 * \brief Set a Shader Uniform of type Integer.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param value The value to set
 */
void engineSetInteger(EngineContext* ctx, const char* name, int value);

/**
 * \brief Set a Shader Uniform of type Vector2f.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param x The x value
 * \param y The y value
 */
void engineSetVector2f(EngineContext* ctx, const char* name, float x, float y);

/**
 * \brief Set a Shader Uniform of type Vector2fv.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param value The value to set
 */
void engineSetVector2fv(EngineContext* ctx, const char* name, vec2 value);

/**
 * \brief Set a Shader Uniform of type Vector3f.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param x The x value
 * \param y The y value
 * \param z The z value
 */
void engineSetVector3f(EngineContext* ctx, const char* name, float x, float y, float z);

/**
 * \brief Set a Shader Uniform of type Vector3fv.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param value The value to set
 */
void engineSetVector3fv(EngineContext* ctx, const char* name, vec3 value);

/**
 * \brief Set a Shader Uniform of type Vector4f.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param x The x value
 * \param y The y value
 * \param z The z value
 * \param w The w value
 */
void engineSetVector4f(EngineContext* ctx, const char* name, float x, float y, float z, float w);

/**
 * \brief Set a Shader Uniform of type Vector4fv.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param value The value to set
 */
void engineSetVector4fv(EngineContext* ctx, const char* name, vec4 value);

/**
 * \brief Set a Shader Uniform of type Matrix4fv.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param value The value to set
 */
void engineSetMatrix4fv(EngineContext* ctx, const char* name, mat4 value);

#endif // SHADER_H


