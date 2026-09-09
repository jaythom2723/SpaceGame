#ifndef OB_LOADER_H
#define OB_LOADER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

enum obsidian_asset_type {
    OB_ASSET_TEXTURE,
    OB_ASSET_MODEL,
    OB_ASSET_SOUND,
    OB_ASSET_LOCALE,
    OB_ASSET_CONFIG,
    OB_ASSET_LIST,
    OB_ASSET_LOG,
    OB_ASSET_MOD_DESCRIPTOR,
    OB_ASSET_MOD_CONTENT_FILE
};

struct obsidian_asset {
    char* rdata;
    uint32_t uid;
    enum obsidian_asset_type type;

    union {
        struct {
            uint32_t width;
            uint32_t height;
            uint32_t nrChannels;
            uint8_t* pixels;
        } texture;

        struct {
            uint32_t vaoi;
            uint32_t vboi;
            uint32_t eboi;
            uint32_t texi;
        } mdlprim;
    };

    // TODO: Figure out what to do about model assets?
};

bool OBLDRcreatePrimitive(struct obsidian_asset** restrict mdlasset, const struct obsidian_asset* restrict texasset, const float* restrict vertices, const size_t vsize, const unsigned int* restrict indices, const size_t isize);
bool OBLDRloadAsset(enum obsidian_asset_type type, struct obsidian_asset** restrict asset, const char* const path);
void OBLDRdestroyAsset(struct obsidian_asset* restrict asset);

#endif