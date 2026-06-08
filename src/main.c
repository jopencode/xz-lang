#include <stdio.h>
#include <stdlib.h>

#include "arena.h"
#include "ast.h"
#include "builtins.h"
#include "environment.h"
#include "hashtable.h"
#include "interpreter.h"
#include "parser.h"
#include "utils.h"

#define MB (1024 * 1024)

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage %s <input file>\n", argv[0]);
        return 0;
    }
    const char *filename = argv[1];
    char *code = readFile(filename);
    if (!code) {
        fprintf(stderr, "could not open file: %s\n", filename);
        return 1;
    }
    Arena ast_arena = arena_init(1 * MB);
    Arena global_arena = arena_init(MB >> 1);
    Parser parser = parser_init(&ast_arena, code, filename);
    AstNode *program = parse_program(&parser);
    HashTable global_table = hashTable_init(&global_arena, 128);
    Environment global_env = env_init(&global_arena, &global_table, NULL);
    register_natives(&global_env);

    if (parser.hasError) {
        arena_free(&ast_arena);
        arena_free(&global_arena);
        free(code);
        return 69;
    }

    execute(program, &global_env);

    arena_free(&ast_arena);
    arena_free(&global_arena);
    hashTable_free(&global_table);
    free(code);
    return 0;
}
