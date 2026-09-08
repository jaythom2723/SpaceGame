#include "obsidian.h"
#include "utility/ob_error.h"
#include "utility/ob_logger.h"

#include <stdio.h>
#include <stdint.h>

const uint32_t OBSIDIAN_VERSION_MAJOR = 0;
const uint32_t OBSIDIAN_VERSION_MINOR = 0;
const uint32_t OBSIDIAN_VERSION_PATCH = 0;

static bool __obsidian_fault_break = false; // use this to emergency exit out of the engine and shut everything down!

extern bool __ob_log_initmodule(void);
extern bool __ob_log_closelogfile(void);
extern bool __ob_log_wline(enum ob_logger_message_type, const char* const);
extern bool __ob_log_wsline(const char* const);

extern bool __ob_error_initmodule(void);
extern bool __ob_error_closemodule(void);
extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

extern bool __ob_wnd_initmodule(void);
extern bool __ob_wnd_closemodule(void);

extern bool __ob_shdr_initmodule(void);
extern void __ob_shdr_closemodule(void);

extern bool __ob_buf_initmodule(void);
extern bool __ob_buf_closemodule(void);

void __ob_core_faultbreak(void);
bool __ob_core_checkfault(void);

#define INIT_CORE_MODULE(func, err) \
    if (!func()) \
    { \
        (void) __ob_error_pusherror(ERR_MODULE_INIT, SEV_FATAL, CAT_CORE_SYS, err, __FILE__, __LINE__); \
        (void) __ob_error_readerror(); \
        return false; \
    }

bool OBinit(void)
{
    if (!__ob_error_initmodule())
    {
        printf("Failed to initialize Obisdian's Error Handler. Closing...\n");
        return false;
    }

    INIT_CORE_MODULE(__ob_log_initmodule, "Failed to initialize Obisidian: Logger Core Module");

    __ob_log_wline(LOG_MESSAGE_INFORM, "Successfully initialized Obsidian: Error and Obsidian: Logger...");
    __ob_log_wsline("Initializing Obsidian Core Systems...");

    INIT_CORE_MODULE(__ob_wnd_initmodule, "Failed to initialize Obisidian: Window Core Module");

    __ob_log_wsline("Obsidian: Window [Core Module]\t|\tInitialization Successful.");

    INIT_CORE_MODULE(__ob_shdr_initmodule, "Failed to initialize Obsidian: Shader Core Module");

    __ob_log_wsline("Obsidian: Shader [Core Module]\t|\tInitialization Successful.");

    INIT_CORE_MODULE(__ob_buf_initmodule, "Failed to initialize Obsidian: Buffers Core Module");

    __ob_log_wsline("Obsidian: Buffers [Core Module]\t|\tInitialization Successful.");

    return true;
}

void OBclose(void)
{
    (void)__ob_buf_closemodule();
    (void)__ob_shdr_closemodule();
    (void)__ob_wnd_closemodule();
    (void)__ob_log_closelogfile();
    (void)__ob_error_closemodule();
}

void __ob_core_faultbreak(void)
{
    if (__obsidian_fault_break)
        return;
    __obsidian_fault_break = true;
}

bool __ob_core_checkfault(void)
{
    return __obsidian_fault_break;
}