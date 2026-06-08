#include "ast.h"
#include "arena.h"

AstNode *createNumberNode(Arena *arena, double val) {
    AstNode *node = arena_alloc(arena, sizeof(AstNode));
    node->type = N_NUMBER;
    node->as.number.val = val;
    return node;
}

AstNode *createStringNode(Arena *arena, const char *val) {
    AstNode *node = arena_alloc(arena, sizeof(AstNode));
    node->type = N_STRING;
    node->as.string.val = val;
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

AstNode *createVariableNode(Arena *arena, const char *name) {
    AstNode *node = arena_alloc(arena, sizeof(AstNode));
    node->type = N_VARIABLE;
    node->as.variable.name = name;
    return node;
}

AstNode *createAssignNode(Arena *arena, const char *name, AstNode *value) {
    AstNode *node = arena_alloc(arena, sizeof(AstNode));
    node->type = N_ASSIGN;
    node->as.assign.name = name;
    node->as.assign.value = value;
    return node;
}

AstNode *createIfNode(Arena *arena, AstNode *cond, AstNode *then_b, AstNode *else_b) {
    AstNode *node = arena_alloc(arena, sizeof(AstNode));
    node->type = N_IF;
    node->as.if_stmt.condition = cond;
    node->as.if_stmt.then_branch = then_b;
    node->as.if_stmt.else_branch = else_b;
    return node;
}

AstNode *createWhileNode(Arena *arena, AstNode *cond, AstNode *body) {
    AstNode *node = arena_alloc(arena, sizeof(AstNode));
    node->type = N_WHILE;
    node->as.while_stmt.condition = cond;
    node->as.while_stmt.body = body;
    return node;
}

AstNode *createBlockNode(Arena *arena, AstNode **stmts, int count) {
    AstNode *node = arena_alloc(arena, sizeof(AstNode));
    node->type = N_BLOCK;
    node->as.block.statements = stmts;
    node->as.block.count = count;
    return node;
}

AstNode *createNativeFuncNode(Arena *arena, const char *name, AstNode **args, int count) {
    AstNode *node = arena_alloc(arena, sizeof(AstNode));
    node->type = N_CALL;
    node->as.native_func.args = args;
    node->as.native_func.count = count;
    node->as.native_func.name = name;
    return node;
}
