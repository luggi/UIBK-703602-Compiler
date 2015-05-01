/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

%{
#include <stdio.h>
#include <stdbool.h>

#include "ast.h"

extern int yylex(void);
extern int yylineno;
extern char *yytext;

void yyerror(const char *message);

struct ast_node *root;
%}

%error-verbose

%union {
    /* text of identifier */
    char *identifier;

    /* const value */
    int number;
    char *string;

    /* AST node */
    struct ast_node *node;
}

/* identifier */
%token <identifier> IDENT

/* literals */
%token <number> NUM
%token <string> STR

/* nodes */
%type <node> start varDecList identListType identList type simpleType
%type <node> compStmt stmtList statement assignStmt ifStmt whileStmt forStmt
%type <node> toPart expr exprList simpleExpr term factor ident num str relOp
%type <node> addOp mulOp

/* EOF */
%token _EOF 0 "end of file"

/* keywords compStmt*/
%token _BEGIN "BEGIN"
%token END

/* keywords ifStmt */
%token AND ARRAY DIV DO DOWNTO FOR IF MOD NOT OF OR PROGRAM READ TO VAR WHILE
%token WRITE BOOLEAN INTEGER REAL TRUE FALSE

/* dangling else */
%right THEN ELSE

/* general symbols */
%token COMMA        ","
%token DOT          "."
%token DDOT         ".."
%token COLON        ":"
%token ASGN         ":="
%token SEMCO        ";"

/* relOp symbols */
%token LT           "<"
%token LEQ          "<="
%token NEQ          "<>"
%token EQ           "="
%token GT           ">"
%token GEQ          ">="

/* addOp symbols */
%token PLUS         "+"
%token MINUS        "-"

/* mulOp symbols */
%token ASTR         "*"
%token SLASH        "/"

/* parentheses */
%token PAR_L        "("
%token PAR_R        ")"
%token BRA_L        "["
%token BRA_R        "]"

%start start

%%

start                   : PROGRAM ident SEMCO VAR varDecList compStmt DOT   { $$ = node_new(NODE_PROGRAM); $$->body[0] = $2; $$->body[1] = $5; $$->body[2] = $6; root = $$; }
                        | PROGRAM ident SEMCO compStmt DOT                  { $$ = node_new(NODE_PROGRAM); $$->body[0] = $2; $$->body[1] = $4; root = $$; }
                        ;

varDecList              : identListType SEMCO varDecList                    { $$ = node_new(NODE_VARDECLIST); $$->body[0] = $1; $$->next = $3; }
                        | identListType SEMCO                               { $$ = node_new(NODE_VARDECLIST); $$->body[0] = $1; }
                        ;

identListType           : identList COLON type                              { $$ = node_new(NODE_IDENTLISTTYPE); $$->body[0] = $1; $$->body[1] = $3; }
                        ;

identList               : ident COMMA identList                             { $$ = node_new(NODE_IDENTLIST); $$->body[0] = $1; $$->next = $3; }
                        | ident                                             { $$ = node_new(NODE_IDENTLIST); $$->body[0] = $1; }
                        ;

type                    : simpleType                                        { $$ = node_new(NODE_TYPE); $$->body[0] = $1; }
                        | ARRAY BRA_L num DDOT num BRA_R OF simpleType      { $$ = node_new(NODE_TYPE); $$->body[0] = $3; $$->body[1] = $5; $$->body[2] = $8; }
                        ;

simpleType              : INTEGER                                           { $$ = node_new(NODE_SIMPLETYPE); $$->token = INTEGER; }
                        | REAL                                              { $$ = node_new(NODE_SIMPLETYPE); $$->token = REAL; }
                        | BOOLEAN                                           { $$ = node_new(NODE_SIMPLETYPE); $$->token = BOOLEAN; }
                        ;

compStmt                : _BEGIN stmtList END                               { $$ = $2; }
                        ;

stmtList                : statement SEMCO stmtList                          { $$ = node_new(NODE_STMTLIST); $$->body[0] = $1; $$->next = $3; }
                        | statement                                         { $$ = node_new(NODE_STMTLIST); $$->body[0] = $1; }
                        ;

statement               : assignStmt                                        { $$ = $1; }
                        | compStmt                                          { $$ = $1; }
                        | ifStmt                                            { $$ = $1; }
                        | whileStmt                                         { $$ = $1; }
                        | forStmt                                           { $$ = $1; }
                        | READ PAR_L exprList PAR_R                         { $$ = node_new(STMT_READ); $$->body[0] = $3; }
                        | WRITE PAR_L exprList PAR_R                        { $$ = node_new(STMT_WRITE); $$->body[0] = $3; }
                        ;

assignStmt              : ident ASGN expr                                   { $$ = node_new(STMT_ASSIGN); $$->body[0] = $1; $$->body[1] = $3; }
                        | ident BRA_L expr BRA_R ASGN expr                  { $$ = node_new(STMT_ASSIGN); $$->body[0] = $1; $$->body[1] = $3; $$->body[2] = $6; }
                        ;

ifStmt                  : IF expr THEN statement                            { $$ = node_new(STMT_IF); $$->body[0] = $2; $$->body[1] = $4; }
                        | IF expr THEN statement ELSE statement             { $$ = node_new(STMT_IF); $$->body[0] = $2; $$->body[1] = $4; $$->body[2] = $6; }
                        ;

whileStmt               : WHILE expr DO statement                           { $$ = node_new(STMT_WHILE); $$->body[0] = $2; $$->body[1] = $4; }
                        ;

forStmt                 : FOR ident ASGN expr toPart expr DO statement      { $$ = node_new(STMT_FOR); $$->body[0] = $2; $$->body[1] = $4; $$->body[2] = $5; $$->body[3] = $6; $$->body[4] = $8; }
                        ;

toPart                  : TO                                                { $$ = node_new(NODE_TOPART); $$->token = TO; }
                        | DOWNTO                                            { $$ = node_new(NODE_TOPART); $$->token = DOWNTO; }
                        ;

expr                    : simpleExpr relOp simpleExpr                       { $$ = node_new(NODE_EXPR); $$->body[0] = $1; $$->body[1] = $2; $$->body[2] = $3; }
                        | simpleExpr                                        { $$ = node_new(NODE_EXPR); $$->body[0] = $1; }
                        ;

exprList                : expr COMMA exprList                               { $$ = node_new(NODE_EXPRLIST); $$->body[0] = $1; $$->next = $3; }
                        | expr                                              { $$ = node_new(NODE_EXPRLIST); $$->body[0] = $1; }
                        ;

simpleExpr              : term addOp simpleExpr                             { $$ = node_new(NODE_SIMPLEEXPR); $$->body[0] = $1; $$->body[1] = $2; $$->body[2] = $3; }
                        | term                                              { $$ = node_new(NODE_SIMPLEEXPR); $$->body[0] = $1; }
                        ;

term                    : factor mulOp term                                 { $$ = node_new(NODE_TERM); $$->body[0] = $1; $$->body[1] = $2; $$->body[2] = $3; }
                        | factor                                            { $$ = node_new(NODE_TERM); $$->body[0] = $1; }
                        ;

factor                  : ident                                             { $$ = $1; }
                        | ident BRA_L expr BRA_R                            { $$ = node_new(NODE_IDENT_SUBSCR); $$->body[0] = $1; $$->body[1] = $3; }
                        | num                                               { $$ = $1; }
                        | str                                               { $$ = $1; }
                        | TRUE                                              { $$ = node_new(NODE_BOOL); $$->bValue = true; }
                        | FALSE                                             { $$ = node_new(NODE_BOOL); $$->bValue = false; }
                        | NOT factor                                        { $$ = node_new(FACTOR_NOT); $$->body[0] = $2; }
                        | MINUS factor                                      { $$ = node_new(FACTOR_MINUS); $$->body[0] = $2; }
                        | PAR_L expr PAR_R                                  { $$ = node_new(FACTOR_EXPR); $$->body[0] = $2; }
                        ;

ident                   : IDENT                                             { $$ = node_new(NODE_IDENT); $$->ident = $1; }
                        ;

num                     : NUM                                               { $$ = node_new(NODE_NUM); $$->iValue = $1; }
                        ;

str                     : STR                                               { $$ = node_new(NODE_STR); $$->sValue = $1; }
                        ;

relOp                   : LT                                                { $$ = node_new(NODE_RELOP); $$->token = LT; }
                        | LEQ                                               { $$ = node_new(NODE_RELOP); $$->token = LEQ; }
                        | GT                                                { $$ = node_new(NODE_RELOP); $$->token = GT; }
                        | GEQ                                               { $$ = node_new(NODE_RELOP); $$->token = GEQ; }
                        | EQ                                                { $$ = node_new(NODE_RELOP); $$->token = EQ; }
                        | NEQ                                               { $$ = node_new(NODE_RELOP); $$->token = NEQ; }
                        ;

addOp                   : PLUS                                              { $$ = node_new(NODE_ADDOP); $$->token = PLUS; }
                        | MINUS                                             { $$ = node_new(NODE_ADDOP); $$->token = MINUS; }
                        | OR                                                { $$ = node_new(NODE_ADDOP); $$->token = OR; }
                        ;

mulOp                   : ASTR                                              { $$ = node_new(NODE_MULOP); $$->token = ASTR; }
                        | SLASH                                             { $$ = node_new(NODE_MULOP); $$->token = SLASH; }
                        | DIV                                               { $$ = node_new(NODE_MULOP); $$->token = DIV; }
                        | MOD                                               { $$ = node_new(NODE_MULOP); $$->token = MOD; }
                        | AND                                               { $$ = node_new(NODE_MULOP); $$->token = AND; }
                        ;

%%

void yyerror(const char *message) {
    fprintf(stderr, "Error: Line %d: %s got \"%s\"\n", yylineno, message,
            yytext);
}
