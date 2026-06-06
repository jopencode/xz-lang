#include "ast.h"
#include "arena.h"

AstNode *createNumberNode(Arena *arena, int val) {
    AstNode *node = arena_alloc(arena, sizeof(AstNode));
    node->type = N_NUMBER;
    node->as.number.val = val;
    return node;
}
AstNode *createBinaryNode(Arena *arena, AstNode *l, AstNode *r, TokenType op) {
    AstNode *node = arena_alloc(arena, sizeof(AstNode));
    node->type = N_BINARY;
    node->as.binary.l = l;
    node->as.binary.r = r;
    node->as.binary.op = op;
    return node;
}
