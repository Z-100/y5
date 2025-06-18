#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include "types.h"

static const char* VERTEX_SHADER		   = "vertex_shader.glsl";
static const char* FRAGMENT_SHADER_NOISE_1 = "fragment_shader_noise1.glsl";
static const char* FRAGMENT_SHADER_NOISE_2 = "fragment_shader_noise2.glsl";

unsigned int compile_shaders_to_shader_program(struct Array* shadersArray);

void use_shader(const unsigned int* shaderPtr);

int set_uniform_bool(const unsigned int* shaderPtr, const char* name, int value);
int set_uniform_int(const unsigned int* shaderPtr, const char* name, int value);
int set_uniform_float(const unsigned int* shaderPtr, const char* name, float value);
int set_uniform_double(const unsigned int* shaderPtr, const char* name, double value);

int set_uniform_vec4(const unsigned int* shaderPtr, const char* name, float value[]);
int set_uniform_vec3(const unsigned int* shaderPtr, const char* name, float value[]);

#endif
