#ifndef ECS_H
#define ECS_H

#include <stddef.h>
#include "component.h"

#define ENTITY_CAP 100
#define TICK_RATE 60

typedef uint64_t entity_id_t;
typedef uint64_t component_id_t;

typedef struct {
	component_id_t* types;
	size_t		type_count;
} component_type_t;

// ====================
// = Archetypes Graph =
// ====================

typedef struct Archetype {
	component_group_t signature;

	// Data & size
	void**	columns;
	int32_t column_mapping[NUM_COMPONENTS];

	// Entity->row
	entity_id_t* entities;
	size_t		 row_count;
	size_t		 row_capacity;
} archetype_t;

typedef struct {
	archetype_t* archetype;
	uint32_t	 row;
} entity_record_t;

typedef struct {
	archetype_t* archetype;
	size_t		 column_index;
} archetype_record_t;

// For a single component type, it points to all archetypes that include it
typedef struct {
	component_id_t		component;
	archetype_record_t* records;
	size_t				record_count;
} component_index_entry_t;

// ======================
// = ECS global manager =
// ======================

typedef struct {
	archetype_t** archetypes;
	size_t		  arch_count;
	size_t		  arch_capacity;

	entity_record_t* entity_index;
	size_t			 entity_count;
	size_t			 entity_capacity;

	float delta_time;
} ecs_engine_t;

// =================
// = ECS Functions =
// =================

ecs_engine_t* ecs_engine_new();
void		  ecs_engine_tick(ecs_engine_t* engine, float delta_time);

archetype_t**
ecs_get_matching_archetypes(ecs_engine_t* engine, component_group_t filter, size_t* out_count);

entity_id_t ecs_entity_create(ecs_engine_t* engine, component_group_t signature);

#endif
