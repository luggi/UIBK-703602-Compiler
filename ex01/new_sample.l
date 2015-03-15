%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void toUpper(char *string);
void toLower(char *string);

void yyerror(const char *message);
%}

%x COMMENT

keyword (?i:and|array|begin|div|do|else|end|false|for|if|integer|mod|of|program|real|then|to|true|var|while)

operator ":="|"<>"|"<="|">="|".."|">"|"<"|"+"|"-"|"*"|"/"|":"|"="|";"|"^"|"."

comma ","

whitespace [ \t\r\n]*

parentheses "("|")"|"["|"]"

string \"[^"]*\"

number [[:digit:]][[:digit:]]*

identifier (?i:[[:alpha:]][[:alnum:]]*)

%%

 /* uppercase keywords */
{keyword}               {toUpper(yytext);printf("%s", yytext);}

 /* lowercase identifiers */
{identifier}            {toLower(yytext);printf("%s", yytext);}

 /* collapse newline and whitespace */
{whitespace}            printf(" ");

 /* comments */
"{"                     {BEGIN(COMMENT);printf("(*");}
<COMMENT>\n             printf("\n");
<COMMENT>"}"            {BEGIN(INITIAL);printf("*)");}

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

int main(int argc, char *argv[]) {
    yylex();
    return EXIT_SUCCESS;
}
