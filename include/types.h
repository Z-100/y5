#ifndef TYPES_H
#define TYPES_H

// =============
// Define arrays
// =============
struct Array {
	void*		 data;
	unsigned int length;
}; // General Array type

struct FloatArray {
	float*		 data;
	unsigned int length;
};

struct IntArray {
	int*		 data;
	unsigned int length;
};

// ==========
// New arrays
// ==========
struct FloatArray* new_float_array(float data[], unsigned int dataLength);
struct IntArray*   new_int_array(int data[], unsigned int dataLength);

// ===========
// Free arrays
// ===========
void free_float_array(struct FloatArray* array);
void free_int_array(struct IntArray* array);

#endif
