#ifndef SHADERS_COLLECTION_H
#define SHADERS_COLLECTION_H

#include "utils/collection_sys.h"

// Must be kept aligned to actual number of shader programs
#define NUM_SHADER_PROGRAMS 2

typedef struct shader_t {
	GLuint		id;
	const char* name;
	int			type;
} shader_t;

typedef struct shader_texture_t {
	GLuint		id;
	const char* texture_name;
	const char* uniform_name;
} shader_texture_t;

typedef struct shader_program_t {
	GLuint id;

	shader_t* shaders;
	size_t	  shaders_count;

	shader_texture_t* textures;
	size_t			  textures_count;

} shader_program_t;

// ===============
// Shader creators
// ===============

shader_program_t* shaders_collection_default();
shader_program_t* shaders_collection_light();

#endif
