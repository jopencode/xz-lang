#include <stdio.h>
#include <stdlib.h>

#include "arena.h"
#include "ast.h"
#include "parser.h"
#include "utils.h"

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
    Arena arena = arena_init(1024 * 1024);
    Parser parser = parser_init(&arena, code, filename);
    AstNode *node = parse_expr(&parser);

    if (parser.hasError) {
        arena_free(&arena);
        free(code);
        return 69;
    }

    int result = execute(node);

    printf("Result = %d\n", result);

    arena_free(&arena);
    free(code);
    return 0;
}
