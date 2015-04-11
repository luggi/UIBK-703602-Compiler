/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

%{
#include "lexer.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "tokens.h"

/* contains all read tokens */
struct list_head *tokens = NULL;

/* count number of tokens read */
unsigned long int count = 0;

static void yyerror(const char *message);

%}

%option noyywrap
%option yylineno

%x COMMENT1 COMMENT2

number                  [0-9]+
string                  \"(\\.|[^"])*\"
identifier              [a-zA-Z_][a-zA-Z0-9_]*

%%

 /* keywords */
(?i:and)                return AND;
(?i:array)              return ARRAY;
(?i:begin)              return _BEGIN;
(?i:div)                return DIV;
(?i:do)                 return DO;
(?i:downto)             return DOWNTO;
(?i:else)               return ELSE;
(?i:end)                return END;
(?i:for)                return FOR;
(?i:if)                 return IF;
(?i:mod)                return MOD;
(?i:not)                return NOT;
(?i:of)                 return OF;
(?i:or)                 return OR;
(?i:program)            return PROGRAM;
(?i:read)               return READ;
(?i:then)               return THEN;
(?i:to)                 return TO;
(?i:var)                return VAR;
(?i:while)              return WHILE;
(?i:write)              return WRITE;

 /* opreators */
"*"                     return ASTR;
"+"                     return PLUS;
","                     return COMMA;
"-"                     return MINUS;
"."                     return DOT;
".."                    return DDOT;
"/"                     return SLASH;
":"                     return COLON;
":="                    return ASGN;
";"                     return SEMCO;
"<"                     return LT;
"<="                    return LEQ;
"<>"                    return NEQ;
"="                     return EQ;
">"                     return GT;
">="                    return GEQ;
"^"                     return CARET;

 /* parentheses */
"("                     return PAR_L;
")"                     return PAR_R;
"["                     return BRA_L;
"]"                     return BRA_R;

 /* types */
boolean                 return BOOLEAN;
integer                 return INTEGER;
real                    return REAL;

 /* literals */
false                   return FALSE;
true                    return TRUE;
{number}                return NUM;
{string}                return STRING;

 /* identifier */
{identifier}            return IDENT;

 /* comments */
"{"                     BEGIN(COMMENT1);
<COMMENT1>"}"           BEGIN(INITIAL);
<COMMENT1>\r?\n         ;
<COMMENT1><<EOF>>       yyerror("EOF reached inside COMMENT1");
<COMMENT1>.             ;

"(*"                    BEGIN(COMMENT2);
<COMMENT2>"*)"          BEGIN(INITIAL);
<COMMENT2>\r?\n         ;
<COMMENT2><<EOF>>       yyerror("EOF reached inside COMMENT2");
<COMMENT2>.             ;

 /* white space */
[ \t]+                  ;
\r?\n                   ;

 /* EOF */
<<EOF>>                 return _EOF;

 /* other */
.                       yyerror("unknown symbol");

%%

bool lexer_create(void) {
    tokens = list_create();
    if (tokens == NULL) {
        return false;
    }
    return true;
}

void lexer_destroy(void) {
    for (struct list_node *node = tokens->first; node != NULL; node = node->next) {
        free(node->data);
    }
    list_destroy(tokens);
    yylex_destroy();
}

struct token *lexer_next(void) {
    struct token *t = NULL;
    if (count < tokens->size) {
        /* token has already been read by yylex */
        t = (struct token *) list_get(tokens, count);
    } else {
        /* create token instance */
        t = (struct token *) malloc(sizeof(struct token));
        if (t == NULL) {
            fprintf(stderr, "could not allocate token\n");
            exit(EXIT_FAILURE);
        }

        /* read token */
        t->type = yylex();
        t->text = yytext;
        t->line = yylineno;

        /* store token */
        if (list_append(tokens, t) < 0) {
            fprintf(stderr, "could not append token to list\n");
            exit(EXIT_FAILURE);;
        }
    }
    count++;
    return t;
}

void lexer_undo(void) {
    if (count > 0) {
        count--;
    }
}

struct token *lexer_last(void) {
    return list_get(tokens, tokens->size - 1);
}

unsigned long int lexer_save(void) {
    return count;
}

void lexer_restore(unsigned long int saved) {
    count = saved;
}

static void yyerror(const char *message) {
    fprintf(stderr, "Error: Line %d: %s \"%s\"\n", yylineno, message, yytext);
    exit(EXIT_FAILURE);
}
