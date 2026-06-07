#pragma once

#include "arena.h"
#include "token.h"

typedef enum {
    N_NUMBER,
    N_BINARY,
    N_VARIABLE,
    N_ASSIGN,
    N_IF,
    N_WHILE,
    N_BLOCK
} NodeType;

typedef struct AstNode {
    NodeType type;
    union {
        struct {
            int val;
        } number;
        struct {
            struct AstNode *l;
            struct AstNode *r;
            TokenType op;
        } binary;
        struct {
            const char *name;
        } variable;
        struct {
            const char *name;
            struct AstNode *value;
        } assign;
        struct {
            struct AstNode *condition;
            struct AstNode *then_branch;
            struct AstNode *else_branch;
        } if_stmt;
        struct {
            struct AstNode *condition;
            struct AstNode *body;
        } while_stmt;
        struct {
            struct AstNode **statements;
            int count;
        } block;
    } as;
} AstNode;

AstNode *createNumberNode(Arena *arena, int val);
AstNode *createBinaryNode(Arena *arena, AstNode *l, AstNode *r, TokenType op);
AstNode *createVariableNode(Arena *arena, const char *name);
AstNode *createAssignNode(Arena *arena, const char *name, AstNode *value);
AstNode *createIfNode(Arena *arena, AstNode *cond, AstNode *then, AstNode *els);
AstNode *createWhileNode(Arena *arena, AstNode *cond, AstNode *body);
AstNode *createBlockNode(Arena *arena, AstNode **stmts, int count);
