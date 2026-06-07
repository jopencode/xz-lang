#include <stdarg.h>
#include <stdio.h>

#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "utils.h"

Parser parser_init(Arena *arena, const char *source, const char *filename) {
    Parser p = {0};
    p.lexer = lexer_init(source, filename);
    p.arena = arena;
    p.current_token = nextToken(&p.lexer);
    return p;
}

void advance(Parser *p) { p->current_token = nextToken(&p->lexer); }

void error_at(Parser *p, Token t, const char *msgFmt, ...) {
    p->hasError = 1;

    fprintf(stderr, "%s:%d:%d error: ", t.loc.filename, t.loc.line, t.loc.col);

    va_list args;
    va_start(args, msgFmt);
    vfprintf(stderr, msgFmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}

AstNode *parse_factor(Parser *p) {
    Token t = p->current_token;
    advance(p);
    switch (t.type) {
    case T_NUMBER:
        return createNumberNode(p->arena, parseInt(t.val.start, t.val.length));
    case T_NEWLINE:
        return parse_expr(p);
    case T_LPAREN: {
        AstNode *node = parse_expr(p);
        t = p->current_token;
        advance(p);
        if (t.type != T_RPAREN) {
            error_at(p, t, "Expected ')', got %s",
                     tokenTypeToString(p->current_token.type));
            return NULL;
        }
        return node;
    }
    default:
        error_at(p, t, "Expected number, got %s",
                 tokenTypeToString(p->current_token.type));
        advance(p);
        return NULL;
    }
}

AstNode *parse_term(Parser *p) {
    AstNode *left = parse_factor(p);

    while (p->current_token.type == T_STAR || p->current_token.type == T_SLASH) {
        TokenType op = p->current_token.type;
        advance(p);
        AstNode *right = parse_factor(p);
        left = createBinaryNode(p->arena, left, right, op);
    }

    return left;
}
AstNode *parse_expr(Parser *p) {
    AstNode *left = parse_term(p);

    while (p->current_token.type == T_PLUS || p->current_token.type == T_MINUS) {
        TokenType op = p->current_token.type;
        advance(p);
        AstNode *right = parse_term(p);
        left = createBinaryNode(p->arena, left, right, op);
    }

    return left;
}

int execute(AstNode *node) {
    if (!node)
        return 0;
    switch (node->type) {
    case N_NUMBER:
        return node->as.number.val;
    case N_BINARY: {
        int l = execute(node->as.binary.l);
        int r = execute(node->as.binary.r);
        switch (node->as.binary.op) {
        case T_PLUS:
            return l + r;
        case T_MINUS:
            return l - r;
        case T_STAR:
            return l * r;
        case T_SLASH:
            return l / r;
        default:
            return 0;
        }
    }
    default:
        return 0;
    }
}
