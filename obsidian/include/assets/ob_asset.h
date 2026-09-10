#ifndef OB_ASSET_H
#define OB_ASSET_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define __OB_ASSET_UID_GEN_PROT 0x8000
#define __OB_ASSET_NO_UID_PROT  0x4000

enum obsidian_asset_type {
    OB_ASSET_TEXTURE            = __OB_ASSET_NO_UID_PROT    | 0x0001,
    OB_ASSET_MODEL              = __OB_ASSET_UID_GEN_PROT   | 0x0002,
    OB_ASSET_SOUND              = __OB_ASSET_NO_UID_PROT    | 0x0004,
    OB_ASSET_LOCALE             = __OB_ASSET_UID_GEN_PROT   | 0x0008,
    OB_ASSET_CONFIG             = __OB_ASSET_UID_GEN_PROT   | 0x0010,
    OB_ASSET_FLIST              = __OB_ASSET_UID_GEN_PROT   | 0x0020,
    OB_ASSET_LOG                = __OB_ASSET_UID_GEN_PROT   | 0x0040,
    OB_ASSET_MOD_DESCRIPTOR     = __OB_ASSET_UID_GEN_PROT   | 0x0080,
    OB_ASSET_MOD_CONTENT_FILE   = __OB_ASSET_UID_GEN_PROT   | 0X0100,
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
            uint32_t texi;
        } texture;

        struct {
            uint32_t vaoi;
            uint32_t vboi;
            uint32_t eboi;
        } mdlprim;
    };
};

bool OBASTcreatePrimitiveModel(struct obsidian_asset** asset, const float* vertices, const size_t vsize, const unsigned int* indices, const size_t isize);
void OBASTdestroyAsset(struct obsidian_asset* asset);

#endif