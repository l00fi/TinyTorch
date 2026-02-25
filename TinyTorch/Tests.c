#include "Tests.h"
#include "Vector.h"
#include "Tensor.h"
#include <math.h>
#include <string.h>

// Реализуем состояние здесь
TestState global_state = { 0, 0, 0, {NULL}, {0} };

// --- ТЕСТЫ ---

int vector_emptyCorrect_init() {
    vector* v = vector_empty(5, FLOAT);

    if (v == NULL) {
        vector_destroy(v);
        return 0;
    }
    int result = (v->size == 0) && \
        (v->capacity == 5) && \
        (v->type == FLOAT) && \
        (v->item_size == sizeof(float)) && \
        (v->data != NULL);

    vector_destroy(v);

    return result;
}

int vector_Correct_init() {
    float data[5] = { 1, 2, 3, 4, 5 };
    vector* v = vector_(5, &data, FLOAT);

    if (v == NULL) {
        vector_destroy(v);
        return 0;
    }
    int result = (v->size == 5) && \
        (v->capacity == 5) && \
        (v->type == FLOAT) && \
        (v->item_size == sizeof(float)) && \
        (v->data != NULL);

    vector_destroy(v);

    return result;
}

int vector_Empty_arr() {
    float data[5];
    vector* v = vector_(5, &data, FLOAT);

    if (v == NULL) {
        vector_destroy(v);
        return 0;
    }
    int result = (v->size == 5) && \
        (v->capacity == 5) && \
        (v->type == FLOAT) && \
        (v->item_size == sizeof(float)) && \
        (v->data != NULL);

    vector_destroy(v);

    return result;
}

int vector_getPositive_index() {
    float val_arr[1] = { 42.0f };
    vector* v = vector_(1, &val_arr, FLOAT);

    float retrieved = vfloat(vector_get(v, 0));
    int result = (retrieved == 42.0f);

    vector_destroy(v);

    return result;
}

int vector_getNegative_index() {
    float val_arr[1] = { 42.0f };
    vector* v = vector_(1, &val_arr, FLOAT);

    float retrieved = vfloat(vector_get(v, -1));
    int result = (retrieved == 42.0f);

    vector_destroy(v);

    return result;
}

int vector_appendCorrect_works() {
    vector* v = vector_empty(0, FLOAT);
    float val = 42.0f;
    vector_append(v, &val);

    int result = ((int)v->capacity == 2) && \
        ((int)v->size == 1) && \
        (vfloat(vector_get(v, 0)) == 42.0f);

    vector_destroy(v);

    return result;
}

int vector_editPositive_index() {
    float val_arr[1] = { 42.0f };
    vector* v = vector_(1, &val_arr, FLOAT);
    float new_val = 43.0f;
    vector_edit(v, 0, &new_val);

    int result = (vfloat(vector_get(v, 0)) == 43.0f);

    vector_destroy(v);

    return result;
}

int vector_editNegative_index() {
    float val_arr[1] = { 42.0f };
    vector* v = vector_(1, &val_arr, FLOAT);
    float new_val = 43.0f;
    vector_edit(v, -1, &new_val);

    int result = (vfloat(vector_get(v, 0)) == 43.0f);

    vector_destroy(v);

    return result;
}

int vector_mergeCorrect_works() {
    float val_arr_a[1] = { 1.0f };
    vector* v_a = vector_(1, &val_arr_a, FLOAT);
    float val_arr_b[1] = { 2.0f };
    vector* v_b = vector_(1, &val_arr_b, FLOAT);

    vector* v_c = vector_merge(v_a, v_b);
    if (v_c == NULL) {
        vector_destroy(v_a);
        vector_destroy(v_b);
        return 0;
    }

    int result = (v_c->size == 2) && \
        (v_c->capacity == 2) && \
        (v_c->type == FLOAT) && \
        (v_c->item_size == sizeof(float)) && \
        (v_c->data != NULL);

    vector_destroy(v_a);
    vector_destroy(v_b);
    vector_destroy(v_c);

    return result;
}

int vector_mergeWith_zero_size_vector() {
    vector* v_a = vector_empty(0, FLOAT);
    float val_arr_b[1] = { 2.0f };
    vector* v_b = vector_(1, &val_arr_b, FLOAT);

    vector* v_c = vector_merge(v_a, v_b);
    if (v_c == NULL) {
        vector_destroy(v_a);
        vector_destroy(v_b);
        return 0;
    }

    int result = (v_c->size == 1) && \
        (v_c->capacity == 1) && \
        (v_c->type == FLOAT) && \
        (v_c->item_size == sizeof(float)) && \
        (v_c->data != NULL);

    vector_destroy(v_a);
    vector_destroy(v_b);
    vector_destroy(v_c);

    return result;
}

int vector_mergeWith_zero_size_vectors() {
    vector* v_a = vector_empty(0, FLOAT);
    vector* v_b = vector_empty(0, FLOAT);

    vector* v_c = vector_merge(v_a, v_b);
    if (v_c == NULL) {
        vector_destroy(v_a);
        vector_destroy(v_b);
        return 0;
    }

    int result = (v_c->size == 0) && \
        (v_c->capacity == 0) && \
        (v_c->type == FLOAT)  && \
        (v_c->item_size == sizeof(float)) && \
        (v_c->data != NULL);

    vector_destroy(v_a);
    vector_destroy(v_b);
    vector_destroy(v_c);

    return result;
}

int vector_copyCorrect_works() {
    float val_arr[1] = { 1.0f };
    vector* v = vector_(1, &val_arr, FLOAT);

    vector* v_copy = vector_copy(v);

    if (v_copy == NULL) {
        vector_destroy(v);
        return 0;
    }

    int result = (v_copy->size == 1) && \
        (v_copy->capacity == 1) && \
        (v_copy->type == FLOAT) && \
        (v_copy->item_size == sizeof(float)) && \
        (v_copy->data != NULL) && \
        (v->data != v_copy->data) && \
        // Сравнивает побитово данные, 0 в случае идентичности
        (memcmp(v->data, v_copy->data, sizeof(float)) == 0);

    vector_destroy(v);
    vector_destroy(v_copy);

    return result;
}

int vector_sumCorrect_works_int() {
    int val_arr_a[1] = { 1 };
    vector* v_a = vector_(1, &val_arr_a, INT);
    int val_arr_b[1] = { 2 };
    vector* v_b = vector_(1, &val_arr_b, INT);

    vector* v_c = vector_sum(v_a, v_b);
    if (v_c == NULL) {
        vector_destroy(v_a);
        vector_destroy(v_b);
        return 0;
    }

    int result = (v_c->size == 1) && \
        (v_c->type == INT) && \
        (v_c->item_size == sizeof(int)) && \
        (v_c->capacity == 1) && \
        (v_c->data != NULL) && \
        (*(int*)vector_get(v_c, 0) == 3);

    vector_destroy(v_a);
    vector_destroy(v_b);
    vector_destroy(v_c);

    return result;
}

int vector_sumCorrect_works_float() {
    float val_arr_a[1] = { 1.0f };
    vector* v_a = vector_(1, &val_arr_a, FLOAT);
    float val_arr_b[1] = { 2.0f };
    vector* v_b = vector_(1, &val_arr_b, FLOAT);

    vector* v_c = vector_sum(v_a, v_b);
    if (v_c == NULL) {
        vector_destroy(v_a);
        vector_destroy(v_b);
        return 0;
    }

    int result = (v_c->size == 1) && \
        (v_c->type == FLOAT) && \
        (v_c->item_size == sizeof(float)) && \
        (v_c->capacity == 1) && \
        (v_c->data != NULL) && \
        (*(float*)vector_get(v_c, 0) == 3.0f);

    vector_destroy(v_a);
    vector_destroy(v_b);
    vector_destroy(v_c);

    return result;
}

int vector_sumCorrect_works_double() {
    double val_arr_a[1] = { 1.0f };
    vector* v_a = vector_(1, &val_arr_a, DOUBLE);
    double val_arr_b[1] = { 2.0f };
    vector* v_b = vector_(1, &val_arr_b, DOUBLE);

    vector* v_c = vector_sum(v_a, v_b);
    if (v_c == NULL) {
        vector_destroy(v_a);
        vector_destroy(v_b);
        return 0;
    }

    int result = (v_c->size == 1) && \
        (v_c->type == DOUBLE) && \
        (v_c->item_size == sizeof(double)) && \
        (v_c->capacity == 1) && \
        (v_c->data != NULL) && \
        (*(double*)vector_get(v_c, 0) == 3.0f);

    vector_destroy(v_a);
    vector_destroy(v_b);
    vector_destroy(v_c);

    return result;
}

int tensor_Correct_init() {
    float val_arr[1] = { 1.0f };
    int shape_arr[1] = { 1 };
    vector* val = vector_(1, &val_arr, FLOAT);
    vector* shape = vector_(1, &shape_arr, INT);
    tensor* t = tensor_(val, shape);

    if (t == NULL) {
        vector_destroy(val);
        vector_destroy(shape);
        return 0;
    }

    int result = (t->data != NULL) && \
        (t->shape != NULL) && \
        (t->strides != NULL) && \
        (t->type == FLOAT) && \
        (vfloat(vector_get(t->data, 0)) == 1.0f) && \
        (vint(vector_get(t->shape, 0)) == 1) && \
        (vint(vector_get(t->strides, 0)) == 1);

    vector_destroy(val);
    vector_destroy(shape);
    tensor_destroy(t);

    return result;
}

int tensor_getCorrect_works() {
    float val_arr[1] = { 1.0f };
    int shape_arr[1] = { 1 };
    vector* val = vector_(1, &val_arr, FLOAT);
    vector* shape = vector_(1, &shape_arr, INT);
    tensor* t = tensor_(val, shape);

    int idx_arr[1] = { 0 };
    vector* idx = vector_(1, &idx_arr, INT);

    void* result_val = tensor_get(t, idx);
    if (result_val == NULL) {
        vector_destroy(val);
        vector_destroy(shape);
        vector_destroy(idx);
        tensor_destroy(t);
        return 0;
    }

    int result = (vfloat(result_val) == 1.0f);

    vector_destroy(val);
    vector_destroy(shape);
    vector_destroy(idx);
    tensor_destroy(t);

    return result;
}

int tensor_sumCorrect_works_int() {
    int val_arr[1] = { 1 };
    int shape_arr[1] = { 1 };
    vector* val = vector_(1, &val_arr, INT);
    vector* shape = vector_(1, &shape_arr, INT);
    tensor* t1 = tensor_(val, shape);
    tensor* t2 = tensor_(val, shape);

    int idx_arr[1] = { 0 };
    vector* idx = vector_(1, &idx_arr, INT);

    tensor* t3 = tensor_sum(t1, t2);
    if (t3 == NULL) {
        vector_destroy(val);
        vector_destroy(shape);
        vector_destroy(idx);
        tensor_destroy(t1);
        tensor_destroy(t2);
        tensor_destroy(t3);
        return 0;
    }

    int result = (t3->data != NULL) && \
        (t3->shape != NULL) && \
        (t3->strides != NULL) && \
        (t3->type == INT) && \
        (*(int*)vector_get(t3->data, 0) == 2) && \
        (*(int*)vector_get(t3->shape, 0) == 1) && \
        (*(int*)vector_get(t3->strides, 0) == 1);

    vector_destroy(val);
    vector_destroy(shape);
    vector_destroy(idx);
    tensor_destroy(t1);
    tensor_destroy(t2);
    tensor_destroy(t3);

    return result;
}

int tensor_sumCorrect_works_float() {
    float val_arr[1] = { 1.0f };
    int shape_arr[1] = { 1 };
    vector* val = vector_(1, &val_arr, FLOAT);
    vector* shape = vector_(1, &shape_arr, INT);
    tensor* t1 = tensor_(val, shape);
    tensor* t2 = tensor_(val, shape);

    int idx_arr[1] = { 0 };
    vector* idx = vector_(1, &idx_arr, INT);

    tensor* t3 = tensor_sum(t1, t2);
    if (t3 == NULL) {
        vector_destroy(val);
        vector_destroy(shape);
        vector_destroy(idx);
        tensor_destroy(t1);
        tensor_destroy(t2);
        tensor_destroy(t3);
        return 0;
    }

    int result = (t3->data != NULL) && \
        (t3->shape != NULL) && \
        (t3->strides != NULL) && \
        (t3->type == FLOAT) && \
        (*(float*)vector_get(t3->data, 0) == 2.0f) && \
        (*(int*)vector_get(t3->shape, 0) == 1) && \
        (*(int*)vector_get(t3->strides, 0) == 1);

    vector_destroy(val);
    vector_destroy(shape);
    vector_destroy(idx);
    tensor_destroy(t1);
    tensor_destroy(t2);
    tensor_destroy(t3);

    return result;
}

int tensor_sumCorrect_works_double() {
    double val_arr[1] = { 1.0f };
    int shape_arr[1] = { 1 };
    vector* val = vector_(1, &val_arr, DOUBLE);
    vector* shape = vector_(1, &shape_arr, INT);
    tensor* t1 = tensor_(val, shape);
    tensor* t2 = tensor_(val, shape);

    int idx_arr[1] = { 0 };
    vector* idx = vector_(1, &idx_arr, INT);

    tensor* t3 = tensor_sum(t1, t2);
    if (t3 == NULL) {
        vector_destroy(val);
        vector_destroy(shape);
        vector_destroy(idx);
        tensor_destroy(t1);
        tensor_destroy(t2);
        tensor_destroy(t3);
        return 0;
    }

    int result = (t3->data != NULL) && \
        (t3->shape != NULL) && \
        (t3->strides != NULL) && \
        (t3->type == DOUBLE) && \
        (*(double*)vector_get(t3->data, 0) == 2) && \
        (*(int*)vector_get(t3->shape, 0) == 1) && \
        (*(int*)vector_get(t3->strides, 0) == 1);

    vector_destroy(val);
    vector_destroy(shape);
    vector_destroy(idx);
    tensor_destroy(t1);
    tensor_destroy(t2);
    tensor_destroy(t3);

    return result;
}

int tensor_TCorrect_works() {
    int val_arr[4] = { 1, 2, 3, 4 };
    int shape_arr[2] = { 2, 2 };
    vector* val = vector_(4, &val_arr, INT);
    vector* shape = vector_(2, &shape_arr, INT);
    tensor* t = tensor_(val, shape);

    tensor_T(t);
    
    int idx_arr[2] = {0, 1};
    vector* idx = vector_(2, &idx_arr, INT);

    int result = (t->strides != NULL) && \
        (vint(vector_get(t->strides, 0)) == 1) && \
        (t->data != NULL) && \
        (*(int*)tensor_get(t, idx) == 3);

    tensor_destroy(t);
    vector_destroy(val);
    vector_destroy(shape);
    vector_destroy(idx);

    return result;
}

int tensor_sumTTensor() {
    int val_arr[4] = { 1, 2, 3, 4 };
    int shape_arr[2] = { 2, 2 };
    vector* val = vector_(4, &val_arr, INT);
    vector* shape = vector_(2, &shape_arr, INT);
    tensor* t1 = tensor_(val, shape);
    tensor* t2 = tensor_(val, shape);
    tensor_T(t2);

    int idx_arr[2] = { 0, 1 };
    vector* idx = vector_(2, &idx_arr, INT);

    tensor* t3 = tensor_sum(t1, t2);
    if (t3 == NULL) {
        vector_destroy(val);
        vector_destroy(shape);
        vector_destroy(idx);
        tensor_destroy(t1);
        tensor_destroy(t2);
        return 0;
    }

    int result = (t3->data != NULL) && \
        (t3->shape != NULL) && \
        (t3->strides != NULL) && \
        (t3->type == INT) && \
        (vint(tensor_get(t3, idx)) == 5) && \
        (vint(vector_get(t3->shape, 0)) == 2) && \
        (vint(vector_get(t3->strides, 0)) == 2);

    vector_destroy(val);
    vector_destroy(shape);
    vector_destroy(idx);
    tensor_destroy(t1);
    tensor_destroy(t2);
    tensor_destroy(t3);

    return result;
}

int tensor_scalar_multCorrect_works() {
    int val_arr[4] = { 1, 2, 3, 4 };
    int shape_arr[2] = { 2, 2 };
    vector* val = vector_(4, &val_arr, INT);
    vector* shape = vector_(2, &shape_arr, INT);
    tensor* t = tensor_(val, shape);

    int scalar = 2;
    tensor* t_result = tensor_scalar_mult(t, &scalar);

    if (t_result == NULL) {
        vector_destroy(val);
        vector_destroy(shape);
        tensor_destroy(t);
        return 0;
    }

    int result_empty_data = (t_result->data != NULL) && \
        (t_result->shape != NULL) && \
        (t_result->strides != NULL) && \
        (t_result->type == INT);

    int total_sum = 0;
    for (int i = 0; i < (int)t_result->data->size; ++i) {
        total_sum += vint(vector_get(t_result->data, i));
    }

    int result = result_empty_data && (total_sum == 20);

    vector_destroy(val);
    vector_destroy(shape);
    tensor_destroy(t);
    tensor_destroy(t_result);

    return result;
}

int tensor_hadamard_multCorrect_works() {
    int val_arr[4] = { 1, 2, 3, 4 };
    int shape_arr[2] = { 2, 2 };
    vector* val = vector_(4, &val_arr, INT);
    vector* shape = vector_(2, &shape_arr, INT);
    tensor* t1 = tensor_(val, shape);
    tensor* t2 = tensor_(val, shape);

    tensor* t_result = tensor_hadamard_mult(t1, t2);

    if (t_result == NULL) {
        tensor_destroy(t1);
        tensor_destroy(t2);
        vector_destroy(val);
        vector_destroy(shape);
        return 0;
    }

    int result_empty_data = (t_result->data != NULL) && \
        (t_result->shape != NULL) && \
        (t_result->strides != NULL) && \
        (t_result->type == INT);

    int total_sum = 0;
    for (int i = 0; i < (int)t_result->data->size; ++i) {
        total_sum += vint(vector_get(t_result->data, i));
    }

    int result = result_empty_data && (total_sum == 30);

    tensor_destroy(t1);
    tensor_destroy(t2);
    tensor_destroy(t_result);
    vector_destroy(val);
    vector_destroy(shape);

    return result;
}

// --- ОТЧЕТ ---

void print_test_report() {
    printf("\n==============================\n");
    printf("       TEST SUMMARY\n");
    printf("==============================\n");
    printf("Total:  %d\n", global_state.total_count);
    printf("Passed: %d\n", global_state.passed_count);
    printf("Failed: %d\n", global_state.failed_count);

    if (global_state.failed_count > 0) {
        printf("\nDetailed failures:\n");
        for (int i = 0; i < global_state.failed_count; i++) {
            printf("- %s\n", global_state.failed_names[i]);
        }
    }
    printf("==============================\n");
}