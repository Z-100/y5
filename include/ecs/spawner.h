#ifndef SPAWNER_H
#define SPAWNER_H

#include "utils/collection_sys.h"

void spawner_spawn(ecs_engine_t* ecs_engine, char* model_name);
void spawner_summon(ecs_engine_t* ecs_engine, uint8_t model_id);

#endif
