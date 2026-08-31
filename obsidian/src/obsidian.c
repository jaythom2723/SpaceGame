#include "obsidian.h"

#include <stdbool.h>

extern bool __ob_log_getlogfilename(char** buffer, size_t* bufferSize);
extern bool __ob_log_openlogfile(const char* const name);

struct obsidian_context* OBcreateContext(void)
{
    struct obsidian_context* ctx = (struct obsidian_context*) calloc(1, sizeof(struct obsidian_context));
    if (ctx == NULL) {
        // TODO: woohoo, logger and error handler being developed first will help a lot with debugging!
        return NULL;
    }

    char* logfile_name = NULL;
    size_t logfile_name_size = 0;
    if (__ob_log_getlogfilename(&logfile_name, &logfile_name_size) == true)
    {
        if (__ob_log_openlogfile(logfile_name) == false)
        {
            // TODO: error handler message here please
            printf("[OBSIDIAN]:[FATAL ERROR]: FAILED TO OPEN A LOG FILE, CANNOT INITIALIZE OBSIDIAN WITHOUT ONE.\n");
            logfile_name = NULL; // no wild pointers here
            return NULL;
        }
    }

    logfile_name = NULL; // no wild pointers here
    return ctx;
}

void OBdestroyContext(struct obsidian_context* ctx)
{
    free((void*) ctx);
    ctx = NULL;
}