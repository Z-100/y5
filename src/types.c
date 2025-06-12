#include "types.h"

#include <stdio.h>
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

void free_array(struct Array* array) {
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

struct Array* new_array(void* data, unsigned int dataLength) {

	struct Array* arrayPtr = malloc(sizeof(struct Array));

	arrayPtr->data	 = data;
	arrayPtr->length = dataLength;

	return arrayPtr;
}

void fill_array_if_exists(const void* data, unsigned int dataLength, struct Array* dataPointer) {

	if (dataPointer == NULL) {
		fprintf(stderr, "Array pointer cannot be null\n");
		return;
	}

	unsigned long long intArrayDataSize = dataLength * sizeof(unsigned int);

	dataPointer->data = malloc(intArrayDataSize);
	if (dataPointer->data == NULL) {
		fprintf(stderr, "Failed malloc for array data\n");
		free(dataPointer);
		return;
	}

	memcpy(dataPointer->data, data, intArrayDataSize);
	dataPointer->length = dataLength;
}
