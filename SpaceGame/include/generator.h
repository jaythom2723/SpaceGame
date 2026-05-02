#ifndef GENERATOR_H
#define GENERATOR_H

#include "game_defs.h"

// generate all stars via perlin noise and shape mask
void generationStepOne(EngineContext* ctx, GameStar* stars, const EngineTextureKey key);

// sort all stars based on their ID
// remove stars too close to other stars
void generationStepTwo(EngineContext* ctx, GameStar* stars);

void generateStars(EngineContext* ctx, GameStar* stars, float* noise);

#endif // GENERATOR_H
