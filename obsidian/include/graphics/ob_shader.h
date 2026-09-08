#ifndef OB_SHADER_H
#define OB_SHADER_H

#include <stdbool.h>
#include <stdint.h>

#include <cglm/cglm.h>

// TODO: refactor these into better names eventually please
typedef uint32_t obsidian_program_t; // used to represent indices
typedef uint32_t obsidian_shader_t;  // used to represent indices

#define SHADER_TYPES(X) \
    X(OBSHDR_VERTEX_SHADER, "Vertex Shader") \
    X(OBSHDR_FRAGMENT_SHADER, "Fragment Shader") \
    X(OBSHDR_COMPUTE_SHADER, "Compute Shader") \
    X(OBSHDR_GEOMETRY_SHADER, "Geometry Shader") \

enum obsidian_shader_type {
#define X(name, message) name,
    SHADER_TYPES(X)
#undef X
};

obsidian_shader_t OBSHDRcreateShader(const char* path, enum obsidian_shader_type type);
void OBSHDRdestroyShader(const obsidian_shader_t shader);

obsidian_program_t OBSHDRcreateProgram(void);
void OBSHDRdestroyProgram(const obsidian_program_t);
void OBSHDRprogramAttach(const obsidian_program_t, int n, ...);
bool OBSHDRprogramLink(const obsidian_program_t);
void OBSHDRuseProgram(const obsidian_program_t);

bool OBSHDRseti(const obsidian_program_t, const char* name, const int value);
bool OBSHDRsetf(const obsidian_program_t, const char* name, const float value);
bool OBSHDRset2f(const obsidian_program_t, const char* name, const float x, const float y);
bool OBSHDRset2fv(const obsidian_program_t, const char* name, const vec2 value);
bool OBSHDRset3f(const obsidian_program_t, const char* name, const float x, const float y, const float z);
bool OBSHDRset3fv(const obsidian_program_t, const char* name, const vec3 value);
bool OBSHDRset4f(const obsidian_program_t, const char* name, const float x, const float y, const float z, const float w);
bool OBSHDRset4fv(const obsidian_program_t, const char* name, const vec4 value);
bool OBSHDRsetmat4f(const obsidian_program_t, const char* name, const mat4 value);

#endif