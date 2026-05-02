#ifndef ENGINE_PERLIN_H
#define ENGINE_PERLIN_H

#include "engine_defs.h"

#include <cglm/cglm.h>

void enginePerlinInit(const int width, const int height);
void engineSetNoiseMask(EngineContext* ctx, const EngineNoiseMask* const mask, const EngineTextureKey texkey);
float* enginePerlinGenerate(void);
float* enginePerlinGenerateMask(EngineContext* ctx, EngineTextureKey texkey);
void enginePerlinClose(EngineContext* ctx, EngineTextureKey texkey);

#endif // ENGINE_PERLIN_H
