/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#ifndef PRASCAL_WRITER
#define PRASCAL_WRITER

#include "ast.h"

/* AST back to prascal */
void print_ast_as_prascal(const struct ast_node *node);

#endif
