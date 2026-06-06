#pragma once

#include <stddef.h>
#include <stdint.h>

#include "arena.h"

typedef struct HashBucket {
    const char *key;
    uint32_t value;
    struct HashBucket *next;
} HashBucket;

typedef struct {
    Arena arena;
    HashBucket **buckets;
    size_t capacity;
} HashTable;

HashTable hashTable_init(Arena arena, uint32_t size);
void hashTable_insert(HashTable *table, const char *key, uint32_t value);
uint32_t *hashTable_get(HashTable *table, const char *key);
void hashTable_free(HashTable *table);
