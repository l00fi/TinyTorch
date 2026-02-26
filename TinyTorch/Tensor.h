#ifndef TENSOR_H
#define TENSOR_H

#include "Vector.h"
// Структура тензора
typedef struct {
	vector* data;		// Одномерный вектор данных
	vector* shape;		// Размерность тензора
	vector* strides;	// Смещения для представления одномерного массива как тензора
	DataType type;      // Тип данных тензора
} tensor;
// Процедуры и функции для тензора
tensor* tensor_(vector* data, vector* shape);								// Конструктор тензора
tensor* tensor_copy(tensor* t);												// Копирование тензора
void	tensor_destroy(tensor* t);											// Деструктои тензора
void*   tensor_get(tensor* data, vector* idx);								// Получение доступа к элементу тензора
void	tensor_edit(tensor* t, vector* idx, void* val);						// Реадктирование тензора по индексу
void	tensor_T(tensor* t);												// Транспонирование
tensor* tensor_sum(tensor* t1, tensor* t2);									// Сумма двух тензоров
tensor* tensor_scalar_mult(tensor* t, void* scalar);						// Скалярное произведение
tensor* tensor_hadamard_mult(tensor* t1, tensor* t2);						// Поэлементное умножение 
tensor* tensor_mult_2d(tensor* t1, tensor* t2);								// Умножение матриц
tensor* tensor_mult(tensor* t1, tensor* t2);								// Умножение тензоров
void	tensor_print(tensor* t, int dim_idx, int offset);					// Вывод тензора на терминал

#endif

