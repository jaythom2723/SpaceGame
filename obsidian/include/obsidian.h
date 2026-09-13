#ifndef OBISIDIAN_H
#define OBISIDIAN_H

#include <stdint.h>
#include <stdbool.h>

enum obsidian_extension {
    OB_EXT_PERLIN_NOISE = 0x0001,
};

bool OBinit(void);
bool OBbootstrap(uint32_t* program, const char* title, const uint32_t width, const uint32_t height);
bool OBinitExtension(enum obsidian_extension exts);
void OBclose(void);

#endif