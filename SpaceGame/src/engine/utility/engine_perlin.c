#include "engine_perlin.h"

#include "engine.h"
#include "engine_utils.h"
#include "engine_shaders.h"
#include "engine_textures.h"
#include "engine_logger.h"

#include <glad/glad.h>

#include <string.h>

static EngineProgram noiseProgram;
static EngineProgram maskProgram;

static EngineShader perlinShader;
static EngineShader maskShader;

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

void engineSetNoiseMask(EngineContext* ctx, const EngineNoiseMask* const mask, const EngineTextureKey texkey)
{
	engineWriteMessage(ctx, "Setting Perlin Noise Mask...", ELOG_MSGTYPE_INFORM);
	if (mask->type == ENGINE_NOISEMASK_GALAXY)
	{
		maskProgram = engineCreateLocalProgram();
		maskShader = engineCreateShader(ENGINE_COMPUTE_SHADER);
		engineCompileShader(&maskShader, "Shaders/noise_starmask.glsl");
		engineAttachAndLinklv(maskProgram, 1, &maskShader);
		engineUseShaderl(maskProgram);

		engineCreateTexture(ctx, texkey);
		engineGenEmptyTexture(ctx, texkey, ETRUE, mask->width, mask->height);

		engineSetIntegerl(maskProgram, "width", mask->width);
		engineSetIntegerl(maskProgram, "height", mask->height);

		if (mask->mask_galaxy.bulgeintensity != -1)
			engineSetFloatl(maskProgram, "bulgeIntensity", mask->mask_galaxy.bulgeintensity);

		if (mask->mask_galaxy.bulgecenter != -1)
			engineSetFloatl(maskProgram, "centerBulge", mask->mask_galaxy.bulgecenter);

		if (mask->mask_galaxy.armcurve != -1)
			engineSetFloatl(maskProgram, "armCurve", mask->mask_galaxy.armcurve);

		if (mask->mask_galaxy.arms != -1)
			engineSetFloatl(maskProgram, "arms", mask->mask_galaxy.arms);

		if (mask->mask_galaxy.armthickness != -1)
			engineSetFloatl(maskProgram, "armThickness", mask->mask_galaxy.armthickness);

		if (mask->mask_galaxy.inradius != -1)
			engineSetFloatl(maskProgram, "innerRadius", mask->mask_galaxy.inradius);

		if (mask->mask_galaxy.outradius != -1)
			engineSetFloatl(maskProgram, "outerRadius", mask->mask_galaxy.outradius);
	}
}

void _engine_initPerlinComputeShader(void)
{
	perlinShader = engineCreateShader(GL_COMPUTE_SHADER);
	engineCompileShader(&perlinShader, "Shaders/noise_perlin.glsl");

	noiseProgram = engineCreateLocalProgram();
	engineAttachAndLinklv(noiseProgram, 1, &perlinShader);
}

float* enginePerlinGenerate(void)
{
	engineUseShaderl(noiseProgram);
	engineSetFloatl(noiseProgram, "u_scale", 5.0f);
	engineSetIntegerl(noiseProgram, "width", _engine_perlinwidth);
	engineSetIntegerl(noiseProgram, "height", _engine_perlinheight);

	float* ptr = NULL;
	engineInvokeComputeShader(_engine_perlinwidth, _engine_perlinheight, &ptr, sizeof(float), _engine_perlinssbo);
	if (ptr == NULL)
		return NULL;

	return ptr;
}

float* enginePerlinGenerateMask(EngineContext* ctx, EngineTextureKey texkey)
{
	engineWriteMessage(ctx, "Generating perlin noise!", ELOG_MSGTYPE_INFORM);
	engineUseShaderl(maskProgram);
	engineBindImageTexture(ctx, texkey, ENGINE_IMG_WRITE_ONLY, ENGINE_RGBA32F);
	engineInvokeComputeShaderImage(_engine_perlinwidth, _engine_perlinheight);
	engineBindImageTexture(ctx, texkey, ENGINE_IMG_READ_ONLY, ENGINE_RGBA32F);

	return enginePerlinGenerate();
}

void enginePerlinClose(EngineContext* ctx, EngineTextureKey texkey)
{
	engineWriteMessage(ctx, "Destroying perlin noise data", ELOG_MSGTYPE_INFORM);
	extern EngineTexturePair* _engineGetTexturePair(EngineContext*, EngineTextureKey,unsigned int*);

	int index = 0;
	EngineTexturePair* pair = _engineGetTexturePair(ctx, texkey, &index);
	assert(pair != NULL);

	if (glIsTexture(pair->value->id) == GL_TRUE)
		engineDestroyTexture(ctx, texkey);

	if (glIsProgram(maskProgram) == GL_TRUE)
		glDeleteProgram(maskProgram);

	glDeleteProgram(noiseProgram);
	glDeleteBuffers(1, &_engine_perlinssbo);
}
