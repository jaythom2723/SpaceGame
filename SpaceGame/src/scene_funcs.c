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

	generationStepOne(ctx, stars, MAP_TEXTURE_KEY);
	struct stardata* data = generationStepTwo(ctx, stars);
	generationStepThree(ctx, stars, data);
	
	int sums[STAR_CLASS_M + 1] = { 0 };
	const char* classStrings[STAR_CLASS_M + 1] = {
		"Giant",
		"SGiant",
		"BHole",
		"WDwarf",
		"Neutron",
		"Pulsar",
		"Quasar",
		"O",
		"B",
		"A",
		"F",
		"G",
		"K",
		"M",
	};

	for (int i = 0; i < GAME_MAX_STARS; i++)
	{
		if (stars[i].entity == -1)
			continue;

		for (int j = 0; j < STAR_CLASS_M + 1; j++)
		{
			if (stars[i].class == j)
				sums[j] += 1;
		}
	}

	printf("Number of stars based on class: \n");
	for (int i = 0; i < STAR_CLASS_M + 1; i++)
	{
		float percentage = (float)sums[i] / (float)GAME_MAX_STARS;
		printf("%s\t\t|\t\t%d\t\t|%.02f%%\n", classStrings[i], sums[i], percentage);
	}

	free(data);
	data = NULL;

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
		if (engineGetEntityScene(ctx, GAME_GALAXY_MAP, i) == NULL)
			continue;

		_setDrawColorFromStarClass(ctx, i);
		engineRenderEntity(ctx, i);
	}
}

static const vec3 starColors[STAR_CLASS_M + 1] = {
	{ 0.45f, 0.55f, 1.00f }, // giant
	{ 0.45f, 1.00f, 1.00f }, // super giant
	{ 0.25f, 0.25f, 0.25f }, // black hole
	{ 1.00f, 1.00f, 1.00f }, // white dwarf
	{ 1.00f, 0.41f, 0.16f }, // neutron
	{ 1.00f, 0.27f, 0.17f }, // pulsar
	{ 0.58f, 0.41f, 0.36f }, // quasar
	{ 0.60f, 0.70f, 1.00f }, // O
	{ 0.70f, 0.80f, 1.00f }, // B
	{ 0.85f, 0.85f, 1.00f }, // A
	{ 1.00f, 1.00f, 0.90f }, // F
	{ 1.00f, 0.95f, 0.80f }, // G
	{ 1.00f, 0.80f, 0.50f }, // K
	{ 1.00f, 0.60f, 0.40f }, // M
};

void _setDrawColorFromStarClass(EngineContext* ctx, int index)
{
	if (index > GAME_MAX_STARS)
		return;

	vec3 color;

	GameStarClass starClass = stars[index].class;
	color[0] = starColors[starClass][0];
	color[1] = starColors[starClass][1];
	color[2] = starColors[starClass][2];
	engineSetDrawColor(ctx, color[0], color[1], color[2]);
}
