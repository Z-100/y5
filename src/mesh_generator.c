#include "mesh_generator.h"

#include <stdlib.h>
#include <string.h>

struct FloatArray* generate_vertices(unsigned int width, unsigned int height) {

	if (width == 0 || height == 0) {
		perror("Cannot create 0 vertices");
		return nullptr;
	}

	unsigned int verticesLength = width * height * 3;
	float		 vertices[verticesLength];

	// le stretch to top&right
	float y_ceil = (float) (height - 1);
	float x_ceil = (float) (width - 1);

	float y_normalizer = 1.f / y_ceil;
	float x_normalizer = 1.f / x_ceil;

	int verticeIndex = 0;
	for (int y = 0; y < height; y++) {

		float y_normalized = ((float) y - y_ceil / 2) * y_normalizer * 2;

		for (int x = 0; x < width; x++) {

			float x_normalized = ((float) x - x_ceil / 2) * x_normalizer * 2;

			vertices[verticeIndex + 0] = x_normalized;
			vertices[verticeIndex + 1] = y_normalized;
			vertices[verticeIndex + 2] = 0;

			verticeIndex += 3;
		}
	}

	struct FloatArray* verticesData = malloc(sizeof(struct FloatArray));
	if (verticesData == nullptr) {
		perror("Failed to allocate memory for vertices array");
		return nullptr;
	}

	unsigned long long verticesDataSize = verticesLength * sizeof(float);

	verticesData->data = malloc(verticesDataSize);
	if (verticesData->data == nullptr) {
		perror("Failed to allocate memory for vertices data");
		free(verticesData);
		return nullptr;
	}

	memcpy(verticesData->data, vertices, verticesDataSize);
	verticesData->length = verticesLength;

	return verticesData;
}

struct IntArray* generate_indices(unsigned int width, unsigned int height) {

	if (width == 0 || height == 0) {
		perror("Cannot create 0 indices");
		return nullptr;
	}

	unsigned int indicesLength = (width - 1) * (height - 1) * 3;
	unsigned int indices[indicesLength];

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
	if (indicesData == nullptr) {
		perror("Failed to allocate memory for indices array");
		return nullptr;
	}

	unsigned long long indicesDataSize = indicesLength * sizeof(unsigned int);

	indicesData->data = malloc(indicesDataSize);
	if (indicesData->data == nullptr) {
		perror("Failed to allocate memory for indices data");
		free(indicesData);
		return nullptr;
	}

	memcpy(indicesData->data, indices, indicesDataSize);
	indicesData->length = indicesLength;

	return indicesData;
}

struct IntArray* generate_indices2(unsigned int width, unsigned int height) {

	if (width == 0 || height == 0) {
		perror("Cannot create 0 indices");
		return nullptr;
	}

	unsigned int indicesLength = (width - 1) * (height - 1) * 3;
	unsigned int indices[indicesLength];

	int indicesIndex = 0;
	for (int y = 0; y < height - 1; y++) {
		for (int x = 0; x < width - 1; x++) {

			unsigned int i = y * width + x;

			// Let's do half-quads only
			indices[indicesIndex + 0] = i + width + 1;
			indices[indicesIndex + 1] = i + 1;
			indices[indicesIndex + 2] = i;

			indicesIndex += 3;
		}
	}

	struct IntArray* indicesData = malloc(sizeof(struct IntArray));
	if (indicesData == nullptr) {
		perror("Failed to allocate memory for indices array");
		return nullptr;
	}

	unsigned long long indicesDataSize = indicesLength * sizeof(unsigned int);

	indicesData->data = malloc(indicesDataSize);
	if (indicesData->data == nullptr) {
		perror("Failed to allocate memory for indices data");
		free(indicesData);
		return nullptr;
	}

	memcpy(indicesData->data, indices, indicesDataSize);
	indicesData->length = indicesLength;

	return indicesData;
}
