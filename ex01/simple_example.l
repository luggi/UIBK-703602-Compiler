%{
#include <stdio.h>

void yyerror(char *message);

unsigned int keywords = 0;
unsigned int literals = 0;
unsigned int operators = 0;
unsigned int types = 0;
unsigned int numbers = 0;
unsigned int identifiers = 0;
unsigned int line_number = 0;
%}

%option noyywrap

string        \'.*\'
whitespace    [ \t]+
keywords      program|var|array|of|begin|end|while|do|for|to|if|then|else|mod|and|div
types         real|integer
literals      true|false
operators     "+"|"-"|"/"|"*"|"<>"|":="|"="|">"|"<"|"<="|">="|".."|":"|"."
parent_l      "("
parent_r      ")"
braket_l      "["
braket_r      "]"
comma         ","
semicolon     ";"
numbers       [0-9]+
identifiers   [a-zA-z_][a-zA-z0-9_]*


%x COMMENT1 COMMENT2

%%

\r?\n          line_number++;
{string}       printf("STRING:      %s\n", yytext);
{keywords}     {printf("KEYWORD:    %s\n", yytext);keywords++;}
{operators}    {printf("OPERATOR:   %s\n", yytext);operators++;}
{comma}         printf("COMMA:      %s\n", yytext);
{semicolon}     printf("SEMICOLON:  %s\n", yytext);
{types}        {printf("TYPE:       %s\n", yytext);types++;}
{numbers}      {printf("NUMBER:     %s\n", yytext);numbers++;}
{literals}     {printf("LITERAL:    %s\n", yytext);literals++;}


{parent_l}      printf("PARENT L:   %s\n", yytext);
{parent_r}      printf("PARENT R:   %s\n", yytext);
{braket_l}      printf("BRAKET L:   %s\n", yytext);
{braket_r}      printf("BRAKET R:   %s\n", yytext);

{identifiers}  {printf("IDENTIFIER: %s\n", yytext);identifiers++;}

"{"             BEGIN(COMMENT1);
<COMMENT1>"}"   BEGIN(INITIAL);
<COMMENT1>\n    line_number++;
<COMMENT1>.     ;

"(*"            BEGIN(COMMENT2);
<COMMENT2>"*)"  BEGIN(INITIAL);
<COMMENT2>\n    line_number++;
<COMMENT2>.     ;

{whitespace}   ;

.              yyerror("no symbol");

%%

int main(int argc, char **argv) {
  yylex();

  printf("---- finished ----\n");
  printf("Number of keywords:    %d\n", keywords);
  printf("Number of literals:    %d\n", literals);
  printf("Number of types:       %d\n", types);
  printf("Number of numbers:     %d\n", numbers);
  printf("Number of identifiers: %d\n", identifiers);
  printf("Number of operators:   %d\n", operators);
  printf("Number of lines:       %d\n", line_number);

  return 0;
}

void yyerror(char *message) {
    fprintf(stderr, "Error: \"%s\" in line %d. Token = %s\n",
            message, line_number, yytext);
}


