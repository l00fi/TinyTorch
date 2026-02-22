#include "Tests.h"
#include "Vector.h"
#include "Tensor.h"
#include <math.h>
#include <string.h>

// Реализуем состояние здесь
TestState global_state = { 0, 0, 0, {NULL}, {0} };

// --- ТЕСТЫ ---

int vector_emptyCorrect_init() {
    vector* v = vector_empty(5, sizeof(float));

    if (v == NULL) {
        vector_destroy(v);
        return 0;
    }
    int result = (v->size == 0) && \
        (v->capacity == 5) && \
        (v->item_size == sizeof(float)) && \
        (v->data != NULL);

    vector_destroy(v);

    return result;
}

int vector_Correct_init() {
    float data[5] = { 1, 2, 3, 4, 5 };
    vector* v = vector_(5, data, sizeof(float));

    if (v == NULL) {
        vector_destroy(v);
        return 0;
    }
    int result = (v->size == 5) && \
        (v->capacity == 5) && \
        (v->item_size == sizeof(float)) && \
        (v->data != NULL);

    vector_destroy(v);

    return result;
}

int vector_Empty_arr() {
    float data[5];
    vector* v = vector_(5, data, sizeof(float));

    if (v == NULL) {
        vector_destroy(v);
        return 0;
    }
    int result = (v->size == 5) && \
        (v->capacity == 5) && \
        (v->item_size == sizeof(float)) && \
        (v->data != NULL);

    vector_destroy(v);

    return result;
}

int vector_getPositive_index() {
    float val_arr[1] = { 42.0f };
    vector* v = vector_(1, val_arr, sizeof(float));

    float retrieved = *(float*)vector_get(v, 0);
    int result = (retrieved == 42.0f);

    vector_destroy(v);

    return result;
}

int vector_getNegative_index() {
    float val_arr[1] = { 42.0f };
    vector* v = vector_(1, val_arr, sizeof(float));

    float retrieved = *(float*)vector_get(v, -1);
    int result = (retrieved == 42.0f);

    vector_destroy(v);

    return result;
}

int vector_appendCorrect_works() {
    vector* v = vector_empty(0, sizeof(float));
    float val = 42.0f;
    vector_append(v, &val);

    int result = ((int)v->capacity == 2) && \
        ((int)v->size == 1) && \
        (*(float*)vector_get(v, 0) == 42.0f);

    vector_destroy(v);

    return result;
}

int vector_editPositive_index() {
    float val_arr[1] = { 42.0f };
    vector* v = vector_(1, val_arr, sizeof(float));
    float new_val = 43.0f;
    vector_edit(v, 0, &new_val);

    int result = (*(float*)vector_get(v, 0) == 43.0f);

    vector_destroy(v);

    return result;
}

int vector_editNegative_index() {
    float val_arr[1] = { 42.0f };
    vector* v = vector_(1, val_arr, sizeof(float));
    float new_val = 43.0f;
    vector_edit(v, -1, &new_val);

    int result = (*(float*)vector_get(v, 0) == 43.0f);

    vector_destroy(v);

    return result;
}

int vector_mergeCorrect_works() {
    float val_arr_a[1] = { 1.0f };
    vector* v_a = vector_(1, val_arr_a, sizeof(float));
    float val_arr_b[1] = { 2.0f };
    vector* v_b = vector_(1, val_arr_b, sizeof(float));

    vector* v_c = vector_merge(v_a, v_b);
    if (v_c == NULL) {
        vector_destroy(v_a);
        vector_destroy(v_b);
        return 0;
    }

    int result = (v_c->size == 2) && \
        (v_c->capacity == 2) && \
        (v_c->item_size == sizeof(float)) && \
        (v_c->data != NULL);

    vector_destroy(v_a);
    vector_destroy(v_b);
    vector_destroy(v_c);

    return result;
}

int vector_mergeWith_zero_size_vector() {
    vector* v_a = vector_empty(0, sizeof(float));
    float val_arr_b[1] = { 2.0f };
    vector* v_b = vector_(1, val_arr_b, sizeof(float));

    vector* v_c = vector_merge(v_a, v_b);
    if (v_c == NULL) {
        vector_destroy(v_a);
        vector_destroy(v_b);
        return 0;
    }

    int result = (v_c->size == 1) && \
        (v_c->capacity == 1) && \
        (v_c->item_size == sizeof(float)) && \
        (v_c->data != NULL);

    vector_destroy(v_a);
    vector_destroy(v_b);
    vector_destroy(v_c);

    return result;
}

int vector_mergeWith_zero_size_vectors() {
    vector* v_a = vector_empty(0, sizeof(float));
    vector* v_b = vector_empty(0, sizeof(float));

    vector* v_c = vector_merge(v_a, v_b);
    if (v_c == NULL) {
        vector_destroy(v_a);
        vector_destroy(v_b);
        return 0;
    }

    int result = (v_c->size == 0) && \
        (v_c->capacity == 0) && \
        (v_c->item_size == sizeof(float)) && \
        (v_c->data != NULL);

    vector_destroy(v_a);
    vector_destroy(v_b);
    vector_destroy(v_c);

    return result;
}

int vector_copyCorrect_works() {
    float val_arr[1] = { 1.0f };
    vector* v = vector_(1, val_arr, sizeof(float));

    vector* v_copy = vector_copy(v);

    if (v_copy == NULL) {
        vector_destroy(v);
        return 0;
    }

    int result = (v_copy->size == 1) && \
        (v_copy->capacity == 1) && \
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
    vector* v_a = vector_(1, val_arr_a, sizeof(int));
    int val_arr_b[1] = { 2 };
    vector* v_b = vector_(1, val_arr_b, sizeof(int));

    vector* v_c = vector_sum(v_a, v_b, INT);
    if (v_c == NULL) {
        vector_destroy(v_a);
        vector_destroy(v_b);
        return 0;
    }

    int result = (v_c->size == 1) && \
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
    vector* v_a = vector_(1, val_arr_a, sizeof(float));
    float val_arr_b[1] = { 2.0f };
    vector* v_b = vector_(1, val_arr_b, sizeof(float));

    vector* v_c = vector_sum(v_a, v_b, FLOAT);
    if (v_c == NULL) {
        vector_destroy(v_a);
        vector_destroy(v_b);
        return 0;
    }

    int result = (v_c->size == 1) && \
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
    vector* v_a = vector_(1, val_arr_a, sizeof(double));
    double val_arr_b[1] = { 2.0f };
    vector* v_b = vector_(1, val_arr_b, sizeof(double));

    vector* v_c = vector_sum(v_a, v_b, DOUBLE);
    if (v_c == NULL) {
        vector_destroy(v_a);
        vector_destroy(v_b);
        return 0;
    }

    int result = (v_c->size == 1) && \
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
    vector* val = vector_(1, val_arr, sizeof(float));
    vector* shape = vector_(1, shape_arr, sizeof(int));
    tensor* t = tensor_(val, shape);

    if (t == NULL) {
        vector_destroy(val);
        vector_destroy(shape);
        return 0;
    }

    int result = (t->data != NULL) && \
        (t->shape != NULL) && \
        (t->strides != NULL) && \
        (*(float*)vector_get(t->data, 0) == 1.0f) && \
        (*(int*)vector_get(t->shape, 0) == 1) && \
        (*(int*)vector_get(t->strides, 0) == 1);

    vector_destroy(val);
    vector_destroy(shape);
    tensor_destroy(t);

    return result;
}

int tensor_getCorrect_works() {
    float val_arr[1] = { 1.0f };
    int shape_arr[1] = { 1 };
    vector* val = vector_(1, val_arr, sizeof(float));
    vector* shape = vector_(1, shape_arr, sizeof(int));
    tensor* t = tensor_(val, shape);

    int idx_arr[1] = { 0 };
    vector* idx = vector_(1, idx_arr, sizeof(int));

    void* result_val = tensor_get(t, idx);
    if (result_val == NULL) {
        vector_destroy(val);
        vector_destroy(shape);
        vector_destroy(idx);
        tensor_destroy(t);
        return 0;
    }

    int result = (*(float*)result_val == 1.0f);

    return result;
}

int tensor_sumCorrect_works_int() {
    int val_arr[1] = { 1 };
    int shape_arr[1] = { 1 };
    vector* val = vector_(1, val_arr, sizeof(int));
    vector* shape = vector_(1, shape_arr, sizeof(int));
    tensor* t1 = tensor_(val, shape);
    tensor* t2 = tensor_(val, shape);

    int idx_arr[1] = { 0 };
    vector* idx = vector_(1, idx_arr, sizeof(int));

    tensor* t3 = tensor_sum(t1, t2, INT);
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
        (*(int*)vector_get(t3->data, 0) == 2) && \
        (*(int*)vector_get(t3->shape, 0) == 1) && \
        (*(int*)vector_get(t3->strides, 0) == 1);

    return result;
}

int tensor_sumCorrect_works_float() {
    float val_arr[1] = { 1.0f };
    int shape_arr[1] = { 1 };
    vector* val = vector_(1, val_arr, sizeof(float));
    vector* shape = vector_(1, shape_arr, sizeof(int));
    tensor* t1 = tensor_(val, shape);
    tensor* t2 = tensor_(val, shape);

    int idx_arr[1] = { 0 };
    vector* idx = vector_(1, idx_arr, sizeof(int));

    tensor* t3 = tensor_sum(t1, t2, FLOAT);
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
        (*(float*)vector_get(t3->data, 0) == 2.0f) && \
        (*(int*)vector_get(t3->shape, 0) == 1) && \
        (*(int*)vector_get(t3->strides, 0) == 1);

    return result;
}

int tensor_sumCorrect_works_double() {
    double val_arr[1] = { 1.0f };
    int shape_arr[1] = { 1 };
    vector* val = vector_(1, val_arr, sizeof(double));
    vector* shape = vector_(1, shape_arr, sizeof(int));
    tensor* t1 = tensor_(val, shape);
    tensor* t2 = tensor_(val, shape);

    int idx_arr[1] = { 0 };
    vector* idx = vector_(1, idx_arr, sizeof(int));

    tensor* t3 = tensor_sum(t1, t2, DOUBLE);
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
        (*(double*)vector_get(t3->data, 0) == 2) && \
        (*(int*)vector_get(t3->shape, 0) == 1) && \
        (*(int*)vector_get(t3->strides, 0) == 1);

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