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
        int new_stride = *(int*)vector_get(shape, -(i - 1)) * *(int*)vector_get(strides, -(i - 1));
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

void tensor_print(tensor* t, int dim_idx, int offset, DataType type) {
    if (t == NULL) {
        printf("[NULL]\n");
        return;
    }

    int shape_size = t->shape->size;
    // Базовый случай, когда доходим до shape[-1]
    if (dim_idx == shape_size - 1) {
        int last = *(int*)vector_get(t->shape, dim_idx);
        printf("[");
        for (int i = 0; i < last; ++i) {
            int current_stride = *(int*)vector_get(t->strides, dim_idx);
            int true_index = offset + (i * current_stride);

            if (type == FLOAT) {
                vfloat(vector_get(t->data, true_index));
            }

            if(type == DOUBLE) {
                vdouble(vector_get(t->data, true_index));
            }

            if(type == INT) {
                vint(vector_get(t->data, true_index));
            }

            if (i < last - 1) printf(", ");
        }
        printf("]");
    }
    else {
        printf("[");
        int shape_val = *(int*)vector_get(t->shape, dim_idx);
        for (int i = 0; i < shape_val; ++i) {
            int current_stride = *(int*)vector_get(t->strides, dim_idx);
            int new_offset = offset + (i * current_stride);
            if (i > 0) printf("%*c", dim_idx + 1, ' ');
            // Для каждого измерения рекурсивно вызываем print_tensor c пересчитанным offset
            tensor_print(t, dim_idx + 1, new_offset, type);
            if (i < shape_val - 1) printf(",\n");
        }
        printf("]");
    }
}

// Переношу значение тензора согласно shape
static void __tensor_values_by_shape(tensor* t, vector* rez, int dim_idx, int offset) {
    if (t == NULL) {
        printf("[NULL]\n");
        return;
    }

    int shape_size = t->shape->size;
    // Базовый случай, когда доходим до shape[-1]
    if (dim_idx == shape_size - 1) {
        int last = *(int*)vector_get(t->shape, dim_idx);
        for (int i = 0; i < last; ++i) {
            int current_stride = *(int*)vector_get(t->strides, dim_idx);
            int true_index = offset + (i * current_stride);
            vector_append(rez, vector_get(t->data, true_index));
        }
    }
    else {
        int shape_val = *(int*)vector_get(t->shape, dim_idx);
        for (int i = 0; i < shape_val; ++i) {
            int current_stride = *(int*)vector_get(t->strides, dim_idx);
            int new_offset = offset + (i * current_stride);
            // Для каждого измерения рекурсивно вызываем print_tensor c пересчитанным offset
            __tensor_values_by_shape(t, rez, dim_idx + 1, new_offset);
        }
    }
}
// Сумма работает только с тензорами одинаковых типов!
tensor* tensor_sum(tensor* t1, tensor* t2, DataType type) {
    if (t1 == NULL || t2 == NULL) return NULL;


    int t1_shape_size = (int)t1->shape->size;
    int t2_shape_size = (int)t1->shape->size;
    if (t1_shape_size != t2_shape_size) return NULL;

    for (int i = 0; i < t1_shape_size; ++i) {
        int t1_shape_i = *(int*)vector_get(t1->shape, i);
        int t2_shape_i = *(int*)vector_get(t2->shape, i);

        if (t1_shape_i != t2_shape_i) return NULL;
    }
    // Если никто не транспонирован, то просто складываем вектора
    if (*(int*)vector_get(t1->strides, t1_shape_size - 1) == 1 && *(int*)vector_get(t2->strides, t2_shape_size - 1) == 1) {
        tensor* t3 = tensor_(vector_sum(t1->data, t2->data, type), vector_copy(t1->shape));
        return t3;
    }

    size_t type_size = NULL;
    if (type == FLOAT) {
        type_size = sizeof(float);
    } else if(type == INT) {
        type_size = sizeof(int);
    } else if(type == DOUBLE) {
        type_size = sizeof(double);
    }
    else {
        return NULL;
    }

    vector* t1_data = vector_empty(t1->data->size, type_size); __tensor_values_by_shape(t1, t1_data, 0, 0);
    vector* t2_data = vector_empty(t1->data->size, type_size); __tensor_values_by_shape(t2, t2_data, 0, 0);

    tensor* t3 = tensor_(vector_sum(t1_data, t2_data, type), vector_copy(t1->shape));

    vector_destroy(t1_data);
    vector_destroy(t2_data);

    return t3;
}