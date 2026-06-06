#include <stdlib.h>

#include "token.h"

const char *tokenTypeToString(TokenType type) {
    static const char *strings[] = {
        [T_ILLEGAL] = "illegal character",
        [T_EOF] = "end of file",
        [T_NEWLINE] = "newline",

        [T_MINUS] = "'-'",
        [T_PLUS] = "'+'",
        [T_STAR] = "'*'",
        [T_SLASH] = "'/'",
        [T_ASSIGN] = "'='",
        [T_LPAREN] = "'('",
        [T_RPAREN] = "')'",
        [T_LBRACE] = "'}'",
        [T_RBRACE] = "'}'",
        [T_COMMA] = "','",
        [T_DOT] = "'.'",

        [T_NUMBER] = "number",
        [T_IDENT] = "identifier",
    };

    if (type < 0 || type >= sizeof(strings) / sizeof(strings[0]) ||
        strings[type] == NULL) {
        return "unknown token";
    }

    return strings[type];
}
