/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#ifndef LEXER_H
#define LEXER_H

#include "tokens.h"

/* create lexer instance */
void lexer_create(void);

/* destroy lexer instance */
void lexer_destroy(void);

/* returns next token */
struct token lexer_next(void);

#endif
