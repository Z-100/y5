#ifndef SPAWNER_H
#define SPAWNER_H

#include "utils/collection_sys.h"

void spawner_summon(
	ecs_engine_t*	  ecs_engine,
	component_group_t group,
	uint8_t			  model_id,
	GLuint			  shader_id,
	GLuint			  diffuse_id,
	GLuint			  specular_id
);

#endif
