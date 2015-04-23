/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

%{
#include <stdio.h>
extern int yylineno;
extern char *yytext;
%}

%error-verbose

%token _EOF 0 "end of file"

%token AND ARRAY _BEGIN DIV DO DOWNTO ELSE END FOR IF MOD NOT OF OR PROGRAM READ THEN TO VAR WHILE WRITE ASTR PLUS COMMA MINUS DOT DDOT SLASH COLON ASGN SEMCO LT LEQ NEQ EQ GT GEQ CARET PAR_L PAR_R BRA_L BRA_R BOOLEAN INTEGER REAL FALSE TRUE NUM STRING IDENT
%start start

%%
start                   : PROGRAM IDENT SEMCO varDec compStmt DOT
                        ;

varDec                  : VAR varDecList
                        |
                        ;

varDecList              : varDecList identListType SEMCO
                        | identListType SEMCO
                        ;

identListType           : identList COLON type
                        ;

identList               : identList COMMA IDENT
                        | IDENT
                        ;

type                    : simpleType
                        | ARRAY BRA_L NUM DDOT NUM BRA_R OF simpleType
                        ;

simpleType              : INTEGER
                        | REAL
                        | BOOLEAN
                        ;

compStmt                : _BEGIN stmtList END
                        ;

stmtList                : stmtList SEMCO statement
                        | statement
                        ;

statement               : assignStmt
                        | compStmt
                        | ifStmt
                        | whileStmt
                        | forStmt
                        | READ PAR_L exprList PAR_R
                        | WRITE PAR_L exprList PAR_R
                        ;

assignStmt              : IDENT ASGN expr
                        | IDENT BRA_L expr BRA_R ASGN expr
                        ;

ifStmt                  : IF expr THEN statement elsePart
                        ;

elsePart                : ELSE statement
                        |
                        ;

whileStmt               : WHILE expr DO statement
                        ;

forStmt                 : FOR IDENT ASGN expr toPart expr DO statement
                        ;

toPart                  : TO
                        | DOWNTO
                        ;

expr                    : simpleExpr relOp simpleExpr
                        | simpleExpr
                        ;

exprList                : exprList COMMA expr
                        | expr
                        ;

simpleExpr              : simpleExpr addOp term
                        | term
                        ;

term                    : term mulOp factor
                        | factor
                        ;

factor                  : NUM
                        | STRING
                        | FALSE
                        | TRUE
                        | IDENT
                        | IDENT BRA_L expr BRA_R
                        | NOT factor
                        | MINUS factor
                        | PAR_L expr PAR_R
                        ;

relOp                   : LT
                        | LEQ
                        | GT
                        | GEQ
                        | EQ
                        | NEQ
                        ;

addOp                   : PLUS
                        | MINUS
                        | OR
                        ;

mulOp                   : ASTR
                        | SLASH
                        | DIV
                        | MOD
                        | AND
                        ;
%%

int main(int argc, char *argv[])
{
    return yyparse();
}

int yyerror(const char *message) {
    fprintf(stderr, "Error: Line %d: %s got \"%s\"\n", yylineno, message, yytext);
    return 0;
}

