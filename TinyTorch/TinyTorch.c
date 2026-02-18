#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Vector.h"
#include "Tensor.h"

main() {
	//vector* v1 = vector_new(2);
	//vector_append(v1, 1);
	//vector_append(v1, 2);
	//vector_append(v1, 3);
	//vector_append(v1, 4);

	//vector* v2 = vector_new(2);
	//vector_append(v2, 5);
	//vector_append(v2, 6);
	//vector_append(v2, 7);
	//vector_append(v2, 8);

	//vector* v3 = vector_add(v1, v2);

	//vector_edit(v3, -1, 1);

	//vector_print(v3);

	//float data[2] = { 1.0f, 2.0f };
	//vector* v = vector_new(2, data);
	//vector_print(v);

	vector* v = vector_new(8);
	for (int i = 0; i < 8; ++i) {
		vector_append(v, i);
	}

	vector* shape = vector_new(3);
	vector_append(shape, 2);
	vector_append(shape, 2);
	vector_append(shape, 2);

	tensor* t = tensor_new(v, shape);
	vector_print(t->strides);

	vector* idx = vector_new(3);
	vector_append(idx, 1);
	vector_append(idx, 1);
	vector_append(idx, 1);

	printf("%.1f\n", tensor_get(t, idx));

	return 0;
}