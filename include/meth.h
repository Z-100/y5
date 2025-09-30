#ifndef METH_H
#define METH_H

#define MATH_MAX_TWO(C_TYPE) \
	C_TYPE math_max_##C_TYPE(C_TYPE var1, C_TYPE var2) { \
		return var1 >= var2 \
			? var1 \
			: var2; \
	}

MATH_MAX_TWO(int)
MATH_MAX_TWO(float)

#define MATH_MAX_THREE(C_TYPE) \
	C_TYPE math_max_triple_##C_TYPE(C_TYPE var1, C_TYPE var2, C_TYPE var3) { \
		return var1 >= var2 \
			? (var1 >= var3 ? var1 : var3) \
			: var2; \
	}

MATH_MAX_THREE(int)
MATH_MAX_THREE(float)

#endif