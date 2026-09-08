#include "utility/ob_logger.h"
#include "utility/ob_time.h"
#include "utility/ob_error.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

bool __ob_log_initmodule(void);
bool __ob_log_closelogfile(void); // effectively: closemodule()
bool __ob_log_getlogfilename(char**, size_t*);
bool __ob_log_openlogfile(const char*);
void __ob_log_wheader(void);
void __ob_log_wline(enum ob_logger_message_type, const char* const);
void __ob_log_wsline(const char* const);
void __ob_log_wlinef(enum ob_logger_message_type, const char* const, ...);
void __ob_log_wslinef(const char* const, ...);
void __ob_log_werror(const struct obsidian_error* const error);

extern bool __ob_util_openfile(FILE**, const char*, const char*);
extern char* __ob_util_readfile(const char*);
extern char* __ob_util_dtostr(const uint32_t);

extern uint32_t __ob_math_ndgts(uint32_t n);

extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

static FILE* logfile = NULL;

bool __ob_log_initmodule(void)
{
    char* logfile_name = NULL;
    size_t logfile_namelen = 0;
    if (__ob_log_getlogfilename(&logfile_name, &logfile_namelen))
        if (!__ob_log_openlogfile(logfile_name))
        {
            (void)__ob_error_pusherror(ERR_FILE_PERMISSION_DENIED, SEV_FATAL, CAT_FILESYSTEM, "Failed to create a log file!", __FILE__, __LINE__);
            (void)__ob_error_readerror();
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
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_FATAL, CAT_MEMORY, "Failed to allocate enough memory for logfile's name!", __FILE__, __LINE__);
        (void)__ob_error_readerror();
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

void __ob_log_wheader(void)
{
    // TODO: prototype
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

void __ob_log_wline(enum ob_logger_message_type type, const char* const line)
{
    if (logfile == NULL)
        return;
    
    // Timestamp\t|\t[<type>]\t<message>\n
    uint32_t hours, minutes, seconds;
    OBTIMEgetTimestamp(&hours, &minutes, &seconds);
    fprintf(logfile, "%02d:%02d:%02d\t|\t[%s]\t%s\n", hours, minutes, seconds, __translate_logger_message_type(type), line);
}

void __ob_log_wsline(const char* const line)
{
    if (logfile == NULL)
        return;

    // Timestamp\t|\t\t<sub-message>\n
    uint32_t hours, minutes, seconds;
    OBTIMEgetTimestamp(&hours, &minutes, &seconds);
    fprintf(logfile, "%02d:%02d:%02d\t|\t\t%s\n", hours, minutes, seconds, line);
}

void ob_log_wlinef(enum ob_logger_message_type type, const char* const line, ...)
{
    // TODO: prototype
    printf("%d, %s\n", type, line);
}

void __ob_log_wslinef(const char* const line, ...)
{
    // TODO: prototype
    printf("%s\n", line);
}

void __ob_log_werror(const struct obsidian_error* const error)
{
    if (logfile == NULL)
        return;

    uint32_t hours, minutes, seconds;
    OBTIMEgetTimestamp(&hours, &minutes, &seconds);
    fprintf(logfile, "%02d:%02d:%02d\t|\t[%s (%04x)]\t%s\n", hours, minutes, seconds, __translate_logger_message_type((enum ob_logger_message_type)error->severity), error->code, error->message);
    fprintf(logfile, "%02d:%02d:%02d\t|\t\tCategory: %d\n", hours, minutes, seconds, error->category);
    fprintf(logfile, "%02d:%02d:%02d\t|\t\tAt: %s:%d\n", hours, minutes, seconds, error->file, error->line);
}