#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "environment.h"
#include "interpreter.h"
#include "value.h"

void runtime_error_at(const char *msgFmt, ...) {
    fprintf(stderr, "runtime error: ");
    va_list args;
    va_start(args, msgFmt);
    vfprintf(stderr, msgFmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    exit(1);
}

Value execute_block(AstNode *node, Environment *env) {
    Value result = NULL_VAL();

    for (int i = 0; i < node->as.block.count; i++) {
        result = execute(node->as.block.statements[i], env);
    }
    return result;
}

Value execute_binary(AstNode *node, Environment *env) {
    Value l = execute(node->as.binary.l, env);
    Value r = execute(node->as.binary.r, env);
    switch (node->as.binary.op) {
    case T_PLUS:
        return NUMBER_VAL(l.as.number + r.as.number);
    case T_MINUS:
        return NUMBER_VAL(l.as.number - r.as.number);
    case T_STAR:
        return NUMBER_VAL(l.as.number * r.as.number);
    case T_SLASH:
        return NUMBER_VAL(l.as.number / r.as.number);
    case T_GREATER:
        return BOOL_VAL(l.as.number > r.as.number);
    case T_LESS:
        return BOOL_VAL(l.as.number < r.as.number);
    default:
        return NULL_VAL();
    }
}

Value execute(AstNode *node, Environment *env) {
    if (!node)
        return NULL_VAL();
    switch (node->type) {
    case N_BLOCK:
        return execute_block(node, env);
    case N_NUMBER:
        return NUMBER_VAL(node->as.number.val);
    case N_BINARY:
        return execute_binary(node, env);
    case N_VARIABLE: {
        Value *val = env_get(env, node->as.variable.name);
        if (!val) {
            runtime_error_at("undefined '%s' variable", node->as.variable.name);
        }
        return *val;
    }
    case N_ASSIGN: {
        Value value = execute(node->as.assign.value, env);
        env_insert(env, node->as.assign.name, value);
        return value;
    }
    case N_CALL: {
        Value *val = env_get(env, node->as.variable.name);
        if (!val) {
            runtime_error_at("undefined '%s' function", node->as.variable.name);
            return NULL_VAL();
        }
        if (val->type == VAL_NATIVE_FUNC) {
            Value args[node->as.native_func.count];
            for (int i = 0; i < node->as.native_func.count; i++) {
                args[i] = execute(node->as.native_func.args[i], env);
            }
            return val->as.native(node->as.native_func.count, args);
        } else {
            runtime_error_at("user function is not implemented");
            return NULL_VAL();
        }
    }
    case N_STRING:
        return STRING_VAL(node->as.string.val);

    default:
        return NULL_VAL();
    }
}
