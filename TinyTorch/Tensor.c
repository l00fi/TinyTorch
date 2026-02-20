#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "Tensor.h"

tensor* tensor_(vector* data, vector* shape) {
    if (data == NULL || shape == NULL) return NULL;

    tensor* t = malloc(sizeof(tensor));
    if (t == NULL) return NULL;

    size_t rank = shape->size;
    int elements_in_tensor = 1;
    for (int i = 0; i < (int)rank; ++i) {
        elements_in_tensor *= *(int*)vector_get(shape, i);
    }
    // Проверка на размерность и кол-во элементов
    if (data->size != elements_in_tensor) {
        free(t);
        return NULL;
    }

    t->data = data;
    t->shape = shape;

    vector* strides = vector_empty(rank, sizeof(int));

    for (size_t i = 0; i < rank; i++) {
        int val = 0;
        vector_append(strides, &val);
    }

    int val = 1;
    vector_edit(strides, -1, &val);
    for (int i = 2; i <= (int)rank; i++) {
        int prev_shape = *(int*)vector_get(shape, -(i - 1));
        int prev_stride = *(int*)vector_get(strides, -(i - 1));
        int new_stride = prev_shape * prev_stride;
        vector_edit(strides, -i, &new_stride);
    }

    t->strides = strides;
    return t;
}

void* tensor_get(tensor* t, vector* idx) {
    if (t == NULL || t->data == NULL) return;

    if (idx == NULL || idx->data == NULL) return;

    if (t->shape->size != idx->size) return;
    //Проверка на корректность индексов
    for (int i = 0; i < t->shape->size; ++i) {
        if (*(int*)vector_get(idx, i) + 1 > *(int*)vector_get(t->shape, i) || *(int*)vector_get(idx, i) < 0) return;
    }

    int true_index = 0;
    for (int i = 0; i < t->strides->size; ++i) {
        true_index += *(int*)vector_get(t->strides, i) * *(int*)vector_get(idx, i);
    }

    return vector_get(t->data, true_index);
}