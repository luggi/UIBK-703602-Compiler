/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#ifndef RULES_H
#define RULES_H

static const unsigned int rules_count = 11;

void start(void);
void varDecList(void);
void type(void);
void compStmt(void);
void stmtList(void);
void statement(void);
void toPart(void);
void expr(void);
void simpleExpr(void);
void term(void);
void factor(void);

#endif
