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

void exit_hook(void) {
    yylex_destroy();
    if (root) {
        node_destroy(root);
    }
}

int main(int argc, char *argv[]) {
    if (atexit(exit_hook) != 0) {
        fprintf(stderr, "could not register exit hook\n");
        exit(EXIT_FAILURE);
    }

    int ret = yyparse();

    if (ret == 0) {
        puts("{* input looks ok *}");
        print_ast_as_prascal(root);
        /* print_ast_as_graphviz(root); */
    }

    return ret;
}
