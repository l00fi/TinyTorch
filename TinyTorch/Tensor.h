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

#endif

