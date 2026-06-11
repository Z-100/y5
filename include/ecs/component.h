#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

// Must be same as num of component_type
#define NUM_COMPONENTS 3

enum component_type {
	RENDER,
	TRANSFORM,
	ROTATION,
};

typedef union {

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

const component_group_t filter_render	 = { .bit_mask = { .render = 1 } };
const component_group_t filter_transform = { .bit_mask = { .transform = 1 } };
const component_group_t filter_rotation	 = { .bit_mask = { .rotation = 1 } };

bool has_component(component_group_t filter, component_group_t group) {
	return (filter.components_raw & group.components_raw) != 0;
}

#endif
