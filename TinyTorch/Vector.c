#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vector.h"


vector* vector_new(size_t capacity) {
	vector* v = malloc(sizeof(vector));
	// Проверка, что память есть для структуры
	if (v == NULL) return NULL;
	v->size = 0;
	v->capacity = capacity;
	v->data = malloc(capacity * sizeof(float));
	// Проверка, что память есть и для данных
	if (v->data == NULL) {
		//Если нет - очищаем память
		free(v);
		return NULL;
	}
	return v;
}

//vector* vector_new(size_t capacity, float data[]) {
//	vector* v = malloc(sizeof(vector));
//	// Проверка, что память есть для структуры
//	if (v == NULL) return NULL;
//	v->size = capacity;
//	v->capacity = capacity;
//
//	v->data = malloc(capacity * sizeof(float));
//	for (int i = 0; i < capacity; ++i) {
//		v->data[i] = data[i];
//	}
//
//	// Проверка, что память есть и для данных
//	if (v->data == NULL) {
//		//Если нет - очищаем память
//		free(v);
//		return NULL;
//	}
//	return v;
//}

void vector_destroy(vector* v) {
	if (v == NULL) return;

	if (v->data != NULL) {
		free(v->data);
	}
	free(v);
}

void vector_append(vector* v, float val) {
	if (v == NULL) return;
	// Если элементов и доступной памяти равное кол-во, то нужно увеличить объём памяти
	if (v->size == v->capacity) {
		size_t new_capacity = v->capacity * 2;
		//realloc либо увеличивает размер памяти вправо, либо ищет область памяти нужного размера
		float* temp = realloc(v->data, new_capacity * sizeof(float));

		if (temp == NULL) return;

		v->data = temp;
		v->capacity = new_capacity;
	}

	v->data[v->size] = val;
	++v->size;
}

float vector_get(vector* v, int index) {
	if (v == NULL || v->data == NULL) return NAN;

	int true_index = index;

	if (index < 0) true_index = (int)v->size + index;

	if (true_index < 0 || (size_t)true_index >= v->size) return NAN;

	return v->data[true_index];
}

void vector_edit(vector* v, int index, float new_value) {
	if (v == NULL || v->data == NULL) return;

	int true_index = index;

	if (index < 0) true_index = (int)v->size + index;

	if (true_index < 0 || (size_t)true_index >= v->size) return;

	v->data[true_index] = new_value;
}

vector* vector_add(vector* v1, vector* v2) {
	if (v1 == NULL || v2 == NULL) return NULL;

	size_t new_size = v1->size + v2->size;
	vector* v3 = vector_new(new_size);

	for (int i = 0; i < v1->size; ++i) {
		vector_append(v3, v1->data[i]);
	}

	for (int i = 0; i < v2->size; ++i) {
		vector_append(v3, v2->data[i]);
	}

	return v3;
}

vector* vector_sum(vector* v1, vector* v2) {
	if (v1 == NULL || v2 == NULL) return NULL;

	if (v1->size != v2->size) return NULL;

	vector* v3 = vector_new(v1->size);
	v3->size = v1->size;

	for (int i = 0; i < v1->size; ++i) {
		v3->data[i] = v1->data[i] + v2->data[i];
	}

	return v3;
}

void vector_print(vector* v) {
	for (int i = 0; i < v->size; ++i) {
		printf("%.1f\n", vector_get(v, i));
	}
}