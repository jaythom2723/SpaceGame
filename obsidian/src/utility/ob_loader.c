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

extern uint32_t __ob_asset_genUniqueIdentity(void);

static void __ob_loader_texture_load(struct obsidian_asset* restrict);
static void __ob_loader_model_load(struct obsidian_asset* restrict);

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

    (*asset)->uid = 0;
    if (((*asset)->type | __OB_ASSET_UID_GEN_PROT) == __OB_ASSET_UID_GEN_PROT)
        (*asset)->uid = __ob_asset_genUniqueIdentity();

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

void __ob_loader_texture_load(struct obsidian_asset* restrict asset)
{
    extern uint32_t __ob_tex_createtex(void);
    extern bool __ob_tex_bindtex(uint32_t);
    extern bool __ob_tex_gendata(uint32_t,const struct obsidian_asset*);

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

    glActiveTexture(GL_TEXTURE0);
    asset->texture.texi = __ob_tex_createtex();
    (void)__ob_tex_bindtex(asset->texture.texi);
    (void)__ob_tex_gendata(asset->texture.texi, asset);
}

void __ob_loader_model_load(struct obsidian_asset* restrict asset)
{
    printf("%d\n", asset->type);
}