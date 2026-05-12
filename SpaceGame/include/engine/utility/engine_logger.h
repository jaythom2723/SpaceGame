#ifndef ENGINE_LOGGER_H
#define ENGINE_LOGGER_H

#include "engine_defs.h"

typedef enum engine_logger_message_type {
	ELOG_MSGTYPE_ERROR,
	ELOG_MSGTYPE_WARN,
	ELOG_MSGTYPE_INFORM
} EngineLoggerMessageType;

typedef struct engine_logger_file_metadata {
	char engine_version_major[3];
	char engine_version_minor[3];
	char engine_version_patch[3];
	char gl_version_major[3];
	char gl_version_minor[3];
	char cglm_version_major[3];
	char cglm_version_minor[3];
	char date_year[5];
	char date_month[3];
	char date_day[3];
} EngineLoggerFileMetadata;

// 40 byte metadata log header
#define ELOGGER_META_START_BYTE 0x10
#define ELOGGER_META_EVERSION_MAJOR ELOGGER_META_START_BYTE
#define ELOGGER_META_EVERSION_MINOR ELOGGER_META_EVERSION_MAJOR + 0x04
#define ELOGGER_META_EVERSION_PATCH ELOGGER_META_EVERSION_MINOR + 0x04
#define ELOGGER_META_GLVERSION_MAJOR ELOGGER_META_EVERSION_PATCH + 0x04
#define ELOGGER_META_GLVERSION_MINOR ELOGGER_META_GLVERSION_MAJOR + 0x04
#define ELOGGER_META_CGLMVERSION_MAJOR ELOGGER_META_GLVERSION_MINOR + 0x04
#define ELOGGER_META_CGLMVERSION_MINOR ELOGGER_META_CGLMVERSION_MAJOR + 0x04
#define ELOGGER_META_DATEYEAR ELOGGER_META_CGLMVERSION_MINOR + 0x04
#define ELOGGER_META_DATEMONTH ELOGGER_META_DATEYEAR + 0x04
#define ELOGGER_META_DATEDAY ELOGGER_META_DATEMONTH + 0x04

void engineInitializeLogger(EngineContext* ctx);
void engineCloseLogger(EngineContext* ctx);

EBOOL engineWriteMessage(EngineContext* ctx, const char* message, EngineLoggerMessageType type);

#endif // ENGINE_LOGGER_H
