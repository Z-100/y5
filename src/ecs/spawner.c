#include "ecs/spawner.h"

void spawn_element(ecs_engine_t* ecs_engine, char* model_name) {

	component_group_t renderer_group = {
		.bit_mask = {
			.renderer = 1,
			.material = 1,
			.model_object = 1,
			.rotation = 1,
		}
	};

	component_group_t light_group = {
		.bit_mask = {
			.renderer = 1,
			.model_object = 1,
			.transform = 1,
		}
	};

	entity_id_t monkey = ecs_entity_create(ecs_engine, renderer_group);
	entity_id_t light = ecs_entity_create(ecs_engine, renderer_group);

	entity_record_t record = ecs_engine->entity_index[monkey];
	archetype_t* arch = record.archetype;

	// Find the columns
	int32_t renderer_col = arch->column_mapping[RENDERER];
	int32_t material_col = arch->column_mapping[MATERIAL];
	int32_t model_object_col = arch->column_mapping[MODEL_OBJECT];

	// Get the arrays and cast them
	component_renderer_t* renderers = arch->columns[renderer_col];


	renderers[record.row].vao = 1;
	renderers[record.row].vbo = 2;
	renderers[record.row].ebo = 3;

	ModelObject* monkey_obj = load_model("res/models", model_name);
	renderer_init_model(monkey_obj);
	renderers[record.row].model = monkey_obj;

}