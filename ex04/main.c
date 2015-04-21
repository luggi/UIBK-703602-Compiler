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

#include "stack.h"
#include "lexer.h"
#include "recovery.h"
#include "rules.h"
#include "tokens.h"

#define RULE_PREFIX(); \
    if (panic_mode) { \
        return; \
    }

#define RULE_SUFFIX(); \
    if (panic_mode) { \
        if (panic_recovery() == 1) { \
            goto top; \
        } else { \
            stack_pop(call_stack); \
            return; \
        } \
    } \
    stack_pop(call_stack);

/* holds the current token received from the lexer */
static struct token current;

/* holds call path of non-terminal */
static struct stack *call_stack;

/* shows whether panic mode is on */
static bool panic_mode = false;

/* compares type with current token type, advances and returns true on match */
bool accept(const enum token_type type, ...) {
    if (panic_mode || current.type != type) {
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
    if (panic_mode) {
        return;
    }
    if (!accept(type)) {
        fprintf(stderr, "Error: Line %d: read '%s', expected '%s'\n",
                current.line, token_type_string(current.type),
                token_type_string(type));

        /* engage panic mode */
        panic_mode = true;
    }
}

/* scans a head until token in FIRST or FOLLOW is found returns 1 if token in
 * FIRST, 2 if token in FOLLOW or exists if EOF is reached
 */
unsigned int panic_recovery(void) {
    const struct rule_token_pair *first = recovery_lookup_first(
            (void (*)(void)) stack_peek(call_stack));
    const struct rule_token_pair *follow = recovery_lookup_follow(
            (void (*)(void)) stack_peek(call_stack));

    while (current.type != _EOF) {
        for (unsigned int i = 0; i < first->num; i++) {
            if (first->types[i] == current.type) {
                panic_mode = false;
                fprintf(stderr, "Recover: Line %d: recovering using '%s'"
                        " (FIRST)\n", current.line,
                        token_type_string(current.type));
                return 1;
            }
        }
        for (unsigned int i = 0; i < follow->num; i++) {
            if (follow->types[i] == current.type) {
                panic_mode = false;
                fprintf(stderr, "Recover: Line %d: recovering using '%s'"
                        " (FOLLOW)\n", current.line,
                        token_type_string(current.type));
                return 2;
            }
        }
        current = lexer_next();
    }

    fprintf(stderr, "Recover: EOF reached, could not recover\n");
    stack_destroy(call_stack);
    lexer_destroy();
    exit(EXIT_FAILURE);
}

void start(void) {
    RULE_PREFIX();

    stack_push(call_stack, start);

top:
    expect(PROGRAM);
    expect(IDENT);
    expect(SEMCO);
    if (accept(VAR)) {
        varDecList();
    }
    compStmt();
    expect(DOT);

    RULE_SUFFIX();
}

void varDecList(void) {
    RULE_PREFIX();

    stack_push(call_stack, varDecList);

top:
    expect(IDENT);
    do {
        while (accept(COMMA)) {
            expect(IDENT);
        }
        expect(COLON);
        type();
        expect(SEMCO);
    } while (accept(IDENT));

    RULE_SUFFIX();
}

void type(void) {
    RULE_PREFIX();

    stack_push(call_stack, type);

top:
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
        goto ret;
    }

    /* must not be empty */
    panic_mode = true;

ret:
    RULE_SUFFIX();
}

void compStmt(void) {
    RULE_PREFIX();

    stack_push(call_stack, compStmt);

top:
    expect(_BEGIN);
    stmtList();
    expect(END);

    RULE_SUFFIX();
}

void stmtList(void) {
    RULE_PREFIX();

    stack_push(call_stack, stmtList);

top:
    statement();
    while (accept(SEMCO)) {
        statement();
    }

    RULE_SUFFIX();
}

void statement(void) {
    RULE_PREFIX();

    stack_push(call_stack, statement);

top:
    /* assignStmt */
    if (accept(IDENT)) {
        if (accept(BRA_L)) {
            expr();
            expect(BRA_R);
        }
        expect(ASGN);
        expr();
        goto ret;
    }

    /* compStmt */
    if (accept(_BEGIN)) {
        stmtList();
        expect(END);
        goto ret;
    }

    /* ifStmt */
    if (accept(IF)) {
        expr();
        expect(THEN);
        statement();
        if (accept(ELSE)) {
            statement();
        }
        goto ret;
    }

    /* whileStmt */
    if (accept(WHILE)) {
        expr();
        expect(DO);
        statement();
        goto ret;
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
        goto ret;
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
        goto ret;
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
        goto ret;
    }

    /* must not be empty */
    panic_mode = true;

ret:
    RULE_SUFFIX();
}

void toPart(void) {
    RULE_PREFIX();

    stack_push(call_stack, toPart);

top:
    if (accept_any(2, TO, DOWNTO)) {
        goto ret;
    }

    /* must not be empty */
    panic_mode = true;

ret:
    RULE_SUFFIX();
}

void expr(void) {
    RULE_PREFIX();

    stack_push(call_stack, expr);

top:
    simpleExpr();
    /* relOp */
    if (accept_any(6, LT, LEQ, GT, GEQ, EQ, NEQ)) {
        simpleExpr();
    }

    RULE_SUFFIX();
}

void simpleExpr(void) {
    RULE_PREFIX();

    stack_push(call_stack, simpleExpr);

top:
    term();
    /* addOp */
    while (accept_any(3, PLUS, MINUS, OR)) {
        term();
    }

    RULE_SUFFIX();
}

void term(void) {
    RULE_PREFIX();

    stack_push(call_stack, term);

top:
    factor();
    /* mulOp */
    while (accept_any(5, ASTR, SLASH, DIV, MOD, AND)) {
        factor();
    }

    RULE_SUFFIX();
}

void factor(void) {
    RULE_PREFIX();

    stack_push(call_stack, factor);

top:
    if (accept(IDENT)) {
        /* subscript */
        if (accept(BRA_L)) {
            expr();
            expect(BRA_R);
        }
        goto ret;
    }

    if (accept_any(4, NUM, STRING, FALSE, TRUE)) {
        goto ret;
    }

    if (accept_any(2, NOT, MINUS)) {
        factor();
        goto ret;
    }

    if (accept(PAR_L)) {
        expr();
        expect(PAR_R);
        goto ret;
    }

    /* must not be empty */
    panic_mode = true;

ret:
    RULE_SUFFIX();
}

int main(int argc, char *argv[]) {
    current = lexer_create();

    call_stack = stack_create();

    start();
    expect(_EOF);

    puts("input looks ok");

    stack_destroy(call_stack);

    lexer_destroy();

    return EXIT_SUCCESS;
}
