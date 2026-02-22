#ifndef TENSOR_H
#define TENSOR_H

#include "Vector.h"
// Структура тензора
typedef struct {
	vector* data;		// Одномерный вектор данных
	vector* shape;		// Размерность тензора
	vector* strides;	// Смещения для представления одномерного массива как тензора
} tensor;
// Процедуры и функции для тензора
tensor* tensor_(vector* data, vector* shape);								// Конструктор тензора
void	tensor_destroy(tensor* t);											// Деструктои тензора
void*   tensor_get(tensor* data, vector* idx);								// Получение доступа к элементу тензора
void	tensor_print(tensor* t, int dim_idx, int offset, DataType type);	// Вывод тензора на терминал
tensor* tensor_sum(tensor* t1, tensor* t2, DataType type);					// Сумма двух тензоров

#endif

