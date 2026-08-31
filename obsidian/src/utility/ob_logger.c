#include "utility/ob_logger.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

bool __ob_log_getlogfilename(char**, size_t*);
bool __ob_log_openlogfile(const char*);
bool __ob_log_initmodule(void);

extern bool __ob_util_openfile(FILE** fp, const char* const path, const char* const mode);
extern const char* __ob_util_readfile(const char* path);

extern uint32_t __ob_math_ndgts(uint32_t n);

static FILE* logfile = NULL;

bool __ob_log_initmodule(void)
{
    char* logfile_name = NULL;
    size_t logfile_namelen = 0;
    if (__ob_log_getlogfilename(&logfile_name, &logfile_namelen))
        if (!__ob_log_openlogfile(logfile_name))
        {
            // TODO: error handler message here
            printf("[OBSIDIAN:FATAL] Failed to open a log file! Required by Obsidian for proper functionality and debugging!\n");
            free(logfile_name);
            logfile_name = NULL;
            return false;            
        }

    return true;
}

bool __ob_log_getlogfilename(char** buffer, size_t* bufferSize)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    uint32_t ydigits, mdigits, ddigits;
    ydigits = __ob_math_ndgts(tm.tm_year + 1900);
    mdigits = __ob_math_ndgts(tm.tm_mon + 1);
    ddigits = __ob_math_ndgts(tm.tm_mday);

    char* ybuf, *mbuf, *dbuf;
    ybuf = calloc(ydigits + 1, sizeof(char));
    mbuf = calloc(mdigits + 1, sizeof(char));
    dbuf = calloc(ddigits + 1, sizeof(char));

    if (ybuf == NULL || mbuf == NULL || dbuf == NULL)
    {
        // TODO: error handler message here
        printf("[OBSIDIAN]: Failed to allocate enough memory!\n");
        return false;
    }

    sprintf(ybuf, "%d", tm.tm_year + 1900);
    sprintf(mbuf, "%d", tm.tm_mon + 1);
    sprintf(dbuf, "%d", tm.tm_mday);

    // new buffer size, allocating buffer
    (*bufferSize) = ydigits + mdigits + ddigits + strlen(".log") + 3; // +2 for '.' and + 1 for \0
    (*buffer) = calloc(*bufferSize, sizeof(char));
    if (*buffer == NULL)
    {
        // TODO: error handler message here
        printf("[OBSIDIAN]: Failed to allocate enough memory!\n");
        free(ybuf);
        free(mbuf);
        free(dbuf);
        return false;
    }

    memset(*buffer, 0, *bufferSize);
    strcpy(*buffer, mbuf);
    strcat(*buffer, ".");
    strcat(*buffer, dbuf);
    strcat(*buffer, ".");
    strcat(*buffer, ybuf);
    strcat(*buffer, ".log");

    free(ybuf);
    free(mbuf);
    free(dbuf);

    return true;
}

bool __ob_log_openlogfile(const char* name)
{
    return __ob_util_openfile(&logfile, name, "w");
}

bool OBLOGcloseLogFile(void)
{
    return false;
}

bool OBLOGwriteHeader(void)
{
    return false;
}

bool OBLOGwriteLine(enum ob_logger_message_type type, const char* const line)
{
    printf("%d, %s\n", type, line);
    return false;
}

bool OBLOGwriteFormat(enum ob_logger_message_type type, const char* const line, ...)
{
    printf("%d, %s\n", type, line);
    return false;
}