#include "utils/math_utils.h"

#define MATH_MAX_TWO(C_TYPE) \
	inline C_TYPE math_max2_##C_TYPE(C_TYPE a, C_TYPE b) { \
		return a >= b ? a : b; \
	}

MATH_MAX_TWO(int)
MATH_MAX_TWO(float)

#define MATH_MAX_THREE(C_TYPE) \
	inline C_TYPE math_max3_##C_TYPE(C_TYPE a, C_TYPE b, C_TYPE c) { \
		C_TYPE max_ab = a >= b ? a : b; \
		return max_ab >= c ? max_ab : c; \
	}

MATH_MAX_THREE(int)
MATH_MAX_THREE(float)
