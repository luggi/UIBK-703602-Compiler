/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "tokens.h"

static struct token current;

/* compares type with current token type, advances and returns true on match */
bool accept(const enum token_type type) {
    if (current.type != type) {
        return false;
    }
    current = lexer_next();
    return true;
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
/* TODO */
void start(void);
void start(void) {
    if (accept(PAR_L)) {
        expect(NUM);
        expect(PLUS);
        expect(NUM);
        expect(PAR_R);
    } else if (accept(BRA_L)) {
        expect(NUM);
        expect(PLUS);
        expect(NUM);
        expect(BRA_R);
    } else {
        no_match();
    }
}

int main(int argc, char *argv[]) {
    lexer_create();

    current = lexer_next();

    start();

    puts("input looks ok");

    lexer_destroy();

    return EXIT_SUCCESS;
}
