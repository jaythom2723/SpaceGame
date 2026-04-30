#include "engine_shaders.h"
#include "engine_utils.h"

#include <glad/glad.h>

#include <stdio.h>
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

void engineSetFloat(EngineContext* ctx, const char* name, float value)
{
	engineUseShader(ctx);

	int loc = glGetUniformLocation(ctx->program, name);
	if (loc == -1)
	{
		fprintf(stderr, "Unknown '%s' not found!\n", name);
		return;
	}

	glUniform1f(loc, value);
}

void engineSetInteger(EngineContext* ctx, const char* name, int value)
{
	engineUseShader(ctx);

	int loc = glGetUniformLocation(ctx->program, name);
	if (loc == -1)
	{
		fprintf(stderr, "Unknown '%s' not found!\n", name);
		return;
	}

	glUniform1i(loc, value);
}

void engineSetVector2f(EngineContext* ctx, const char* name, float x, float y)
{
	engineUseShader(ctx);

	int loc = glGetUniformLocation(ctx->program, name);
	if (loc == -1)
	{
		fprintf(stderr, "Unknown '%s' not found!\n", name);
		return;
	}

	glUniform2f(loc, x, y);
}

void engineSetVector2fv(EngineContext* ctx, const char* name, vec2 value)
{
	engineUseShader(ctx);

	int loc = glGetUniformLocation(ctx->program, name);
	if (loc == -1)
	{
		fprintf(stderr, "Unknown '%s' not found!\n", name);
		return;
	}

	glUniform2fv(loc, 1, value);
}

void engineSetVector3f(EngineContext* ctx, const char* name, float x, float y, float z)
{
	engineUseShader(ctx);

	int loc = glGetUniformLocation(ctx->program, name);
	if (loc == -1)
	{
		fprintf(stderr, "Unknown '%s' not found!\n", name);
		return;
	}

	glUniform3f(loc, x, y, z);
}

void engineSetVector3fv(EngineContext* ctx, const char* name, vec3 value)
{
	engineUseShader(ctx);

	int loc = glGetUniformLocation(ctx->program, name);
	if (loc == -1)
	{
		fprintf(stderr, "Unknown '%s' not found!\n", name);
		return;
	}

	glUniform3fv(loc, 1, value);
}

void engineSetVector4f(EngineContext* ctx, const char* name, float x, float y, float z, float w)
{
	engineUseShader(ctx);

	int loc = glGetUniformLocation(ctx->program, name);
	if (loc == -1)
	{
		fprintf(stderr, "Unknown '%s' not found!\n", name);
		return;
	}

	glUniform4f(loc, x, y, z, w);
}

void engineSetVector4fv(EngineContext* ctx, const char* name, vec4 value)
{
	engineUseShader(ctx);

	int loc = glGetUniformLocation(ctx->program, name);
	if (loc == -1)
	{
		fprintf(stderr, "Unknown '%s' not found!\n", name);
		return;
	}

	glUniform4fv(loc, 1, value);
}

void engineSetMatrix4fv(EngineContext* ctx, const char* name, mat4 value)
{
	engineUseShader(ctx);

	int loc = glGetUniformLocation(ctx->program, name);
	if (loc == -1)
	{
		fprintf(stderr, "Unknown '%s' not found!\n", name);
		return;
	}

	glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)value);
}