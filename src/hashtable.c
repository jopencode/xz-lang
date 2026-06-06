#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"
#include "hashtable.h"

uint32_t fnv1a_hash(const char *key) {
    uint32_t hash = 2166136261U; // FNV offset basis
    while (*key) {
        hash ^= (uint8_t)*key++;
        hash *= 16777619U; // FNV prime
    }
    return hash;
}

HashTable hashTable_init(Arena arena, uint32_t size) {
    if (size < 16)
        size = 16;
    uint32_t p = 1;

    while (p < size)
        p *= 2;
    size = p;

    HashTable table = {
        .arena = arena, .buckets = calloc(size, sizeof(HashBucket *)), .capacity = size};

    assert(table.buckets != NULL && "Out of memory");
    return table;
}
void hashTable_insert(HashTable *table, const char *key, uint32_t value) {
    uint32_t hash = fnv1a_hash(key);
    uint32_t index = hash & (table->capacity - 1);
    HashBucket *current = table->buckets[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    HashBucket *new_bucket = arena_alloc(&table->arena, sizeof(HashBucket));
    new_bucket->key = key;
    new_bucket->value = value;

    new_bucket->next = table->buckets[index];
    table->buckets[index] = new_bucket;
}
uint32_t *hashTable_get(HashTable *table, const char *key) {
    uint32_t hash = fnv1a_hash(key);
    uint32_t index = hash & (table->capacity - 1);
    HashBucket *current = table->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return &current->value;
        }
        current = current->next;
    }

    return NULL;
}
void hashTable_free(HashTable *table) {
    free(table->buckets);
    table->capacity = 0;
}
