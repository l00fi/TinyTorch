#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "Vector.h"
#include "Tensor.h"

main() {
	// ----- Пример работы vector ----- 
	//float v_arr[3] = { 0.0f, 1.0f, 2.0f };
	//vector* v = vector_(3, v_arr, sizeof(float));
	//vector_print(v, float_);

	//float shape_arr[3] = { 2.0f, 2.0f, 2.0f };
	//vector* shape = vector_(3, shape_arr, sizeof(float));
	//vector_print(shape, float_);

	//vector* v3 = vector_sum(v, shape);

	//float val = 10.0f;
	//vector_edit(v3, 0, &val);

	//vector_print(v3, float_);
	
	float v_arr[8] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f };
	vector* v = vector_(8, v_arr, sizeof(float));

	int shape_arr[3] = { 2, 2, 2 };
	vector* shape = vector_(3, shape_arr, sizeof(int));

	tensor* t = tensor_(v, shape);

	int idx_arr[3] = { 1, 1, 1 };
	vector* idx = vector_(3, idx_arr, sizeof(int));

	printf("%.1f\n", *(float*)tensor_get(t, idx));

	return 0;
}