#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

#include <stdint.h>

typedef struct Material Material;

enum component_type {
	TRANSFORM,
	ROTATION,
	MATERIAL,
	MODEL_OBJECT,
	RENDERER,
};

typedef struct ComponentRenderer {
	uint32_t  vbo;
	uint32_t  vao;
	uint32_t  ebo;
	Material* material;
	uint32_t* textures;
} component_renderer_t;

typedef struct {
	enum component_type type;
} component_t;

#endif
