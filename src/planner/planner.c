#include "planner/planner.h"
#include <string.h>

/* Extremely small query planner that understands two forms:
 * 1. SELECT * FROM <table>;
 * 2. INSERT INTO <table> VALUES ('value');
 */

bool planner_plan(const char *query, Plan *out_plan)
{
    if (!query || !out_plan)
        return false;
    Parser parser;
    parser_init(&parser, query);
    Token tok = parser_next_token(&parser);

    memset(out_plan, 0, sizeof(*out_plan));
    out_plan->type = PLAN_UNKNOWN;

    if (tok.type == TOKEN_KEYWORD_SELECT) {
        tok = parser_next_token(&parser); /* expect '*' */
        if (tok.type != TOKEN_STAR)
            return false;
        tok = parser_next_token(&parser); /* expect FROM */
        if (tok.type != TOKEN_KEYWORD_FROM)
            return false;
        tok = parser_next_token(&parser); /* table name */
        if (tok.type != TOKEN_IDENTIFIER)
            return false;
        size_t len = tok.length < sizeof(out_plan->table) - 1 ? tok.length : sizeof(out_plan->table) - 1;
        strncpy(out_plan->table, tok.start, len);
        out_plan->table[len] = '\0';
        out_plan->type = PLAN_SELECT_ALL;
        return true;
    } else if (tok.type == TOKEN_KEYWORD_INSERT) {
        tok = parser_next_token(&parser); /* expect INTO */
        if (tok.type != TOKEN_KEYWORD_INTO)
            return false;
        tok = parser_next_token(&parser); /* table name */
        if (tok.type != TOKEN_IDENTIFIER)
            return false;
        size_t len = tok.length < sizeof(out_plan->table) - 1 ? tok.length : sizeof(out_plan->table) - 1;
        strncpy(out_plan->table, tok.start, len);
        out_plan->table[len] = '\0';
        tok = parser_next_token(&parser); /* VALUES */
        if (tok.type != TOKEN_KEYWORD_VALUES)
            return false;
        tok = parser_next_token(&parser); /* LPAREN */
        if (tok.type != TOKEN_LPAREN)
            return false;
        tok = parser_next_token(&parser); /* string or number */
        if (tok.type != TOKEN_STRING && tok.type != TOKEN_NUMBER)
            return false;
        len = tok.length < sizeof(out_plan->value) - 1 ? tok.length : sizeof(out_plan->value) - 1;
        strncpy(out_plan->value, tok.start, len);
        out_plan->value[len] = '\0';
        tok = parser_next_token(&parser); /* RPAREN */
        if (tok.type != TOKEN_RPAREN)
            return false;
        out_plan->type = PLAN_INSERT_VALUE;
        return true;
    }

    return false;
}
