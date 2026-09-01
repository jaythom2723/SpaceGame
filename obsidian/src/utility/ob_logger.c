#include "utility/ob_logger.h"
#include "utility/ob_time.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

bool __ob_log_getlogfilename(char**, size_t*);
bool __ob_log_openlogfile(const char*);
bool __ob_log_initmodule(void);
bool __ob_log_closelogfile(void);
bool __ob_log_wheader(void);
bool __ob_log_wline(enum ob_logger_message_type, const char* const);
bool __ob_log_wsline(const char* const);
bool __ob_log_wlinef(enum ob_logger_message_type, const char* const, ...);
bool __ob_log_wslinef(const char* const, ...);

extern bool __ob_util_openfile(FILE**, const char*, const char*);
extern char* __ob_util_readfile(const char*);
extern char* __ob_util_dtostr(const uint32_t);

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
    uint32_t year, month, day;
    OBTIMEgetDayMonthYear(&year, &month, &day);
    char* ybuf = __ob_util_dtostr(year);
    char* mbuf = __ob_util_dtostr(month);
    char* dbuf = __ob_util_dtostr(day);

    // new buffer size, allocating buffer
    (*bufferSize) = strlen(ybuf) + strlen(mbuf) + strlen(dbuf) + strlen(".log") + 3; // +2 for '.' and + 1 for \0
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

bool __ob_log_closelogfile(void)
{
    if (logfile == NULL)
        return false;
    fclose(logfile);
    logfile = NULL;
    return true;
}

bool __ob_log_wheader(void)
{
    // TODO: prototype
    return false;
}

const char* __translate_logger_message_type(const enum ob_logger_message_type type)
{
    switch(type)
    {
#define X(name, message) \
        case name: return message;
        LOGGER_MESSAGE_TYPES(X)
#undef X

        default:
            return "NULL";
    }
}

bool __ob_log_wline(enum ob_logger_message_type type, const char* const line)
{
    if (logfile == NULL)
        return false;
    
    // Timestamp\t|\t[<type>]\t<message>\n
    uint32_t hours, minutes, seconds;
    OBTIMEgetTimestamp(&hours, &minutes, &seconds);
    fprintf(logfile, "%2d:%2d:%02d\t|\t[%s]\t%s\n", hours, minutes, seconds, __translate_logger_message_type(type), line);

    return true;
}

bool __ob_log_wsline(const char* const line)
{
    if (logfile == NULL)
        return false;

    // Timestamp\t|\t\t<sub-message>\n
    uint32_t hours, minutes, seconds;
    OBTIMEgetTimestamp(&hours, &minutes, &seconds);
    fprintf(logfile, "%2d:%2d:%02d\t|\t\t%s\n", hours, minutes, seconds, line);

    return true;
}

bool ob_log_wlinef(enum ob_logger_message_type type, const char* const line, ...)
{
    // TODO: prototype
    printf("%d, %s\n", type, line);
    return false;
}

bool __ob_log_wslinef(const char* const line, ...)
{
    // TODO: prototype
    printf("%s\n", line);
    return false;
}