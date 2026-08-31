#include "engine_logger.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void _populateMetadataHeader(EngineLoggerFileMetadata* data);
void _buildHeaderBuffer(size_t* size, char** buffer, const EngineLoggerFileMetadata data);

void engineInitializeLogger(EngineContext* ctx)
{
	EngineLoggerFileMetadata meta = { 0 };

	ctx->logfile = fopen("log.log", "w");
	if (ctx->logfile == NULL)
		return;

	size_t size = 0;
	char* header = NULL;

	_populateMetadataHeader(&meta);
	_buildHeaderBuffer(&size, &header, meta);

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



void _buildHeaderBuffer(size_t* size, char** buffer, const EngineLoggerFileMetadata data)
{
	char* HEADER_SEPERATOR = calloc(ELOG_HDRSEP_WIDTH, sizeof(char));
	char* engineVersion = calloc(strlen(ELOG_ENGVER_STEMP) + 1, sizeof(char));
	char* glVersion = calloc(strlen(ELOG_OGLVER_STEMP) + 1, sizeof(char));
	char* cglmVersion = calloc(strlen(ELOG_CGLMVR_STEMP) + 1, sizeof(char));
	char* date = calloc(strlen(ELOG_DATE_STEMP) + 1, sizeof(char));

	assert(HEADER_SEPERATOR != NULL);
	assert(engineVersion != NULL);
	assert(glVersion != NULL);
	assert(cglmVersion != NULL);
	assert(date != NULL);

	(*size) += (ELOG_HDRSEP_WIDTH * 2);
	(*size) += strlen(ELOG_ENGVER_STEMP);
	(*size) += strlen(ELOG_OGLVER_STEMP);
	(*size) += strlen(ELOG_CGLMVR_STEMP);
	(*size) += strlen(ELOG_DATE_STEMP);

	memset(HEADER_SEPERATOR, '=', 255 * sizeof(char));
	memset(HEADER_SEPERATOR + 256, '\n', sizeof(char));

	sprintf(engineVersion, ELOG_ENGVER_STEMP, data.engine_version_major, data.engine_version_minor, data.engine_version_patch);
	sprintf(glVersion, ELOG_OGLVER_STEMP, data.gl_version_major, data.gl_version_minor);
	sprintf(cglmVersion, ELOG_CGLMVR_STEMP, data.cglm_version_major, data.cglm_version_minor);
	sprintf(date, "%s.%s.%s\n", data.date_day, data.date_month, data.date_year);

	(*buffer) = calloc(*size, sizeof(char));
	assert((*buffer) != NULL);

	strcpy(*buffer, HEADER_SEPERATOR);
	strcat(*buffer, engineVersion);
	strcat(*buffer, glVersion);
	strcat(*buffer, cglmVersion);
	strcat(*buffer, date);
	strcat(*buffer, HEADER_SEPERATOR);

	free(HEADER_SEPERATOR);
	free(engineVersion);
	free(glVersion);
	free(cglmVersion);
	free(date);

	HEADER_SEPERATOR = NULL;
	engineVersion = NULL;
	glVersion = NULL;
	cglmVersion = NULL;
	date = NULL;
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
	"\n[ERRR]\t|>\t",
	"\n[WARN]\t|>\t",
	"\n[INFO]\t|>\t"
};

EBOOL engineWriteMessage(EngineContext* ctx, const char* message, EngineLoggerMessageType type)
{
	if (ctx->logfile == NULL)
		return EFALSE;

	const char* typeString = typeStrings[type];

	size_t typeStringLength = strlen(typeString);
	size_t messageLength = strlen(message);
	size_t bufferSize = typeStringLength + messageLength;
	char* buffer = calloc(bufferSize, sizeof(char));
	assert(buffer != NULL);

	strcpy(buffer, typeString);
	strcat(buffer, message);

	if (ctx->logfile != NULL)
		fwrite(buffer, sizeof(char), bufferSize, ctx->logfile);

	return ETRUE;
}
