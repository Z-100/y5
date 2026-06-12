#ifndef RENDERER_H
#define RENDERER_H

#include "utils/collection_sys.h"

// ==============
// Renderer types
// ==============

typedef struct gl_object_t {
	model_object_t* model_object;
	uint32_t		vbo;
	uint32_t		vao;
	uint32_t		ebo;
} gl_object_t;

// =================
// General rendering
// =================

void renderer_init(const game_t* game);
void renderer_update(const game_t* game);
void renderer_destroy();

void renderer_draw(uint32_t model_id);

// =================
// Loading/unloading
// =================

void	renderer_load_shader(shader_program_t* shader_program);
uint8_t renderer_load_model(model_object_t* model_object);

#endif
