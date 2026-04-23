#ifndef GAME_DEFS_H
#define GAME_DEFS_H

#include "engine_defs.h"

#define GAME_MAX_STARS 0x1000
#define GAME_GALAXY_WIDTH 1000
#define GAME_GALAXY_HEIGHT 1000
#define GAME_GALAXY_X -(GAME_GALAXY_WIDTH/2)
#define GAME_GALAXY_Y -(GAME_GALAXY_HEIGHT/2)

#define SOLAR_MASS 

typedef enum game_starclass {
	STAR_CLASS_O, // HOTTEST
	STAR_CLASS_B,
	STAR_CLASS_A,
	STAR_CLASS_F,
	STAR_CLASS_G,
	STAR_CLASS_K,
	STAR_CLASS_M, // COLDEST
} GameStarClass;

enum gamestate {
	GAME_GENERATE_GALAXY_MAP,
	GAME_GENERATE_SOLAR_SYSTEM_MAP,
	GAME_GENERATE_PLANET_MAP,
	GAME_GALAXY_MAP,
	GAME_DEBUG_TEST_STATE
};

typedef struct game_star {
	GameStarClass class;
	EngineEntityKey entity;

	double mass;
	double luminosity;			// L = M^3.5
	double diameter;			// D = M^0.74
	double surfaceTemp;			// T = M^0.505
	double lifetime;			// V = M^-2.5
	double habitableZoneInner;	// R = sqrt(L)*0.95
	double habitableZoneOuter;	// R = sqrt(L)*1.37
} GameStar;

#endif // GAME_DEFS_H
