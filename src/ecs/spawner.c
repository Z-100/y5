// #include "ecs/spawner.h"
// #include "utils/headers_collection.h"
//
// component_group_t group_obj = { .bit_mask = {
// 									.render = 1,
// 								} };
//
// component_group_t group_light = { .bit_mask = {
// 									  .render	= 1,
// 									  .rotation = 1,
// 								  } };
//
// void _spawn_entity(ecs_engine_t* ecs_engine) {
//
// 	entity_id_t entity = ecs_entity_create(ecs_engine, group_obj);
// }
//
// void spawner_summon(ecs_engine_t* ecs_engine, component_group_t group, uint8_t model_id) {
//
// 	entity_id_t		entity = ecs_entity_create(ecs_engine, group_obj);
// 	entity_record_t record = ecs_engine->entity_index[entity];
// 	archetype_t*	arch   = record.archetype;
//
// 	if (has_component(filter_render, group)) {
//
// 		int32_t				render_col_idx	  = arch->column_map[RENDER];
// 		component_render_t* render_components = arch->columns[render_col_idx];
// 	}
// }
//
// void spawn_elements(ecs_engine_t* ecs_engine, char* model_name) {
//
// 	entity_id_t light = ecs_entity_create(ecs_engine, group_light);
//
// 	// Find the columns
// 	int32_t material_col	 = arch->column_map[MATERIAL];
// 	int32_t model_object_col = arch->column_map[MODEL_OBJECT];
//
// 	// Get the arrays and cast them
//
// 	renderers[record.row].vao = 1;
// 	renderers[record.row].vbo = 2;
// 	renderers[record.row].ebo = 3;
//
// 	renderer_init_model(monkey_obj);
// 	renderers[record.row].model = monkey_obj;
// }
