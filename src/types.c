#include "types.h"

#include <stdlib.h>
#include <string.h>

void fill_array_if_exists(const void* data, unsigned int dataLength, struct Array* dataPointer);

void free_float_array(struct FloatArray* array) {
	free(array->data);
	free(array);
}

void free_int_array(struct IntArray* array) {
	free(array->data);
	free(array);
}

struct FloatArray* new_float_array(float data[], unsigned int dataLength) {

	struct FloatArray* floatArray = malloc(sizeof(struct FloatArray));
	fill_array_if_exists(data, dataLength, (struct Array*) floatArray);

	return floatArray;
}

struct IntArray* new_int_array(int data[], unsigned int dataLength) {

	struct IntArray* intArray = malloc(sizeof(struct IntArray));
	fill_array_if_exists(data, dataLength, (struct Array*) intArray);

	return intArray;
}

void fill_array_if_exists(const void* data, unsigned int dataLength, struct Array* dataPointer) {

	if (dataPointer == NULL) {
		perror("Failed to allocate memory for int array");
		return;
	}

	unsigned long long intArrayDataSize = dataLength * sizeof(unsigned int);

	dataPointer->data = malloc(intArrayDataSize);
	if (dataPointer->data == NULL) {
		perror("Failed to allocate memory for int array data");
		free(dataPointer);
		return;
	}

	memcpy(dataPointer->data, data, intArrayDataSize);
	dataPointer->length = dataLength;
}
