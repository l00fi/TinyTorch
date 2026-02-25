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
    DataType type;      // Тип данных вектора
} vector;
// Методы и функции для вектора
vector* vector_empty(size_t capacity, DataType type);           // Конструктор пустого вектора
vector* vector_(size_t capacity, void* data, DataType type);    // Конструктор не пустого вектора
void*   vector_get(vector* v, int index);                       // Процедура получения элемента по индексу
void    vector_destroy(vector* v);                              // Деструктор вектора
void    vector_append(vector* v, void* val);                    // Добавление элемента в конец вектора
void    vector_edit(vector* v, int index, void* new_value);     // Редактирование значения вектора по индексу
vector* vector_merge(vector* v1, vector* v2);                   // Слияние двух векторов
vector* vector_copy(vector* v);                                 // Копирование вектора
vector* vector_sum(vector* v1, vector* v2);                     // Поэлементная сумма двух векторов
void    vector_print(vector* v);                                // Вывод вектора на терминал
float   vfloat(void* ptr);                                      // Тип float 
int     vint(void* ptr);                                        // Тип int 
double  vdouble(void* ptr);                                     // Тип double 


#endif;