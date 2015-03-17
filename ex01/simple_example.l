%{
#include <stdio.h>

void yyerror(char *message);

int line_number = 0;

int keywords = 0;
int literals = 0;
int operators = 0;
int types = 0;
int numbers = 0;
int identifiers = 0;
%}

%option noyywrap

string        "'".*"'"
whitespace    [ \t]+
keywords      program|var|array|of|begin|end|while|do|for|to|if|then|else|mod|and|div
types         real|integer
literals      true|false
operators     "+"|"-"|"/"|"*"|"<>"|":="|"="|">"|"<"|"<="|">="|".."|":"|";"|","|")"|"("|"["|"]"|"."
commas        ","
numbers       [0-9]+
identifiers   [[:alpha:]][[:alnum:]]*


%x COMMENT

%%

\r?\n          line_number++;
{string}       ;
{keywords}     keywords++;
{operators}    operators++;
{types}        types++;
{numbers}      numbers++;
{literals}     literals++;

{identifiers}  identifiers++;

"{"            BEGIN(COMMENT);
<COMMENT>"}"   BEGIN(INITIAL);
<COMMENT>.     ;

{whitespace}   ;
.              yyerror("no match");
%%

int main(int argc, char **argv) {
  yylex();

  printf("\n\n\nNumber of keywords: %d", keywords);
  printf("\nNumber of literals: %d", literals);
  printf("\nNumber of types: %d", types);
  printf("\nNumber of numbers: %d", numbers);
  printf("\nNumber of identifiers: %d", identifiers);
  printf("\nNumber of operators: %d\n", operators);
  printf("line_number %d", line_number);
  return 0;
}

void yyerror(char *message) {
   fprintf(stderr,"Error: \"%s\" in line %d. Token = %s\n",
           message,line_number,yytext);
}


