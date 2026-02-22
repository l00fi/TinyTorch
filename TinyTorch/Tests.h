// test_framework.h
#ifndef TESTS_H
#define TESTS_H

#include <stdio.h>

#define MAX_FAILURES 100

typedef struct {
    int total_count;
    int passed_count;
    int failed_count;
    const char* failed_names[MAX_FAILURES];
    int failed_lines[MAX_FAILURES]; // Добавим хранение строк
} TestState;

// Объявляем переменную как extern, чтобы она была доступна везде, 
// но реально жила в одном .c файле
extern TestState global_state;

// Макрос для проверки внутри теста (теперь с номером строки)
#define ASSERT_TRUE(condition) do { \
    if (!(condition)) return 0; \
} while (0)

// Макрос для запуска теста
#define RUN_TEST(test_func) do { \
    global_state.total_count++; \
    printf("Running %s", #test_func); \
    if (test_func()) { \
        printf("%.*s[OK]\n", (unsigned int)(50 - strlen(#test_func)), ".................................................."); \
        global_state.passed_count++; \
    } else { \
        printf("%.*s[FAILED]\n", (unsigned int)(50 - strlen(#test_func)), ".................................................."); \
        if (global_state.failed_count < MAX_FAILURES) { \
            global_state.failed_names[global_state.failed_count] = #test_func; \
        } \
        global_state.failed_count++; \
    } \
} while (0)

void print_test_report();

#endif