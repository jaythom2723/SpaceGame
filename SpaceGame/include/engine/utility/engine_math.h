#ifndef ENGINE_MATH_H
#define ENGINE_MATH_H

#include "engine_defs.h"

#include <cglm/cglm.h>

float engineGetDistance(float x1, float x2, float y1, float y2);
float engineGetDistancev(vec2 pos1, vec2 pos2);
const EBOOL engineCheckDistanceToCenter(EngineContext* ctx, float x1, float x2, float y1, float y2, float width, float height);

#endif // ENGINE_MATH_H
