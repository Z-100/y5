#include "ecs/spawner.h"
#include "utils/headers_collection.h"

void spawner_summon(
	ecs_engine_t*	  ecs_engine,
	component_group_t group,
	uint8_t			  model_id,
	GLuint			  shader_id,
	GLuint			  diffuse_id,
	GLuint			  specular_id
) {

	entity_id_t		entity = ecs_entity_create(ecs_engine, group);
	entity_record_t record = ecs_engine->entity_index[entity];
	archetype_t*	arch   = record.archetype;

	if (has_component(filter_render, group)) {

		int32_t				render_col_idx	  = arch->column_map[RENDER];
		component_render_t* render_components = arch->columns[render_col_idx];

		render_components->model_id	   = model_id;
		render_components->material_id = 1;
		render_components->shader_id   = shader_id;

		render_components->diffuse_id  = diffuse_id;
		render_components->specular_id = specular_id;
	}

	if (has_component(filter_transform, group)) {

		int32_t				   transform_col_idx = arch->column_map[TRANSFORM];
		component_transform_t* transform		 = arch->columns[transform_col_idx];

		transform->x = 1.5f;
		transform->y = 1.5f;
		transform->z = 1.5f;
	}
}
