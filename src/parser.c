#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"
#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "utils.h"

#define STMTS_CAPACITY (64)

Parser parser_init(Arena *arena, const char *source, const char *filename) {
    Parser p = {0};
    p.lexer = lexer_init(source, filename);
    p.arena = arena;
    p.current_token = nextToken(&p.lexer);
    p.peek_token = nextToken(&p.lexer);
    return p;
}

void advance(Parser *p) {
    p->current_token = p->peek_token;
    p->peek_token = nextToken(&p->lexer);
}

void skip_panic(Parser *p) {
    while (p->current_token.type != T_NEWLINE && p->current_token.type != T_EOF) {
        advance(p);
    }
}

void error_at(Parser *p, Token t, const char *msgFmt, ...) {
    p->hasError = 1;

    fprintf(stderr, "%s:%d:%d error: ", t.loc.filename, t.loc.line, t.loc.col);

    va_list args;
    va_start(args, msgFmt);
    vfprintf(stderr, msgFmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    skip_panic(p);
}

AstNode *parse_factor(Parser *p) {
    Token t = p->current_token;
    advance(p);
    switch (t.type) {
    case T_NUMBER:
        return createNumberNode(p->arena, parseInt(t.val.start, t.val.length));
    case T_STRING: {
        const char *token_start = t.val.start;
        int token_len = t.val.length;
        char *str = arena_alloc(p->arena, t.val.length + 1);
        memcpy(str, token_start, token_len);
        str[token_len] = '\0';
        return createStringNode(p->arena, str);
    }
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
    case T_IDENT: {
        const char *token_start = t.val.start;
        int token_len = t.val.length;
        char *name = arena_alloc(p->arena, t.val.length + 1);
        memcpy(name, token_start, token_len);
        name[token_len] = '\0';

        if (p->current_token.type == T_LPAREN) {
            advance(p);
            int count = 0;
            size_t capacity = 12;

            AstNode **args = arena_alloc(p->arena, capacity * sizeof(AstNode *));

            while (p->current_token.type != T_EOF && p->current_token.type != T_RPAREN) {
                AstNode *node = parse_expr(p);
                if (!node)
                    break;

                args[count++] = node;

                if (p->current_token.type == T_COMMA) {
                    advance(p);
                } else if (p->current_token.type != T_RPAREN) {
                    error_at(p, p->current_token, "Expected ',' or ')' in argument list");
                    break;
                }
            }

            if (p->current_token.type == T_RPAREN) {
                advance(p);
            } else {
                error_at(p, p->current_token, "Expected ')' after argument list");
            }

            return createNativeFuncNode(p->arena, name, args, count);
        }
        return createVariableNode(p->arena, name);
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

AstNode *parse_ifstmt(Parser *p);
AstNode *parse_block(Parser *p);
AstNode *parse_whilestmt(Parser *p);
AstNode *parse_variable(Parser *p) {
    Token t = p->current_token;
    advance(p);
    if (t.type != T_IDENT) {
        error_at(p, t, "Expected identifier, got %s", tokenTypeToString(t.type));
        return NULL;
    }
    const char *token_start = t.val.start;
    int token_len = t.val.length;
    char *name = arena_alloc(p->arena, token_len + 1);
    memcpy(name, token_start, token_len);
    name[token_len] = '\0';

    t = p->current_token;
    advance(p);
    if (t.type != T_ASSIGN) {
        error_at(p, t, "Expected '=', got %s", tokenTypeToString(t.type));
        return NULL;
    }
    AstNode *value = parse_expr(p);
    if (!value) {
        return NULL;
    }

    return createAssignNode(p->arena, name, value);
}
AstNode *parse_stmt(Parser *p) {
    switch (p->current_token.type) {
    case T_VAR:
        advance(p);
        return parse_variable(p);
    default:
        return parse_expr(p);
    }
}

AstNode *parse_program(Parser *p) {
    size_t capacity = STMTS_CAPACITY;
    AstNode **stmts = arena_alloc(p->arena, capacity * sizeof(AstNode *));
    int count = 0;

    while (p->current_token.type != T_EOF) {
        switch (p->current_token.type) {
        case T_NEWLINE:
            advance(p);
            continue;
        case T_ERROR:
            error_at(p, p->current_token, "%.*s", p->current_token.val.length,
                     p->current_token.val.start);
            break;
        case T_ILLEGAL:
            error_at(p, p->current_token, "illegal character: %.*s",
                     p->current_token.val.length, p->current_token.val.start);
            break;
        default:
            break;
        }
        AstNode *stmt = parse_stmt(p);

        if (stmt != NULL) {
            if ((size_t)count >= capacity) {
                size_t old_capacity = capacity;
                capacity = capacity << 1;
                AstNode **new_stmts = arena_alloc(p->arena, capacity * sizeof(AstNode *));
                memcpy(new_stmts, stmts, old_capacity * sizeof(AstNode *));
                stmts = new_stmts;
            }
            stmts[count++] = stmt;
        } else {
            // skip
            advance(p);
        }
    }

    return createBlockNode(p->arena, stmts, count);
}
