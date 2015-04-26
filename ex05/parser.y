/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

%{
#include <stdio.h>

extern int yylex(void);
extern int yylineno;
extern char *yytext;

void yyerror(const char *message);
%}

%error-verbose

%token _EOF 0 "end of file"
%token _BEGIN "BEGIN"
%token AND ARRAY DIV DO DOWNTO END FOR IF MOD NOT OF OR PROGRAM READ TO VAR
%token WHILE WRITE BOOLEAN INTEGER REAL FALSE TRUE NUM STRING IDENT
%token ASTR "*"
%token PLUS "+"
%token COMMA ","
%token MINUS "-"
%token DOT "."
%token DDOT ".."
%token SLASH "/"
%token COLON ":"
%token ASGN ":="
%token SEMCO ";"
%token LT "<"
%token LEQ "<="
%token NEQ "<>"
%token EQ "="
%token GT ">"
%token GEQ ">="
%token CARET "^"
%token PAR_L "("
%token PAR_R ")"
%token BRA_L "["
%token BRA_R "]"

/* dangling else */
%right THEN ELSE

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

ifStmt                  : IF expr THEN statement
                        | IF expr THEN statement ELSE statement
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

void yyerror(const char *message) {
    fprintf(stderr, "Error: Line %d: %s got \"%s\"\n", yylineno, message,
            yytext);
}
