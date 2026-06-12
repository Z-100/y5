#include "ecs/ecs_engine.h"
#include "utils/headers_collection.h"

static size_t get_component_size(enum component_type type);

// =========================
// = ECS Engine Archetypes =
// =========================

// TODO: Malloc checks
ecs_engine_t* ecs_engine_new() {

	ecs_engine_t* engine = malloc(sizeof(ecs_engine_t));

	engine->arch_count	  = 0;
	engine->arch_capacity = 8;
	engine->archetypes	  = calloc(engine->arch_capacity, sizeof(archetype_t*));

	engine->entity_count	= 0;
	engine->entity_capacity = 1024;
	engine->entity_index	= calloc(engine->entity_capacity, sizeof(entity_record_t));

	engine->delta_time = 0.0f;

	return engine;
}

void ecs_engine_tick(ecs_engine_t* engine, float delta_time) {
	engine->delta_time = delta_time;

	// 1. Process player inputs
	// TODO: Process inputs here

	// 2. Do physics stuff
	// TODO: Process physics here

	// 3. Render stuff
	sys_renderer_update(engine);

	// X. Delete entities
	// TODO: Delete entities here
}

archetype_t**
ecs_get_matching_archetypes(ecs_engine_t* engine, component_group_t filter, size_t* out_count) {

	archetype_t** matches = calloc(engine->arch_count, sizeof(archetype_t*));
	size_t		  count	  = 0;

	for (size_t i = 0; i < engine->arch_count; i++) {

		archetype_t* arch = engine->archetypes[i];

		if ((arch->signature.components_raw & filter.components_raw) == filter.components_raw) {
			matches[count++] = arch;
		}
	}

	*out_count = count;
	return matches;
}

archetype_t* ecs_get_or_create_archetype(ecs_engine_t* engine, component_group_t signature) {

	for (size_t i = 0; i < engine->arch_count; i++) {
		if (engine->archetypes[i]->signature.components_raw == signature.components_raw) {
			return engine->archetypes[i];
		}
	}

	archetype_t* arch  = malloc(sizeof(archetype_t));
	arch->signature	   = signature;
	arch->row_count	   = 0;
	arch->row_capacity = 4;
	arch->entities	   = calloc(arch->row_capacity, sizeof(entity_id_t));

	size_t active_components = 0;
	for (int i = 0; i < NUM_COMPONENTS; i++) {

		// Default: Not exists
		arch->column_map[i] = -1;

		if (signature.components_raw & (1 << i)) {
			active_components++;
		}
	}

	arch->columns = calloc(active_components, sizeof(void*));

	size_t col_idx = 0;
	for (int i = 0; i < NUM_COMPONENTS; i++) {

		if (signature.components_raw & (1 << i)) {

			arch->column_map[i] = col_idx;

			size_t comp_size	   = get_component_size(i);
			arch->columns[col_idx] = calloc(arch->row_capacity, comp_size);

			col_idx++;
		}
	}

	if (engine->arch_count >= engine->arch_capacity) {
		engine->arch_capacity *= 2;
		engine->archetypes =
			realloc(engine->archetypes, engine->arch_capacity * sizeof(archetype_t*));
	}

	engine->archetypes[engine->arch_count++] = arch;

	return arch;
}

// =========================
// = ECS Engine Entities   =
// =========================

static entity_id_t id_counter = 0;

entity_id_t ecs_entity_create(ecs_engine_t* engine, component_group_t signature) {

	entity_id_t entity = id_counter++;

	archetype_t* arch = ecs_get_or_create_archetype(engine, signature);

	if (arch->row_count >= arch->row_capacity) {

		arch->row_capacity *= 2;
		arch->entities = realloc(arch->entities, arch->row_capacity * sizeof(entity_id_t));

		for (int i = 0; i < NUM_COMPONENTS; i++) {

			int32_t col_idx = arch->column_map[i];

			if (col_idx != -1) {
				size_t comp_size = get_component_size(i);
				arch->columns[col_idx] =
					realloc(arch->columns[col_idx], arch->row_capacity * comp_size);
			}
		}
	}

	size_t assigned_row			 = arch->row_count;
	arch->entities[assigned_row] = entity;
	arch->row_count++;

	if (entity >= engine->entity_capacity) {
		engine->entity_capacity *= 2;
		engine->entity_index =
			realloc(engine->entity_index, engine->entity_capacity * sizeof(entity_record_t));
	}

	engine->entity_index[entity].archetype = arch;
	engine->entity_index[entity].row	   = assigned_row;

	return entity;
}

static size_t get_component_size(enum component_type type) {
	switch (type) {
		case RENDER:
			return sizeof(component_render_t);
		case TRANSFORM:
			return sizeof(component_transform_t);
		case ROTATION:
			return sizeof(component_rotation_t);
		default:
			return 0;
	}
}

