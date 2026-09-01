#include "utility/ob_error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

bool __ob_error_initmodule(void);
bool __ob_error_closemodule(void);
bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char* message, const char*, const uint32_t);
bool __ob_error_pollerrors(void);
bool __ob_error_readerror(void);
struct obsidian_error __ob_error_poperror(void);

static bool __module_init = false;
static struct obsidian_error* __errors = NULL;
static struct obsidian_error* __errorptr = NULL;
static const uint32_t __max_errors = 256;

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

bool __ob_error_pusherror(enum obsidian_error_code code, enum obsidian_error_severity severity, enum obsidian_error_category category, const char* message, const char* file, const uint32_t line) 
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
        message = (const char*)message,
        file = (const char*)file,
        .line = line
    };

    (*__errorptr) = error;
    __errorptr++;
    return true;
}

bool __ob_error_pollerrors(void) { return __errors - __errorptr > 0; }

const char* __translate_error_code(const struct obsidian_error* error)
{
    switch(error->code)
    {
#define X(name, message) \
        case name: return message;

        ERROR_CODES(X)
#undef X

        default:
            return "Unknown error";
    }
}

const char* __translate_severity_code(const struct obsidian_error* error)
{
    switch (error->severity)
    {
#define X(name, message) \
        case name: return message;
        SEVERITY_CODES(X)
#undef X

        default:
            return "Unknown severity";
    }
}

const char* __translate_category_code(const struct obsidian_error* error)
{
    switch (error->category)
    {
#define X(name, message) \
        case name: return message;
        CATEGORY_CODES(X)
#undef X

        default:
            return "Unknown category";
    }
}

bool __ob_error_readerror(void)
{
    struct obsidian_error top = __ob_error_poperror();
    const char* codestr = __translate_error_code(&top);
    const char* sevstr = __translate_severity_code(&top);
    const char* catstr = __translate_category_code(&top);

    printf("[%s ($%04X)] %s\n", sevstr, top.code, codestr);
    printf("\tCategory: %d \"%s\"\n", top.category, catstr);
    printf("\tAt: %s:%d\n", top.file, top.line);
    printf("\tMessage: %s\n", top.message);

    // TODO: propogate to the log file as well

    return true;
}

struct obsidian_error __ob_error_poperror(void)
{
    struct obsidian_error top = {0};
    if (__errors - __errorptr == 0 && (*__errors).message == NULL)
        return top;
    top = *(__errorptr - 1);
    *(__errorptr - 1) = (struct obsidian_error){0};
    __errorptr--;
    return top;
}