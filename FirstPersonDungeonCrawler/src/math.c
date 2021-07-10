#include "math.h"

#include <nustd/math.h>

void get_forward_vector_from_angle(float angle, float *x, float *y) {
	*x = cos(angle);
	*y = sin(angle);
}
