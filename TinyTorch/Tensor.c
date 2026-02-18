#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Tensor.h"

tensor* tensor_new(vector* data, vector* shape) {
    if (data == NULL || shape == NULL) return NULL;

    tensor* t = malloc(sizeof(tensor));
    if (t == NULL) return NULL;

    size_t rank = shape->size;
    int elements_in_tensor = 1;
    for (int i = 0; i < rank; ++i) {
        elements_in_tensor *= (int)shape->data[i];
    }
    // Дополнить потом!
    if (data->size != elements_in_tensor) return NULL;

    t->data = data;
    t->shape = shape;

    vector* strides = vector_new(rank);

    for (size_t i = 0; i < rank; i++) {
        vector_append(strides, 0.0f);
    }

    vector_edit(strides, -1, 1.0f);
    for (int i = 2; i <= (int)rank; i++) {
        float prev_shape = vector_get(shape, -(i - 1));
        float prev_stride = vector_get(strides, -(i - 1));

        vector_edit(strides, -i, prev_shape * prev_stride);
    }

    t->strides = strides;
    return t;
}

float tensor_get(tensor* t, vector* idx) {
    if (t == NULL || t->data == NULL) return NAN;

    if (idx == NULL || idx->data == NULL) return NAN;

    if (t->shape->size != idx->size) return NAN;

    for (int i = 0; i < t->shape->size; ++i) {
        if (idx->data[i] + 1 > t->shape->data[i] || idx->data[i] < 0) return NAN;
    }

    int true_index = 0;
    for (int i = 0; i < t->strides->size; ++i) {
        true_index += (int)t->strides->data[i] * (int)idx->data[i];
    }

    return t->data->data[true_index];
}