#include "generator.h"
#include "generator_defs.h"

#include "engine.h"
#include "engine_utils.h"
#include "engine_entity.h"
#include "engine_components.h"
#include "engine_perlin.h"
#include "engine_scene.h"
#include "engine_math.h"
#include "engine_logger.h"

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

extern EngineTextureKey STAR_TEXTURE;

const char* STAR_CLASS_TRANSLATIONS[STAR_CLASS_M + 1] = {
	"O", "B", "A", "F", "G", "K", "M"
};

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

void _generateStar(GameStar* stars, int index);
double _generateMass(double chance);
void _determineSpectralClass(GameStar* star);

void _generateStarEntity(EngineContext* ctx, GameStar* stars, int index, int x, int y);

void generateGalaxyStepOne(EngineContext* ctx, GameStar* stars, const EngineTextureKey key)
{
	EngineNoiseMask mask = {
		.type = ENGINE_NOISEMASK_GALAXY,
		.width = GAME_GALAXY_WIDTH,
		.height = GAME_GALAXY_HEIGHT,
		.mask_galaxy = {
			.bulgeintensity = 0.95,
			.bulgecenter = 14,
			.armcurve = 12.5,
			.arms = 2.0,
			.armthickness = 10.0,
			.inradius = -1,
			.outradius = -1
		}
	};
	engineSetNoiseMask(ctx, &mask, key);
	enginePerlinInit(GAME_GALAXY_WIDTH, GAME_GALAXY_HEIGHT);
	float* noise = enginePerlinGenerateMask(ctx, key);
	generateStars(ctx, stars, noise);
	free(noise);
	enginePerlinClose(ctx, key);
}

struct stardata* _cachePositionAndEntityData(EngineContext* ctx, GameStar* stars)
{
	struct stardata* data = malloc(GAME_MAX_STARS * sizeof(struct stardata));
	assert(data != NULL);

	for (int i = 0; i < GAME_MAX_STARS-1; i++)
	{
		EngineEntity* ent = engineGetEntityScene(ctx, GAME_GALAXY_MAP, stars[i].entity);
		if (ent != NULL)
		{
			data[i].id = ent->key;
			data[i].x = ent->x;
			data[i].y = ent->y;
			data[i].w = ent->width;
			data[i].h = ent->height;
		}
		else
		{
			data[i].id = -1;
			data[i].x = -9999.0f;
			data[i].y = -9999.0f;
			data[i].w = -9999.0f;
			data[i].h = -9999.0f;
		}
	}

	return data;
}

int* _checkToDestroy(struct stardata* data, int* numToDestroy)
{
	int* destroy = malloc(GAME_MAX_STARS * sizeof(int));
	assert(destroy != NULL);
	memset(destroy, 0, GAME_MAX_STARS * sizeof(int));
	int* tmp = destroy;

	for (int i = 0; i < GAME_MAX_STARS-1; i++)
	{
		if (data[i].id == -1) continue;

		for (int j = i + 1; j < GAME_MAX_STARS-1; j++)
		{
			if (data[j].id == -1) continue;

			float dx = data[j].x - data[i].x;
			float dy = data[j].y - data[i].y;
			float dist = (dx * dx) + (dy * dy);
			float collr = (data[i].w * data[i].h);

			if (dist <= collr)
			{
				(*tmp) = data[j].id;
				tmp++;
				(*numToDestroy)++;
			}
		}
	}

	return destroy;
}

// TODO: remove all asserts in favor of a logger and error handler
struct stardata* generateGalaxyStepTwo(EngineContext* ctx, GameStar* stars)
{
	int numToDestroy = 0;
	struct stardata* data = _cachePositionAndEntityData(ctx, stars);
	int* destroy = _checkToDestroy(data, &numToDestroy);
	assert(data != NULL);
	assert(destroy != NULL);

	for (int i = 0; i < numToDestroy; i++)
	{
		EngineEntity* ent = engineGetEntityScene(ctx, GAME_GALAXY_MAP, destroy[i]);
		
		if (ent == NULL)
			continue;

		if (ent->components != NULL)
		{
			free(ent->components);
			ent->components = NULL;
		}

		(*ent) = (EngineEntity){ 0 };
		stars[i].entity = -1;
	}

	EngineScene* map = engineGetScene(ctx, GAME_GALAXY_MAP);
	assert(map != NULL);

	map->nentities -= numToDestroy;

	free(destroy);

	destroy = NULL;

	return data;
}

static const long maxes[6] = {
	GALAXY_GIANT_MAX,
	GALAXY_SGIANT_MAX,
	GALAXY_BHOLE_MAX,
	GALAXY_WDWARF_MAX,
	GALAXY_NEUTRON_MAX,
	GALAXY_PULSAR_MAX
};

static const GameStarClass classes[6] = {
	STAR_CLASS_GIANT,
	STAR_CLASS_SUPER_GIANT,
	STAR_CLASS_BLACK_HOLE,
	STAR_CLASS_WHITE_DWARF,
	STAR_CLASS_NEUTRON,
	STAR_CLASS_PULSAR
};

void _stepthree_generateNonMainSequence(EngineContext* ctx, GameStar* stars, struct stardata* data, int* coreIndex)
{
	int curMax = 0;
	int swappedStars = 0;
	int i = 0;

	while (curMax < 6)
	{
		if (swappedStars >= maxes[curMax])
		{
			curMax++;
			swappedStars = 0;
		}

		if (stars[i].entity == -1)
		{
			if ((*coreIndex) < 0)
				(*coreIndex) = i;
			i++;
			continue;
		}

		stars[i].class = classes[curMax];

		if (classes[curMax] == STAR_CLASS_GIANT || classes[curMax] == STAR_CLASS_SUPER_GIANT)
		{
			stars[i].mass = engineGetRandomRangeD(1.0, 20.0);
			stars[i].diameter = engineGetRandomRangeD(200, 2000);
			stars[i].surfaceTemp = engineGetRandomRangeD(0.5, 1.0);
			stars[i].lifetime = engineGetRandomRangeD(10000000, 100000000);

			double radii = stars[i].diameter / 2;
			stars[i].luminosity = pow(radii, 2) * pow(stars[i].surfaceTemp, 4);
		}

		if (classes[curMax] == STAR_CLASS_NEUTRON || classes[curMax] == STAR_CLASS_PULSAR)
		{
			stars[i].mass = engineGetRandomRangeD(1.4, 2.1);
			stars[i].luminosity = engineGetRandomRangeD(0.1, 1000);
			stars[i].diameter = 0.00003;
			stars[i].surfaceTemp = engineGetRandomRangeD(100, 200);
			stars[i].lifetime = 100000000000;
		}

		if (classes[curMax] == STAR_CLASS_WHITE_DWARF)
		{
			stars[i].mass = engineGetRandomRangeD(0.5, 1.44);
			stars[i].luminosity = 0.0001;
			stars[i].diameter = 0.01;
			stars[i].surfaceTemp = engineGetRandomRangeD(2, 17);
			stars[i].lifetime = engineGetRandomRangeD(10000000000, 1000000000000);
		}

		if (classes[curMax] == STAR_CLASS_BLACK_HOLE)
		{
			stars[i].mass = engineGetRandomRangeD(3, 20);
			stars[i].luminosity = -1;
			stars[i].diameter = stars[i].mass * GALAXY_BHOLE_DIAMETER_SCALE;
			stars[i].surfaceTemp = -1;
			stars[i].lifetime = -1;
			stars[i].habitableZoneInner = 0;
			stars[i].habitableZoneOuter = 0;
		}

		if (classes[curMax] != STAR_CLASS_BLACK_HOLE)
		{
			stars[i].habitableZoneInner = sqrt(stars[i].luminosity) * 0.95;
			stars[i].habitableZoneOuter = sqrt(stars[i].luminosity) * 1.37;
		}

		swappedStars++;
		i++;
	}
}

void _stepthree_generateGalacticCore(EngineContext* ctx, GameStar* stars, struct stardata* data, const int coreIndex)
{
	float quasarProb = (float)engineGetRandomRangeD(0, 1);
	if (quasarProb <= 0.325f)
	{
		// core is a chad ah quasar
		stars[coreIndex].class = STAR_CLASS_QUASAR;
		stars[coreIndex].mass = engineGetRandomRangeD(1000000, 1000000000);
		stars[coreIndex].luminosity = 1000000000;
		stars[coreIndex].diameter = engineGetRandomRangeD(100000, 10000000);
		stars[coreIndex].surfaceTemp = engineGetRandomRangeD(30, 30);
		stars[coreIndex].lifetime = engineGetRandomRangeD(10000000, 100000000);
		stars[coreIndex].habitableZoneInner = -1;
		stars[coreIndex].habitableZoneOuter = -1;
	}
	else
	{
		// core is a regular ah super massive black hole
		stars[coreIndex].class = STAR_CLASS_BLACK_HOLE;
		stars[coreIndex].mass = engineGetRandomRangeD(21, 100);
		stars[coreIndex].luminosity = -1;
		stars[coreIndex].diameter = stars[coreIndex].mass * GALAXY_BHOLE_DIAMETER_SCALE;
		stars[coreIndex].surfaceTemp = -1;
		stars[coreIndex].lifetime = -1;
		stars[coreIndex].habitableZoneInner = 0;
		stars[coreIndex].habitableZoneOuter = 0;
	}

	stars[coreIndex].entity = coreIndex;
	EngineEntity* ent = &(ctx->scenes[GAME_GALAXY_MAP]->entities[coreIndex]);
	ent->x = (float)(GAME_GALAXY_WIDTH / 2);
	ent->y = (float)(GAME_GALAXY_HEIGHT / 2);
	ent->width = 2.0f;
	ent->height = 2.0f;
	ent->r = 1.0f;
	ent->g = 1.0f;
	ent->b = 1.0f;
	ent->key = coreIndex;
	ent->texkey = STAR_TEXTURE;
	ctx->scenes[GAME_GALAXY_MAP]->nentities++;
	engineAddComponentScene(ctx, coreIndex, ENGINE_COMPONENT_RENDERER, GAME_GALAXY_MAP);

}

void generateGalaxyStepThree(EngineContext* ctx, GameStar* stars, struct stardata* data)
{
	srand(time(NULL));

	// keep track of all stars swapped to a different spectral class
	int coreIndex = -1;

	_stepthree_generateNonMainSequence(ctx, stars, data, &coreIndex);
	_stepthree_generateGalacticCore(ctx, stars, data, coreIndex);
}

void generateStars(EngineContext* ctx, GameStar* stars, float* noise)
{
	srand(time(NULL));

	int numStars = 0;
	int x, y, index;
	int attempts = 0;
	int maxAttempts = GAME_GALAXY_WIDTH * GAME_GALAXY_HEIGHT;

	while (numStars < GAME_MAX_STARS && attempts < maxAttempts)
	{
		x = engineGetRandomRangeI(0, GAME_GALAXY_WIDTH);
		y = engineGetRandomRangeI(0, GAME_GALAXY_HEIGHT);
		index = y * GAME_GALAXY_WIDTH + x;

		if (index >= 0 && index < (GAME_GALAXY_WIDTH * GAME_GALAXY_HEIGHT))
		{
			float prob = (float)engineGetRandomRangeD(0, 1);
			if (prob < noise[index])
			{
				if (numStars >= GAME_MAX_STARS)
					break;
				_generateStar(stars, numStars);
				_generateStarEntity(ctx, stars, numStars, x, y);
				numStars++;
				attempts = 0;
			}
		}
		attempts++;
	}

	if (attempts >= maxAttempts)
		engineWriteMessage(ctx, "Could not place all the stars  (4096)! Threshold might be too high!", ELOG_MSGTYPE_WARN);
}

void _generateStarEntity(EngineContext* ctx, GameStar* stars, int index, int x, int y)
{
	GameStar* cur = stars + index;

	vec2 pos = {
		(float)x,
		(float)y
	};
	vec2 size = { 2.0f, 2.0f };
	engineCreateEntityScene(ctx, pos, size, cur->entity, STAR_TEXTURE, GAME_GALAXY_MAP);
	engineAddComponentScene(ctx, cur->entity, ENGINE_COMPONENT_RENDERER, GAME_GALAXY_MAP);
}

void _generateStar(GameStar* stars, int index)
{
	GameStar tmp = { 0 };

	// TODO: prototype a star mass generation weight setting that can be configurable by the user
	double starmasschance = (double)rand() / (double)RAND_MAX;
	tmp.mass = _generateMass(pow(starmasschance, STAR_RAND_MASS_BIAS));
	
	tmp.luminosity = pow(tmp.mass, STAR_LUMINOSITY_POWER);
	tmp.diameter = pow(tmp.mass, STAR_DIAMETER_POWER);
	tmp.surfaceTemp = pow(tmp.mass, STAR_SURFACE_POWER);
	tmp.lifetime = pow(tmp.mass, STAR_LIFETIME_POWER);
	tmp.habitableZoneInner = sqrt(tmp.luminosity) * STAR_HABITABLE_INNER_COEFF;
	tmp.habitableZoneOuter = sqrt(tmp.luminosity) * STAR_HABITABLE_OUTER_COEFF;
	tmp.numJumpPoints = 0;

	_determineSpectralClass(&tmp);

	tmp.entity = index;

	memcpy(stars + index, &tmp, sizeof(GameStar));
}

double _generateMass(double chance)
{
	if (chance < 0.6)
		return engineGetRandomRangeD(0.08, 0.5); // M
	else if (chance < 0.8)
		return engineGetRandomRangeD(0.5, 0.8);	// K
	else if (chance < 0.92)
		return engineGetRandomRangeD(0.8, 1.04); // G
	else if (chance < 0.97)
		return engineGetRandomRangeD(1.04, 1.4); // F
	else if (chance < 0.995)
		return engineGetRandomRangeD(1.4, 3.0); // A/B
	else
		return engineGetRandomRangeD(3.0, 50.0); // big rare giants
}

void _determineSpectralClass(GameStar* star)
{
	if (star->luminosity >= 30000 && star->luminosity <= 1000000)
		star->class = STAR_CLASS_O;
	if (star->luminosity >= 25 && star->luminosity < 30000)
		star->class = STAR_CLASS_B;
	if (star->luminosity >= 5 && star->luminosity < 25)
		star->class = STAR_CLASS_A;
	if (star->luminosity >= 1.5 && star->luminosity < 5)
		star->class = STAR_CLASS_F;
	if (star->luminosity >= 0.6 && star->luminosity < 1.5)
		star->class = STAR_CLASS_G;
	if (star->luminosity >= 0.08 && star->luminosity < 0.6)
		star->class = STAR_CLASS_K;
	if (star->luminosity >= 0.0001 && star->luminosity < 0.08)
		star->class = STAR_CLASS_M;
}
