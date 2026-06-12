#include "ecs/component.h"

const component_group_t filter_render	 = { .bit_mask = { .render = 1 } };
const component_group_t filter_transform = { .bit_mask = { .transform = 1 } };
const component_group_t filter_rotation	 = { .bit_mask = { .rotation = 1 } };

bool has_component(component_group_t filter, component_group_t group) {
	return (filter.components_raw & group.components_raw) != 0;
}
