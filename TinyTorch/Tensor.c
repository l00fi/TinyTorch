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

    t->data = vector_copy(data);
    t->shape = vector_copy(shape);
    t->type = data->type;

    vector* strides = vector_empty(rank, INT);

    for (size_t i = 0; i < rank; i++) {
        int val = 0;
        vector_append(strides, &val);
    }

    int val = 1;
    vector_edit(strides, -1, &val);
    for (int i = 2; i <= (int)rank; i++) {
        int new_stride = vint(vector_get(shape, -(i - 1))) * vint(vector_get(strides, -(i - 1)));
        vector_edit(strides, -i, &new_stride);
    }

    t->strides = strides;
    return t;
}

tensor* tensor_copy(tensor* t) {
    if (t == NULL) return NULL;

    tensor* new_t = tensor_(t->data, t->shape);

    return new_t;
}

void tensor_destroy(tensor* t) {
    if (t == NULL) return;

    if (t->data != NULL) {
        vector_destroy(t->data);
    }
    if (t->shape != NULL) {
        vector_destroy(t->shape);
    }
    if (t->strides != NULL) {
        vector_destroy(t->strides);
    }

    free(t);
}


void* tensor_get(tensor* t, vector* idx) {
    if (t == NULL || t->data == NULL) return NULL;

    if (idx == NULL || idx->data == NULL) return NULL;

    if (t->shape->size != idx->size) return;
    //Проверка на корректность индексов
    for (int i = 0; i < t->shape->size; ++i) {
        if (vint(vector_get(idx, i)) + 1 > vint(vector_get(t->shape, i)) || vint(vector_get(idx, i)) < 0) return;
    }

    int true_index = 0;
    for (int i = 0; i < t->strides->size; ++i) {
        true_index += vint(vector_get(t->strides, i)) * vint(vector_get(idx, i));
    }

    return vector_get(t->data, true_index);
}

// Переношу значение тензора согласно shape
void __tensor_values_by_shape(tensor* t, vector* rez, int dim_idx, int offset) {
    if (t == NULL) {
        printf("[NULL]\n");
        return;
    }

    int shape_size = t->shape->size;
    // Базовый случай, когда доходим до shape[-1]
    if (dim_idx == shape_size - 1) {
        int last = vint(vector_get(t->shape, dim_idx));
        for (int i = 0; i < last; ++i) {
            int current_stride = vint(vector_get(t->strides, dim_idx));
            int true_index = offset + (i * current_stride);
            vector_append(rez, vector_get(t->data, true_index));
        }
    }
    else {
        int shape_val = vint(vector_get(t->shape, dim_idx));
        for (int i = 0; i < shape_val; ++i) {
            int current_stride = vint(vector_get(t->strides, dim_idx));
            int new_offset = offset + (i * current_stride);
            // Для каждого измерения рекурсивно вызываем print_tensor c пересчитанным offset
            __tensor_values_by_shape(t, rez, dim_idx + 1, new_offset);
        }
    }
}

// Сумма работает только с тензорами одинаковых типов!
tensor* tensor_sum(tensor* t1, tensor* t2) {
    if (t1 == NULL || t2 == NULL) return NULL;
    if (t1->type != t2->type) return NULL;

    int t1_shape_size = (int)t1->shape->size;
    int t2_shape_size = (int)t1->shape->size;
    if (t1_shape_size != t2_shape_size) return NULL;

    for (int i = 0; i < t1_shape_size; ++i) {
        int t1_shape_i = vint(vector_get(t1->shape, i));
        int t2_shape_i = vint(vector_get(t2->shape, i));

        if (t1_shape_i != t2_shape_i) return NULL;
    }
    // Если никто не транспонирован, то просто складываем вектора
    if (vint(vector_get(t1->strides, t1_shape_size - 1)) == 1 && vint(vector_get(t2->strides, t2_shape_size - 1)) == 1) {
        tensor* t3 = tensor_(vector_sum(t1->data, t2->data), vector_copy(t1->shape));
        return t3;
    }

    vector* t1_data = vector_empty(t1->data->size, t1->type); __tensor_values_by_shape(t1, t1_data, 0, 0);
    vector* t2_data = vector_empty(t2->data->size, t2->type); __tensor_values_by_shape(t2, t2_data, 0, 0);

    tensor* t3 = tensor_(vector_sum(t1_data, t2_data), vector_copy(t1->shape));

    vector_destroy(t1_data);
    vector_destroy(t2_data);

    return t3;
}

tensor* tensor_scalar_mult(tensor* t, void* scalar) {
    if (t == NULL || scalar == NULL) return NULL;

    tensor* scalar_t = tensor_copy(t);

    int size = (int)scalar_t->data->size;
    for (int i = 0; i < size; ++i) {
        if (t->type == INT) {
            int val = vint(scalar) * vint(vector_get(scalar_t->data, i));
            vector_edit(scalar_t->data, i, &val);
        }
        else if (t->type == FLOAT) {
            float val = vfloat(scalar) * vfloat(vector_get(scalar_t->data, i));
            vector_edit(scalar_t->data, i, &val);
        }
        else if (t->type == DOUBLE) {
            double val = vdouble(scalar) * vdouble(vector_get(scalar_t->data, i));
            vector_edit(scalar_t->data, i, &val);
        }
        else {
            return NULL;
        }
    }

    return scalar_t;
}

tensor* tensor_hadamard_mult(tensor* t1, tensor* t2) {
    if (t1 == NULL || t2 == NULL) return NULL;
    if (t1->type != t2->type) return NULL;

    int t1_shape_size = (int)t1->shape->size;
    int t2_shape_size = (int)t1->shape->size;
    if (t1_shape_size != t2_shape_size) return NULL;

    for (int i = 0; i < t1_shape_size; ++i) {
        int t1_shape_i = vint(vector_get(t1->shape, i));
        int t2_shape_i = vint(vector_get(t2->shape, i));

        if (t1_shape_i != t2_shape_i) return NULL;
    }
    
    vector* t1_data = vector_copy(t1->data);
    vector* t2_data = vector_copy(t1->data);

    // Если кто-то транспонирован, то достаём достоверные вектора
    if (vint(vector_get(t1->strides, t1_shape_size - 1)) != 1) {
        t1_data = vector_empty(t1->data->size, t1->type); __tensor_values_by_shape(t1, t1_data, 0, 0);
    }

    if (vint(vector_get(t2->strides, t2_shape_size - 1)) != 1) {
        t2_data = vector_empty(t1->data->size, t1->type); __tensor_values_by_shape(t2, t2_data, 0, 0);
    }

    int t1_data_size = (int)t1->data->size;

    vector* t3_data = vector_empty(0, t1->type);
    for (int i = 0; i < t1_data_size; ++i) {
        if (t1->type == FLOAT) {
            float val = vfloat(vector_get(t1_data, i)) * vfloat(vector_get(t2_data, i));
            vector_append(t3_data, &val);
        }
        if (t1->type == INT) {
            int val = vint(vector_get(t1_data, i)) * vint(vector_get(t2_data, i));
            vector_append(t3_data, &val);
        }
        if (t1->type == DOUBLE) {
            double val = vdouble(vector_get(t1_data, i)) * vdouble(vector_get(t2_data, i));
            vector_append(t3_data, &val);
        }
    }

    tensor* t3 = tensor_(t3_data, t1->shape);

    vector_destroy(t1_data);
    vector_destroy(t2_data);
    vector_destroy(t3_data);

    return t3;
}

tensor* tensor_mult_2d(tensor* t1, tensor* t2, DataType type) {
    if (t1 == NULL || t2 == NULL) return NULL;
    if (t1->type != t2->type) return NULL;

    if (vint(vector_get(t1->shape, -1)) != vint(vector_get(t2->shape, 0))) return NULL;
    
    return 0;
}

tensor* tensor_mult(tensor* t1, tensor* t2, DataType type) {
    return 0;
}

void tensor_T(tensor* t) {
    if (t == NULL) return;

    int rank = (int)t->strides->size;
    size_t item_size = t->strides->item_size;
    // Выделяю память для копии элемента массива
    void* temp = malloc(item_size);
    if (temp == NULL) return;

    int i = 0;
    int j = rank - 1;
    while (i < j)
    {
        memcpy(temp, vector_get(t->strides, i), item_size);
        vector_edit(t->strides, i, vector_get(t->strides, j));
        vector_edit(t->strides, j, temp);
        --j; ++i;
    }

    free(temp);
}

void tensor_print(tensor* t, int dim_idx, int offset) {
    if (t == NULL) {
        printf("[NULL]\n");
        return;
    }

    int shape_size = t->shape->size;
    // Базовый случай, когда доходим до shape[-1]
    if (dim_idx == shape_size - 1) {
        int last = vint(vector_get(t->shape, dim_idx));
        printf("[");
        for (int i = 0; i < last; ++i) {
            int current_stride = vint(vector_get(t->strides, dim_idx));
            int true_index = offset + (i * current_stride);

            if (t->type == FLOAT) {
                printf("%.2f", vfloat(vector_get(t->data, true_index)));
            }

            if(t->type == DOUBLE) {
                printf("%.4f", vdouble(vector_get(t->data, true_index)));
            }

            if(t->type == INT) {
                printf("%d", vint(vector_get(t->data, true_index)));
            }

            if (i < last - 1) printf(", ");
        }
        printf("]");
    }
    else {
        printf("[");
        int shape_val = vint(vector_get(t->shape, dim_idx));
        for (int i = 0; i < shape_val; ++i) {
            int current_stride = vint(vector_get(t->strides, dim_idx));
            int new_offset = offset + (i * current_stride);
            if (i > 0) printf("%*c", dim_idx + 1, ' ');
            // Для каждого измерения рекурсивно вызываем print_tensor c пересчитанным offset
            tensor_print(t, dim_idx + 1, new_offset);
            if (i < shape_val - 1) printf(",\n");
        }
        printf("]");
    }
}