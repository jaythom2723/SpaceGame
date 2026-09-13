#ifndef OB_EXT_PERLIN_H
#define OB_EXT_PERLIN_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define OB_EXT_PERLIN_MAX_WIDTH         10000
#define OB_EXT_PERLIN_MAX_HEIGHT        10000

void OBEXTperlinSetWidth(const uint32_t value);
void OBEXTperlinSetHeight(const uint32_t value);
void OBEXTperlinSetSize(const uint32_t width, const uint32_t height);
bool OBEXTperlinInvoke(void** data, const size_t dsize);

#endif