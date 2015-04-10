/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

/* create lexer instance */
bool lexer_create(void);

/* destroy lexer instance */
void lexer_destroy(void);

/* get next token from lexer */
struct token *lexer_next(void);

/* undo last next */
void lexer_undo(void);

/* returns last token in list */
struct token *lexer_last(void);

/* get current lexer position */
unsigned long int lexer_save(void);

/* set lexer position */
void lexer_restore(unsigned long int saved);

#endif
