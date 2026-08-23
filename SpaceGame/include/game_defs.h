#ifndef GAME_DEFS_H
#define GAME_DEFS_H

#include "engine_defs.h"

#define ONE_LIGHTYEAR_PIXELS 100

// FIXME: Locate and find a bug related to being unable to generate more than 4096 stars
#define GAME_MAX_STARS 0x1000
#define GAME_MAX_JPTS_PER_STAR 0xF
#define GAME_GALAXY_WIDTH 1000
#define GAME_GALAXY_HEIGHT 1000
#define GAME_GALAXY_X -(GAME_GALAXY_WIDTH/2)
#define GAME_GALAXY_Y -(GAME_GALAXY_HEIGHT/2)
#define GAME_MAX_SOLAR_SYSTEMS GAME_MAX_STARS

#define GAME_SOLAR_SYSTEM_MAX_WIDTH 1000
#define GAME_SOLAR_SYSTEM_MAX_HEIGHT 1000
#define GAME_MAX_PLANETS_PER_SYSTEM 25					// Hard-cap max of num planets to spawn at any point
#define GAME_NUM_PLANETS_TO_SPAWN_WEIGHT 0.25f			// A weight used to control the minimum and maximum planets to spawn in a solar system

typedef enum game_starclass {
	STAR_CLASS_GIANT, // NON-MAIN SEQUENCE
	STAR_CLASS_SUPER_GIANT,
	STAR_CLASS_BLACK_HOLE,
	STAR_CLASS_WHITE_DWARF,
	STAR_CLASS_NEUTRON,
	STAR_CLASS_PULSAR,
	STAR_CLASS_QUASAR,
	STAR_CLASS_O, // MAIN SEQUENCE [HOTTEST]
	STAR_CLASS_B,
	STAR_CLASS_A,
	STAR_CLASS_F,
	STAR_CLASS_G,
	STAR_CLASS_K,
	STAR_CLASS_M, // MAIN SEQUENCE [COLDEST]
} GameStarClass;

// TODO: add some more bullshit to this perchance?
typedef enum game_planetclass
{
	PLANET_CLASS_TERRESTRIAL,
	PLANET_CLASS_GAS_GIANT
} GamePlanetClass;

enum gamestate {
	GAME_GENERATE_GALAXY_MAP,
	GAME_GENERATE_SOLAR_SYSTEM_MAP,
	GAME_GENERATE_PLANET_MAP,
	GAME_GALAXY_MAP,
	GAME_DEBUG_TEST_STATE,
	GAME_ABORT_FATAL_ERROR_STATE,
};

typedef struct game_star {
	GameStarClass class;
	EngineEntityKey entity;
	int numJumpPoints;

	double mass;
	double luminosity;			// L = M^3.5
	double diameter;			// D = M^0.74
	double surfaceTemp;			// T = M^0.505
	double lifetime;			// V = M^-2.5
	double habitableZoneInner;	// R = sqrt(L)*0.95
	double habitableZoneOuter;	// R = sqrt(L)*1.37
} GameStar;

typedef struct game_planet
{
	GamePlanetClass class;
	EngineEntityKey entity;
} GamePlanet;

// TODO: work on binary and trinary star systems (could rework generation steps for this)
typedef struct game_solar_system
{
	EngineEntityKey* stars;
	EngineEntityKey* planets;
} GameSolarSystem;

struct stardata {
	float x;
	float y;
	float w;
	float h;
	int id;
};

#endif // GAME_DEFS_H
