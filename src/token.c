#include <string.h>

#include "token.h"

const char *tokenTypeToString(TokenType type) {
    static const char *strings[] = {[T_ILLEGAL] = "illegal character",
                                    [T_ERROR] = "error message",
                                    [T_EOF] = "end of file",
                                    [T_NEWLINE] = "newline",

                                    [T_MINUS] = "'-'",
                                    [T_PLUS] = "'+'",
                                    [T_STAR] = "'*'",
                                    [T_SLASH] = "'/'",
                                    [T_ASSIGN] = "'='",
                                    [T_GREATER] = "'>'",
                                    [T_LESS] = "'<'",

                                    [T_LPAREN] = "'('",
                                    [T_RPAREN] = "')'",
                                    [T_LBRACE] = "'}'",
                                    [T_RBRACE] = "'}'",

                                    [T_COMMA] = "','",
                                    [T_DOT] = "'.'",

                                    [T_NUMBER] = "number",
                                    [T_STRING] = "string",
                                    [T_IDENT] = "identifier",

                                    [T_VAR] = "var",
                                    [T_IF] = "if",
                                    [T_ELSE] = "else",
                                    [T_WHILE] = "while"};

    if (type < 0 || type >= sizeof(strings) / sizeof(strings[0]) ||
        strings[type] == NULL) {
        return "unknown token";
    }

    return strings[type];
}

TokenType findKeyword(const char *start, int length) {
    if (memcmp(start, "var", length) == 0)
        return T_VAR;
    if (memcmp(start, "if", length) == 0)
        return T_IF;
    if (memcmp(start, "else", length) == 0)
        return T_ELSE;
    if (memcmp(start, "while", length) == 0)
        return T_WHILE;
    return T_IDENT;
}

Token make_error_token(TokenLocation loc, const char *message) {
    Token token = {.type = T_ERROR,
                   .val.start = message,
                   .val.length = (int)strlen(message),
                   .loc.line = loc.line,
                   .loc.col = loc.col,
                   .loc.filename = loc.filename};
    return token;
}
