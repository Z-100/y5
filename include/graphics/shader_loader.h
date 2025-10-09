#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include "core/types.h"
#include <cglm/call/mat4.h>

static const char* VERTEX_SHADER_TEXTURES	= "vertex_shader_textures.glsl";
static const char* FRAGMENT_SHADER_TEXTURES = "fragment_shader_textures.glsl";

static const char* VERTEX_SHADER_LIGHT_SOURCE	= "vertex_shader_light_source.glsl";
static const char* FRAGMENT_SHADER_LIGHT_SOURCE = "fragment_shader_light_source.glsl";

unsigned int compile_shaders_to_shader_program(struct Array* shadersArray);

// ====================
// Direct shader access
// ====================

void use_shader(const unsigned int* shaderPtr);

int set_uniform_vec3(const unsigned int* shaderPtr, const char* name, const vec3* value);
int set_uniform_vec4(const unsigned int* shaderPtr, const char* name, const vec4* value);

int set_uniform_mat4(const unsigned int* shaderPtr, const char* name, const mat4* matrix);

// ==============================================
// Pure declarations - impl by cool ass macros :)
// ==============================================

int set_uniform_bool(const unsigned int* shaderPtr, const char* name, int value);
int set_uniform_int(const unsigned int* shaderPtr, const char* name, int value);
int set_uniform_float(const unsigned int* shaderPtr, const char* name, float value);
int set_uniform_double(const unsigned int* shaderPtr, const char* name, double value);

#endif
