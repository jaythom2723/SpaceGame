#include "ext/ob_perlin.h"
#include "graphics/ob_shader.h"
#include "utility/ob_error.h"
#include "utility/ob_logger.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <glad/glad.h>

extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

extern bool __ob_log_wline(enum ob_logger_message_type, const char* const);
extern bool __ob_log_wsline(const char* const);

static uint32_t __width, __height;
static obsidian_program_t __program;
static uint32_t __texid;

static bool __init = false;

bool __ob_ext_perlin_init(void);
bool __ob_ext_perlin_close(void);

void OBEXTperlinSetWidth(const uint32_t value)
{
    if (!__init)
        return;
    __width = value;
}

void OBEXTperlinSetHeight(const uint32_t value)
{
    if (!__init)
        return;
    __height = value;
}

void OBEXTperlinSetSize(const uint32_t width, const uint32_t height)
{
    if (!__init)
        return;
    __width = width;
    __height = height;
}

bool OBEXTperlinInvoke(void** data, const size_t dsize)
{
    if (!__init)
        return false;

    OBSHDRuseProgram(__program);
    OBSHDRseti(__program, "imageWidth", __width);
    OBSHDRseti(__program, "imageHeight", __height);

    glBindTexture(GL_TEXTURE_2D, __texid);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, __width, __height);
    glBindImageTexture(0, __texid, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

    // TODO: don't hardcore "8"
    glDispatchCompute(__width / 8, __height / 8, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    (*data) = malloc(dsize);
    if (*data == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_WARNING, CAT_MEMORY, "Failed to allocate enough memory for perlin noise!", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &__texid);
        return false;
    }

    memset(*data, 0, dsize);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, *data);

    glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

bool __ob_ext_perlin_init(void)
{
    if (__init)
        return false;

    __width = OB_EXT_PERLIN_MAX_WIDTH;
    __height = OB_EXT_PERLIN_MAX_HEIGHT;

    obsidian_shader_t __shader = OBSHDRcreateShader("res/shaders/perlin_noise.glsl", OBSHDR_COMPUTE_SHADER);
    __program = OBSHDRcreateProgram();
    glGenTextures(1, &__texid);
    glBindTexture(GL_TEXTURE_2D, __texid);
    glBindTexture(GL_TEXTURE_2D, 0);

    OBSHDRprogramAttach(__program, 1, __shader);
    OBSHDRprogramLink(__program);

    OBSHDRdestroyShader(__shader);

    __init = true;
    return true;
}

bool __ob_ext_perlin_close(void)
{
    if (!__init)
        return false;

    OBSHDRdestroyProgram(__program);
    glDeleteTextures(1, &__texid);
    __init = false;

    return true;
}