#ifndef COMP_RENDER_H
#define COMP_RENDER_H

#include "utils/headers_collection.h"

typedef struct {

	// Only IDs for cache hits
	uint32_t model_id;
	uint32_t material_id;
	uint32_t shader_id;

} component_render_t;

#endif
