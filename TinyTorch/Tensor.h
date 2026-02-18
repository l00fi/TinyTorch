#ifndef TENSOR_H
#define TENSOR_H

#include "Vector.h"

typedef struct {
	vector* data;
	vector* shape;
	vector* strides;
} tensor;

tensor* tensor_new(vector* data, vector* shape);
float tensor_get(tensor* data, vector* idx);

#endif

