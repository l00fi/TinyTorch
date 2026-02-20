#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct {
    void* data;
    size_t size;
    size_t capacity;
    size_t item_size;
} vector;

vector* vector_empty(size_t capacity, size_t item_size);
vector* vector_(size_t capacity, void* data, size_t item_size);
void*   vector_get(vector* v, int index);
void    vector_destroy(vector* v);
void    vector_append(vector* v, void* val);
void    vector_edit(vector* v, int index, void* new_value);
vector* vector_add(vector* v1, vector* v2);
vector* vector_sum(vector* v1, vector* v2);
void    float_(void* ptr);
void    vector_print(vector* v, void (*print_func)(void*));

#endif;