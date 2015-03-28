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

/* list of rules */
void start(void);

/* compares `expect` with next token, calls `exit()` on missmatch */
void expect(const enum token expect) {
    enum token read = yylex();
    if (read != expect) {
        fprintf(stderr, "Error in %d: read '%s', expected '%s'\n",
                yylineno, token_string(read), token_string(expect));
        exit(EXIT_FAILURE);
    }
}

/* print error message and exit */
void no_match(const enum token t) {
    fprintf(stderr, "Error in %d: unexpected token '%s'\n", yylineno,
            token_string(t));
    exit(EXIT_FAILURE);
}

/* rules: */
void start(void) {
    enum token t = yylex();
    switch (t) {
        case PAR_L:
            expect(NUM);
            expect(PLUS);
            expect(NUM);
            expect(PAR_R);
            break;

        case BRA_L:
            expect(NUM);
            expect(PLUS);
            expect(NUM);
            expect(BRA_R);
            break;

        default:
            no_match(t);
    }
}

int main(int argc, char *argv[]) {

    /* example:
     * ( 1 + 2 )
     * [ 1 + 2 ]
     */
    start();

    return EXIT_SUCCESS;
}
