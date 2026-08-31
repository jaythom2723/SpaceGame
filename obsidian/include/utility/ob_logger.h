#ifndef OB_LOGGER_H
#define OB_LOGGER_H

enum ob_logger_message_type {
    OBLOG_MESSAGE_ERROR     = 0x1000,
    OBLOG_MESSAGE_WARNING   = 0x2000,
    OBLOG_MESSAGE_INFORM    = 0x4000,
};

#endif