#ifndef SPAWNER_H
#define SPAWNER_H

#include "utils/collection_sys.h"

typedef struct spawn_info_t {

	uint8_t model_id;
	GLuint	shader_id;
	GLuint	diffuse_id;
	GLuint	specular_id;

	vec3 initial_pos;
	vec4 initial_rotation;

} spawn_info_t;

void spawner_summon(ecs_engine_t* ecs_engine, component_group_t group, spawn_info_t* spawn_info);

#endif
