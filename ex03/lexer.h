/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#ifndef LEXER_H
#define LEXER_H

/* current linenumber of lexer*/
extern int yylineno;

/* text of the current token */
extern char *yytext;

/* returns next token */
extern int yylex(void);

/* put last read token back */
void yyunlex(void);

#endif
