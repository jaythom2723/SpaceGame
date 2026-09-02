#include "display/ob_window.h"

#include "utility/ob_logger.h"
#include "utility/ob_error.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define __OB_MAX_INIT_ATTEMPTS 4

static uint32_t __init_attempts = 0;

static struct obsidian_window* window = NULL;

bool __ob_wnd_initmodule(void);
bool __ob_wnd_closemodule(void);

extern void __ob_log_wline(enum ob_logger_message_type, const char* const);
extern void __ob_log_wsline(const char* const);

extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

bool __ob_wnd_initmodule(void)
{   
    if (__init_attempts >= __OB_MAX_INIT_ATTEMPTS)
    {
        (void)__ob_error_pusherror(ERR_MODULE_INIT, SEV_FATAL, CAT_CORE_SYS, "Failed to initialize Obisidian: Window Core Module", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }

    __init_attempts ++;
    if (window != NULL)
    {
        (void)__ob_error_pusherror(ERR_MODULE_INIT, SEV_WARNING, CAT_CORE_SYS, "Obsidian: Window Core Module, has already been initialized...", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }

    window = calloc(1, sizeof(struct obsidian_window));
    if (window == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_RECOVER, CAT_MEMORY, "Failed to allocate enough memory for the window... Attempting again...", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return __ob_wnd_initmodule();
    }

    window->handle = NULL;
    window->width = 0;
    window->height = 0;
    window->title = NULL;

    return true;
}

bool __ob_wnd_closemodule(void)
{
    if (window == NULL)
        return false;

    if (window->handle != NULL)
    {
        OBWNDdestroyWindow();
    }

    if (window->title != NULL)
    {
        free(window->title);
        window->title = NULL;
    }

    free(window);
    window = NULL;
    return true;
}

bool OBWNDcreateWindow(void)
{
    if (window == NULL || window->title == NULL || window->width == 0 || window->height == 0)
    {
        (void)__ob_error_pusherror(ERR_MODULE_INIT, SEV_WARNING, CAT_WINDOW, "Must initialize the module and set the width, height, and title.", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }

    if (!glfwInit())
    {
        (void)__ob_error_pusherror(ERR_GLFW_INIT, SEV_FATAL, CAT_WINDOW, "Failed to initialize GLFW!", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }

    __ob_log_wsline("Obsidian: Window [Core Module]\t|\tGLFW Initialization Successful.");

    // TODO: let the user decide this information.
            // for now, do this instead for prototyping
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    window->handle = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);
    if (window->handle == NULL)
    {
        char buffer[1024];
        glfwGetError((const char**) &buffer);
        (void)__ob_error_pusherror(ERR_GLFW_ERROR, SEV_FATAL, CAT_WINDOW, buffer, __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }

    __ob_log_wsline("Obsidian: Window [Core Module]\t|\tGLFW Window Created Successfully.");

    glfwMakeContextCurrent(window->handle);
    
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != GLFW_TRUE)
    {
        (void)__ob_error_pusherror(ERR_GLAD_INIT, SEV_FATAL, CAT_GRAPHICS, "Failed to initialize OpenGL through GLAD.", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        glfwDestroyWindow(window->handle);
        return false;
    }

    __ob_log_wsline("Obsidian: Graphics [Core Module]\t|\tGLAD Initialized Successfully.");
    if (glGetString(GL_VERSION) != NULL)
        __ob_log_wsline("Obsidian: Graphics [Core Module]\t|\tOpenGL Context Is Current And Valid.");

    glfwShowWindow(window->handle);

    return true;
}

bool OBWNDdestroyWindow(void)
{
    if (window == NULL || window->handle == NULL)
        return false;

    glfwDestroyWindow(window->handle);
    window->handle = NULL;
    return true;
}

void OBWNDpollEvents(void)
{
    if (window == NULL || window->handle == NULL)
        return;

    glfwPollEvents();
}

void OBWNDswapBuffers(void)
{
    if (window == NULL || window->handle == NULL)
        return;

    glfwSwapBuffers(window->handle);
}

bool OBWNDsetTitle(const char* buffer)
{
    if (window == NULL || window->title != NULL)
        return false;

    window->title = calloc(strlen(buffer) + 1, sizeof(char));
    if (window->title == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_FATAL, CAT_MEMORY, "Failed to allocate enough memory for the title of the window.", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }

    strcpy(window->title,  buffer);

    return true;
}

bool OBWNDsetSize(const uint32_t width, const uint32_t height)
{
    if (window == NULL)
        return false;

    window->width = width;
    window->height = height;
    
    return true;
}

bool OBWNDshouldClose(void)
{
    if (window == NULL || window->handle == NULL)
        return true;

    return glfwWindowShouldClose(window->handle);
}

const char* OBWNDgetTitle(void)
{
    if (window == NULL || window->title == NULL)
        return NULL;

    char* buffer = calloc(strlen(window->title)+1, sizeof(char));
    if (buffer == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_WARNING, CAT_MEMORY, "Could not return the title of the window!", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return NULL;
    }

    strcpy(buffer, window->title);

    return buffer;
}

uint32_t OBWNDgetWidth(void)
{
    if (window == NULL)
        return 0;

    return window->width;
}

uint32_t OBWNDgetHeight(void)
{
    if (window == NULL)
        return 0;

    return window->height;
}

void OBWNDgetSize(uint32_t* restrict width, uint32_t* restrict height)
{
    if (window == NULL)
    {
        (*width) = 0;
        (*height) = 0;
        return;
    }

    (*width) = window->width;
    (*height) = window->height;
}