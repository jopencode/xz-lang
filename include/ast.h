#pragma once

#include "arena.h"
#include "token.h"

typedef enum { N_NUMBER, N_BINARY } AstNodeType;

typedef struct AstNode {
    AstNodeType type;
    union {
        struct {
            int val;
        } number;
        struct {
            struct AstNode *l;
            struct AstNode *r;
            TokenType op;
        } binary;
    } as;
} AstNode;

AstNode *createNumberNode(Arena *arena, int val);
AstNode *createBinaryNode(Arena *arena, AstNode *l, AstNode *r, TokenType op);
