#include "obsidian.h"

extern bool __ob_log_getlogfilename(char**, size_t*);
extern bool __ob_log_openlogfile(const char*);
extern bool __ob_log_initmodule(void);
extern bool __ob_log_closelogfile(void);

extern bool __ob_error_initmodule(void);
extern bool __ob_error_closemodule(void);

bool OBinit(void)
{
    if (!__ob_error_initmodule())
    {
        printf("Failed to initialize Obisdian's Error Handler. Closing...\n");
        return false;
    }

    if (!__ob_log_initmodule())
    {
        // TODO: error handler message
        printf("[OBSIDIAN:FATAL] Logger module failed to initialize!\n");
        return false;
    }

    return true;
}

void OBclose(void)
{
    (void)__ob_log_closelogfile();
    (void)__ob_error_closemodule();
}