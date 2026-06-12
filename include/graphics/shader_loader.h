#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include "utils/collection_sys.h"

bool shader_loader_compile(shader_program_t* shader_program);

// ====================
// Direct shader access
// ====================

void use_shader(const unsigned int* shaderPtr);

int set_uniform_vec3(const unsigned int* shaderPtr, const char* name, const vec3* value);
int set_uniform_vec4(const unsigned int* shaderPtr, const char* name, const vec4* value);

int set_uniform_mat4(const unsigned int* shaderPtr, const char* name, const mat4* matrix);

int set_uniform_material(
	const unsigned int* shaderPtr,
	const char*			name,
	const material_t*	material
);

// ==============================================
// Pure declarations - impl by cool ass macros :)
// ==============================================

int set_uniform_bool(const unsigned int* shaderPtr, const char* name, int value);
int set_uniform_int(const unsigned int* shaderPtr, const char* name, int value);
int set_uniform_float(const unsigned int* shaderPtr, const char* name, float value);
int set_uniform_double(const unsigned int* shaderPtr, const char* name, double value);

#endif
