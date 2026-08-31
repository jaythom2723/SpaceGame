#include "utility/ob_logger.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

extern bool __ob_util_openfile(FILE** fp, const char* const path, const char* const mode);
extern const char* __ob_util_readfile(const char* path);
extern uint32_t __ob_math_ndigits(uint32_t n);

// static FILE* logfile = NULL;

bool __ob_log_getlogfilename(char** buffer, size_t* bufferSize)
{
    if (*buffer != NULL)
        return false;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    uint32_t ydigits = __ob_math_ndigits(tm.tm_year + 1900);
    uint32_t mdigits = __ob_math_ndigits(tm.tm_mon + 1);
    uint32_t ddigits = __ob_math_ndigits(tm.tm_mday);

    char* yearbuf, *monbuf, *daybuf;
    yearbuf = calloc(ydigits + 1, sizeof(char));
    monbuf = calloc(mdigits + 1, sizeof(char));
    daybuf = calloc(ddigits + 1, sizeof(char));

    if (yearbuf == NULL || monbuf == NULL || daybuf == NULL)
    {
        // TODO: Change to an error handler message
        printf("Could not allocate enough memory for the log file!\n");
        return false;
    }

    sprintf(yearbuf, "%d", tm.tm_year + 1900);
    sprintf(monbuf, "%d", tm.tm_mon + 1);
    sprintf(daybuf, "%d", tm.tm_mday);

    // Set up the new name buffer
    (*bufferSize) = strlen(yearbuf) + 1 + strlen(monbuf) + 1 + strlen(daybuf) + 1 + strlen(".log") + 1;
    (*buffer) = calloc(*bufferSize, sizeof(char));
    if (*buffer == NULL)
    {
        // TODO: Change to an error handler message
        printf("Could not allocate enough memory for the log file!\n");
        return false;
    }

    memset(*buffer, 0, (*bufferSize) * sizeof(char));
    strcpy(*buffer, monbuf);
    strcat(*buffer, ".");
    strcat(*buffer, daybuf);
    strcat(*buffer, ".");
    strcat(*buffer, yearbuf);
    strcat(*buffer, ".log");

    return true;
}

bool __ob_log_openlogfile(const char* const name)
{
    printf("%s\n", name);


    free((void*) name);

    return false;
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