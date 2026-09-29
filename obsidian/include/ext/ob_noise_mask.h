#ifndef OB_EXT_NOISE_MASK_H
#define OB_EXT_NOISE_MASK_H

#include "assets/ob_asset.h"

#include <stdint.h>
#include <stdbool.h>

#define OB_EXT_MAX_NOISE_MASK_WIDTH         10000
#define OB_EXT_MAX_NOISE_MASK_HEIGHT        10000

void OBEXTnoiseMaskSetWidth(int width);
void OBEXTnoiseMaskSetHeight(int height);
void OBEXTnoiseMaskSetSize(int width, int height);
void OBEXTnoiseMaskSetGalaxyCenter(float x, float y);
void OBEXTnoiseMaskSetGalaxyRadius(float radius);
void OBEXTnoiseMaskSetSpiralArmCount(float armCount);
void OBEXTnoiseMaskSetSpiralArmTightness(float tightness);
void OBEXTnoiseMaskSetSpiralArmWidth(float width);

float* OBEXTnoiseMaskInvoke(void);
struct obsidian_asset* OBEXTnoiseMaskInvokeAsset(void);

#endif