/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#ifndef TOKENS_H
#define TOKENS_H

enum token_type {

    /* EOF = 0 */
    _EOF,

    /* keywords */
    AND,
    ARRAY,
    _BEGIN,
    DIV,
    DO,
    DOWNTO,
    ELSE,
    END,
    FOR,
    IF,
    MOD,
    NOT,
    OF,
    OR,
    PROGRAM,
    READ,
    THEN,
    TO,
    VAR,
    WHILE,
    WRITE,

    /* opreators */
    ASTR,
    PLUS,
    COMMA,
    MINUS,
    DOT,
    DDOT,
    SLASH,
    COLON,
    ASGN,
    SEMCO,
    LT,
    LEQ,
    NEQ,
    EQ,
    GT,
    GEQ,
    CARET,

    /* parentheses */
    PAR_L,
    PAR_R,
    BRA_L,
    BRA_R,

    /* types */
    BOOLEAN,
    INTEGER,
    REAL,

    /* literals */
    FALSE,
    TRUE,
    NUM,
    STRING,

    /* identifier */
    IDENT

};

struct token {
    enum token_type type;
    char *text;
    unsigned int line;
};

/* convert given token type to string */
const char *token_type_string(const enum token_type type);

#endif
