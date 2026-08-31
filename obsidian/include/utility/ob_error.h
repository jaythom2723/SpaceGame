#ifndef OB_ERROR_H
#define OB_ERROR_H

/*
File:       0x1XXX
-Open:      0x10XX
-Read:      0x11XX
Memory:     0x2XXX
Decode:     0x3XXX
*/

enum obsidian_error_code {
    ERROR_FILE_NOT_FOUND            = 0x1001,
    ERROR_FILE_PERMISSION_DENIED    = 0x1002,
    ERROR_FILE_PATH_INVALID         = 0x1003,
    ERROR_FILE_OS_ERROR             = 0x1004,
    ERROR_OUT_OF_MEMORY             = 0x2001,
    ERROR_FILE_IO                   = 0x1101,
    ERROR_FILE_EOF                  = 0x1102,
    ERROR_INVALID_UTF8              = 0x3001
};

enum obsidian_error_severity {
    SEVERITY_FATAL,
    SEVERITY_WARNING,
    SEVERITY_INFORM,
    SEVERITY_RECOVERABLE,
};

enum obsidian_error_category {
    ERROR_CAT_FILESYSTEM,
    ERROR_CAT_MEMORY,
    ERROR_CAT_DISPLAY,
    ERROR_CAT_GRAPHICS,
    ERROR_CAT_WINDOW,
};

struct obsidian_error {
    enum obsidian_error_code code;
    enum obsidian_error_severity severity;
    enum obsidian_error_category category;
    const char* message;
};

#endif