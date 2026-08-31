#ifndef ENGINE_SERIAL_H
#define ENGINE_SERIAL_H

#include "engine_defs.h"

void engineSerialOpenFile(EngineSerialFileType type, const char* mode, const char* name);
void engineSerialReadContents(void);
void engineSerialWriteLine(const char* line);
void engineSerialCloseFile(void);

#endif // ENGINE_SERIAL_H