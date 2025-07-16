#include "parser/parser.h"
#include <ctype.h>
#include <string.h>
#include <strings.h>

/*
 * Simple SQL tokenizer implementation.
 *
 * The tokenizer is deliberately minimal and only understands a handful of
 * keywords and symbols. It is used by the example program to demonstrate how a
 * higher-level parser might iterate over an input query string.
 */

static bool is_keyword(const char *start, size_t len, const char *kw)
{
    return strlen(kw) == len && strncasecmp(start, kw, len) == 0;
}

void parser_init(Parser *parser, const char *input)
{
    parser->input = input ? input : "";
    parser->current = parser->input;
}

static void skip_whitespace(Parser *parser)
{
    while (isspace((unsigned char)*parser->current))
        parser->current++;
}

Token parser_next_token(Parser *parser)
{
    skip_whitespace(parser);

    Token token;
    token.start = parser->current;
    token.length = 1;
    token.type = TOKEN_EOF;

    char c = *parser->current;
    if (c == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    parser->current++;

    switch (c) {
    case '*': token.type = TOKEN_STAR; break;
    case ',': token.type = TOKEN_COMMA; break;
    case ';': token.type = TOKEN_SEMICOLON; break;
    case '(': token.type = TOKEN_LPAREN; break;
    case ')': token.type = TOKEN_RPAREN; break;
    case '\'':
    {
        /* Parse string literal */
        token.start = parser->current;
        while (*parser->current && *parser->current != '\'')
            parser->current++;
        token.length = (size_t)(parser->current - token.start);
        token.type = TOKEN_STRING;
        if (*parser->current == '\'')
            parser->current++; /* consume closing quote */
        return token;
    }
    default:
        if (isalpha((unsigned char)c) || c == '_') {
            while (isalnum((unsigned char)*parser->current) || *parser->current == '_')
                parser->current++;
            token.length = (size_t)(parser->current - token.start);
            /* Check for keywords */
            if (is_keyword(token.start, token.length, "SELECT"))
                token.type = TOKEN_KEYWORD_SELECT;
            else if (is_keyword(token.start, token.length, "INSERT"))
                token.type = TOKEN_KEYWORD_INSERT;
            else if (is_keyword(token.start, token.length, "INTO"))
                token.type = TOKEN_KEYWORD_INTO;
            else if (is_keyword(token.start, token.length, "VALUES"))
                token.type = TOKEN_KEYWORD_VALUES;
            else if (is_keyword(token.start, token.length, "FROM"))
                token.type = TOKEN_KEYWORD_FROM;
            else
                token.type = TOKEN_IDENTIFIER;
            return token;
        } else if (isdigit((unsigned char)c)) {
            while (isdigit((unsigned char)*parser->current))
                parser->current++;
            token.length = (size_t)(parser->current - token.start);
            token.type = TOKEN_NUMBER;
            return token;
        }
        token.type = TOKEN_UNKNOWN;
        return token;
    }

    return token;
}
