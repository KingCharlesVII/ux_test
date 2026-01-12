#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef char * string;
typedef void(*ux_test)(void);

typedef struct ux_test_function{
    const char *name;
    ux_test execute;
    bool    success;
}   ux_test_function;

static ux_test_function tests[256];
static size_t  ux_test_index = 0;

void    ux_test_register(ux_test_function this) {
    tests[ux_test_index++] = this;
}

bool    ux_is_supported_type(char *type) {
    return (!strcmp(type, "int") || !strcmp(type, "double") || !strcmp(type, "string"));
}

#define Ux_str_equality(str_a, str_b) (strcmp((#str_a), (#str_b)) == 0) ? (1): (0)

#define Ux_test(name) \
    static void name(void); \
    static const ux_test_function test_##name = {#name, name, true}; \
    static void __register_##name(void) __attribute((constructor)); \
    static void __register_##name(void) {ux_test_register(test_##name);} \
    static void name(void) \

#define Ux_static_assert(condition) typedef char static_assert__LINE__[(condition) ? (0): (-1)];

#define Ux_assert(condition) \
    do { \
        if (!condition) { \
            fprintf(stderr, "Assertion failed: %s:%d %s\n", __FILE__, __LINE__, #condition); \
        } \
    }   while(0) \

#define Ux_assert_eq(type, expression_a, expression_b) do { \
    Ux_static_assert(ux_is_supported_type(#type)) \
    type left_expression = expression_a; \
    type right_expression = expression_b; \
    if (right_expression != left_expression) \
        abort(); \
    } while (0) \

#define Ux_expect_eq(type, expression_a, expression_b) do { \
    Ux_static_assert(ux_is_supported_type(#type)) \
    type left_expression = expression_a; \
    type right_expression = expression_b; \
    if (right_expression != left_expression) \
        if (ux_test_index >= 1) \
            tests[ux_test_index - 1].success = false; \
    } while (0) \

// #define Ux_assert_str_eq()

Ux_test(addition) {
    Ux_expect_eq(int, 1, 2);
}

Ux_test(multiplication) {
    printf("World");
}

void    ux_test_run(void) {
    for(size_t index = 0; index < ux_test_index; index++) {
        tests[index].execute();
        if (tests[index].success == false)
            fprintf(stderr, "Error in this test\n");
    }
}

int main(void) {
    //Ux_assert_eq(int, 1, 1);
    ux_test_run();
}