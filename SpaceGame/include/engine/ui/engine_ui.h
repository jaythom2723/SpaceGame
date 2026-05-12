#ifndef ENGINE_UI_H
#define ENGINE_UI_H

#include "engine_defs.h"

#include <cglm/cglm.h>

EngineUIElement* engineCreateUIElement(EngineUIElementType type, void* data, size_t dsize, vec2 pos, vec2 size);

void engineSetUIPadding(EngineUIElement* elem, float padding);
void engineSetUIMargin(EngineUIElement* elem, float margin);
void engineSetUIBorder(EngineUIElement* elem, float border);

void engineSetUIBackgroundColor(EngineUIElement* elem, float r, float g, float b);
void engineSetUIForegroundColor(EngineUIElement* elem, float r, float g, float b);
void engineSetUIBorderColor(EngineUIElement* elem, float r, float g, float b);

void engineDrawUIElement(EngineUIElement* elem);

void engineDestroyUIElement(EngineUIElement* element);

#endif // ENGINE_UI_H
