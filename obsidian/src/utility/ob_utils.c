#include "utility/ob_error.h"
#include "utility/ob_time.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

bool __ob_util_openfile(FILE**, const char* const, const char* const);
char* __ob_util_readfile(const char* const);
char* __ob_util_dtostr(const uint32_t value);
uint32_t __ob_util_genUniqueIdentity(const void* aptr, const void* bptr);

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
        printf("[OBSIDIAN]: Could not allocate enough memory for a file!\n");
        return NULL;
    }

    fread(buffer, 1, size, fp);

    fclose(fp);

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

#define __OB_32BTO8B(base,a,b,c,d) \
    a = (base & 0xFF000000) >> 24; \
    b = (base & 0x00FF0000) >> 16; \
    c = (base & 0x0000FF00) >> 8; \
    d = base & 0x000000FF; 

uint32_t __ob_util_genUniqueIdentity(const void* aptr, const void* bptr)
{
    srand(OBTIMEgenerateSeed());
    uint32_t base = rand();
    uint32_t hilo = (uint32_t)(aptr-bptr);
    uint32_t lohi = (uint32_t)(bptr-aptr);
    uint32_t unique_id = 0;
    uint8_t ba,bb,bc,bd;
    uint8_t hia,hib,hic,hid;
    uint8_t loa,lob,loc,lod;
    uint8_t a,b,c,d;

    __OB_32BTO8B(base,ba,bb,bc,bd);
    __OB_32BTO8B(hilo,hia,hib,hic,hid);
    __OB_32BTO8B(lohi,loa,lob,loc,lod);
    a = (hia / ba) ^ (loa | ~ba);
    b = (hib / bb) ^ (lob | ~bb);
    c = (hic / bc) ^ (loc | ~bc);
    d = (hid / bd) ^ (lod | ~bd);

    unique_id |= (a << 24);
    unique_id |= (b << 16);
    unique_id |= (c << 8);
    unique_id |= d;

    return unique_id;
}