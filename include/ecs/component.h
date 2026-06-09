#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

#include <stdint.h>

// Must be same as num of component_type
#define NUM_COMPONENTS 5

enum component_type {
	TRANSFORM,
	ROTATION,
	MATERIAL,
	MODEL_OBJECT,
	RENDERER,
};

typedef union {

	uint32_t components_raw;

	struct {
		// clang-format off
		uint32_t transform    : 1;
		uint32_t rotation     : 1;
		uint32_t material     : 1;
		uint32_t model_object : 1;
		uint32_t renderer     : 1;
		uint32_t padding      : 27;
		// clang-format on
	} bit_mask;

} component_group_t;

#endif
