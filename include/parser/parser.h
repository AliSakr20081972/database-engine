#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

/**
 * @file parser.h
 * @brief Minimal tokenizer for a subset of SQL.
 *
 * The parser module exposes a very small tokenizer used by the demo program
 * to iterate over keywords, identifiers and punctuation in an input string.
 */

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Token types for a very small subset of SQL */
typedef enum TokenType {
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STAR,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_STRING,
    TOKEN_KEYWORD_SELECT,
    TOKEN_KEYWORD_INSERT,
    TOKEN_KEYWORD_INTO,
    TOKEN_KEYWORD_VALUES,
    TOKEN_UNKNOWN
} TokenType;

typedef struct Token {
    TokenType type;
    const char *start;
    size_t length;
} Token;

typedef struct Parser {
    const char *input;
    const char *current;
} Parser;

/* Initialize the parser with an input string */
void parser_init(Parser *parser, const char *input);

/* Obtain the next token from the input */
Token parser_next_token(Parser *parser);

#ifdef __cplusplus
}
#endif

#endif /* PARSER_PARSER_H */
