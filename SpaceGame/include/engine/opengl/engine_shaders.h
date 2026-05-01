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

EngineProgram engineCreateLocalProgram(void);

const EBOOL engineAttachAndLinkv(EngineContext* ctx, int numShaders, ...);
const EBOOL engineAttachAndLinklv(EngineProgram program, int numShaders, ...);

#define engineAttachAndLink(ctx,vrt,frg,geo) engine_renamed_engineAttachAndLinkv

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
 * \brief Use a locally created shader program, as opposed to a valid context's.
 * 
 * \param program A valid OpenGL Shader Program
 */
void engineUseShaderl(EngineProgram program);

/**
 * \brief Invoke a compute shader, return the data as a specified type array.
 * 
 * \param width The width of the noise texture to generate
 * \param height The height of the noise texture to generate
 * \param ptr The destination for all of the data
 * \param tsize The size of the type returned in bytes.
 * \param ssbo Shader Storage Buffer Object
 */
void engineInvokeComputeShader(int width, int height, void** ptr, size_t tsize, int ssbo);

/**
 * \brief Set a Shader Uniform of type Float.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param value The value to set
 */
void engineSetFloat(EngineContext* ctx, const char* name, float value);
void engineSetFloatl(EngineProgram program, const char* name, float value);

/**
 * \brief Set a Shader Uniform of type Integer.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param value The value to set
 */
void engineSetInteger(EngineContext* ctx, const char* name, int value);
void engineSetIntegerl(EngineProgram program, const char* name, int value);

/**
 * \brief Set a Shader Uniform of type Vector2f.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param x The x value
 * \param y The y value
 */
void engineSetVector2f(EngineContext* ctx, const char* name, float x, float y);
void engineSetVector2fl(EngineProgram program, const char* name, float x, float y);

/**
 * \brief Set a Shader Uniform of type Vector2fv.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param value The value to set
 */
void engineSetVector2fv(EngineContext* ctx, const char* name, vec2 value);
void engineSetVector2fvl(EngineProgram program, const char* name, vec2 value);

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
void engineSetVector3fl(EngineProgram program, const char* name, float x, float y, float z);

/**
 * \brief Set a Shader Uniform of type Vector3fv.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param value The value to set
 */
void engineSetVector3fv(EngineContext* ctx, const char* name, vec3 value);
void engineSetVector3fvl(EngineProgram program, const char* name, vec3 value);

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
void engineSetVector4fl(EngineProgram program, const char* name, float x, float y, float z, float w);

/**
 * \brief Set a Shader Uniform of type Vector4fv.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param value The value to set
 */
void engineSetVector4fv(EngineContext* ctx, const char* name, vec4 value);
void engineSetVector4fvl(EngineProgram program, const char* name, vec4 value);

/**
 * \brief Set a Shader Uniform of type Matrix4fv.
 * 
 * \param ctx A valid context
 * \param name The name of the uniform to set
 * \param value The value to set
 */
void engineSetMatrix4fv(EngineContext* ctx, const char* name, mat4 value);
void engineSetMatrix4fvl(EngineProgram program, const char* name, mat4 value);

#endif // SHADER_H


