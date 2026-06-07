#include <assert.h>
#include <stdlib.h>

#include "arena.h"

ArenaBlock *arenaBlock_init(size_t capacity) {
    ArenaBlock *block = malloc(sizeof(ArenaBlock));
    assert(block != NULL && "Out of memory");
    block->offset = 0;
    block->capacity = capacity;
    block->next = NULL;
    block->buffer = malloc(capacity);
    assert(block->buffer != NULL && "Out of memory");
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
        while (current->next != NULL && size > current->next->capacity) {
            current = current->next;
        }
        if (current->next != NULL) {
            current = current->next;
        } else {
            size_t new_capacity = arena->capacity;
            if (new_capacity < size)
                new_capacity = size;
            ArenaBlock *new_block = arenaBlock_init(new_capacity);
            arena->current->next = new_block;
            arena->current = new_block;
        }
        current = arena->current;
    }
    void *ptr = &current->buffer[current->offset];
    current->offset += size;
    return ptr;
}
void arena_reset(Arena *arena) {
    ArenaBlock *current = arena->first->next;
    while (current != NULL) {
        current->offset = 0;
        current = current->next;
    }
    arena->current = arena->first;
}
void arena_free(Arena *arena) {
    ArenaBlock *current = arena->first;
    while (current != NULL) {
        ArenaBlock *next = current->next;
        free(current->buffer);
        free(current);
        current = next;
    }
    arena->current = NULL;
    arena->first = NULL;
}
