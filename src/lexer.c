#include <ctype.h>

#include "lexer.h"
#include "token.h"

Lexer lexer_init(const char *source, const char *filename) {
    Lexer l = {
        .source = source,
        .loc.line = 1,
        .loc.col = 1,
        .loc.filename = filename,
    };
    return l;
}

Token nextToken(Lexer *l) {
    // skip whitespaces
    while (l->source[l->pos] == ' ' || l->source[l->pos] == '\t' ||
           l->source[l->pos] == '\r') {
        l->pos++;
        l->loc.col++;
    }
    Token t = {.loc = l->loc, .val.start = &l->source[l->pos], .val.length = 1};
    if (l->source[l->pos] == '\0') {
        t.type = T_EOF;
        return t;
    }
    char c = l->source[l->pos];
    // check single characters
    l->pos++;
    l->loc.col++;
    switch (c) {
    case '\n':
        t.type = T_NEWLINE;
        l->loc.line++;
        l->loc.col = 1;
        return t;
    case '+':
        t.type = T_PLUS;
        return t;
    case '-':
        t.type = T_MINUS;
        return t;
    case '*':
        t.type = T_STAR;
        return t;
    case '/':
        // handle single-line comments
        if (l->source[l->pos] == '/') {
            l->pos++;
            l->loc.col++;
            while (l->source[l->pos] != '\0') {
                if (l->source[l->pos] == '\n') {
                    l->pos++;
                    l->loc.line++;
                    l->loc.col = 1;
                    break;
                }
                l->pos++;
                l->loc.col++;
            }
            return nextToken(l);
        }
        t.type = T_SLASH;
        return t;
    case '"': {
        int startPos = l->pos;
        while (l->source[l->pos] != '\0' && l->source[l->pos] != '"') {
            if (l->source[l->pos] == '\n') {
                Token token = make_error_token(l->loc, "Unterminated string");
                l->pos++;
                l->loc.line++;
                l->loc.col = 1;
                return token;
            }

            l->pos++;
            l->loc.col++;
        }
        if (l->source[l->pos] == '"') {
            t.val.start = &l->source[startPos];
            t.val.length = l->pos - startPos;
            t.type = T_STRING;

            l->pos++;
            l->loc.col++;
            return t;
        }
        return nextToken(l);
    }
    case '=':
        t.type = T_ASSIGN;
        return t;
    case '>':
        t.type = T_GREATER;
        return t;
    case '<':
        t.type = T_LESS;
        return t;
    case '.':
        t.type = T_DOT;
        return t;
    case ',':
        t.type = T_COMMA;
        return t;
    case '(':
        t.type = T_LPAREN;
        return t;
    case ')':
        t.type = T_RPAREN;
        return t;
    case '{':
        t.type = T_LBRACE;
        return t;
    case '}':
        t.type = T_RBRACE;
        return t;
    }
    // If the character wasn't a single-character token, roll back
    // the pointer to parse it as part of a multi-character literal (number/identifier).
    l->pos--;
    l->loc.col--;

    int startPos = l->pos;
    // parse numeric literals
    if (isdigit(c)) {
        while (isdigit(l->source[l->pos])) {
            l->pos++;
            l->loc.col++;
        }
        t.type = T_NUMBER;
        t.val.length = l->pos - startPos;
        return t;
    }
    // parse identifiers
    if (isalnum(c)) {
        while (isalnum(l->source[l->pos])) {
            l->pos++;
            l->loc.col++;
        }
        t.val.length = l->pos - startPos;
        t.type = findKeyword(t.val.start, t.val.length);
        return t;
    }
    t.type = T_ILLEGAL;
    return t;
}
