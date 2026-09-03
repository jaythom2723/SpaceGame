#include "graphics/ob_shader.h"

#include "utility/ob_error.h"
#include "utility/ob_logger.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <glad/glad.h>

#define __OB_MAX_SHADERS 0xFF
#define __OB_MAX_PROGRAMS 0xF

typedef uint32_t __ob_gl_shader_t;
typedef uint32_t __ob_gl_program_t;

extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

extern bool __ob_log_wline(enum ob_logger_message_type, const char*);
extern bool __ob_log_wsline(const char*);

extern char* __ob_util_readfile(const char* const);

static obsidian_program_t* __programs = NULL;
static obsidian_program_t* __programptr = NULL;

static obsidian_shader_t* __shaders = NULL;
static obsidian_shader_t* __shaderptr = NULL;

bool __ob_shdr_initmodule(void);
void __ob_shdr_closemodule(void);

bool __ob_shdr_initmodule(void)
{
    if (__shaders != NULL || __programs != NULL)
        return false;

    __shaders = calloc(__OB_MAX_SHADERS, sizeof(__ob_gl_shader_t));
    if (__shaders == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_FATAL, CAT_MEMORY, "Failed to allocate memory for Obsidian: Shader Core Module", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }
    __shaderptr = __shaders;

    __programs = calloc(__OB_MAX_PROGRAMS, sizeof(__ob_gl_program_t));
    if (__programs == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_FATAL, CAT_MEMORY, "Failed tto allocate memory for Obsidian: Shader Core Module", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }
    __programptr = __programs;

    return true;
}

void __ob_shdr_closemodule(void)
{
    if (__shaders == NULL)
        return;

    __programptr = NULL;
    __shaderptr = NULL;
    
    free(__shaders);
    __shaders = NULL;

    free(__programs);
    __programs = NULL;
}

char* __translate_shader_type(const enum obsidian_shader_type type)
{
    switch(type)
    {
#define X(name, message) \
        case name: return message;
#undef X

        default:
            return "Unkown shader type";
    }
}

GLenum __shader_type_to_gl_type(const enum obsidian_shader_type type)
{
    switch(type)
    {
        case OBSHDR_VERTEX_SHADER: return GL_VERTEX_SHADER;
        case OBSHDR_FRAGMENT_SHADER: return GL_FRAGMENT_SHADER;
        case OBSHDR_COMPUTE_SHADER: return GL_COMPUTE_SHADER;
        case OBSHDR_GEOMETRY_SHADER: return GL_GEOMETRY_SHADER;
        default:
            return 0;
    }
}

obsidian_shader_t OBSHDRcreateShader(const char* path, enum obsidian_shader_type type)
{
    if (__shaders == NULL)
    {
        (void)__ob_error_pusherror(ERR_MODULE_INIT, SEV_WARNING, CAT_GRAPHICS, "Cannot create a shader if the shader module is not initialized...", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return 0;
    }

    if ((__shaders - __shaderptr) >= __OB_MAX_SHADERS)
        return 0;

    __ob_log_wline(LOG_MESSAGE_INFORM, "Attempting to create shader...");
    __ob_log_wsline(__translate_shader_type(type));
    __ob_log_wsline(path);

    __ob_gl_shader_t shader = glCreateShader(__shader_type_to_gl_type(type));
    char* source = __ob_util_readfile(path);
    if (source == NULL)
    {
        (void)__ob_error_pusherror(ERR_MODULE_INIT, SEV_WARNING, CAT_GRAPHICS, "Failed to get shader source.", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return 0;
    }
    glShaderSource(shader, 1, (const char* const*)&source, NULL);
    glCompileShader(shader);
    
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0)
    {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        (void)__ob_error_pusherror(ERR_GL_ERROR, SEV_WARNING, CAT_GRAPHICS, infoLog, __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return 0;
    }

    (*__shaderptr) = shader;
    __shaderptr++;

    free(source);
    source = NULL;

    return (obsidian_shader_t)(__shaders - (__shaderptr - 1));
}

void OBSHDRdestroyShader(const obsidian_shader_t index)
{
    __ob_gl_shader_t* shader = __shaders + index;

    if (glIsShader(*shader) == GL_FALSE)
        return;

    glDeleteShader(*shader);
    (*shader) = 0;
}

obsidian_program_t OBSHDRcreateProgram(void)
{
    if (__shaders == NULL || __programs == NULL)
        return 0;

    if ((__programs - __programptr) >= __OB_MAX_PROGRAMS)
        return 0;

    (*__programptr) = glCreateProgram();
    __programptr++;
    return (__programs - (__programptr - 1));
}

void OBSHDRdestroyProgram(const obsidian_program_t index)
{
    if (__programs == NULL || __shaders == NULL)
        return;

    __ob_gl_program_t* program = __programs + index;
    if (glIsProgram(*program) != GL_TRUE)
        return;
    glDeleteProgram(*program);
}

void OBSHDRprogramAttach(const obsidian_program_t index, int n, ...)
{
    // TODO: segfault here, rewrite please
    if (__programs == NULL || __shaders == NULL)
        return;

    __ob_gl_program_t* program = (__programs + index);
    if (glIsProgram(*program) == GL_FALSE)
        return;

    va_list args;
    va_start(args, n);

    for (int i = 0; i < n; i++)
    {
        const obsidian_shader_t sindex = va_arg(args, obsidian_shader_t);
        __ob_gl_shader_t* shader = (__shaders + sindex);
        if (glIsShader(*shader) != GL_TRUE)
            continue;
        glAttachShader(*program, *shader);
    }
}

bool OBSHDRprogramLink(const obsidian_program_t index)
{
    if (__programs == NULL || __shaders == NULL)
        return false;

    __ob_gl_program_t* program = (__programs + index);
    if (glIsProgram((*program) == GL_FALSE))
        return false;

    glLinkProgram(*program);
    int success;
    char infoLog[1024];
    glGetProgramiv(*program, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        glGetProgramInfoLog(*program, 1024, NULL, infoLog);
        (void)__ob_error_pusherror(ERR_GL_ERROR, SEV_WARNING, CAT_GRAPHICS, infoLog, __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }

    __ob_log_wline(LOG_MESSAGE_WARNING, "Remember to delete linked shaders to free up unused resources!");

    return true;
}

void OBSHDRuseProgram(const obsidian_program_t index)
{
    if (__programs == NULL || __shaders == NULL)
        return;

    __ob_gl_program_t* program = (__programs + index);
    if (glIsProgram(*program) == GL_FALSE)
        return;

    glUseProgram(*program);
}