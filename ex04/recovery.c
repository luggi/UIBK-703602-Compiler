/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#include "recovery.h"

#include <stdlib.h>

#include "rules.h"
#include "tokens.h"

/* FIRST */
static enum token_type start_first[]      = {PROGRAM};
static enum token_type varDecList_first[] = {IDENT};
static enum token_type type_first[]       = {ARRAY, INTEGER, REAL, BOOLEAN};
static enum token_type compStmt_first[]   = {_BEGIN};
static enum token_type stmtList_first[]   = {IDENT, _BEGIN, IF, WHILE, FOR, READ, WRITE};
static enum token_type statement_first[]  = {IDENT, _BEGIN, IF, WHILE, FOR, READ, WRITE};
static enum token_type toPart_first[]     = {TO, DOWNTO};
static enum token_type expr_first[]       = {IDENT, NUM, STRING, FALSE, TRUE, NOT, MINUS, PAR_L};
static enum token_type simpleExpr_first[] = {IDENT, NUM, STRING, FALSE, TRUE, NOT, MINUS, PAR_L};
static enum token_type term_first[]       = {IDENT, NUM, STRING, FALSE, TRUE, NOT, MINUS, PAR_L};
static enum token_type factor_first[]     = {IDENT, NUM, STRING, FALSE, TRUE, NOT, MINUS, PAR_L};

static struct rule_token_pair FIRST[] = {
    {start,        1,   start_first},
    {varDecList,   1,   varDecList_first},
    {type,         4,   type_first},
    {compStmt,     1,   compStmt_first},
    {stmtList,     7,   stmtList_first},
    {statement,    7,   statement_first},
    {toPart,       2,   toPart_first},
    {expr,         8,   expr_first},
    {simpleExpr,   8,   simpleExpr_first},
    {term,         8,   term_first},
    {factor,       8,   factor_first},
};

/* FOLLOW */
static enum token_type start_follow[]      = {/* TODO */};
static enum token_type varDecList_follow[] = {/* TODO */};
static enum token_type type_follow[]       = {/* TODO */};
static enum token_type compStmt_follow[]   = {/* TODO */};
static enum token_type stmtList_follow[]   = {/* TODO */};
static enum token_type statement_follow[]  = {/* TODO */};
static enum token_type toPart_follow[]     = {/* TODO */};
static enum token_type expr_follow[]       = {/* TODO */};
static enum token_type simpleExpr_follow[] = {/* TODO */};
static enum token_type term_follow[]       = {/* TODO */};
static enum token_type factor_follow[]     = {/* TODO */};

static struct rule_token_pair FOLLOW[] = {
    {start,        0,   start_follow},
    {varDecList,   0,   varDecList_follow},
    {type,         0,   type_follow},
    {compStmt,     0,   compStmt_follow},
    {stmtList,     0,   stmtList_follow},
    {statement,    0,   statement_follow},
    {toPart,       0,   toPart_follow},
    {expr,         0,   expr_follow},
    {simpleExpr,   0,   simpleExpr_follow},
    {term,         0,   term_follow},
    {factor,       0,   factor_follow},
};

const struct rule_token_pair *recovery_lookup_first(void (*key)(void)) {
    for (unsigned int i = 0; i < rules_count; i++) {
        if (FIRST[i].rule == key) {
            return &FIRST[i];
        }
    }
    return NULL;
}

const struct rule_token_pair *recovery_lookup_follow(void (*key)(void)) {
    for (unsigned int i = 0; i < rules_count; i++) {
        if (FOLLOW[i].rule == key) {
            return &FOLLOW[i];
        }
    }
    return NULL;
}
