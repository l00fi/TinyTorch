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

	//vector_print(v3, vfloat);
	// --------- Конец примера --------

	float v_arr_a[6] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f };
	vector* v_a = vector_(6, v_arr_a, sizeof(float));

	int shape_arr_a[2] = { 2, 3 };
	vector* shape_a = vector_(2, shape_arr_a, sizeof(int));

	tensor* t_a = tensor_(v_a, shape_a);

	float v_arr_b[6] = { 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f };
	vector* v_b = vector_(6, v_arr_b, sizeof(float));

	int shape_arr_b[2] = { 2, 3 };
	vector* shape_b = vector_(2, shape_arr_b, sizeof(int));

	tensor* t_b = tensor_(v_b, shape_b);

	tensor* t_c = tensor_sum(t_a, t_b, FLOAT);
	tensor_print(t_c, 0, 0, FLOAT);

	return 0;
}