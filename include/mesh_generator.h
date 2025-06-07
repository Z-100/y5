#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include "types.h"

struct FloatArray* generate_vertices(float minX, float maxX, float minY, float maxY);

struct IntArray* generate_indices(int rowLength, int columnLength);

#endif
