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

void engineSerialOpenFile(EngineSerialFileType type, const char* mode, const char* name) {
	if (fp != NULL)
		return;

	fp = fopen(name, mode);
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

/*
	(STAR) {
		CLASS, ENTITY KEY, AND MASS ARE STORED IN THE FILE

		CLASS (C)
		ENTITY KEY (E)
		MASS (M)

		C3E2M120
	}
*/

void _encodeline(const uint8_t* line) {
	uint8_t* ptr = line;
	uint8_t* numbuffer = NULL;
	uint32_t numlen = 0;
	uint8_t curchar = '';
	uint32_t number = 0;

	while (*ptr != '\0') {
		if ((*ptr >= 'A' && *ptr <= 'Z')
			|| (*ptr >= 'a' && *ptr <= 'z')) {
			// TODO: get the number!
			numbuffer = calloc(numlen + 1, sizeof(uint8_t));
			if (numbuffer == NULL)
				return;
			memcpy(numbuffer, line + (ptr - line), numlen + 1);
			printf("%s\n", numbuffer);

			numlen = 0;
			curchar = *ptr;
			continue;
		}

		numlen++;
		ptr++;
	}
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
	_encodeline(line);
}

void engineSerialCloseFile(void) {
	if (fp == NULL)
		return;

	fclose(fp);
}