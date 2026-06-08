#pragma once

#include <stddef.h>
#include <stdint.h>

#include "arena.h"
#include "value.h"

typedef struct HashBucket {
    const char *key;
    Value value;
    struct HashBucket *next;
} HashBucket;

typedef struct {
    Arena *arena;
    HashBucket **buckets;
    size_t capacity;
    size_t size;
    size_t max_size;
} HashTable;

HashTable hashTable_init(Arena *arena, size_t capacity);
void hashTable_insert(HashTable *table, const char *key, Value value);
Value *hashTable_get(HashTable *table, const char *key);
void hashTable_free(HashTable *table);
