#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include "types.h"

struct FloatArray* generate_vertices(unsigned int x_rowlength, unsigned int y_rowlength);

struct IntArray* generate_indices(int width, int height);

#endif
