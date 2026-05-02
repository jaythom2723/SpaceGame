#include "scene_funcs.h"

#include "engine.h"
#include "engine_entity.h"
#include "engine_components.h"
#include "engine_renderer.h"
#include "engine_perlin.h"
#include "engine_utils.h"
#include "engine_shaders.h"
#include "engine_textures.h"

#include "game_defs.h"
#include "generator.h"

extern EngineTextureKey STAR_TEXTURE;
extern GameStar stars[GAME_MAX_STARS];

static EngineTextureKey MAP_TEXTURE_KEY = 0xFADA;

void game_generate_galaxy_map_init(EngineScene* scene, EngineContext* ctx, float deltaTime)
{
	printf("Generating a galaxy!\n");

	// Step 1: Create the mask program, link it, and use it
	EngineProgram maskProgram = engineCreateLocalProgram();
	EngineShader maskShader = engineCreateShader(ENGINE_COMPUTE_SHADER);
	engineCompileShader(&maskShader, "Shaders/noise_starmask.glsl");
	engineAttachAndLinklv(maskProgram, 1, &maskShader);
	engineUseShaderl(maskProgram);

	// Step 2: Create the texture to write to and bind it
	engineCreateTexture(ctx, MAP_TEXTURE_KEY);
	engineGenEmptyTexture(ctx, MAP_TEXTURE_KEY, ETRUE, GAME_GALAXY_WIDTH, GAME_GALAXY_HEIGHT);
	engineBindImageTexture(ctx, MAP_TEXTURE_KEY, ENGINE_IMG_WRITE_ONLY, ENGINE_RGBA32F);

	// Step 3: Set the uniforms & invoke
	engineSetIntegerl(maskProgram, "width", GAME_GALAXY_WIDTH);
	engineSetIntegerl(maskProgram, "height", GAME_GALAXY_HEIGHT);
	engineSetFloatl(maskProgram, "armCurve", 10.0);
	engineSetFloatl(maskProgram, "bulgeIntensity", 1.5);
	engineInvokeComputeShaderImage(GAME_GALAXY_WIDTH, GAME_GALAXY_HEIGHT);

	// Step 4: Initialize Perlin Noise
	enginePerlinInit(GAME_GALAXY_WIDTH, GAME_GALAXY_HEIGHT);
	
	// Step 5: Rebind the image stored as read only
	engineBindImageTexture(ctx, MAP_TEXTURE_KEY, ENGINE_IMG_READ_ONLY, ENGINE_RGBA32F);

	// Step 6: Run
	float* noise = enginePerlinGenerate();
	float sum = 0.f;

	for (int y = 0; y < GAME_GALAXY_WIDTH; y++)
	{
		for (int x = 0; x < GAME_GALAXY_HEIGHT; x++)
		{
			int index = y * GAME_GALAXY_WIDTH + x;
			sum += noise[index];
		}
	}

	printf("Average of all generated points: %.02f\n", sum / (GAME_GALAXY_WIDTH * GAME_GALAXY_HEIGHT));

	generateStars(ctx, stars, noise);

	free(noise);
	enginePerlinClose();

	engineSetGameState(ctx, GAME_GALAXY_MAP);
}

void game_generate_solar_system_init(EngineScene* scene, EngineContext* ctx, float deltaTime)
{
	printf("Generating Solar System!\n");
	engineSetGameState(ctx, GAME_GENERATE_PLANET_MAP);
}

void game_generate_planet_map_init(EngineScene* scene, EngineContext* ctx, float deltaTime)
{
	printf("Generating Planet!\n");
	engineSetGameState(ctx, GAME_GALAXY_MAP);
}

void game_galaxy_map_init(EngineScene* scene, EngineContext* ctx, float deltaTime)
{
	printf("Inside Galaxy Map!\n");
}

void game_galaxy_map_update(EngineScene* scene, EngineContext* ctx, float deltaTime)
{
	
}

void _setDrawColorFromStarClass(EngineContext* ctx, int index);

void game_galaxy_map_draw(EngineScene* scene, EngineContext* ctx, float deltaTime)
{
	vec2 size = {
		GAME_GALAXY_WIDTH,
		GAME_GALAXY_HEIGHT
	};

	engineSetDrawColor(ctx, 1.0f, 1.0f, 1.0f);
	engineRenderTexture(ctx, MAP_TEXTURE_KEY, (vec2) { 0.0f, 0.0f }, size);

	for (int i = 0; i < scene->nentities; i++)
	{
		_setDrawColorFromStarClass(ctx, i);
		engineRenderEntity(ctx, i);
	}
}

void _setDrawColorFromStarClass(EngineContext* ctx, int index)
{
	switch (stars[index].class)
	{
	case STAR_CLASS_O:
		engineSetDrawColor(ctx, 0.60f, 0.70f, 1.0f);
		break;
	case STAR_CLASS_B:
		engineSetDrawColor(ctx, 0.70f, 0.80f, 1.00f);
		break;
	case STAR_CLASS_A:
		engineSetDrawColor(ctx, 0.85f, 0.85f, 1.00f);
		break;
	case STAR_CLASS_F:
		engineSetDrawColor(ctx, 1.00f, 1.00f, 0.90f);
		break;
	case STAR_CLASS_G:
		engineSetDrawColor(ctx, 1.00f, 0.95f, 0.80f);
		break;
	case STAR_CLASS_K:
		engineSetDrawColor(ctx, 1.00f, 0.80f, 0.50f);
		break;
	case STAR_CLASS_M:
		engineSetDrawColor(ctx, 1.00f, 0.60f, 0.40f);
		break;
	default:
		engineSetDrawColor(ctx, 1.0f, 1.0f, 1.0f);
		break;
	}
}
