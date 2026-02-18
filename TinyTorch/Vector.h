#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct {
    float* data;
    size_t size;
    size_t capacity;
} vector;

vector* vector_empty(size_t capacity);
vector* vector_(size_t capacity, float data[]);
void    vector_destroy(vector* v);
void    vector_append(vector* v, float val);
float   vector_get(vector* v, int index);
vector* vector_add(vector* v1, vector* v2);
vector* vector_sum(vector* v1, vector* v2);
void    vector_print(vector* v);
void    vector_edit(vector* v, int index, float new_value);

#endif;