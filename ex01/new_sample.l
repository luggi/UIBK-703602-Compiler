/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void toUpper(char *string);
void toLower(char *string);
%}

%option noyywrap

%x COMMENT

keyword (?i:and|array|begin|div|do|else|end|false|for|if|integer|mod|of|program|real|then|to|true|var|while)
operator ":="|"<>"|"<="|">="|".."|">"|"<"|"+"|"-"|"*"|"/"|":"|"="|";"|"^"|"."|","
whitespace [ \t]+
parentheses "("|")"|"["|"]"
number [[:digit:]]+
identifier [[:alpha:]][[:alnum:]]*

%%

 /* uppercase keywords */
{keyword}               {toUpper(yytext);printf("%s", yytext);}

 /* lowercase identifiers */
{identifier}            {toLower(yytext);printf("%s", yytext);}

 /* remove leading whitespace */
^{whitespace}           ;

 /* compress whitespace */
{whitespace}            printf(" ");

 /* compress empty lines */
({whitespace}?\r?\n)+   printf("\n");

 /* convert comments */
"{"                     {BEGIN(COMMENT);printf("(*");}
<COMMENT>"}"            {BEGIN(INITIAL);printf("*)");}

%%

void toUpper(char *string) {
    for (unsigned int i = 0; string[i] != '\0'; i++) {
        string[i] = toupper(string[i]);
    }
}

void toLower(char *string) {
    for (unsigned int i = 0; string[i] != '\0'; i++) {
        string[i] = tolower(string[i]);
    }
}

int main(int argc, char *argv[]) {
    yylex();
    return EXIT_SUCCESS;
}
