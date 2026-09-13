#include "obsidian.h"
#include "utility/ob_error.h"
#include "utility/ob_logger.h"
#include "display/ob_window.h"
#include "graphics/ob_shader.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <GLFW/glfw3.h>

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

extern bool __ob_tex_initmodule(void);
extern bool __ob_tex_closemodule(void);

extern bool __ob_ecs_initmodule(void);
extern bool __ob_ecs_closemodule(void);

extern bool __ob_ext_perlin_init(void);
extern bool __ob_ext_perlin_close(void);

void __ob_core_faultbreak(void);
bool __ob_core_checkfault(void);

static bool __obsidian_initialized = false;
static enum obsidian_extension __extensions = 0;

#define INIT_CORE_MODULE(func, err) \
    if (!func()) \
    { \
        (void) __ob_error_pusherror(ERR_MODULE_INIT, SEV_FATAL, CAT_CORE_SYS, err, __FILE__, __LINE__); \
        (void) __ob_error_readerror(); \
        return false; \
    }

#define INIT_EXT_MODULE(func, err) \
    if (!func()) \
    { \
        (void) __ob_error_pusherror(ERR_MODULE_INIT, SEV_WARNING, CAT_EXT_SYS, err, __FILE__, __LINE__); \
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

    INIT_CORE_MODULE(__ob_log_initmodule, "Failed to initialize Obisidian: Logger Core Module.");

    __ob_log_wline(LOG_MESSAGE_INFORM, "Successfully initialized Obsidian: Error and Obsidian: Logger...");
    __ob_log_wsline("Initializing Obsidian Core Systems...");

    INIT_CORE_MODULE(__ob_wnd_initmodule, "Failed to initialize Obisidian: Window Core Module.");

    __ob_log_wsline("Obsidian: Window [Core Module]\t|\tInitialization Successful.");

    INIT_CORE_MODULE(__ob_shdr_initmodule, "Failed to initialize Obsidian: Shader Core Module.");

    __ob_log_wsline("Obsidian: Shader [Core Module]\t|\tInitialization Successful.");

    INIT_CORE_MODULE(__ob_buf_initmodule, "Failed to initialize Obsidian: Buffers Core Module.");

    __ob_log_wsline("Obsidian: Buffers [Core Module]\t|\tInitialization Successful.");

    INIT_CORE_MODULE(__ob_tex_initmodule, "Failed to initialize Obsidian: Textures Core Module.");

    __ob_log_wsline("Obsidian: Textures [Core Module]\t|\tInitialization Successful.");

    INIT_CORE_MODULE(__ob_ecs_initmodule, "Failed to initialize Obsidian: Entity-Component System (ECS) Core Module.");

    __ob_log_wsline("Obsidian: ECS [Core Module]\t|\tInitializatiion Successful.");

    __obsidian_initialized = true;

    return true;
}

bool OBbootstrap(uint32_t* program, const char* title, const uint32_t width, const uint32_t height)
{
    if (!__obsidian_initialized)
    {
        printf("Cannot bootstrap the engine without first initializing it!\n");
        return false;
    }

    OBWNDsetTitle(title);
    OBWNDsetSize(width, height);
    OBWNDcreateWindow();

    obsidian_shader_t vertex, fragment;
    vertex = OBSHDRcreateShader("res/shaders/global_vertex.glsl", OBSHDR_VERTEX_SHADER);
    fragment = OBSHDRcreateShader("res/shaders/global_fragment.glsl", OBSHDR_FRAGMENT_SHADER);
    (*program) = OBSHDRcreateProgram();

    OBSHDRprogramAttach(*program, 2, vertex, fragment);
    if(!OBSHDRprogramLink(*program)) return false;

    OBSHDRdestroyShader(vertex);
    OBSHDRdestroyShader(fragment);

    OBSHDRuseProgram(*program);
    mat4 projection;
    glm_ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f, projection);

    OBSHDRseti(*program, "OBTex", 0);
    OBSHDRsetmat4f(*program, "projection", projection);

    return true;
}

bool OBinitExtension(enum obsidian_extension exts)
{
    if ((exts & OB_EXT_PERLIN_NOISE) == OB_EXT_PERLIN_NOISE)
    {
        INIT_EXT_MODULE(__ob_ext_perlin_init, "Failed to initialize Obsidian Perlin Noise: Extension.");
        __extensions |= OB_EXT_PERLIN_NOISE;
    }

    return true;
}

void OBclose(void)
{
    if ((__extensions & OB_EXT_PERLIN_NOISE) == OB_EXT_PERLIN_NOISE)
        (void)__ob_ext_perlin_close();

    (void)__ob_ecs_closemodule();
    (void)__ob_tex_closemodule();
    (void)__ob_buf_closemodule();
    (void)__ob_shdr_closemodule();
    (void)__ob_wnd_closemodule();
    glfwTerminate();
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