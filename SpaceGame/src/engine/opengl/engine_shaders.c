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

const EBOOL engineAttachAndLink(EngineContext* ctx, EngineShader* vertex, EngineShader* fragment, EngineShader* geometry)
{
	// attach shaders
	glAttachShader(ctx->program, *vertex);
	glAttachShader(ctx->program, *fragment);

	if (geometry != NULL)
		glAttachShader(ctx->program, *geometry);

	// link program
	int success;
	char infoLog[512];
	glLinkProgram(ctx->program);
	glGetProgramiv(ctx->program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ctx->program, 512, NULL, infoLog);
		fprintf(stderr, "%s\n", infoLog);
		return EFALSE;
	}

	glDeleteShader(*vertex);
	glDeleteShader(*fragment);

	if (geometry != NULL)
		glDeleteShader(*geometry);

	glUseProgram(ctx->program);

	return ETRUE;
}

void engineDeleteProgram(EngineContext* ctx)
{
	glDeleteProgram(ctx->program);
}

void engineUseShader(EngineContext* ctx)
{
	glUseProgram(ctx->program);
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