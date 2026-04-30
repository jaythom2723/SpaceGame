#include "engine_perlin.h"

#include "engine.h"
#include "engine_utils.h"
#include "engine_shaders.h"

#include <glad/glad.h>

#include <string.h>

static EngineProgram program;
static EngineShader perlinShader;

static int _engine_perlinssbo;
static int _engine_perlinwidth;
static int _engine_perlinheight;

void _engine_initPerlinComputeShader(void);

void enginePerlinInit(const int width, const int height)
{
	_engine_initPerlinComputeShader();

	// create shader storage buffer
	glGenBuffers(1, &_engine_perlinssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _engine_perlinssbo);
	
	size_t sizeData = width * height * sizeof(float);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeData, NULL, GL_DYNAMIC_READ);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _engine_perlinssbo);

	_engine_perlinwidth = width;
	_engine_perlinheight = height;
}

void _engine_initPerlinComputeShader(void)
{
	perlinShader = engineCreateShader(GL_COMPUTE_SHADER);
	engineCompileShader(&perlinShader, "Shaders/noise_perlin.glsl");

	program = engineCreateLocalProgram();
	engineAttachAndLinklv(program, 1, &perlinShader);
}

float* enginePerlinGenerate(void)
{
	engineUseShaderl(program);

	glUniform1i(glGetUniformLocation(program, "width"), _engine_perlinwidth);
	glUniform1i(glGetUniformLocation(program, "height"), _engine_perlinheight);
	glUniform1f(glGetUniformLocation(program, "u_scale"), 5.0);

	glDispatchCompute(_engine_perlinwidth / 8, _engine_perlinheight / 8, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _engine_perlinssbo);

	float* ptr = malloc(_engine_perlinwidth * _engine_perlinheight * sizeof(float));
	assert(ptr != NULL);

	float* tmp = (float*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	memcpy(ptr, tmp, _engine_perlinwidth * _engine_perlinheight * sizeof(float));
	tmp = NULL;

	return ptr;
}

void enginePerlinClose(void)
{
	glDeleteProgram(program);
	glDeleteBuffers(1, &_engine_perlinssbo);
}
