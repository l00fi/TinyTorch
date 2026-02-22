#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
// Структура типов
typedef enum {
    FLOAT,
    INT,
    DOUBLE
} DataType;
// Структура вектора
typedef struct {
    void* data;         // Данные
    size_t size;        // Кол-во не пустых ячеек
    size_t capacity;    // Длинна всего вектора
    size_t item_size;   // Кол-во байт одного элемента
} vector;
// Методы и функции для вектора
vector* vector_empty(size_t capacity, size_t item_size);        // Конструктор пустого вектора
vector* vector_(size_t capacity, void* data, size_t item_size); // Конструктор не пустого вектора
void*   vector_get(vector* v, int index);                       // Процедура получения элемента по индексу
void    vector_destroy(vector* v);                              // Деструктор вектора
void    vector_append(vector* v, void* val);                    // Добавление элемента в конец вектора
void    vector_edit(vector* v, int index, void* new_value);     // Редактирование значения вектора по индексу
vector* vector_merge(vector* v1, vector* v2);                   // Слияние двух векторов
vector* vector_copy(vector* v);                                 // Копирование вектора
vector* vector_sum(vector* v1, vector* v2, DataType type);      // Поэлементная сумма двух векторов
void    vector_print(vector* v, void (*print_func)(void*));     // Вывод вектора на терминал
void    vfloat(void* ptr);                                      // Тип float для вывода
void    vint(void* ptr);                                        // Тип int для вывода
void    vdouble(void* ptr);                                     // Тип double для вывода


#endif;