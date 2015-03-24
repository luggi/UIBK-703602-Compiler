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

int c_comment = 0;

int c_depth = 0;
int c_depth_max = 0;

%}

%option noyywrap

%x COMMENT

%%

"(*"              {BEGIN(COMMENT);c_depth++; c_comment++;if(c_depth >= c_depth_max) c_depth_max++;}
<COMMENT>"(*"     {c_comment++; if(c_depth >= c_depth_max) c_depth_max++;c_depth++;}
<COMMENT>"*)"     {if(c_depth == 1) BEGIN(INITIAL); c_depth--;}
<COMMENT>\r?\n    ;
<COMMENT>.        ;
.                 ;
%%

int main(int argc, char *argv[]) {
    yylex();

    if (c_depth != 0)
        printf("Not all comments were closed!\ntotal comments(open): %d\n", c_comment);
    else
        printf("total comments: %d \tmax comment depth: %d\n",c_comment, c_depth_max);

    return EXIT_SUCCESS;
}
