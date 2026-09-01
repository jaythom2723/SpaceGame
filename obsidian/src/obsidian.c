#include "obsidian.h"
#include "utility/ob_error.h"
#include "utility/ob_logger.h"

#include <stdint.h>

const uint32_t OBSIDIAN_VERSION_MAJOR = 0;
const uint32_t OBSIDIAN_VERSION_MINOR = 0;
const uint32_t OBSIDIAN_VERSION_PATCH = 0;

static bool __obsidian_fault_break = false; // use this to emergency exit out of the engine and shut everything down!

extern bool __ob_log_initmodule(void);
extern bool __ob_log_getlogfilename(char**, size_t*);
extern bool __ob_log_openlogfile(const char*);
extern bool __ob_log_closelogfile(void);
extern bool __ob_log_wline(enum ob_logger_message_type, const char* const);
extern bool __ob_log_wsline(const char* const);

extern bool __ob_error_initmodule(void);
extern bool __ob_error_closemodule(void);
extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

void __ob_core_faultbreak(void);

bool OBinit(void)
{
    if (!__ob_error_initmodule())
    {
        printf("Failed to initialize Obisdian's Error Handler. Closing...\n");
        return false;
    }

    if (!__ob_log_initmodule())
    {
        (void)__ob_error_pusherror(ERR_MODULE_INIT, SEV_FATAL, CAT_CORE_SYS, "Failed to initialize Obsidian Logger Module", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }

    __ob_log_wline(LOG_MESSAGE_INFORM, "Successfully initialized Obsidian: Error and Obsidian: Logger...");
    __ob_log_wsline("Initializing Obsidian Core Systems...");

    return true;
}

void OBclose(void)
{
    (void)__ob_log_closelogfile();
    (void)__ob_error_closemodule();
}

void __ob_core_faultbreak(void)
{
    if (__obsidian_fault_break)
        return;
    __obsidian_fault_break = true;
}