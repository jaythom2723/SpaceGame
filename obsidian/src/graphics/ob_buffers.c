#include "utility/ob_error.h"
#include "utility/ob_logger.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <glad/glad.h>

#define __OB_MAX_VBOS 0xFF
#define __OB_MAX_EBOS 0xFF
#define __OB_MAX_VAOS 0xFF

bool __ob_buf_initmodule(void);
bool __ob_buf_closemodule(void);

uint32_t __ob_buf_createvbo(void);
bool __ob_buf_deletevbo(uint32_t);
bool __ob_buf_bindvbo(uint32_t);
bool __ob_buf_setvbodata(uint32_t, size_t, const void*, uint32_t);
void __ob_buf_unbindvbo(void);

uint32_t __ob_buf_createebo(void); // TODO: RnD
bool __ob_buf_deleteebo(uint32_t);
bool __ob_buf_bindebo(uint32_t);
bool __ob_buf_setebodata(uint32_t, size_t, const void*, uint32_t);
void __ob_buf_unbindebo(void);

uint32_t __ob_buf_createvao(void);
bool __ob_buf_deletevao(uint32_t);
bool __ob_buf_bindvao(uint32_t);
void __ob_buf_setattribpointer(uint32_t,uint32_t,size_t,void*);
void __ob_buf_unbindvao(void);

extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

extern void __ob_log_wline(enum ob_logger_message_type, const char* const);
extern void __ob_log_wsline(const char* const);

static uint32_t* vaos = NULL;
static uint32_t* vbos = NULL;
static uint32_t* ebos = NULL;

static uint32_t* vaoptr = NULL;
static uint32_t* vboptr = NULL;
static uint32_t* eboptr = NULL;

bool __ob_buf_initmodule(void)
{
    if (vbos != NULL || ebos != NULL || vaos != NULL)
        return false;

    vbos = calloc(__OB_MAX_VBOS, sizeof(uint32_t));
    ebos = calloc(__OB_MAX_EBOS, sizeof(uint32_t));
    vaos = calloc(__OB_MAX_VAOS, sizeof(uint32_t));

    if (vbos == NULL || ebos == NULL || vaos == NULL)
    {
        (void)__ob_error_pusherror(ERR_MODULE_INIT, SEV_FATAL, CAT_CORE_SYS, "Failed to initialize Obisidian: Buffers Core Module", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }

    memset(vbos, 0, __OB_MAX_VBOS * sizeof(uint32_t));
    memset(ebos, 0, __OB_MAX_EBOS * sizeof(uint32_t));
    memset(vaos, 0, __OB_MAX_VAOS * sizeof(uint32_t));

    vaoptr = vaos;
    vboptr = vbos;
    eboptr = ebos;

    return true;
}

bool __ob_buf_closemodule(void)
{
    if (vbos == NULL || ebos == NULL || vaos == NULL)
        return false;

    // delete any buffers that haven't yet been deleted
    for (int i = 0; i < __OB_MAX_VBOS; i++)
    {
        if (glIsBuffer(*(vbos + i)) == GL_FALSE)
            continue;
        glDeleteBuffers(1, (vbos + i));
    }

    for (int i = 0; i < __OB_MAX_EBOS; i++)
    {
        if (glIsBuffer(*(ebos + i)) == GL_FALSE)
            continue;
        glDeleteBuffers(1, (ebos + i));
    }

    // do the same for vertex arrays
    for (int i = 0; i < __OB_MAX_VAOS; i++)
    {
        if (glIsVertexArray(*(vaos + i)) == GL_FALSE)
            continue;
        glDeleteVertexArrays(1, (vaos + i));
    }

    vaoptr = NULL;
    vboptr = NULL;
    eboptr = NULL;

    free(vaos);
    free(vbos);
    free(ebos);
    vaos = NULL;
    vbos = NULL;
    ebos = NULL;

    return true;
}

uint32_t __ob_buf_createvbo(void)
{
    if ((vboptr - vbos) >= __OB_MAX_VBOS)
        return 0xFF;

    uint32_t index = (uint32_t)(vboptr - vbos);
    glGenBuffers(1, vboptr);

    /*
        Must bind buffer name immediately after creation or the name is not valid for glIsBuffer. (Stupid...)

        https://registry.khronos.org/OpenGL-Refpages/gl4/html/glIsBuffer.xhtml

        glIsBuffer returns GL_FALSE, if buffer is zero, or is a non-zero value that is not currently the name of a buffer object, or if an error occurs.
        A name returned by glGenBuffers, but not yet associated with a buffer object by calling glBindBuffer, is not the name of a buffer object.
    */
    glBindBuffer(GL_ARRAY_BUFFER, *vboptr);
    vboptr++;
    return index;
}

bool __ob_buf_deletevbo(uint32_t i)
{
    uint32_t* buffer = (vbos + i);
    if (glIsBuffer(*buffer) == GL_FALSE)
    {
        // TODO: replace with log/error
        printf("Failed to delete VBO!\n");
        return false;
    }
    glDeleteBuffers(1, buffer);
    return true;
}

bool __ob_buf_bindvbo(uint32_t i)
{
    uint32_t* buffer = (vbos + i);
    if (glIsBuffer(*buffer) == GL_FALSE)
    {
        // TODO: replace with log/error
        printf("Failed to bind buffer!\n");
        return false;
    }
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    return true;
}

bool __ob_buf_setvbodata(uint32_t i, size_t sbytes, const void* data, uint32_t usage)
{
    uint32_t* buffer = (vbos + i);
    if (glIsBuffer(*buffer) == GL_FALSE)
    {
        // TODO: replace with log/error
        printf("Failed to set vbo data!\n");
        return false;
    }

    (void)__ob_buf_bindvbo(i);
    glBufferData(GL_ARRAY_BUFFER, sbytes, data, usage);

    return true;
}

void __ob_buf_unbindvbo(void)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*
uint32_t __ob_buf_createebo(void); // TODO: RnD
bool __ob_buf_deleteebo(uint32_t);
bool __ob_buf_bindebo(uint32_t);
bool __ob_buf_setebodata(uint32_t, size_t, const void*, uint32_t);
void __ob_buf_unbindebo(void);
*/

uint32_t __ob_buf_createebo(void)
{
    if (ebos == NULL || vbos == NULL || vaos == NULL)
        return 0xFF;

    if((eboptr - ebos) >= __OB_MAX_EBOS)
        return 0xFF;

    uint32_t index = (uint32_t)(eboptr - ebos);
    glGenBuffers(1, eboptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *eboptr);
    eboptr++;
    return index;
}

bool __ob_buf_deleteebo(uint32_t index)
{
    if (ebos == NULL || vbos == NULL || vaos == NULL)
        return false;

    uint32_t* buffer = (ebos + index);
    if (glIsBuffer(*buffer) == GL_FALSE)
        return false;

    glDeleteBuffers(1, ebos + index);
    return true;
}

bool __ob_buf_bindebo(uint32_t index)
{
    if (ebos == NULL || vbos == NULL || vaos == NULL)
        return false;

    uint32_t* buffer = (ebos + index);
    if (glIsBuffer(*buffer) == GL_FALSE)
        return false;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *buffer);
    return true;
}

bool __ob_buf_setebodata(uint32_t index, size_t sbytes, const void* data, uint32_t usage)
{
    if (ebos == NULL || vbos == NULL || vaos == NULL)
        return false;

    uint32_t* buffer = (ebos + index);
    if (glIsBuffer(*buffer) == GL_FALSE)
        return false;
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sbytes, data, usage);

    return true;
}

void __ob_buf_unbindebo(void)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

/*
uint32_t __ob_buf_createvao(void);
bool __ob_buf_deletevao(uint32_t);
bool __ob_buf_bindvao(uint32_t);
void __ob_buf_setattribpointer(uint32_t,uint32_t,size_t,void*);
void __ob_buf_unbindvao(void);
*/

uint32_t __ob_buf_createvao(void)
{
    if (vaos == NULL || vbos == NULL || ebos == NULL)
        return 0xFF;

    if ((vaoptr - vaos) >= __OB_MAX_VAOS)
        return 0xFF;

    uint32_t index = (uint32_t)(vaoptr - vaos);
    glGenVertexArrays(1, vaoptr);
    glBindVertexArray(*vaoptr);
    vaoptr++;
    return index;
}

bool __ob_buf_deletevao(uint32_t index)
{
    if (vaos == NULL || vbos == NULL || ebos == NULL)
        return false;

    uint32_t* vao = vaos + index;
    if (glIsVertexArray(*vao) == GL_FALSE)
        return false;
    glDeleteVertexArrays(1, vao);
    return true;
}

bool __ob_buf_bindvao(uint32_t index)
{
    if (vaos == NULL || vbos == NULL || ebos == NULL)
        return false;

    uint32_t* vao = vaos + index;
    if (glIsVertexArray(*vao) == GL_FALSE)
        return false;
    glBindVertexArray(*vao);
    return true;
}

void __ob_buf_setattribpointer(uint32_t index, uint32_t size, size_t stride, void* offset)
{
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(index);
}

void __ob_buf_unbindvao(void)
{
    glBindVertexArray(0);
}