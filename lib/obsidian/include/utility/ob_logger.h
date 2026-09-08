#ifndef OB_LOGGER_H
#define OB_LOGGER_H

#define LOGGER_MESSAGE_TYPES(X) \
    X(LOG_MESSAGE_FATAL, "FATL") \
    X(LOG_MESSAGE_WARNING, "WARN") \
    X(LOG_MESSAGE_INFORM, "INFO") \
    X(LOG_MESSAGE_RECOVER, "RCVR") \
    X(LOG_MESSAGE_DEBUG, "DBUG") \

enum ob_logger_message_type {
#define X(name, message) name,
    LOGGER_MESSAGE_TYPES(X)
#undef X
};

#endif