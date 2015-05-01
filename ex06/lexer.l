/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

%{
#include <string.h>

#include "parser.h"

extern void yyerror(const char *message);
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
{number}                { yylval.number = atoi(yytext); return NUM; }
{string}                { yylval.string = strndup(yytext, yyleng); return STR; }

 /* identifier */
{identifier}            { yylval.identifier = strndup(yytext, yyleng); return IDENT; }

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
