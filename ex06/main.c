/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#include <stdio.h>

#include "ast.h"
#include "parser.h"

/* root node of AST */
extern struct ast_node *root;

/* old bison version does not declare yyparse in header */
extern int yyparse(void);

extern void yylex_destroy(void);

int main(int argc, char *argv[]) {
    int ret = yyparse();

    if (ret == 0) {
        puts("input looks ok");
    }

    puts("- - - - -   PRINTING  PRASCAL  - - - - -");
    print_as_prascal(root);

    yylex_destroy();
    return ret;
}
