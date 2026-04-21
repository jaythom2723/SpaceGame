#ifndef SHADER_H
#define SHADER_H

#include <cglm/cglm.h>

#include "engine_defs.h"

EngineShader engineCreateShader(int shaderType);
const EBOOL engineCompileShader(EngineShader* shader, const char* path);
void engineDeleteShader(EngineShader* shader);

void engineCreateProgram(EngineContext* ctx);
const EBOOL engineAttachAndLink(EngineContext* ctx, EngineShader* vertex, EngineShader* fragment, EngineShader* geometry);
void engineDeleteProgram(EngineContext* ctx);

void engineUseShader(EngineContext* ctx);
void engineSetFloat(EngineContext* ctx, const char* name, float value);
void engineSetInteger(EngineContext* ctx, const char* name, int value);
void engineSetVector2f(EngineContext* ctx, const char* name, float x, float y);
void engineSetVector2fv(EngineContext* ctx, const char* name, vec2 value);
void engineSetVector3f(EngineContext* ctx, const char* name, float x, float y, float z);
void engineSetVector3fv(EngineContext* ctx, const char* name, vec3 value);
void engineSetVector4f(EngineContext* ctx, const char* name, float x, float y, float z, float w);
void engineSetVector4fv(EngineContext* ctx, const char* name, vec4 value);
void engineSetMatrix4fv(EngineContext* ctx, const char* name, mat4 value);

#endif // SHADER_H