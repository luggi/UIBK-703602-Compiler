/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

%{
#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>

#include "tokens.h"

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
(?i:else)               return ELSE;
(?i:end)                return END;
(?i:for)                return FOR;
(?i:if)                 return IF;
(?i:mod)                return MOD;
(?i:not)                return NOT;
(?i:of)                 return OF;
(?i:program)            return PROGRAM;
(?i:then)               return THEN;
(?i:to)                 return TO;
(?i:var)                return VAR;
(?i:while)              return WHILE;

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
boolean                 return BOOL;
integer                 return INTEGER;
real                    return REAL;

 /* literals */
false                   return FALSE;
true                    return TRUE;
{number}                return NUM;
{string}                return STR;

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

void yyunlex(void) {
    for (char *ptr = yytext + yyleng - 1; ptr >= yytext; ptr--) {
        unput(*ptr);
    }
}

static void yyerror(const char *message) {
    fprintf(stderr, "Error in line %d: %s \"%s\"\n", yylineno, message,
            yytext);
    exit(EXIT_FAILURE);
}
