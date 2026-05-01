#include "scene_funcs.h"

#include "engine.h"
#include "engine_entity.h"
#include "engine_components.h"
#include "engine_renderer.h"
#include "engine_perlin.h"
#include "engine_utils.h"

#include "game_defs.h"
#include "generator.h"

extern EngineTextureKey STAR_TEXTURE;
extern GameStar stars[GAME_MAX_STARS];

void game_generate_galaxy_map_init(EngineScene* scene, EngineContext* ctx, float deltaTime)
{
	printf("Generating Galaxy Map!\n");

	enginePerlinInit(GAME_GALAXY_WIDTH, GAME_GALAXY_HEIGHT);
	float* noise = enginePerlinGenerate();

	generateStars(ctx, stars, noise);

	free(noise);
	noise = NULL;
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
