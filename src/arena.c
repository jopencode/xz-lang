#include <assert.h>
#include <stdlib.h>

#include "arena.h"

ArenaBlock *arenaBlock_init(size_t capacity) {
    ArenaBlock *block = malloc(sizeof(ArenaBlock));
    block->offset = 0;
    block->capacity = capacity;
    block->next = NULL;
    block->buffer = malloc(capacity);
    return block;
}

Arena arena_init(size_t capacity) {
    if (capacity == 0)
        capacity = 16;
    ArenaBlock *block = arenaBlock_init(capacity);
    Arena arena = {.capacity = capacity, .first = block, .current = block};
    return arena;
}
void *arena_alloc(Arena *arena, size_t size) {
    size = (size + 7) & ~7;
    ArenaBlock *current = arena->current;
    if (current->offset + size > current->capacity) {
        size_t new_capacity = arena->capacity;
        if (new_capacity < size)
            new_capacity = size;
        ArenaBlock *new_block = arenaBlock_init(current->capacity + size);
        arena->current->next = new_block;
        arena->current = new_block;
        current = arena->current;
    }
    void *ptr = &current->buffer[current->offset];
    current->offset += size;
    return ptr;
}
void arena_free(Arena *arena) {
    ArenaBlock *current = arena->first;
    while (current != NULL) {
        ArenaBlock *next = current->next;
        free(current->buffer);
        free(current);
        current = next;
    }
}
