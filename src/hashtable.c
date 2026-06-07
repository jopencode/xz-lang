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

HashTable hashTable_init(Arena *arena, size_t capacity) {
    if (capacity < 16)
        capacity = 16;
    uint32_t p = 1;

    // find the nearest higher power of two
    while (p < capacity)
        p *= 2;
    capacity = p;

    HashTable table = {
        .arena = arena,
        .buckets = calloc(capacity, sizeof(HashBucket *)),
        .capacity = capacity,
        .max_size = (capacity * 3) >> 2,
    };

    assert(table.buckets != NULL && "Out of memory");
    return table;
}
void hashtable_rehash(HashTable *table) {
    size_t old_capacity = table->capacity;
    size_t new_capacity = old_capacity << 1;

    HashBucket **old_buckets = table->buckets;
    HashBucket **new_buckets = calloc(new_capacity, sizeof(HashBucket *));
    assert(new_buckets != NULL && "Out of memory during rehash");

    for (size_t i = 0; i < old_capacity; i++) {
        HashBucket *current = old_buckets[i];
        while (current != NULL) {
            HashBucket *next = current->next;

            uint32_t hash = fnv1a_hash(current->key);
            uint32_t new_index = hash & (new_capacity - 1);

            current->next = new_buckets[new_index];
            new_buckets[new_index] = current;

            current = next;
        }
    }

    free(old_buckets);
    table->buckets = new_buckets;
    table->capacity = new_capacity;
    table->max_size = (new_capacity * 3) >> 2;
}

void hashTable_insert(HashTable *table, const char *key, uint32_t value) {
    if (table->size >= table->max_size) {
        hashtable_rehash(table);
    }
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

    HashBucket *new_bucket = arena_alloc(table->arena, sizeof(HashBucket));
    new_bucket->key = key;
    new_bucket->value = value;

    new_bucket->next = table->buckets[index];
    table->buckets[index] = new_bucket;
    table->size++;
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
    table->buckets = NULL;
    table->capacity = 0;
    table->size = 0;
}
