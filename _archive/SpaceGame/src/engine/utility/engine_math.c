#include "engine_math.h"

float engineGetDistance(float x1, float x2, float y1, float y2)
{
	float a, b;
	a = x2 - x1;
	b = y2 - y1;
	return (a * a) + (b * b);
}

float engineGetDistancev(vec2 pos1, vec2 pos2)
{
	float a, b;
	a = pos2[0] - pos1[0];
	b = pos2[1] - pos1[1];
	return (a * a) + (b * b);
}

const EBOOL engineCheckDistanceToCenter(EngineContext* ctx, float x1, float x2, float y1, float y2, float width, float height)
{
	float dist = engineGetDistance(x1, x2, y1, y2);
	float area = width * height;
	return dist <= area;
}
