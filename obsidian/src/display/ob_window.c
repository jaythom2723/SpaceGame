#include "display/ob_window.h"

#include "utility/ob_logger.h"
#include "utility/ob_error.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define __OB_MAX_INIT_ATTEMPTS 4

static uint32_t __init_attempts = 0;

static struct obsidian_window* window = NULL;

bool __ob_wnd_initmodule(void);
bool __ob_wnd_closemodule(void);

extern void __ob_log_wline(enum ob_logger_message_type, const char* const);
extern void __ob_log_wsline(const char* const);

extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

bool __ob_wnd_initmodule(void)
{   
    if (__init_attempts >= __OB_MAX_INIT_ATTEMPTS)
    {
        (void)__ob_error_pusherror(ERR_MODULE_INIT, SEV_FATAL, CAT_CORE_SYS, "Failed to initialize Obisidian: Window Core Module", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }

    __init_attempts ++;
    if (window != NULL)
    {
        (void)__ob_error_pusherror(ERR_MODULE_INIT, SEV_WARNING, CAT_CORE_SYS, "Obsidian: Window Core Module, has already been initialized...", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }

    window = calloc(1, sizeof(struct obsidian_window));
    if (window == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_RECOVER, CAT_MEMORY, "Failed to allocate enough memory for the window... Attempting again...", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return __ob_wnd_initmodule();
    }

    window->handle = NULL;
    window->width = 0;
    window->height = 0;
    window->title = NULL;

    return true;
}

bool __ob_wnd_closemodule(void)
{
    if (window == NULL)
        return false;


    if (window->title != NULL)
    {
        free(window->title);
        window->title = NULL;
    }

    free(window);
    window = NULL;
    return true;
}

bool OBWNDsetTitle(const char* buffer)
{
    if (window == NULL || window->title != NULL)
        return false;

    window->title = calloc(strlen(buffer) + 1, sizeof(char));
    if (window->title == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_FATAL, CAT_MEMORY, "Failed to allocate enough memory for the title of the window.", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }

    strcpy(window->title,  buffer);

    return true;
}

bool OBWNDsetSize(const uint32_t width, const uint32_t height)
{
    if (window == NULL)
        return false;

    window->width = width;
    window->height = height;
    
    return true;
}

const char* OBWNDgetTitle(void)
{
    if (window == NULL || window->title == NULL)
        return NULL;

    char* buffer = calloc(strlen(window->title)+1, sizeof(char));
    if (buffer == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_WARNING, CAT_MEMORY, "Could not return the title of the window!", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return NULL;
    }

    strcpy(buffer, window->title);

    return buffer;
}

uint32_t OBWNDgetWidth(void)
{
    if (window == NULL)
        return 0;

    return window->width;
}

uint32_t OBWNDgetHeight(void)
{
    if (window == NULL)
        return 0;

    return window->height;
}

void OBWNDgetSize(uint32_t* restrict width, uint32_t* restrict height)
{
    if (window == NULL)
    {
        (*width) = 0;
        (*height) = 0;
        return;
    }

    (*width) = window->width;
    (*height) = window->height;
}