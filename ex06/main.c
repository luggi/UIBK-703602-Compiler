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
#include "parser.h"

/* root node of AST */
extern struct ast_node *root;

/* old bison version does not declare yyparse in header */
extern int yyparse(void);

extern void yylex_destroy(void);

void on_exit(void) {
    yylex_destroy();
    node_destroy(root);
}

int main(int argc, char *argv[]) {
    int ret = yyparse();
    if (atexit(on_exit) != 0) {
        fprintf(stderr, "could not register exit hook\n");
        exit(EXIT_FAILURE);
    }

    if (ret == 0) {
        puts("input looks ok");
        puts("- - - - -   PRINTING  PRASCAL  - - - - -");
        print_as_prascal(root);
    }

    return ret;
}
