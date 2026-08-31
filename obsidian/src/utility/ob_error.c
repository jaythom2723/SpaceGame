#include "utility/ob_error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

bool __ob_error_initmodule(void);
bool __ob_error_closemodule(void);
bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char* message);

static bool __module_init = false;
static struct obsidian_error* __errors = NULL;
static struct obsidian_error* __errorptr = NULL;
static uint32_t __max_errors = 256;

bool __ob_error_initmodule(void)
{
    if (__module_init)
        return false;

    __errors = calloc(__max_errors, sizeof(struct obsidian_error));
    if (__errors == NULL)
    {
        printf("[OBSIDIAN:FATAL] Failed to allocate enough memory for the error handler!\n");
        return false;
    }
 
    __errorptr = __errors;
    __module_init = true;
    return true;
}

bool __ob_error_closemodule(void)
{
    if(!__module_init || __errors == NULL)
        return false;

    __errorptr = NULL;
    free(__errors);
    __errors = NULL;
    __module_init = false;
    return true;
}

bool __ob_error_pusherror(enum obsidian_error_code code, enum obsidian_error_severity severity, enum obsidian_error_category category, const char* message) 
{
    // overflow reset
    if ((uint32_t)(__errors - __errorptr) >= 256)
    {
        memset(__errors, 0, __max_errors * sizeof(struct obsidian_error));
        __errorptr = __errors;
    }

    struct obsidian_error error = {
        code,
        severity,
        category,
        message = (const char*)message
    };

    (*__errorptr) = error;
    __errorptr++;
    return true;
}