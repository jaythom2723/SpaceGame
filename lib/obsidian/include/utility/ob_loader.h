#ifndef OB_LOADER_H
#define OB_LOADER_H

#include <stdint.h>
#include <stdbool.h>

#include "assets/ob_asset.h"

bool OBLDRloadAsset(enum obsidian_asset_type type, struct obsidian_asset** restrict asset, const char* const path);

#endif