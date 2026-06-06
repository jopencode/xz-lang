#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct ArenaBlock {
    size_t offset;
    size_t capacity;
    uint8_t *buffer;
    struct ArenaBlock *next;
} ArenaBlock;

typedef struct {
    ArenaBlock *current;
    ArenaBlock *first;
    size_t capacity;
} Arena;

Arena arena_init(size_t capacity);
void *arena_alloc(Arena *arena, size_t size);
void arena_free(Arena *arena);
