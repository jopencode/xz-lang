#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    VAL_NUMBER,
    VAL_BOOL,
    VAL_STRING,
    VAL_NULL,
    VAL_NATIVE_FUNC,
} ValueType;

typedef struct Value Value;

typedef Value (*NativeFn)(int argCount, Value *args);

typedef struct Value {
    ValueType type;
    union {
        double number;
        bool boolean;
        const char *string;
        NativeFn native;
    } as;
} Value;

#define NUMBER_VAL(v) ((Value){VAL_NUMBER, {.number = v}})
#define BOOL_VAL(v) ((Value){VAL_BOOL, {.boolean = v}})
#define STRING_VAL(v) ((Value){VAL_STRING, {.string = v}})
#define NULL_VAL() ((Value){VAL_NULL, {.boolean = false}})
