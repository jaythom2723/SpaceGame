#ifndef GENERATOR_H
#define GENERATOR_H

#include "game_defs.h"

// generate all stars via perlin noise and shape mask
void generateGalaxyStepOne(EngineContext* ctx, GameStar* stars, const EngineTextureKey key);

// cull stars that are colliding with other stars
struct stardata* generateGalaxyStepTwo(EngineContext* ctx, GameStar* stars);

// generate N number of giants, supergiants, white dwarves, pulsars, quasars, neutrinos, and black holes
void generateGalaxyStepThree(EngineContext* ctx, GameStar* stars, struct stardata* data);

void generateStars(EngineContext* ctx, GameStar* stars, float* noise);

#endif // GENERATOR_H
