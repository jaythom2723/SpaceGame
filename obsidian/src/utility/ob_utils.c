#include "utility/ob_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

bool __ob_util_openfile(FILE**, const char* const, const char* const);
char* __ob_util_readfile(const char* const);
char* __ob_util_dtostr(const uint32_t value);

extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

extern uint32_t __ob_math_ndgts(uint32_t);

bool __ob_util_openfile(FILE** fp, const char* const path, const char* const mode)
{
    (*fp) = fopen(path, mode);
    if ((*fp) == NULL)
    {
        (void)__ob_error_pusherror(ERR_FILE_NOT_FOUND, SEV_WARNING, CAT_FILESYSTEM, "Could not find a file", path, 0);
        (void)__ob_error_readerror();
        return false;
    }
    return true;
}

char* __ob_util_readfile(const char* const path)
{
    FILE* fp = NULL;
    size_t size = 0;
    char* buffer = NULL;

    if (__ob_util_openfile(&fp, path, "r") == false)
        return NULL;

    fseek(fp, 0l, SEEK_END);
    size = ftell(fp);
    rewind(fp);

    buffer = calloc(size + 1, sizeof(char));
    if (buffer == NULL)
    {
        // TODO: replace with error handler message
        printf("[OBSIDIAN]: Could not ");
        return NULL;
    }

    fread(buffer, 1, size, fp);
    printf("%s\n", buffer);
    return buffer;
}

char* __ob_util_dtostr(const uint32_t value)
{
    uint32_t ndigits = __ob_math_ndgts(value);
    char* buffer = calloc(ndigits + 1, sizeof(char));
    if (buffer == NULL)
    {
        __ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_WARNING, CAT_MEMORY, "Out of Memory! Could not allocate enough bytes for decimal to string conversion!", __FILE__, __LINE__);
        __ob_error_readerror();
        return NULL;
    }
    memset(buffer, 0, ndigits + 1 * sizeof(char));
    sprintf(buffer, "%d", value);
    return buffer;
}