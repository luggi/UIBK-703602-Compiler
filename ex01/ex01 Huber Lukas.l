/*Huber Lukas 
  Compiler construction SS 2015
  Homework 1 
  in collaboration with Alexander Hirsch, Patrick Ober, Michael Tscholl und Franz Josef Haider*/

%{
#include <stdio.h>
#include <stdlib.h>	
#include <ctype.h>

void toUpper(char *string);
void toLower(char *string);

void yyerror(const char *message);

int tokens = 0;
int operators = 0;
int literals =0;
int types=0;
int strings=0;
int numbers=0;
int num_lines=0;
int identifiers=0;
%}

%option noyywrap
%x COMMENT

keywords and|array|begin|div|do|else|end|for|if|mod|of|program|then|to|var|while
type  (integer|real)
literal (true|false)
comment	"{".*"}"
string "\"".*"\""
whitespace	[ \t]+
lineend [\n]

number [0-9]+

identifier [[:alpha:]][[:alnum:]]*

parentheses "("|")"|"["|"]"

operator ":="|"<>"|"<="|">="|".."|">"|"<"|"+"|"-"|"*"|"/"|":"|"="|";"|"^"|"."

comma ","

%%
"{"                     {BEGIN(COMMENT);printf("(*");}
<COMMENT>\n             {printf("\n"); num_lines++;}
<COMMENT>"}"            {BEGIN(INITIAL);printf("*)");}
{lineend}	{num_lines++; printf("\n");}
{keywords}{whitespace}|"end."|"end;" {tokens++; toUpper(yytext); printf("%s", yytext);};
{keywords}{lineend}	{tokens++; num_lines++; toUpper(yytext); printf("%s", yytext);};
{whitespace}+	printf(" ");
{operator}	{operators++; printf("%s", yytext);};
{literal}	{literals++; printf("%s", yytext);};
{type}		{types++; printf("%s", yytext);};
{string}	{strings++; printf("%s", yytext);};
{number}{whitespace}|{number}{operator}	{numbers++; printf("%s", yytext);};
{parentheses} {printf("%s", yytext);};
{comma}		{printf("%s", yytext);};
{identifier}            {toLower(yytext); identifiers++; printf("%s", yytext);}
.			{printf( "\n\t\tOn line %d:Error: %s unrecognized\n",num_lines, yytext );}
%%

void toUpper(char *string) {
    unsigned int i;
    for (i = 0; string[i] != '\0'; i++) {
        string[i] = toupper(string[i]);
    }
}

void toLower(char *string) {
    unsigned int i;
    for (i = 0; string[i] != '\0'; i++) {
        string[i] = tolower(string[i]);
    }
}

int main(int argc, char **argv) {
  yylex();
  printf("\n(* Number of Keywords: %d *)\n", tokens);
  printf("\n(* Number of Operators: %d *)\n", operators);
  printf("\n(* Number of Literals: %d *) \n", literals);
  printf("\n(* Number of Types: %d *)\n", types);
  printf("\n(* Number of Strings: %d *)\n", strings);
  printf("\n(* Number of Numbers: %d *)\n", numbers);
  printf("\n(* Number of Lines: %d *)\n", num_lines);
  return 0;
}