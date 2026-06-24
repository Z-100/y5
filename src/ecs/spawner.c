#include "ecs/spawner.h"
#include "utils/headers_collection.h"

void spawner_summon(ecs_engine_t* ecs_engine, component_group_t group, spawn_info_t* spawn_info) {

	entity_id_t		entity = ecs_entity_create(ecs_engine, group);
	entity_record_t record = ecs_engine->entity_index[entity];

	archetype_t* arch = record.archetype;
	size_t		 row  = record.row;

	if (has_component(filter_renderer, group)) {

		int32_t				render_col_idx = arch->column_map[RENDER];
		component_render_t* render_column  = arch->columns[render_col_idx];

		render_column[row].model_id	   = spawn_info->model_id;
		render_column[row].material_id = 1;
		render_column[row].shader_id   = spawn_info->shader_id;
		render_column[row].diffuse_id  = spawn_info->diffuse_id;
		render_column[row].specular_id = spawn_info->specular_id;

		int32_t				   transform_col_idx = arch->column_map[TRANSFORM];
		component_transform_t* transform_column	 = arch->columns[transform_col_idx];

		transform_column[row].x = spawn_info->initial_pos[0];
		transform_column[row].y = spawn_info->initial_pos[1];
		transform_column[row].z = spawn_info->initial_pos[2];

		int32_t				  rotation_col_idx = arch->column_map[ROTATION];
		component_rotation_t* rotation_column  = arch->columns[rotation_col_idx];

		rotation_column[row].x = spawn_info->initial_rotation[0];
		rotation_column[row].y = spawn_info->initial_rotation[1];
		rotation_column[row].z = spawn_info->initial_rotation[2];
		rotation_column[row].w = spawn_info->initial_rotation[3];
	}
}
