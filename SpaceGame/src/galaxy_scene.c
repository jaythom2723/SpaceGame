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
extern GameStar* stars;

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

void game_galaxy_map_init(EngineScene* scene, EngineContext* ctx, float deltaTime)
{
	printf("Inside Galaxy Map!\n");
}

void game_galaxy_map_update(EngineScene* scene, EngineContext* ctx, float deltaTime)
{
	if (ctx->mousebtns[GLFW_MOUSE_BUTTON_LEFT] == ETRUE)
	{
		EngineEntity* ent = NULL;
		for (int i = 0; i < scene->nentities; i++)
		{
			ent = engineGetEntityScene(ctx, GAME_GALAXY_MAP, i);
			if (ent == NULL)
				continue;

			float x, y, dist;
			x = (ent->x * ctx->zoomfac) - ctx->mousex;
			y = (ent->y * ctx->zoomfac) - ctx->mousey;
			dist = (x * x) + (y * y);
			float area = (ent->width * ctx->zoomfac) * (ent->height * ctx->zoomfac);

			if (dist <= area)
			{
				printf("Mouse Pos: (%f, %f)\n", ctx->mousex, ctx->mousey);
				printf("Entity Pos: (%f, %f)\n", ent->x, ent->y);
				printf("Clicked on a star of class: %s!\n", classStrings[stars[i].class]);
				break;
			}
		}
	}
}

void _setDrawColorFromStarClass(EngineContext* ctx, int index);

void game_galaxy_map_draw(EngineScene* scene, EngineContext* ctx, float deltaTime)
{
	for (int i = 0; i < scene->nentities; i++)
	{
		// TODO: internalize into the engine
		if (engineGetEntityScene(ctx, GAME_GALAXY_MAP, i) == NULL)
			continue;

		_setDrawColorFromStarClass(ctx, i);
		engineRenderEntity(ctx, i);
	}
}

static const vec3 starColors[STAR_CLASS_M + 1] = {
	{ 1.00f, 0.41f, 0.16f }, // giant
	{ 1.00f, 0.27f, 0.17f }, // super giant
	{ 0.25f, 0.25f, 0.25f }, // black hole
	{ 1.00f, 1.00f, 1.00f }, // white dwarf
	{ 0.45f, 0.55f, 1.00f }, // neutron
	{ 0.45f, 1.00f, 1.00f }, // pulsar
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
	if (index > GAME_MAX_STARS - 1)
		return;

	vec3 color;

	GameStarClass starClass = stars[index].class;
	color[0] = starColors[starClass][0];
	color[1] = starColors[starClass][1];
	color[2] = starColors[starClass][2];
	engineSetDrawColor(ctx, color[0], color[1], color[2]);
}
