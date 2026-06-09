#ifndef COMP_RENDER_H
#define COMP_RENDER_H

#include <stdint.h>
typedef struct Material material_t;

typedef struct ComponentRenderer {
	uint32_t	 vbo;
	uint32_t	 vao;
	uint32_t	 ebo;
	ModelObject* model;
} component_renderer_t;

#endif
