#pragma once

#include "arena.h"
#include "ast.h"
#include "lexer.h"

typedef struct {
    Lexer lexer;
    Arena *arena;
    Token current_token;
    int hasError;
} Parser;

Parser parser_init(Arena *arena, const char *source, const char *filename);

AstNode *parse_factor(Parser *p);
AstNode *parse_term(Parser *p);
AstNode *parse_expr(Parser *p);

int execute(AstNode *node);
