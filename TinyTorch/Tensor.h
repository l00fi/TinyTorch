#ifndef TENSOR_H
#define TENSOR_H

#include "Vector.h"

typedef struct {
	vector* data;
	vector* shape;
	vector* strides;
} tensor;

tensor* tensor_(vector* data, vector* shape);
void*   tensor_get(tensor* data, vector* idx);
void	tensor_print(tensor* t, int dim_idx, int offset, DataType type);
tensor* tensor_sum(tensor* t1, tensor* t2, DataType type);

#endif

