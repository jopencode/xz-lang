#include "environment.h"
#include "hashtable.h"

Environment env_init(Arena *arena, HashTable *table, Environment *parent) {
    Environment env = {
        .arena = arena,
        .table = table,
        .parent = parent,
    };
    return env;
}
void env_insert(Environment *env, const char *key, Value value) {
    hashTable_insert(env->table, key, value);
}
Value *env_get(Environment *env, const char *key) {
    Environment *current = env;
    while (current != NULL) {
        Value *value = hashTable_get(env->table, key);
        if (value)
            return value;
        current = current->parent;
    }
    return NULL;
}
