/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#ifndef TOKENS_H
#define TOKENS_H

enum token {

    /* EOF = 0 */
    _EOF,

    /* keywords */
    AND,
    ARRAY,
    _BEGIN,
    DIV,
    DO,
    ELSE,
    END,
    FOR,
    IF,
    MOD,
    NOT,
    OF,
    PROGRAM,
    THEN,
    TO,
    VAR,
    WHILE,

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
    BOOL,
    INTEGER,
    REAL,

    /* literals */
    FALSE,
    TRUE,
    NUM,
    STR,

    /* identifier */
    IDENT

};

#endif
