#pragma once

#include "token.h"

typedef struct {
    const char *source;
    TokenLocation loc;
    int pos; // byte position in the source code
} Lexer;

Lexer lexer_init(const char *source, const char *filename);
Token nextToken(Lexer *l);
