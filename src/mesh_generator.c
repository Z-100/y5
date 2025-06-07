#include "mesh_generator.h"

#include <stdlib.h>
#include <string.h>

struct FloatArray* generate_vertices(float minX, float maxX, float minY, float maxY) {

	// clang-format off
	float vertices[] = {
		0.5f,  0.5f,  0.0f, // Half right top
		0.5f,  -0.5f, 0.0f, // Half right bottom
		-0.5f, -0.5f, 0.0f, // Half left bottom
		-0.5f, 0.5f,  0.0f, // Half left top

		0.0f,  0.0f,  0.0f, // Middle
		0.0f,  -1.0f, 0.0f, // Bottom
		0.0f,  1.0f,  0.0f, // Top

		-1.0f, 0.0f,  0.0f, // Left Middle
		-1.0f, -1.0f, 0.0f, // Left Bottom
		-1.0f, 1.0f,  0.0f, // Left Top

		1.0f,  0.0f,  0.0f, // Right Middle
		1.0f,  -1.0f, 0.0f, // Right Bottom
		1.0f,  1.0f,  0.0f, // Right Top
	};
	// clang-format on

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

struct IntArray* generate_indices(int rowLength, int columnLength) {

	// clang-format off
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
		3, 2, 1,
		0, 12, 11,
		9, 8, 5,
		0, 1, 2,
	};
	// clang-format on

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
