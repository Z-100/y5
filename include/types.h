#ifndef TYPES_H
#define TYPES_H

struct FloatArray {
	float*		 data;
	unsigned int length;
};

struct IntArray {
	int*		 data;
	unsigned int length;
};

void free_float_array(struct FloatArray* array);
void free_int_array(struct IntArray* array);

#endif
