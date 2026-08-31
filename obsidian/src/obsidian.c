#include "obsidian.h"

#include <stdlib.h>

extern bool __ob_log_getlogfilename(char** buffer, size_t* bufferSize);
extern bool __ob_log_openlogfile(const char* name);

struct obsidian_context* OBcreateContext(void)
{
    struct obsidian_context* ctx = (struct obsidian_context*) calloc(1, sizeof(struct obsidian_context));
    if (ctx == NULL) {
        // TODO: woohoo, logger and error handler being developed first will help a lot with debugging!
        return NULL;
    }

    char* logfile_name = NULL;
    size_t logfile_namelen = 0;
    if (__ob_log_getlogfilename(&logfile_name, &logfile_namelen) == true)
    {
        if (__ob_log_openlogfile(logfile_name) == false)
        {
            // TODO: error handler message here
            printf("[OBSIDIAN:FATAL] Failed to open a log file! Required by Obsidian for proper functionality and debugging!\n");
            free(logfile_name);
            free(ctx);
            logfile_name = NULL;
            ctx = NULL;
            return NULL;
        }

        free(logfile_name);
        logfile_name = NULL;
    }

    return ctx;
}

void OBdestroyContext(struct obsidian_context* ctx)
{
    free((void*) ctx);
    ctx = NULL;
}