#include "graphics/shaders_collection.h"
#include "utils/headers_collection.h"

// ==============
// Default shader
// ==============

const shader_t default_vertex = { .name = "vertex_shader_textures.glsl", .type = GL_VERTEX_SHADER };

const shader_t default_fragment = { .name = "fragment_shader_textures.glsl",
									.type = GL_FRAGMENT_SHADER };

const shader_t default_shaders[2] = { default_vertex, default_fragment };

const shader_texture_t default_texture_elmo = {
	.texture_name = "res/textures/elmo.png",
	.uniform_name = "u_elmoTexture",
};

const shader_texture_t default_texture_obama = {
	.texture_name = "res/textures/obama.png",
	.uniform_name = "u_obamaTexture",
};
const shader_texture_t default_textures[] = { default_texture_elmo, default_texture_obama };

const shader_program_t shader_program_default = {

	.shaders	   = (shader_t*) default_shaders,
	.shaders_count = 2,

	.textures		= (shader_texture_t*) default_textures,
	.textures_count = 2,
};

// ==============
// Light shader
// ==============

const shader_t light_vertex = { .name = "vertex_shader_light_source.glsl",
								.type = GL_VERTEX_SHADER };

const shader_t light_fragment = { .name = "fragment_shader_light_source.glsl",
								  .type = GL_FRAGMENT_SHADER };

const shader_t light_shaders[2] = { default_vertex, default_fragment };

const shader_program_t shader_program_light = {
	.shaders	   = (shader_t*) light_shaders,
	.shaders_count = 2,
};
