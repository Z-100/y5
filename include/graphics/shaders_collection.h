#ifndef SHADERS_COLLECTION_H
#define SHADERS_COLLECTION_H

#include "utils/collection_sys.h"

// Must be kept aligned to actual number of shader programs
#define NUM_SHADER_PROGRAMS 2

typedef struct shader_t {
	unsigned int id;
	const char*	 name;
	int			 type;
} shader_t;

typedef struct shader_texture_t {
	char* texture_name;
	char* uniform_name;
} shader_texture_t;

typedef struct shader_program_t {
	uint32_t id;

	shader_t* shaders;
	size_t	  shaders_count;

	shader_texture_t* textures;
	size_t			  textures_count;

} shader_program_t;

// ==============
// Default shader
// ==============

extern const shader_t default_vertex;
extern const shader_t default_fragment;
extern const shader_t default_shaders[2];

extern const shader_texture_t default_texture_elmo;
extern const shader_texture_t default_texture_obama;
extern const shader_texture_t default_textures[2];

extern const shader_program_t shader_program_default;

// ==============
// Light shader
// ==============

extern const shader_t light_vertex;
extern const shader_t light_fragment;
extern const shader_t light_shaders[];

extern const shader_program_t shader_program_light;

#endif
