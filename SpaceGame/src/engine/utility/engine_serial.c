#include "engine_serial.h"

#include "engine_logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static FILE* fp = NULL;
static EngineSerialFileType fileType = -1;
static uint8_t* decodedContents = NULL;
static uint16_t* encodedContents = NULL;
static size_t decodedFileSize = 0;
static size_t encodedFileSize = 0;

void engineSerialOpenFile(EngineSerialFileType type, const char* name) {
	if (fp != NULL)
		return;

	fp = fopen(name, "r");
	if (fp == NULL)
		return;

	fileType = type;
}

extern EngineContext* _engine_get_int_ctx(void);

void engineSerialReadContents(void) {
	// TODO: get the encoded
	// TODO: decode the encoded
	// TODO: save the decoded
	// TODO: free the encoded
}

void engineSerialWriteLine(const uint8_t* line) {
	if (fp == NULL)
		return;

	uint16_t* buffer = calloc((strlen(line) / 2) + 1, sizeof(uint16_t));
	if (buffer == NULL) {
		engineWriteMessage(_engine_get_int_ctx(), "Failed to write line to serial file", ELOG_MSGTYPE_WARN);
		fclose(fp);
		return;
	}

	// encode the line
	uint8_t* lineptr = line;
	while (*lineptr != '\0') {
		if ((*lineptr >= 'A' && *lineptr <= 'Z') 
			|| (*lineptr >= 'a' && *lineptr <= 'z')) {
			// TODO: Figure out how tf to do this later
		}
	}
}

void engineSerialCloseFile(void) {
	if (fp == NULL)
		return;

	fclose(fp);
}