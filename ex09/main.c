/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "list.h"
#include "parser.h"
#include "prascal_writer.h"
#include "symbol_table.h"

/* TODO: this should be part of parser.h */
extern void parser_create();
extern void parser_destroy(void);
extern int parser_run(void);

void exit_hook(void) {
    parser_destroy();
}

int main(int argc, char *argv[]) {
    parser_create();

    if (atexit(exit_hook) != 0) {
        fprintf(stderr, "could not register exit hook\n");
        exit(EXIT_FAILURE);
    }

    int ret = parser_run();

    if (ret == 0) {
        puts("{* input looks ok *}");
        puts("{* symbol table:");
        print_symbol_table();
        puts("*}");
        print_ast_as_prascal(SYNTAX_TREE);
    }

    return ret;
}
