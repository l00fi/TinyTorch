#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "vector.h"


vector* vector_empty(size_t capacity, size_t item_size) {
	vector* v = malloc(sizeof(vector));
	// Проверка, что память есть для структуры
	if (v == NULL) return NULL;
	v->size = 0;
	v->capacity = capacity;
	v->item_size = item_size;
	v->data = malloc(capacity * item_size);
	// Проверка, что память есть и для данных
	if (v->data == NULL) {
		//Если нет - очищаем память
		free(v);
		return NULL;
	}
	return v;
}

vector* vector_(size_t capacity, void* data, size_t item_size) {
	vector* v = malloc(sizeof(vector));
	// Проверка, что память есть для структуры
	if (v == NULL) return NULL;
	v->size = capacity;
	v->capacity = capacity;
	v->item_size = item_size;

	v->data = malloc(capacity * item_size);
	// Проверка, что память есть и для данных
	if (v->data == NULL) {
		//Если нет - очищаем память
		free(v);
		return NULL;
	}
	else {
		// Копирование одной области памяти в другую
		memcpy(v->data, data, capacity * item_size);
	}

	return v;
}

void* vector_get(vector* v, int index) {
	if (v == NULL || v->data == NULL) return;

	int true_index = index;

	if (index < 0) true_index = (int)v->size + index;

	if (true_index < 0 || (size_t)true_index >= v->size) return;

	//Теперь прыгаем по байтам, а не обращаемся к индексу, т.к void* data
	char* base = (char*)v->data;

	return base + (v->item_size * true_index);
}

void vector_destroy(vector* v) {
	if (v == NULL) return;

	if (v->data != NULL) {
		free(v->data);
	}
	free(v);
}

void vector_append(vector* v, void* val) {
	if (v == NULL) return;
	// Если элементов и доступной памяти равное кол-во, то нужно увеличить объём памяти
	if (v->size == v->capacity) {
		size_t new_capacity = v->capacity * 2;
		//realloc либо увеличивает размер памяти вправо, либо ищет область памяти нужного размера
		float* temp = realloc(v->data, new_capacity * v->item_size);

		if (temp == NULL) return;

		v->data = temp;
		v->capacity = new_capacity;
	}

	// Вычисляем адрес, куда нужно положить новые байты
	void* target_address = (char*)v->data + (v->size * v->item_size);

	// Копируем данные из src_value в target_address
	memcpy(target_address, val, v->item_size);

	v->size++;
}

void vector_edit(vector* v, int index, void* new_value) {
	if (v == NULL || v->data == NULL) return;

	int true_index = index;

	if (index < 0) true_index = (int)v->size + index;

	if (true_index < 0 || (size_t)true_index >= v->size) return;

	void* target_address = (char*)v->data + ((size_t)true_index * v->item_size);

	memcpy(target_address, new_value, v->item_size);
}

vector* vector_add(vector* v1, vector* v2) {
	if (v1 == NULL || v2 == NULL) return NULL;

	if (v1->item_size != v2->item_size) return NULL;

	size_t new_size = v1->size + v2->size;
	vector* v3 = vector_empty(new_size, v1->item_size);

	for (int i = 0; i < v1->size; ++i) {
		vector_append(v3, vector_get(v1, i));
	}

	for (int i = 0; i < v2->size; ++i) {
		vector_append(v3, vector_get(v2, i));
	}

	return v3;
}

vector* vector_copy(vector* v) {
	if (v == NULL) return NULL;

	vector* new_v = vector_(v->capacity, v->data, v->item_size);

	if (new_v != NULL) {
		new_v->size = v->size;
	}

	return new_v;
}

vector* vector_sum(vector* v1, vector* v2, DataType type) {
	if (v1 == NULL || v2 == NULL) return NULL;

	if (v1->size != v2->size) return NULL;

	if (v1->item_size != v2->item_size) return NULL;

	vector* v3 = vector_empty(v1->size, v1->item_size);

	for (int i = 0; i < v1->size; ++i) {
		if (type == FLOAT) {
			float sum = *(float*)vector_get(v1, i) + *(float*)vector_get(v2, i);
			vector_append(v3, &sum);
		}
		if (type == INT) {
			int sum = *(int*)vector_get(v1, i) + *(int*)vector_get(v2, i);
			vector_append(v3, &sum);
		}
		if (type == DOUBLE) {
			double sum = *(double*)vector_get(v1, i) + *(double*)vector_get(v2, i);
			vector_append(v3, &sum);
		}
	}

	return v3;
}

void vector_print(vector* v, void (*print_func)(void*)) {
	if (v == NULL) {
		printf("[NULL]\n");
		return;
	}
	printf("[");
	for (size_t i = 0; i < v->size; ++i) {
		print_func(vector_get(v, (int)i));
		if (i < v->size - 1) printf(", ");
	}
	printf("]\n");
}

void vfloat(void* ptr) {
	printf("%.2f", *(float*)ptr);
}

void vint(void* ptr) {
	printf("%d", *(int*)ptr);
}

void vdouble(void* ptr) {
	printf("%.4f", *(double*)ptr);
}