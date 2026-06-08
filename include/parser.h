#pragma once

#include "arena.h"
#include "ast.h"
#include "lexer.h"

typedef struct {
    Lexer lexer;
    Arena *arena;
    Token current_token;
    Token peek_token;
    int hasError;
} Parser;

Parser parser_init(Arena *arena, const char *source, const char *filename);

AstNode *parse_factor(Parser *p);
AstNode *parse_term(Parser *p);
AstNode *parse_expr(Parser *p);

AstNode *parse_ifstmt(Parser *p);
AstNode *parse_block(Parser *p);
AstNode *parse_whilestmt(Parser *p);
AstNode *parse_variable(Parser *p);
AstNode *parse_stmt(Parser *p);

AstNode *parse_program(Parser *p);
