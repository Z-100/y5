#ifndef COMP_TRANSFORM_H
#define COMP_TRANSFORM_H

#include "utils/collection_sys.h"

typedef struct component_transform_t {
	float x;
	float y;
	float z;
} component_transform_t;

typedef struct component_rotation_t {
	float x;
	float y;
	float z;
	float w;
} component_rotation_t;

#endif
