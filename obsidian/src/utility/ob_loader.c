#include "utility/ob_loader.h"
#include "utility/ob_error.h"
#include "utility/ob_logger.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <glad/glad.h>

typedef void (PFNOBASSETLOADPROC)(const struct obsidian_asset* restrict);

extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

extern void __ob_log_wline(enum ob_logger_message_type, const char* const);
extern void __ob_log_wsline(const char* const);

extern char* __ob_util_readfile(const char* const);

static void __ob_loader_texture_load(struct obsidian_asset* restrict);
static void __ob_loader_model_load(struct obsidian_asset* restrict);

bool OBLDRcreatePrimitive(struct obsidian_asset** restrict mdlasset, const struct obsidian_asset* restrict texasset, const float* restrict vertices, const size_t vsize, const unsigned int* restrict indices, const size_t isize)
{
    // TODO: convert this to follow DRY

    if ((*mdlasset) != NULL)
        return false;

    // FIXME: if segfault, check here first.
    (*mdlasset) = calloc(1, sizeof(struct obsidian_asset));
    if (*mdlasset == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_WARNING, CAT_MEMORY, "Failed to allocate memory for an asset.", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }

    (*mdlasset)->type = OB_ASSET_MODEL;
    (*mdlasset)->uid = 0;
    (*mdlasset)->rdata = NULL;

    extern uint32_t __ob_buf_createvbo(void);
    extern bool __ob_buf_bindvbo(uint32_t);
    extern bool __ob_buf_setvbodata(uint32_t, size_t, const void*, uint32_t);
    extern uint32_t __ob_buf_createebo(void);
    extern bool __ob_buf_bindebo(uint32_t);
    extern bool __ob_buf_setebodata(uint32_t, size_t, const void*, uint32_t);
    extern uint32_t __ob_buf_createvao(void);
    extern bool __ob_buf_bindvao(uint32_t);
    extern void __ob_buf_setattribpointer(uint32_t,uint32_t,size_t,void*);
    extern uint32_t __ob_tex_createtex(void);
    extern bool __ob_tex_bindtex(uint32_t);
    extern bool __ob_tex_gendata(uint32_t, const struct obsidian_asset*);

    (*mdlasset)->mdlprim.vaoi = __ob_buf_createvao();
    __ob_buf_bindvao((*mdlasset)->mdlprim.vaoi);
    (*mdlasset)->mdlprim.vboi = __ob_buf_createvbo();
    __ob_buf_bindvbo((*mdlasset)->mdlprim.vboi);
    __ob_buf_setvbodata((*mdlasset)->mdlprim.vboi, vsize, vertices, GL_STATIC_DRAW);
    (*mdlasset)->mdlprim.eboi = __ob_buf_createebo();
    __ob_buf_bindebo((*mdlasset)->mdlprim.eboi);
    __ob_buf_setebodata((*mdlasset)->mdlprim.eboi, isize, indices, GL_STATIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    (*mdlasset)->mdlprim.texi = __ob_tex_createtex();
    __ob_tex_bindtex((*mdlasset)->mdlprim.texi);
    __ob_tex_gendata((*mdlasset)->mdlprim.texi, texasset);

    __ob_buf_setattribpointer(0, 3, 5 * sizeof(float), (void*)0);
    __ob_buf_setattribpointer(1, 2, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    return true;
}

bool OBLDRloadAsset(enum obsidian_asset_type type, struct obsidian_asset** restrict asset, const char* const path)
{
    // TODO: convert this to follow dry
    if ((*asset) != NULL)
    {
        printf("Asset is not NULL\n");
        return false;
    }

    (*asset) = calloc(1, sizeof(struct obsidian_asset));
    if ((*asset) == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_WARNING, CAT_MEMORY, "Failed to allocate enough memory to load an asset properly.", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }
    (*asset)->type = type;
    (*asset)->rdata = __ob_util_readfile(path);
    if ((*asset)->rdata == NULL)
    {
        (void)__ob_error_pusherror(ERR_FILE_IO, SEV_WARNING, CAT_FILESYSTEM, "Failed to read file contents for an asset.", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        free((*asset));
        (*asset) = NULL;
        return false;
    }

    (*asset)->uid = 0; // TODO: implement

    switch(type)
    {
        case OB_ASSET_TEXTURE:
            __ob_loader_texture_load(*asset);
        break;
        case OB_ASSET_MODEL:
            __ob_loader_model_load(*asset);
        break;
        default:
            printf("Unkown or unimplemented\n");
        break;
    }

    return true;
}

void OBLDRdestroyAsset(struct obsidian_asset* restrict asset)
{
    extern void __ob_buf_unbindvao(void);
    extern void __ob_buf_unbindvbo(void);
    extern void __ob_buf_unbindebo(void);
    extern void __ob_tex_unbindtex(void);

    extern void __ob_buf_deletevbo(uint32_t);
    extern void __ob_buf_deleteebo(uint32_t);
    extern void __ob_buf_deletevao(uint32_t);
    extern void __ob_tex_deletetex(uint32_t);

    if (asset == NULL)
        return;

    switch (asset->type)
    {
        case OB_ASSET_TEXTURE:
            free(asset->texture.pixels);
            asset->texture.pixels = NULL;
        break;
        case OB_ASSET_MODEL:
            __ob_buf_unbindvao();
            __ob_buf_unbindvbo();
            __ob_buf_unbindebo();
            __ob_tex_unbindtex();
            __ob_buf_deletevao(asset->mdlprim.vaoi);
            __ob_buf_deletevbo(asset->mdlprim.vboi);
            __ob_buf_deleteebo(asset->mdlprim.eboi);
            __ob_tex_deletetex(asset->mdlprim.texi);
        break;
        default: break;
    }

    free(asset->rdata);
    asset->rdata = NULL;

    free(asset);
    asset = NULL;
}

void __ob_loader_texture_load(struct obsidian_asset* restrict asset)
{
    uint32_t offset = 0;

    memcpy((void*)&(asset->texture.width), asset->rdata, sizeof(int));
    offset += sizeof(int);
    memcpy((void*)&(asset->texture.height), asset->rdata + offset, sizeof(int));
    offset += sizeof(int);
    memcpy((void*)&(asset->texture.nrChannels), asset->rdata + offset, sizeof(int));
    offset += sizeof(int);

    asset->texture.pixels = (uint8_t*) calloc(asset->texture.width * asset->texture.height  * 4,  sizeof(uint8_t));
    if (asset->texture.pixels == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_WARNING, CAT_MEMORY, "Failed to allocate enough memory for pixel data.", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return;
    }
    memcpy(asset->texture.pixels, asset->rdata + offset, asset->texture.width * asset->texture.height * 4 * sizeof(uint8_t));
}

void __ob_loader_model_load(struct obsidian_asset* restrict asset)
{
    printf("%d\n", asset->type);
}