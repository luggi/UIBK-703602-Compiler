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

unsigned int count = 0;
unsigned int depth = 0;
unsigned int max_depth = 0;

void enter_comment(void);
void leave_comment(void);
%}

%option noyywrap

%x COMMENT

%%

"(*"                    {enter_comment();}
<COMMENT>"(*"           {enter_comment();}
<COMMENT>"*)"           {leave_comment();}

 /* ignore everything else */
.                       ;
\r?\n                   ;
<COMMENT>.              ;
<COMMENT>\r?\n          ;

%%

void enter_comment(void) {
    if (depth == 0) {
        BEGIN(COMMENT);
    }
    count++;
    depth++;
    if (depth > max_depth) {
        max_depth = depth;
    }
}

void leave_comment(void) {
    if (depth == 1) {
        BEGIN(INITIAL);
    }
    depth--;
}

int main(int argc, char *argv[]) {
    yylex();
    printf("count:      %d\nmax. depth: %d\n", count, max_depth);
    return EXIT_SUCCESS;
}
