#include <stdio.h>

#include "builtins.h"

Value native_print(int argCount, Value *args) {
    for (int i = 0; i < argCount; i++) {
        if (args[i].type == VAL_NUMBER)
            printf("%f", args[i].as.number);
        if (args[i].type == VAL_NULL)
            printf("(null)");
        if (args[i].type == VAL_BOOL)
            printf("%s", args[i].as.boolean ? "true" : "false");
        if (args[i].type == VAL_STRING)
            printf("%s", args[i].as.string);
    }
    printf("\n");
    return NULL_VAL();
}

void register_natives(Environment *env) {
    Value print_val = {.type = VAL_NATIVE_FUNC, .as.native = native_print};
    env_insert(env, "print", print_val);
}
