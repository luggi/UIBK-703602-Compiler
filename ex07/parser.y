/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

%{
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "list.h"
#include "symbol_table.h"

static struct ast_node *ROOT_NODE = NULL;
static struct list_head *SYMBOL_TABLE = NULL;

extern int yylex(void);
extern int yylineno;
extern char *yytext;
extern void yylex_destroy(void);

void yyerror(const char *message);

static void identListType_to_symbol_table(struct ast_node *identListType);
static void ident_to_symbol_table(struct ast_node *ident);
%}

%error-verbose

%union {
    /* text of identifier */
    char *identifier;

    /* const value */
    int integer;
    double real;
    char *string;

    /* AST node */
    struct ast_node *node;
}

/* identifier */
%token <identifier> IDENT

/* literals */
%token <integer> INT_VALUE
%token <real>   REAL_VALUE
%token <string> STR

/* nodes */
%type <node> start varDec varDecList identListType identList type simpleType
%type <node> compStmt stmtList statement assignStmt ifStmt whileStmt forStmt
%type <node> toPart expr exprList simpleExpr term factor ident num str relOp
%type <node> addOp mulOp bool

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

start                   : PROGRAM ident SEMCO varDec compStmt DOT           { $$ = node_create(NODE_PROGRAM); $$->body[0] = $2; $$->body[1] = $4; $$->body[2] = $5; ROOT_NODE = $$; }
                        ;

varDec                  : VAR varDecList                                    { $$ = node_create(NODE_VARDEC); $$->body[0] = $2; }
                        | /* eps */                                         { $$ = NULL;                                       }
                        ;

varDecList              : identListType SEMCO varDecList                    { $$ = node_create(NODE_VARDECLIST); $$->body[0] = $1; $$->next = $3; }
                        | identListType SEMCO                               { $$ = node_create(NODE_VARDECLIST); $$->body[0] = $1;                }
                        ;

identListType           : identList COLON type                              { $$ = node_create(NODE_IDENTLISTTYPE); $$->body[0] = $1; $$->body[1] = $3; identListType_to_symbol_table($$); }
                        ;

identList               : ident COMMA identList                             { $$ = node_create(NODE_IDENTLIST); $$->body[0] = $1; $$->next = $3; }
                        | ident                                             { $$ = node_create(NODE_IDENTLIST); $$->body[0] = $1;                }
                        ;

type                    : simpleType                                        { $$ = node_create(NODE_TYPE);                                     $$->body[2] = $1; }
                        | ARRAY BRA_L num DDOT num BRA_R OF simpleType      { $$ = node_create(NODE_TYPE); $$->body[0] = $3; $$->body[1] = $5; $$->body[2] = $8; }
                        ;

simpleType              : INTEGER                                           { $$ = node_create(NODE_SIMPLETYPE); $$->token = INTEGER; }
                        | REAL                                              { $$ = node_create(NODE_SIMPLETYPE); $$->token = REAL;    }
                        | BOOLEAN                                           { $$ = node_create(NODE_SIMPLETYPE); $$->token = BOOLEAN; }
                        ;

compStmt                : _BEGIN stmtList END                               { $$ = node_create(NODE_COMPSTMT); $$->body[0] = $2; }
                        ;

stmtList                : statement SEMCO stmtList                          { $$ = node_create(NODE_STMTLIST); $$->body[0] = $1; $$->next = $3; }
                        | statement                                         { $$ = node_create(NODE_STMTLIST); $$->body[0] = $1;                }
                        ;

statement               : assignStmt                                        { $$ = $1;                                             }
                        | compStmt                                          { $$ = $1;                                             }
                        | ifStmt                                            { $$ = $1;                                             }
                        | whileStmt                                         { $$ = $1;                                             }
                        | forStmt                                           { $$ = $1;                                             }
                        | READ PAR_L exprList PAR_R                         { $$ = node_create(NODE_STMT_READ);  $$->body[0] = $3; }
                        | WRITE PAR_L exprList PAR_R                        { $$ = node_create(NODE_STMT_WRITE); $$->body[0] = $3; }
                        ;

assignStmt              : ident ASGN expr                                   { $$ = node_create(NODE_STMT_ASSIGN); $$->body[0] = $1;                   $$->body[2] = $3; ident_to_symbol_table($$->body[0]); }
                        | ident BRA_L expr BRA_R ASGN expr                  { $$ = node_create(NODE_STMT_ASSIGN); $$->body[0] = $1; $$->body[1] = $3; $$->body[2] = $6; ident_to_symbol_table($$->body[0]); }
                        ;

ifStmt                  : IF expr THEN statement                            { $$ = node_create(NODE_STMT_IF); $$->body[0] = $2; $$->body[1] = $4;                   }
                        | IF expr THEN statement ELSE statement             { $$ = node_create(NODE_STMT_IF); $$->body[0] = $2; $$->body[1] = $4; $$->body[2] = $6; }
                        ;

whileStmt               : WHILE expr DO statement                           { $$ = node_create(NODE_STMT_WHILE); $$->body[0] = $2; $$->body[1] = $4; }
                        ;

forStmt                 : FOR ident ASGN expr toPart expr DO statement      { $$ = node_create(NODE_STMT_FOR); $$->body[0] = $2; $$->body[1] = $4; $$->body[2] = $5; $$->body[3] = $6; $$->body[4] = $8; ident_to_symbol_table($$->body[0]); }
                        ;

toPart                  : TO                                                { $$ = node_create(NODE_TOPART); $$->token = TO;     }
                        | DOWNTO                                            { $$ = node_create(NODE_TOPART); $$->token = DOWNTO; }
                        ;

expr                    : simpleExpr relOp simpleExpr                       { $$ = node_create(NODE_EXPR); $$->body[0] = $1; $$->body[1] = $2; $$->body[2] = $3; }
                        | simpleExpr                                        { $$ = node_create(NODE_EXPR); $$->body[0] = $1;                                     }
                        ;

exprList                : expr COMMA exprList                               { $$ = node_create(NODE_EXPRLIST); $$->body[0] = $1; $$->next = $3; }
                        | expr                                              { $$ = node_create(NODE_EXPRLIST); $$->body[0] = $1;                }
                        ;

simpleExpr              : term addOp simpleExpr                             { $$ = node_create(NODE_SIMPLEEXPR); $$->body[0] = $1; $$->body[1] = $2; $$->body[2] = $3; }
                        | term                                              { $$ = node_create(NODE_SIMPLEEXPR); $$->body[0] = $1;                                     }
                        ;

term                    : factor mulOp term                                 { $$ = node_create(NODE_TERM); $$->body[0] = $1; $$->body[1] = $2; $$->body[2] = $3; }
                        | factor                                            { $$ = node_create(NODE_TERM); $$->body[0] = $1;                                     }
                        ;

factor                  : ident                                             { $$ = $1; ident_to_symbol_table($$);                                                                          }
                        | ident BRA_L expr BRA_R                            { $$ = node_create(NODE_IDENT_SUBSCR); $$->body[0] = $1; $$->body[1] = $3; ident_to_symbol_table($$->body[0]); }
                        | num                                               { $$ = $1;                                                                                                     }
                        | str                                               { $$ = $1;                                                                                                     }
                        | bool                                              { $$ = $1;                                                                                                     }
                        | NOT factor                                        { $$ = node_create(NODE_FACTOR_NOT);   $$->body[0] = $2;                                                       }
                        | MINUS factor                                      { $$ = node_create(NODE_FACTOR_MINUS); $$->body[0] = $2;                                                       }
                        | PAR_L expr PAR_R                                  { $$ = node_create(NODE_FACTOR_EXPR);  $$->body[0] = $2;                                                       }
                        ;

ident                   : IDENT                                             { $$ = node_create(NODE_IDENT); $$->ident = $1; }
                        ;

num                     : INT_VALUE                                         { $$ = node_create(NODE_INT);  $$->iValue = $1; }
                        | REAL_VALUE                                        { $$ = node_create(NODE_REAL); $$->fValue = $1; }
                        ;

str                     : STR                                               { $$ = node_create(NODE_STR); $$->sValue = $1; }
                        ;

bool                    : TRUE                                              { $$ = node_create(NODE_BOOL); $$->bValue = true;  }
                        | FALSE                                             { $$ = node_create(NODE_BOOL); $$->bValue = false; }
                        ;

relOp                   : LT                                                { $$ = node_create(NODE_RELOP); $$->token = LT;  }
                        | LEQ                                               { $$ = node_create(NODE_RELOP); $$->token = LEQ; }
                        | GT                                                { $$ = node_create(NODE_RELOP); $$->token = GT;  }
                        | GEQ                                               { $$ = node_create(NODE_RELOP); $$->token = GEQ; }
                        | EQ                                                { $$ = node_create(NODE_RELOP); $$->token = EQ;  }
                        | NEQ                                               { $$ = node_create(NODE_RELOP); $$->token = NEQ; }
                        ;

addOp                   : PLUS                                              { $$ = node_create(NODE_ADDOP); $$->token = PLUS;  }
                        | MINUS                                             { $$ = node_create(NODE_ADDOP); $$->token = MINUS; }
                        | OR                                                { $$ = node_create(NODE_ADDOP); $$->token = OR;    }
                        ;

mulOp                   : ASTR                                              { $$ = node_create(NODE_MULOP); $$->token = ASTR;  }
                        | SLASH                                             { $$ = node_create(NODE_MULOP); $$->token = SLASH; }
                        | DIV                                               { $$ = node_create(NODE_MULOP); $$->token = DIV;   }
                        | MOD                                               { $$ = node_create(NODE_MULOP); $$->token = MOD;   }
                        | AND                                               { $$ = node_create(NODE_MULOP); $$->token = AND;   }
                        ;

%%

void parser_create(void) {
    SYMBOL_TABLE = list_create();
}

void parser_destroy(void) {
    yylex_destroy();

    if (ROOT_NODE) {
        node_destroy(ROOT_NODE);
    }

    if (SYMBOL_TABLE) {
        for (struct list_node *node = SYMBOL_TABLE->first; node != NULL; node = node->next) {
            struct symbol_entry *entry = (struct symbol_entry *) node->data;

            /* symbol has been allocated by the parser using strndup */
            free(entry->symbol);

            /* free the symbol_entry itself */
            free(entry);
        }

        list_destroy(SYMBOL_TABLE);
    }
}

int parser_run(void) {
    return yyparse();
}

struct ast_node *parser_get_root_node(void) {
    return ROOT_NODE;
}

struct list_head *parser_get_symbol_table(void) {
    return SYMBOL_TABLE;
}

void yyerror(const char *message) {
    fprintf(stderr, "Error: Line %d: %s got \"%s\"\n", yylineno, message,
            yytext);
}

void print_symbol_table(void) {
    for (struct list_node *node = SYMBOL_TABLE->first; node != NULL; node = node->next) {
        struct symbol_entry *e = (struct symbol_entry *) node->data;
        printf("%p: %s (%d, %d)\n", e, e->symbol, e->etype, e->dtype);
    }
}

/* adds identifiers in identListType node to symbol table with set type. a
 * reference is stored in the node. */
static void identListType_to_symbol_table(struct ast_node *identListType) {
    struct ast_node* type_node = identListType->body[1];

    /* process entry type */
    enum symbol_type etype;
    if (type_node->body[0]) {
        etype = SYMBOL_SCALAR;
    } else {
        etype = SYMBOL_ARRAY;
    }

    /* process data type */
    enum data_type dtype;
    if (type_node->body[2]->token == INTEGER) {
        dtype = TYPE_INTEGER;
    } else if (type_node->body[2]->token == REAL) {
        dtype = TYPE_REAL;
    } else if (type_node->body[2]->token == BOOLEAN) {
        dtype = TYPE_BOOLEAN;
    } else {
        fprintf(stderr, "Error: Line %d: unknown type in syntax tree\n",
                yylineno);
        exit(EXIT_FAILURE);
    }

    /* process ident list */
    for (struct ast_node *identList = identListType->body[0]; identList != NULL; identList = identList->next) {
        struct ast_node *ident = identList->body[0];

        /* check if ident already present */
        for (struct list_node *node = SYMBOL_TABLE->first; node != NULL; node = node->next) {
            struct symbol_entry *entry = (struct symbol_entry *) node->data;
            if (strcmp(entry->symbol, ident->ident) == 0) {
                fprintf(stderr, "Error: Line %d: symbol already declared '%s'\n",
                        yylineno, ident->ident);
                exit(EXIT_FAILURE);
            }
        }

        /* new entry */
        struct symbol_entry *e = symbol_entry_create(etype, dtype);
        e->symbol = ident->ident;

        /* store reference in node */
        ident->symbol = e;

        list_append(SYMBOL_TABLE, e);
    }
}

/* looks up the corresponding entry in the symbol table and rewrites the node
 * to hold a reference to the entry. calls exit if entry is not present. */
static void ident_to_symbol_table(struct ast_node *ident) {
    for (struct list_node *node = SYMBOL_TABLE->first; node != NULL; node = node->next) {
        struct symbol_entry *entry = (struct symbol_entry *) node->data;
        if (strcmp(entry->symbol, ident->ident) == 0) {
            /* ident has been allocated by the parser using strndup */
            free(ident->ident);

            /* store reference to symbol table inside node */
            ident->symbol = entry;

            return;
        }
    }
    fprintf(stderr, "Error: Line %d: unknown symbol '%s'\n", yylineno,
            ident->ident);
    exit(EXIT_FAILURE);
}
