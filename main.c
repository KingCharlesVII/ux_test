#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef char * string;
typedef void(*ux_test_routine)(void);

typedef struct ux_test_info {
    string  file;
    string  name;
    string  line;    
}   ux_test_info;

typedef struct ux_test_function {
    string  name;
    ux_test_routine execute;
}   ux_test_function;

typedef struct ux_test_result {
    string message;
    bool    success;
}   ux_test_result;

typedef struct ux_test{
    ux_test_function    function;
    ux_test_info info;
    ux_test_result      result;
}   ux_test;

static ux_test test[256];
static size_t  ux_test_index = 0;

void    ux_test_register(ux_test this) {
    test[ux_test_index++] = this;
}

bool    ux_is_supported_type(char *type) {
    return (!strcmp(type, "int") || !strcmp(type, "double") || !strcmp(type, "string"));
}

#define Ux_str_equality(str_a, str_b) (strcmp((#str_a), (#str_b)) == 0) ? (1): (0)

#define Ux_test(name) \
    static void name(void); \
    static const ux_test test_##name = {(ux_test_function){#name, name}, (ux_test_result){NULL, true}}; \
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
    if (ux_test_index < 1) \
        break; \
    ux_test *current_test = &test[ux_test_index - 1]; \
    if (left_expression != right_expression) { \
        current_test->result.success = false; \
    } \
    } while (0) \

void    ux_test_execute(void) {
     for(size_t index = 0; index < ux_test_index; index++)
        test[index].function.execute();
}

void    ux_test_log_error(ux_test the_test) {
    fprintf(stderr, "File [%s]: in function [%s], line [%s]: %s"
        , the_test.function.file, the_test.function.name, the_test.function.line
        , the_test.result.message);
}

void    ux_test_log(void) {
     for(size_t index = 0; index < ux_test_index; index++)
        ux_test_log_error(test[index]);
}

void    ux_test_run(void) {
   
  
}

Ux_test(addition) {
    Ux_expect_eq(int, 1, 2);
    Ux_expect_eq(int, 1, 2);
}

Ux_test(multiplication) {
    printf("World");
}

int main(void) {
    //Ux_assert_eq(int, 1, 1);
    ux_test_run();
}