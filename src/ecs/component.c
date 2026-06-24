#include "ecs/component.h"

bool has_component(component_group_t filter, component_group_t group) {
	return (filter.components_raw & group.components_raw) != 0;
}

// clang-format off

/**
 * Pure rendering system component
 * > Optional rotation is set to { 0, 0, 0, 1 } by default
 */
const component_group_t filter_renderer	 = { .bit_mask = {
	.render    = 1,
	.transform = 1,
	.rotation  = 1
}};

// clang-format off
