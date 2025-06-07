#include "types.h"

#include <stdlib.h>

void free_float_array(struct FloatArray* array) {
	free(array->data);
	free(array);
}

void free_int_array(struct IntArray* array) {
	free(array->data);
	free(array);
}
