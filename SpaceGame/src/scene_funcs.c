#include "scene_funcs.h"

#include "engine.h"
#include "engine_entity.h"
#include "engine_components.h"
#include "engine_renderer.h"
#include "engine_perlin.h"
#include "engine_utils.h"
#include "engine_shaders.h"
#include "engine_textures.h"
#include "engine_scene.h"

#include "game_defs.h"
#include "generator.h"

extern EngineTextureKey STAR_TEXTURE;
extern GameStar* stars;
extern GameSolarSystem* systems;

static EngineTextureKey MAP_TEXTURE_KEY = 0xFADA;

static const char* classStrings[STAR_CLASS_M + 1] = {
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

void game_generate_galaxy_map_init(EngineScene* scene, EngineContext* ctx, float deltaTime)
{
	printf("Generating a galaxy!\n");

	generateGalaxyStepOne(ctx, stars, MAP_TEXTURE_KEY);
	struct stardata* data = generateGalaxyStepTwo(ctx, stars);
	generateGalaxyStepThree(ctx, stars, data);

	EngineScene* map = engineGetScene(ctx, GAME_GALAXY_MAP);
	for (unsigned i = 0; i < map->nentities; i++) // generate the solar system linkage between galaxy map and solar system map
		systems[i].stars = &map->entities[i].key;

	int sums[STAR_CLASS_M + 1] = { 0 };

	for (int i = 0; i < GAME_MAX_STARS-1; i++)
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

