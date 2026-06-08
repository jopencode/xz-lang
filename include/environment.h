#pragma once

#include "hashtable.h"
#include "value.h"

typedef struct Environment {
    Arena *arena;
    HashTable *table;
    struct Environment *parent;
} Environment;

Environment env_init(Arena *arena, HashTable *table, Environment *parent);
void env_insert(Environment *env, const char *key, Value value);
Value *env_get(Environment *env, const char *key);
