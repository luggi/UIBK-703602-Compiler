/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "tokens.h"

static struct token current;

/* compares type with current token type, advances and returns true on match */
bool accept(const enum token_type type, ...) {
    if (current.type != type) {
        return false;
    }
    current = lexer_next();
    return true;
}

/* compare num types with current token type, behaves like accept */
bool accept_any(unsigned int num, ...) {
    va_list ap;
    va_start(ap, num);
    for (unsigned int i = 0; i < num; i++) {
        if (accept(va_arg(ap, const enum token_type))) {
            va_end(ap);
            return true;
        }
    }
    va_end(ap);
    return false;
}

/* compares type with current token, advances on match else call exit */
void expect(const enum token_type type) {
    if (!accept(type)) {
        fprintf(stderr, "Error: Line %d: read '%s', expected '%s'\n",
                current.line, token_type_string(current.type),
                token_type_string(type));
        lexer_destroy();
        exit(EXIT_FAILURE);
    }
}

/* print error message and exit */
void no_match(void) {
    fprintf(stderr, "Error: Line %d: unexpected token '%s'\n", current.line,
            token_type_string(current.type));
    lexer_destroy();
    exit(EXIT_FAILURE);
}

/* rules */
void start(void);
void varDecList(void);
void type(void);
void compStmt(void);
void stmtList(void);
void statement(void);
void toPart(void);
void expr(void);
void simpleExpr(void);
void term(void);
void factor(void);

void start(void) {
    expect(PROGRAM);
    expect(IDENT);
    expect(SEMCO);
    if (accept(VAR)) {
        varDecList();
    }
    compStmt();
    expect(DOT);
}

void varDecList(void) {
    expect(IDENT);
    do {
        while (accept(COMMA)) {
            expect(IDENT);
        }
        expect(COLON);
        type();
        expect(SEMCO);
    } while (accept(IDENT));
}

void type(void) {
    if (accept(ARRAY)) {
        expect(BRA_L);
        expect(NUM);
        expect(DDOT);
        expect(NUM);
        expect(BRA_R);
        expect(OF);
    }
    /* simpleType */
    if (accept_any(3, INTEGER, REAL, BOOLEAN)) {
        return;
    }
    no_match();
}

void compStmt(void) {
    expect(_BEGIN);
    stmtList();
    expect(END);
}

void stmtList(void) {
    statement();
    while (accept(SEMCO)) {
        statement();
    }
}

void statement(void) {
    /* assignStmt */
    if (accept(IDENT)) {
        if (accept(BRA_L)) {
            expr();
            expect(BRA_R);
        }
        expect(ASGN);
        expr();
        return;
    }

    /* compStmt */
    if (accept(_BEGIN)) {
        stmtList();
        expect(END);
        return;
    }

    /* ifStmt */
    if (accept(IF)) {
        expr();
        expect(THEN);
        statement();
        if (accept(ELSE)) {
            statement();
        }
        return;
    }

    /* whileStmt */
    if (accept(WHILE)) {
        expr();
        expect(DO);
        statement();
        return;
    }

    /* forStmt */
    if (accept(FOR)) {
        expect(IDENT);
        expect(ASGN);
        expr();
        toPart();
        expr();
        expect(DO);
        statement();
        return;
    }

    /* READ */
    if (accept(READ)) {
        expect(PAR_L);
        /* exprList */
        expr();
        while (accept(COMMA)) {
            expr();
        }
        expect(PAR_R);
        return;
    }

    /* WRITE */
    if (accept(WRITE)) {
        expect(PAR_L);
        /* exprList */
        expr();
        while (accept(COMMA)) {
            expr();
        }
        expect(PAR_R);
        return;
    }

    no_match();
}

void toPart(void) {
    if (accept_any(2, TO, DOWNTO)) {
        return;
    }
    no_match();
}

void expr(void) {
    simpleExpr();
    /* relOp */
    if (accept_any(6, LT, LEQ, GT, GEQ, EQ, NEQ)) {
        simpleExpr();
    }
}

void simpleExpr(void) {
    term();
    /* addOp */
    while (accept_any(3, PLUS, MINUS, OR)) {
        term();
    }
}

void term(void) {
    factor();
    /* mulOp */
    while (accept_any(5, ASTR, SLASH, DIV, MOD, AND)) {
        factor();
    }
}

void factor(void) {
    if (accept(IDENT)) {
        /* subscript */
        if (accept(BRA_L)) {
            expr();
            expect(BRA_R);
        }
        return;
    }
    if (accept_any(4, NUM, STRING, FALSE, TRUE)) {
        return;
    }
    if (accept_any(2, NOT, MINUS)) {
        factor();
        return;
    }
    if (accept(PAR_L)) {
        expr();
        expect(PAR_R);
        return;
    }
    no_match();
}

int main(int argc, char *argv[]) {
    current = lexer_create();

    start();
    expect(_EOF);

    puts("input looks ok");

    lexer_destroy();

    return EXIT_SUCCESS;
}
