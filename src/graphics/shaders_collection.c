#include "graphics/shaders_collection.h"
#include "utils/collection_hdr.h"

static void _init_shader(shader_t* shader, const char* name, const GLuint type);
static void _init_texture(shader_texture_t* tex, const char* tex_name, const char* uni_name);

// ====================
// = Shaders: Default =
// ====================

static shader_program_t* shader_program_default = nullptr;

shader_program_t* shaders_collection_default() {

	if (shader_program_default) {
		log_warn("shader_program:default already built");
		return shader_program_default;
	}

	shader_program_default = malloc(sizeof(shader_program_t));
	if (!shader_program_default) {
		log_error("Failed allocating for shader program");
		return nullptr;
	}

	shader_program_default->id = -1;

	shader_program_default->shaders		  = calloc(sizeof(shader_t), 2);
	shader_program_default->shaders_count = 2;
	_init_shader(
		&shader_program_default->shaders[0], "vertex_shader_textures.glsl", GL_VERTEX_SHADER
	);
	_init_shader(
		&shader_program_default->shaders[1], "fragment_shader_textures.glsl", GL_FRAGMENT_SHADER
	);

	shader_program_default->textures	   = calloc(sizeof(shader_texture_t), 2);
	shader_program_default->textures_count = 2;
	_init_texture(&shader_program_default->textures[0], "res/textures/elmo.png", "u_elmoTexture");
	_init_texture(&shader_program_default->textures[1], "res/textures/obama.png", "u_obamaTexture");

	return shader_program_default;
}

// ====================
// = Shaders: Light   =
// ====================

static shader_program_t* shader_program_light = nullptr;

shader_program_t* shaders_collection_light() {

	if (shader_program_light) {
		log_warn("shader_program:default already built");
		return shader_program_light;
	}

	shader_program_light = malloc(sizeof(shader_program_t));
	if (!shader_program_light) {
		log_error("Failed allocating for shader program");
		return nullptr;
	}

	shader_program_light->id			 = -1;
	shader_program_light->textures_count = 0;

	shader_program_light->shaders		= calloc(sizeof(shader_t), 2);
	shader_program_light->shaders_count = 2;
	_init_shader(
		&shader_program_light->shaders[0], "vertex_shader_light_source.glsl", GL_VERTEX_SHADER
	);
	_init_shader(
		&shader_program_light->shaders[1], "fragment_shader_light_source.glsl", GL_FRAGMENT_SHADER
	);

	return shader_program_light;
}

// ====================
// = Shaders: Helpers =
// ====================

static void _init_shader(shader_t* shader, const char* name, const GLuint type) {
	shader->id	 = -1;
	shader->name = name;
	shader->type = type;
}

static void _init_texture(shader_texture_t* tex, const char* tex_name, const char* uni_name) {
	tex->id			  = -1;
	tex->texture_name = tex_name;
	tex->uniform_name = uni_name;
}

