#include "utility/ob_loader.h"
#include "utility/ob_error.h"
#include "utility/ob_logger.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef void (PFNOBASSETLOADPROC)(const struct obsidian_asset* restrict);

extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

extern void __ob_log_wline(enum ob_logger_message_type, const char* const);
extern void __ob_log_wsline(const char* const);

extern char* __ob_util_readfile(const char* const);

static void __ob_loader_texture_load(const struct obsidian_asset* restrict);

bool OBLDRloadAsset(enum obsidian_asset_type type, struct obsidian_asset* restrict asset, const char* const path)
{
    if (asset != NULL)
    {
        printf("Asset is not NULL\n");
        return false;
    }

    asset = calloc(1, sizeof(struct obsidian_asset));
    if (asset == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_WARNING, CAT_MEMORY, "Failed to allocate enough memory to load an asset properly.", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }
    asset->type = type;
    asset->rdata = __ob_util_readfile(path);
    if (asset->rdata == NULL)
    {
        (void)__ob_error_pusherror(ERR_FILE_IO, SEV_WARNING, CAT_FILESYSTEM, "Failed to read file contents for an asset.", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        free(asset);
        asset = NULL;
        return false;
    }

    asset->uid = 0; // TODO: implement

    switch(type)
    {
        case OB_ASSET_TEXTURE:
            __ob_loader_texture_load(asset);
        break;
        default:
            printf("Unkown or unimplemented\n");
        break;
    }

    return true;
}

void OBLDRdestroyAsset(struct obsidian_asset* restrict asset)
{
    if (asset == NULL)
        return;

    // TODO: free all the other bullshit too

    free(asset);
    asset = NULL;
}

void __ob_loader_texture_load(const struct obsidian_asset* restrict asset)
{
    printf("Texture being loaded!\n");
    uint8_t* pixels;
    uint32_t width, height, nrChannels;
    uint32_t offset = 0;

    memcpy(&width, asset->rdata, sizeof(int));
    offset += sizeof(int);
    memcpy(&height, asset->rdata + offset, sizeof(int));
    offset += sizeof(int);
    memcpy(&nrChannels, asset->rdata + offset, sizeof(int));
    offset += sizeof(int);

    pixels = calloc(width * height * 4,  sizeof(uint8_t));
    memcpy(pixels, asset->rdata + offset, width * height * 4 * sizeof(uint8_t));

    for (size_t i = 0; i < 32; i++)
    {
        printf("%02x ", *(asset->rdata + i));
    }
    printf("\n");

    // printf("%dx%d, %d\n", width, height, nrChannels);
    // printf("%p\n", pixels);

    free(pixels);
    pixels = NULL;
}