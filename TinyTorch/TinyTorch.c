#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "Vector.h"
#include "Tensor.h"

main() {
	float v_arr[8] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f };
	vector* v = vector_(8, v_arr);

	float shape_arr[3] = { 2.0f, 2.0f, 2.0f };
	vector* shape = vector_(3, shape_arr);

	tensor* t = tensor_(v, shape);

	float idx_arr[3] = { 1.0f, 1.0f, 1.0f };
	vector* idx = vector_(3, idx_arr);

	printf("%.1f\n", tensor_get(t, idx));

	return 0;
}