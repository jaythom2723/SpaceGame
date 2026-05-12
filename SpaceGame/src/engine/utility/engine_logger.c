#include "engine_logger.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void _populateMetadataHeader(EngineLoggerFileMetadata* data);
void _buildHeaderBuffer(size_t* size, char* buffer, const EngineLoggerFileMetadata data);

void engineInitializeLogger(EngineContext* ctx)
{
	EngineLoggerFileMetadata meta = { 0 };

	ctx->logfile = fopen("log.log", "w");
	char* header = NULL;
	size_t size = 0;

	_populateMetadataHeader(&meta);
	_buildHeaderBuffer(&size, header, meta);

	if (header != NULL)
	{
		fwrite(header, sizeof(char), size, ctx->logfile);
		free(header);
		header = NULL;
	}
}

void engineCloseLogger(EngineContext* ctx)
{
	if (ctx->logfile != NULL)
	{
		fclose(ctx->logfile);
		ctx->logfile = NULL;
	}
}

void _buildHeaderBuffer(size_t* size, char* buffer, const EngineLoggerFileMetadata data)
{
	char HEADER_SEPERATOR[257] = { 0 };
	memset(HEADER_SEPERATOR, '=', 255 * sizeof(char));
	HEADER_SEPERATOR[256] = '\n';

	char engineVersion[27] = { 0 };
	char glVersion[23] = { 0 };
	char cglmVersion[25] = { 0 };
	char date[13] = { 0 };

	(*size) += strlen(HEADER_SEPERATOR);
	(*size) += strlen(engineVersion);
	(*size) += strlen(glVersion);
	(*size) += strlen(cglmVersion);
	(*size) += strlen(date);
	(*size) += strlen(HEADER_SEPERATOR)+1;

	// gather all relevant pieces of information and store into buffers
	sprintf(engineVersion, "Engine Version: %s.%s.%s\n", data.engine_version_major, data.engine_version_minor, data.engine_version_patch);
	sprintf(glVersion, "GL Version: %s.%s.00\n", data.gl_version_major, data.gl_version_minor);
	sprintf(cglmVersion, "CGLM Version: %s.%s.00\n", data.cglm_version_major, data.cglm_version_minor);
	sprintf(date, "%s.%s.%s\n", data.date_day, data.date_month, data.date_year);

	buffer = calloc(*size, sizeof(char));
	assert(buffer != NULL);

	strcpy(buffer, HEADER_SEPERATOR);
	strcat(buffer, engineVersion);
	strcat(buffer, glVersion);
	strcat(buffer, cglmVersion);
	strcat(buffer, date);
	strcat(buffer, HEADER_SEPERATOR);
}

void _populateMetadataHeader(EngineLoggerFileMetadata* data)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	sprintf(data->engine_version_major, "%02d", ENGINE_VERSION_MAJOR);
	sprintf(data->engine_version_minor, "%02d", ENGINE_VERSION_MINOR);
	sprintf(data->engine_version_patch, "%02d", ENGINE_VERSION_PATCH);
	sprintf(data->gl_version_major, "%02d", GL_VERSION_MAJOR);
	sprintf(data->gl_version_minor, "%02d", GL_VERSION_MINOR);
	sprintf(data->cglm_version_major, "%02d", CGLM_VERSION_MAJOR);
	sprintf(data->cglm_version_minor, "%02d", CGLM_VERSION_MINOR);
	sprintf(data->date_year, "%04d", 1900 + tm.tm_year);
	sprintf(data->date_month, "%02d", tm.tm_mon);
	sprintf(data->date_day, "%02d", tm.tm_mday);
}

static const char* typeStrings[ELOG_MSGTYPE_INFORM+1] = {
	"[ERRR]\t|> ",
	"[WARN]\t|> ",
	"[INFO]\t|> "
};

EBOOL engineWriteMessage(EngineContext* ctx, const char* message, EngineLoggerMessageType type)
{
	if (ctx->logfile == NULL)
		return EFALSE;

	char* typeString = typeStrings[type];

	size_t typeStringLength = strlen(typeString);
	size_t messageLength = strlen(message)+1;
	
	char buffer[0x1000] = { 0 };
	strcpy(buffer, typeString);
	strcat(buffer, message);
	strcat(buffer, "\n");

	fwrite(buffer, sizeof(char), 0x1000, ctx->logfile);

	return ETRUE;
}
