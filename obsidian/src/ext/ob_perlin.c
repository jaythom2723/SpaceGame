#include "ext/ob_perlin.h"
#include "cglm/util.h"
#include "graphics/ob_shader.h"
#include "utility/ob_error.h"
#include "utility/ob_logger.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
 
#include <cglm/cglm.h>

#include <glad/glad.h>

extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

extern bool __ob_log_wline(enum ob_logger_message_type, const char* const);
extern bool __ob_log_wsline(const char* const);

static uint32_t __width, __height, __resolution, __frequency;
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

void OBEXTperlinSetResolution(const uint32_t value)
{
    if (!__init)
        return;
    __resolution = value;
}

void OBEXTperlinSetFrequency(const uint32_t value)
{
    if (!__init)
        return;
    __frequency = value;
}

float* OBEXTperlinInvoke(void)
{
    if (!__init)
        return NULL;

    OBSHDRuseProgram(__program);
    OBSHDRsetui(__program, "imageWidth", __width);
    OBSHDRsetui(__program, "imageHeight", __height);
    OBSHDRsetui(__program, "noiseFrequency", __frequency);
    
    glBindTexture(GL_TEXTURE_2D, __texid);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, __width, __height);
    glBindImageTexture(0, __texid, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

    glDispatchCompute(__width / 8, __height / 8, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_UPDATE_BARRIER_BIT);

    float* noise = calloc(__width * __height, sizeof(float));
    if (noise == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_WARNING, CAT_MEMORY, "Failed to allocate enough memory to properly execute the perlin noise compute shader", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
        return NULL;
    }
    
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, noise);

    return noise;
}

struct obsidian_asset* OBEXTperlinInvokeAsset(void)
{
    if (!__init)
        return NULL;

    float* noise = OBEXTperlinInvoke();
    uint8_t* pixels = calloc(__width * __height * 4, sizeof(uint8_t));

    for (uint32_t i = 0; i < __width * __height; i++)
    {
        float value = noise[i];
        value = value * 0.5f + 0.5f;

        if (value < 0.0f)
            value = 0.0f;

        if (value > 1.0f)
            value = 1.0f;

        uint8_t red = (uint8_t)floor(255 * value);
        uint32_t index = i * 4;
        pixels[index] = red;
        pixels[index + 1] = red;
        pixels[index + 2] = red;
        pixels[index + 3] = 255;
    }

    free(noise);
    noise = NULL;

    struct obsidian_asset* ret = NULL;
    if (!OBASTcreatePrimitiveTexture(&ret, __width, __height, pixels, __width * __height * 4 * sizeof(uint8_t)))
    {
        printf("Failed to create a texture primitive!\n");
        return NULL;
    }

    free(pixels);
    pixels = NULL;

    return ret;
}

bool __ob_ext_perlin_init(void)
{
    if (__init)
        return false;

    __width = OB_EXT_PERLIN_MAX_WIDTH;
    __height = OB_EXT_PERLIN_MAX_HEIGHT;

    obsidian_shader_t __shader = OBSHDRcreateShader("res/shaders/perlin_noise.glsl", OBSHDR_COMPUTE_SHADER);
    __program = OBSHDRcreateProgram();
    OBSHDRprogramAttach(__program, 1, __shader);
    OBSHDRprogramLink(__program);
    OBSHDRuseProgram(__program);

    glGenTextures(1, &__texid);
    glBindTexture(GL_TEXTURE_2D, __texid);

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
