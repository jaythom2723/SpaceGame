#ifndef OB_ERROR_H
#define OB_ERROR_H

#include <stdint.h>

/*
File:       0x1XXX
-Open:      0x10XX
-Read:      0x11XX
Memory:     0x2XXX
Decode:     0x3XXX
Module:     0x4XXX
*/

#define ERROR_CODES(X) \
    X(ERR_NONE, "No error") \
    X(ERR_FILE_NOT_FOUND, "File not found") \
    X(ERR_FILE_PERMISSION_DENIED, "File access permissions denied") \
    X(ERR_FILE_PATH_INVALID, "File path invalid") \
    X(ERR_FILE_OS_ERROR, "File OS error") \
    X(ERR_OUT_OF_MEMORY, "Out of memory") \
    X(ERR_FILE_IO, "File IO operation error") \
    X(ERR_FILE_EOF, "Unexpected EOF character reached") \
    X(ERR_INVALID_UTF8, "Invalid encoding") \
    X(ERR_MODULE_INIT, "Failed to initialize Obsidian Module") \
    X(ERR_GLFW_INIT, "Failed to initialize GLFW") \
    X(ERR_GLAD_INIT, "Failed to initialize GLAD") \
    X(ERR_GLFW_ERROR, "GLFW error") \
    X(ERR_GL_ERROR, "GL error") \

#define SEVERITY_CODES(X) \
    X(SEV_FATAL, "FATAL") \
    X(SEV_WARNING, "WARNING") \
    X(SEV_INFORM, "INFORM") \
    X(SEV_RECOVER, "RECOVER") \
    X(SEV_DEBUG, "DEBUG") \

#define CATEGORY_CODES(X) \
    X(CAT_CORE_SYS, "Obsidian Core Systems") \
    X(CAT_EXT_SYS, "Obsidian Extensible Systems") \
    X(CAT_FILESYSTEM, "Filesystem") \
    X(CAT_MEMORY, "Memory") \
    X(CAT_DISPLAY, "Display") \
    X(CAT_GRAPHICS, "Graphics") \
    X(CAT_WINDOW, "Window") \

enum obsidian_error_code {
#define X(name, message) name,
    ERROR_CODES(X)
#undef X
};

enum obsidian_error_severity {
#define X(name, message) name,
    SEVERITY_CODES(X)
#undef X
};

enum obsidian_error_category {
#define X(name, message) name,
    CATEGORY_CODES(X)
#undef X
};

struct obsidian_error {
    enum obsidian_error_code code;
    enum obsidian_error_severity severity;
    enum obsidian_error_category category;
    const char* message;
    const char* file;
    uint32_t line;
};

#endif