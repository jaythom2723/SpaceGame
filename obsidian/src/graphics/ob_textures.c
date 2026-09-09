#include "utility/ob_error.h"
#include "utility/ob_logger.h"
#include "utility/ob_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <glad/glad.h>

#define __OB_MAX_TEXTURES 0xFF

bool __ob_tex_initmodule(void);
bool __ob_tex_closemodule(void);

uint32_t __ob_tex_createtex(void);
void __ob_tex_deletetex(uint32_t);
bool __ob_tex_bindtex(uint32_t);
bool __ob_tex_gendata(uint32_t, const struct obsidian_asset*);
void __ob_tex_unbindtex(void);

extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

extern void __ob_log_wline(enum ob_logger_message_type, const char* const);
extern void __ob_log_wsline(const char* const);

static uint32_t* textures = NULL;

static uint32_t* textureptr = NULL;

bool __ob_tex_initmodule(void)
{
    if (textures != NULL)
        return false;

    textures = calloc(__OB_MAX_TEXTURES, sizeof(uint32_t));
    if (textures == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_WARNING, CAT_MEMORY, "Failed to allocate memory for Obsidian: Textures Core Module.", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }
    memset(textures, 0, __OB_MAX_TEXTURES * sizeof(uint32_t));

    textureptr = textures;

    return true;
}

bool __ob_tex_closemodule(void)
{
    if (textures == NULL)
        return false;

    glDeleteTextures(__OB_MAX_TEXTURES, textures);

    free(textures);
    textures = NULL;

    return true;
}

/*
uint32_t __ob_tex_createtex(void);
void __ob_tex_deletetex(uint32_t);
bool __ob_tex_bindtex(uint32_t);
bool __ob_tex_gendata(uint32_t, const struct obsidian_asset*);
void __ob_tex_unbindtex(void);
*/

uint32_t __ob_tex_createtex(void)
{
    if (textures == NULL)
        return 0xFF;

    if ((textureptr - textures) >= __OB_MAX_TEXTURES)
        return 0xFF;

    uint32_t index = (uint32_t)(textureptr - textures);
    glGenTextures(1, textureptr);
    glBindTexture(GL_TEXTURE_2D, *textureptr);

    textureptr++;

    return index;
}

void __ob_tex_deletetex(uint32_t index)
{
    if (textures == NULL)
        return;

    if (glIsTexture(*(textures + index)) == GL_FALSE)
        return;
    glDeleteTextures(1, (textures + index));
}

bool __ob_tex_bindtex(uint32_t index)
{
    if (textures == NULL)
        return false;

    if (glIsTexture(*(textures + index)) == GL_FALSE)
        return false;
    glBindTexture(GL_TEXTURE_2D, *(textures + index));

    return true;
}

bool __ob_tex_gendata(uint32_t index, const struct obsidian_asset* asset)
{
    if (textures == NULL)
        return false;

    if (glIsTexture(*(textures + index)) == GL_FALSE)
        return false;

    if (asset->type != OB_ASSET_TEXTURE)
        return false;

    glBindTexture(GL_TEXTURE_2D, *(textures + index));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, asset->texture.width, asset->texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, asset->texture.pixels);

    return true;
}

void __ob_tex_unbindtex(void)
{
    glBindTexture(GL_TEXTURE_2D, 0);
}