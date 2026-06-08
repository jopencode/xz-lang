#pragma once

typedef enum {
    T_ILLEGAL,
    T_ERROR,
    T_EOF,
    T_NEWLINE,

    // single character
    T_MINUS,
    T_PLUS,
    T_STAR,
    T_SLASH,
    T_ASSIGN,
    T_GREATER,
    T_LESS,

    T_LPAREN,
    T_RPAREN,
    T_LBRACE,
    T_RBRACE,

    T_COMMA,
    T_DOT,

    // literals
    T_NUMBER,
    T_STRING,
    T_IDENT,

    // keywords
    T_VAR,
    T_IF,
    T_ELSE,
    T_WHILE,
} TokenType;

// points to a substring inside the source code without allocating memory
typedef struct {
    const char *start; // a pointer to the first character of the token in the source code
    int length;        // token length in bytes
} TokenValue;

typedef struct {
    const char *filename;
    int line;
    int col;
} TokenLocation;

typedef struct {
    TokenLocation loc;
    TokenValue val;
    TokenType type;
} Token;

const char *tokenTypeToString(TokenType type);
TokenType findKeyword(const char *start, int length);
Token make_error_token(TokenLocation loc, const char *message);
