/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#include "parser.h"

extern void yylex_destroy(void);

int main(int argc, char *argv[]) {
    int ret = yyparse();
    yylex_destroy();
    return ret;
}
