#ifndef ENGINE_PERLIN_H
#define ENGINE_PERLIN_H

#include "engine_defs.h"

#include <cglm/cglm.h>

void enginePerlinInit(const int width, const int height);
float* enginePerlinGenerate(void);
void enginePerlinClose(void);

#endif // ENGINE_PERLIN_H
