#ifndef RENDERER_H
#define RENDERER_H

#include "utils/headers_collection.h"

// ==============
// Renderer types
// ==============

typedef struct {
	model_object_t* model_object;
	uint32_t		vbo;
	uint32_t		vao;
	uint32_t		ebo;
} gl_object_t;

// =================
// General rendering
// =================

void renderer_init(const Game* game);
void renderer_update(const Game* game);
void renderer_destroy();

// =================
// Loading/unloading
// =================

void	renderer_load_shader(shader_program_t* shader_program);
uint8_t renderer_load_model(model_object_t* model_object);

#endif
