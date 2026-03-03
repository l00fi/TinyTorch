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

    if (t->shape->size != idx->size) return NULL;
    //Проверка на корректность индексов
    for (int i = 0; i < t->shape->size; ++i) {
        if (vint(vector_get(idx, i)) + 1 > vint(vector_get(t->shape, i)) || vint(vector_get(idx, i)) < 0) return NULL;
    }

    int true_index = 0;
    for (int i = 0; i < t->strides->size; ++i) {
        true_index += vint(vector_get(t->strides, i)) * vint(vector_get(idx, i));
    }

    return vector_get(t->data, true_index);
}

void tensor_edit(tensor* t, vector* idx, void* val) {
    if (t == NULL || t->data == NULL) return NULL;

    if (idx == NULL || idx->data == NULL) return NULL;

    if (t->shape->size != idx->size) return NULL;
    //Проверка на корректность индексов
    for (int i = 0; i < t->shape->size; ++i) {
        if (vint(vector_get(idx, i)) + 1 > vint(vector_get(t->shape, i)) || vint(vector_get(idx, i)) < 0) return NULL;
    }

    int true_index = 0;
    for (int i = 0; i < t->strides->size; ++i) {
        true_index += vint(vector_get(t->strides, i)) * vint(vector_get(idx, i));
    }

    vector_edit(t->data, true_index, val);
}

// Переношу значение тензора согласно shape
static void __tensor_values_by_shape(tensor* t, vector* rez, int dim_idx, int offset) {
    if (t == NULL) {
        printf("[NULL]\n");
        return;
    }

    int shape_size = (int)t->shape->size;
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

// Тоже вайбкодик, точнее вайбкод над моей реализацией перемножения матриц
static void tensor_mult_2d(tensor* t1, tensor* t2, tensor* t3,
    int off1, int off2, int off3) {

    // 1. Предварительно вытаскиваем размеры и страйды (ОПТИМИЗАЦИЯ)
    // Нам нужны страйды последних двух измерений (-2 и -1)
    int rank = (int)t1->shape->size;
    
    int I = vint(vector_get(t1->shape, rank - 2));
    int K = vint(vector_get(t1->shape, rank - 1));
    int J = vint(vector_get(t2->shape, rank - 1));
    
    int s1_i = vint(vector_get(t1->strides, rank - 2));
    int s1_k = vint(vector_get(t1->strides, rank - 1));
    
    int s2_k = vint(vector_get(t2->strides, rank - 2));
    int s2_j = vint(vector_get(t2->strides, rank - 1));
    
    int s3_i = vint(vector_get(t3->strides, rank - 2));
    int s3_j = vint(vector_get(t3->strides, rank - 1));
    
    // 2. Тройной цикл (i, k, j)
    for (int i = 0; i < I; ++i) {
        for (int k = 0; k < K; ++k) {
    
            // Находим адрес элемента A[i][k] с учетом глобального оффсета
            int idx1 = off1 + (i * s1_i + k * s1_k);
            void* val_a_ptr = vector_get(t1->data, idx1);
    
            for (int j = 0; j < J; ++j) {
                // Находим адрес элемента B[k][j]
                int idx2 = off2 + (k * s2_k + j * s2_j);
                // Находим адрес элемента Res[i][j]
                int idx3 = off3 + (i * s3_i + j * s3_j);
    
                if (t1->type == FLOAT) {
                    float a = vfloat(val_a_ptr);
                    float b = vfloat(vector_get(t2->data, idx2));
    
                    // Читаем старое значение, прибавляем, пишем обратно
                    float current = vfloat(vector_get(t3->data, idx3));
                    float updated = current + (a * b);
                    vector_edit(t3->data, idx3, &updated);
                }
                else if (t1->type == INT) {
                    int a = vint(val_a_ptr);
                    int b = vint(vector_get(t2->data, idx2));
    
                    // Читаем старое значение, прибавляем, пишем обратно
                    int current = vint(vector_get(t3->data, idx3));
                    int updated = current + (a * b);
                    vector_edit(t3->data, idx3, &updated);
                }
                else if (t1->type == DOUBLE) {
                    double a = vdouble(val_a_ptr);
                    double b = vdouble(vector_get(t2->data, idx2));
    
                    // Читаем старое значение, прибавляем, пишем обратно
                    double current = vdouble(vector_get(t3->data, idx3));
                    double updated = current + (a * b);
                    vector_edit(t3->data, idx3, &updated);
                }
            }
        }
    }
}

// Вайбкодик, нужно поразбираться
static void __matmul_rec(tensor* A, tensor* B, tensor* Res,
    int dim_idx, int offA, int offB, int offRes) {

    int rank = A->shape->size;

    // БАЗОВЫЙ СЛУЧАЙ: Мы дошли до последних двух измерений (матриц)
    if (dim_idx == rank - 2) {
        tensor_mult_2d(A, B, Res, offA, offB, offRes);
        return;
    }

    // РЕКУРСИВНЫЙ ШАГ: Идем по текущему батч-измерению
    int current_dim_size = vint(vector_get(A->shape, dim_idx));

    for (int i = 0; i < current_dim_size; i++) {
        // Считаем новые смещения для следующего уровня
        int next_offA = offA + (i * vint(vector_get(A->strides, dim_idx)));
        int next_offB = offB + (i * vint(vector_get(B->strides, dim_idx)));
        int next_offRes = offRes + (i * vint(vector_get(Res->strides, dim_idx)));

        // Уходим глубже
        __matmul_rec(A, B, Res, dim_idx + 1, next_offA, next_offB, next_offRes);
    }
}

// И это вайбкод
tensor* tensor_mult(tensor* t1, tensor* t2) {
    if (t1 == NULL || t2 == NULL || t1->type != t2->type) return NULL;

    size_t rank = t1->shape->size;

    // 1. Проверка рангов (должны совпадать для Batch MatMul)
    if (rank != t2->shape->size) {
        printf("Error: Tensors must have the same rank for MatMul\n");
        return NULL;
    }

    // 2. Проверка совместимости батч-измерений (все, кроме последних двух)
    for (int i = 0; i < (int)rank - 2; i++) {
        if (vint(vector_get(t1->shape, i)) != vint(vector_get(t2->shape, i))) {
            printf("Error: Batch dimensions must match at index %d\n", i);
            return NULL;
        }
    }

    // 3. Проверка матричных измерений (K-измерение)
    // t1: [..., M, K], t2: [..., K, N]
    int K1 = vint(vector_get(t1->shape, -1));
    int K2 = vint(vector_get(t2->shape, -2));
    if (K1 != K2) {
        printf("Error: Matrix dimensions mismatch: %d != %d\n", K1, K2);
        return NULL;
    }

    // 4. Формируем форму результирующего тензора
    // Она будет [Batch..., M, N]
    vector* res_shape = vector_copy(t1->shape);
    int N_val = vint(vector_get(t2->shape, -1));
    vector_edit(res_shape, -1, &N_val); // Меняем K на N

    // 5. Создаем данные для результата, заполненные нулями
    // Нам нужны нули, потому что в internal_2d мы делаем += (аккумуляцию)
    size_t total_elements = 1;
    for (size_t i = 0; i < res_shape->size; i++) {
        total_elements *= vint(vector_get(res_shape, (int)i));
    }

    vector* res_data = vector_empty(total_elements, t1->type);
    if (t1->type == FLOAT) {
        float zero = 0.0f;
        for (size_t i = 0; i < total_elements; i++) vector_append(res_data, &zero);
    }
    else if (t1->type == INT) {
        int zero = 0;
        for (size_t i = 0; i < total_elements; i++) vector_append(res_data, &zero);
    }
    else if (t1->type == DOUBLE) {
        double zero = 0.0f;
        for (size_t i = 0; i < total_elements; i++) vector_append(res_data, &zero);
    }
    tensor* res = tensor_(res_data, res_shape);

    // 6. ЗАПУСК РЕКУРСИИ
    // Начинаем с 0-го измерения и нулевых сдвигов
    __matmul_rec(t1, t2, res, 0, 0, 0, 0);

    return res;
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

    int shape_size = (int)t->shape->size;
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