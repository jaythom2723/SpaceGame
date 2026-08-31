#include "utility/ob_logger.h"

#include <stdio.h>
#include <stdbool.h>

extern bool __ob_util_openfile(FILE** fp, const char* const path, const char* const mode);
extern const char* __ob_util_readfile(const char* path);

static FILE* logfile = NULL;

bool OBLOGgetLogFileName(char* buffer, size_t bufferSize)
{
    printf("%s, %lu\n", buffer, bufferSize);
    return false;
}

bool OBLOGopenNewLogFile(const char* name)
{
    printf("%s\n", name);
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