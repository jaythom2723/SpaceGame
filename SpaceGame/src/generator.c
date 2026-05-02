#include "generator.h"
#include "generator_defs.h"

#include "engine_utils.h"
#include "engine_entity.h"
#include "engine_components.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

extern EngineTextureKey STAR_TEXTURE;

const char* STAR_CLASS_TRANSLATIONS[STAR_CLASS_M + 1] = {
	"O", "B", "A", "F", "G", "K", "M"
};

void _generateStar(GameStar* stars, int index, int* occurrences);
double _generateMass(double chance);
void _determineSpectralClass(GameStar* star);

void _generateStarEntity(EngineContext* ctx, GameStar* stars, int index, int x, int y);

void generateStars(EngineContext* ctx, GameStar* stars, float* noise)
{
	srand(time(NULL));

	// TODO: make a logging system for the engine
	int occurrences[STAR_CLASS_M + 1] = { 0 };

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
			if (noise[index] > 0.05f)
			{
				_generateStar(stars, numStars, occurrences);
				_generateStarEntity(ctx, stars, numStars, x, y);
				numStars++;
			}
		}
		attempts++;
	}
	if (attempts >= maxAttempts)
	{
		printf("Warning: could not place all stars! Threshold might be too high!\n");
	}

	//for (int i = 0; i < STAR_CLASS_M + 1; i++)
	//	printf("[%s] > %d\n", STAR_CLASS_TRANSLATIONS[i], occurrences[i]);

	//printf("Spectral Class Generation Percentage Values\n");
	//for (int i = 0; i < STAR_CLASS_M + 1; i++)
	//	printf("\t[%s] > %%%.2f\n", STAR_CLASS_TRANSLATIONS[i], ((float)occurrences[i] / (float)GAME_MAX_STARS)*100.0f);
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

void _generateStar(GameStar* stars, int index, int* occurrences)
{
	GameStar tmp = { 0 };

	// TODO: prototype a star mass generation weight setting that can be configurable by the user
	double starmasschance = (double)rand() / (double)RAND_MAX;
	tmp.mass = _generateMass(pow(starmasschance, STAR_RAND_MASS_CHANCE));
	
	tmp.luminosity = pow(tmp.mass, STAR_LUMINOSITY_POWER);
	tmp.diameter = pow(tmp.mass, STAR_DIAMETER_POWER);
	tmp.surfaceTemp = pow(tmp.mass, STAR_SURFACE_POWER);
	tmp.lifetime = pow(tmp.mass, STAR_LIFETIME_POWER);
	tmp.habitableZoneInner = sqrt(tmp.luminosity) * STAR_HABITABLE_INNER_COEFF;
	tmp.habitableZoneOuter = sqrt(tmp.luminosity) * STAR_HABITABLE_OUTER_COEFF;

	_determineSpectralClass(&tmp);
	(*(occurrences + tmp.class))++;

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
