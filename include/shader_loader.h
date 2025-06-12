#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include "types.h"

static const char* Y5_VERTEX_SHADER			  = "vertex_shader.glsl";
static const char* Y5_FRAGMENT_SHADER_NOISE_1 = "fragment_shader_noise1.glsl";
static const char* Y5_FRAGMENT_SHADER_NOISE_2 = "fragment_shader_noise2.glsl";

unsigned int compile_shaders_to_shader_program(struct Array* shadersArray);

void use_shader(const unsigned int* shaderPtr);

void set_uniform_bool(const unsigned int* shaderPtr, const char* name, bool value);
void set_uniform_int(const unsigned int* shaderPtr, const char* name, int value);
void set_uniform_float(const unsigned int* shaderPtr, const char* name, float value);

#endif
