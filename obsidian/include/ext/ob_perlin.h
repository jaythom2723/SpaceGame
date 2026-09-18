#ifndef OB_EXT_PERLIN_H
#define OB_EXT_PERLIN_H

#include "assets/ob_asset.h"

#include <stdint.h>
#include <stdbool.h>

#define OB_EXT_PERLIN_MAX_WIDTH         10000
#define OB_EXT_PERLIN_MAX_HEIGHT        10000

void OBEXTperlinSetWidth(const uint32_t value);
void OBEXTperlinSetHeight(const uint32_t value);
void OBEXTperlinSetSize(const uint32_t width, const uint32_t height);
void OBEXTperlinSetResolution(const uint32_t value);
void OBEXTperlinSetFrequency(const uint32_t value);
void OBEXTperlinSetNumLayers(const uint32_t value);

float* OBEXTperlinInvoke(void);
struct obsidian_asset* OBEXTperlinInvokeAsset(void);

#endif