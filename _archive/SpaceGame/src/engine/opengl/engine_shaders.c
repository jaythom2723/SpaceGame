#include "engine_shaders.h"
#include "engine_utils.h"

#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

EngineShader engineCreateShader(int shaderType)
{
	return glCreateShader((GLenum)shaderType);
}

const EBOOL engineCompileShader(EngineShader* shader, const char* path)
{
	char* source = engineReadFile(path);
	int success;
	char infoLog[512];

	glShaderSource(*shader, 1, &source, NULL);
	glCompileShader(*shader);
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(*shader, 512, NULL, infoLog);
		fprintf(stderr, "%s\n", infoLog);
		free(source);
		return EFALSE;
	}

	free(source);
	return ETRUE;
}

void engineDeleteShader(EngineShader* shader)
{
	glDeleteShader(*shader);
}

void engineCreateProgram(EngineContext* ctx)
{
	ctx->program = glCreateProgram();
}

EngineProgram engineCreateLocalProgram(void)
{
	return glCreateProgram();
}

const EBOOL engineAttachAndLinkv(EngineContext* ctx, int numShaders, ...)
{
	EngineShader* shaders[0x4] = { 0 };

	va_list args;
	va_start(args, numShaders);

	for (int i = 0; i < numShaders; i++)
	{
		shaders[i] = va_arg(args, EngineShader*);
	}

	va_end(args);

	return engineAttachAndLinklv(ctx->program, numShaders, shaders[0], shaders[1], shaders[2], shaders[3]);
}

const EBOOL engineAttachAndLinklv(EngineProgram program, int numShaders, ...)
{
	va_list args, arg2;

	va_start(args, numShaders);
	va_start(arg2, numShaders);

	for (int i = 0; i < numShaders; i++)
		glAttachShader(program, *va_arg(args, EngineShader*));

	va_end(args);

	// link program
	int success;
	char infoLog[512];
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		fprintf(stderr, "%s\n", infoLog);
		return EFALSE;
	}

	for (int i = 0; i < numShaders; i++)
		glDeleteShader(*va_arg(arg2, EngineShader*));

	glUseProgram(program);

	va_end(arg2);

	return ETRUE;
}

void engineDeleteProgram(EngineContext* ctx)
{
	glDeleteProgram(ctx->program);
}

void engineUseShader(EngineContext* ctx)
{
	engineUseShaderl(ctx->program);
}

void engineUseShaderl(EngineProgram program)
{
	glUseProgram(program);
}

void engineInvokeComputeShader(int width, int height, void** ptr, size_t tsize, int ssbo)
{
	glDispatchCompute(width / 8, height / 8, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	
	*ptr = malloc(width * height * tsize);
	assert(*ptr != NULL);

	void* tmp = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	memcpy(*ptr, tmp, width * height * tsize);
	tmp = NULL;
}

void engineInvokeComputeShaderImage(int width, int height)
{
	glDispatchCompute(width / 8, height / 8, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

#define LOC_ERROR(l,n) if (l == -1) { fprintf(stderr, "Unknown '%s' not found!\n", n); return; }

void engineSetFloatl(EngineProgram program, const char* name, float value)
{
	engineUseShaderl(program);

	int loc = glGetUniformLocation(program, name);
	LOC_ERROR(loc, name);

	glUniform1f(loc, value);
}

void engineSetIntegerl(EngineProgram program, const char* name, int value)
{
	engineUseShaderl(program);

	int loc = glGetUniformLocation(program, name);
	LOC_ERROR(loc, name);

	glUniform1i(loc, value);
}

void engineSetVector2fl(EngineProgram program, const char* name, float x, float y)
{
	engineUseShaderl(program);

	int loc = glGetUniformLocation(program, name);
	LOC_ERROR(loc, name);

	glUniform2f(loc, x, y);
}

void engineSetVector2fvl(EngineProgram program, const char* name, vec2 value)
{
	engineUseShaderl(program);

	int loc = glGetUniformLocation(program, name);
	LOC_ERROR(loc, name);

	glUniform2fv(loc, 1, value);
}

void engineSetVector3fl(EngineProgram program, const char* name, float x, float y, float z)
{
	engineUseShaderl(program);

	int loc = glGetUniformLocation(program, name);
	LOC_ERROR(loc, name);

	glUniform3f(loc, x, y, z);
}

void engineSetVector3fvl(EngineProgram program, const char* name, vec3 value)
{
	engineUseShaderl(program);
	
	int loc = glGetUniformLocation(program, name);
	LOC_ERROR(loc, name);

	glUniform3fv(loc, 1, value);
}

void engineSetVector4fl(EngineProgram program, const char* name, float x, float y, float z, float w)
{
	engineUseShaderl(program);

	int loc = glGetUniformLocation(program, name);
	LOC_ERROR(loc, name);

	glUniform4f(loc, x, y, z, w);
}

void engineSetVector4fvl(EngineProgram program, const char* name, vec4 value)
{
	engineUseShaderl(program);

	int loc = glGetUniformLocation(program, name);
	LOC_ERROR(loc, name);

	glUniform4fv(loc, 1, value);
}

void engineSetMatrix4fvl(EngineProgram program, const char* name, mat4 value)
{
	engineUseShaderl(program);

	int loc = glGetUniformLocation(program, name);
	LOC_ERROR(loc, name);

	glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)value);
}

void engineSetFloat(EngineContext* ctx, const char* name, float value)
{
	engineSetFloatl(ctx->program, name, value);
}

void engineSetInteger(EngineContext* ctx, const char* name, int value)
{
	engineSetIntegerl(ctx->program, name, value);
}

void engineSetVector2f(EngineContext* ctx, const char* name, float x, float y)
{
	engineSetVector2fl(ctx->program, name, x, y);
}

void engineSetVector2fv(EngineContext* ctx, const char* name, vec2 value)
{
	engineSetVector2fvl(ctx->program, name, value);
}

void engineSetVector3f(EngineContext* ctx, const char* name, float x, float y, float z)
{
	engineSetVector3fl(ctx->program, name, x, y, z);
}

void engineSetVector3fv(EngineContext* ctx, const char* name, vec3 value)
{
	engineSetVector3fvl(ctx->program, name, value);
}

void engineSetVector4f(EngineContext* ctx, const char* name, float x, float y, float z, float w)
{
	engineSetVector4fl(ctx->program, name, x, y, z, w);
}

void engineSetVector4fv(EngineContext* ctx, const char* name, vec4 value)
{
	engineSetVector4fvl(ctx->program, name, value);
}

void engineSetMatrix4fv(EngineContext* ctx, const char* name, mat4 value)
{
	engineSetMatrix4fvl(ctx->program, name, value);
}