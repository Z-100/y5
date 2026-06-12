#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

#include "utils/collection_sys.h"

// Must be same as num of component_type
#define NUM_COMPONENTS 3

enum component_type {
	RENDER,
	TRANSFORM,
	ROTATION,
};

typedef union component_group_t {

	uint32_t components_raw;

	struct {
		// clang-format off
		uint32_t render    : 1;
		uint32_t transform : 1;
		uint32_t rotation  : 1;
		uint32_t padding   : 29;
		// clang-format on
	} bit_mask;

} component_group_t;

extern const component_group_t filter_render;
extern const component_group_t filter_transform;
extern const component_group_t filter_rotation;

bool has_component(component_group_t filter, component_group_t group);

#endif
