#include "ext/ob_perlin.h"
#include "graphics/ob_shader.h"
#include "utility/ob_error.h"
#include "utility/ob_logger.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
 
#include <cglm/cglm.h>

#include <glad/glad.h>

extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

extern bool __ob_log_wline(enum ob_logger_message_type, const char* const);
extern bool __ob_log_wsline(const char* const);

static uint32_t __width, __height;
static obsidian_program_t __program;
static uint32_t __texid;
static uint32_t __gradient_buffer_id;

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

float* OBEXTperlinInvoke(void)
{
    if (!__init)
        return NULL;

    OBSHDRuseProgram(__program);
    OBSHDRseti(__program, "imageWidth", __width);
    OBSHDRseti(__program, "imageHeight", __height);
    
    glBindTexture(GL_TEXTURE_2D, __texid);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, __width, __height);
    glBindImageTexture(0, __texid, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, __gradient_buffer_id);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, __gradient_buffer_id);

    uint32_t wdiv = __width / 100;
    uint32_t hdiv = __height / 100;
    glDispatchCompute(__width / wdiv, __height / hdiv, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    float* noise = calloc(__width * __height, sizeof(float));
    if (noise == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_WARNING, CAT_MEMORY, "Failed to allocate enough memory to properly execute the perlin noise compute shader", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
        return NULL;
    }
    
    // well... The algorithm clearly isn't working anymore...

    glGetTexImage(GL_TEXTURE_2D, 0, GL_R32F, GL_FLOAT, noise);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
    glBindTexture(GL_TEXTURE_2D, 0);

    return noise;
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

    glGenBuffers(1, &__gradient_buffer_id);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, __gradient_buffer_id);

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
