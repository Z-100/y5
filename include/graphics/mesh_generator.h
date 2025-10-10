#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

struct FloatArray* generate_vertices(unsigned int width, unsigned int height);
struct IntArray*   generate_indices(unsigned int width, unsigned int height);
struct IntArray*   generate_indices2(unsigned int width, unsigned int height);

#endif
