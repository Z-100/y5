#include "mesh_generator.h"

#include <stdlib.h>
#include <string.h>

struct FloatArray* generate_vertices(unsigned int x_rowlength, unsigned int y_rowlength) {

	if (x_rowlength == 0 || y_rowlength == 0) {
		perror("Cannot create 0 vertices");
		return nullptr;
	}

	unsigned int totalVertices	  = x_rowlength * y_rowlength;
	unsigned int totalCoordinates = totalVertices * 3;

	float vertices[totalCoordinates];

	float y_ceil = (float) y_rowlength;
	float x_ceil = (float) x_rowlength;

	float y_normalizer = 1.f / y_ceil;
	float x_normalizer = 1.f / x_ceil;

	int coordinateIndex = 0;
	for (int y = 0; y < y_rowlength; y++) {

		float y_normalized = ((float) y - y_ceil / 2) * y_normalizer * 2;

		for (int x = 0; x < x_rowlength; x++) {

			float x_normalized = ((float) x - x_ceil / 2) * x_normalizer * 2;

			vertices[coordinateIndex + 0] = x_normalized;
			vertices[coordinateIndex + 1] = y_normalized;
			vertices[coordinateIndex + 2] = 0;

			coordinateIndex += 3;
		}
	}

	struct FloatArray* verticesData = malloc(sizeof(struct FloatArray));
	if (verticesData == nullptr)
		return nullptr;

	verticesData->data = malloc(sizeof vertices);
	if (verticesData->data == nullptr) {
		free(verticesData);
		return nullptr;
	}

	memcpy(verticesData->data, vertices, sizeof vertices);
	verticesData->length = sizeof vertices / sizeof(float);

	return verticesData;
}

struct IntArray* generate_indices(int width, int height) {

	unsigned int totalIndices = (width - 1) * (height - 1) * 3;
	unsigned int indices[totalIndices];

	int indicesIndex = 0;
	for (int y = 0; y < height - 1; y++) {
		for (int x = 0; x < width - 1; x++) {

			unsigned int i = y * width + x;

			// Let's do half-quads only
			indices[indicesIndex + 0] = i + width + 1;
			indices[indicesIndex + 1] = i + width;
			indices[indicesIndex + 2] = i;

			indicesIndex += 3;
		}
	}

	struct IntArray* indicesData = malloc(sizeof(struct IntArray));
	if (indicesData == nullptr)
		return nullptr;

	indicesData->data = malloc(sizeof indices);
	if (indicesData->data == nullptr) {
		free(indicesData);
		return nullptr;
	}

	memcpy(indicesData->data, indices, sizeof indices);
	indicesData->length = sizeof indices / sizeof(unsigned int);

	return indicesData;
}
