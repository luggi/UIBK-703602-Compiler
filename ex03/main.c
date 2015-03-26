/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "tokens.h"

extern int yylex(void);
extern int yylineno;
extern char *yytext;

/* convert given token to string */
const char *token_string(const enum token t) {
    switch (t) {
        case  AND:      return "and";
        case  ARRAY:    return "array";
        case  _BEGIN:   return "begin";
        case  DIV:      return "div";
        case  DO:       return "do";
        case  ELSE:     return "else";
        case  END:      return "end";
        case  FOR:      return "for";
        case  IF:       return "if";
        case  MOD:      return "mod";
        case  NOT:      return "not";
        case  OF:       return "of";
        case  PROGRAM:  return "program";
        case  THEN:     return "then";
        case  TO:       return "to";
        case  VAR:      return "var";
        case  WHILE:    return "while";
        case  ASTR:     return "*";
        case  PLUS:     return "+";
        case  COMMA:    return ",";
        case  MINUS:    return "-";
        case  DOT:      return ".";
        case  DDOT:     return "..";
        case  SLASH:    return "/";
        case  COLON:    return ":";
        case  ASGN:     return ":=";
        case  SEMCO:    return ";";
        case  LT:       return "<";
        case  LEQ:      return "<=";
        case  NEQ:      return "<>";
        case  EQ:       return "=";
        case  GT:       return ">";
        case  GEQ:      return ">=";
        case  CARET:    return "^";
        case  PAR_L:    return "(";
        case  PAR_R:    return ")";
        case  BRA_L:    return "[";
        case  BRA_R:    return "]";
        case  BOOL:     return "boolean";
        case  INTEGER:  return "integer";
        case  REAL:     return "real";
        case  FALSE:    return "false";
        case  TRUE:     return "true";
        case  NUM:      return "number";
        case  STR:      return "string";
        case  IDENT:    return "identifier";
        case  _EOF:     return "EOF";
        default:        return "unknown symbol";
    }
}

void print(const enum token t) {
    printf("%3d : %20s %10s\n", yylineno, token_string(t), yytext);
}

/* compares `expect` with next token, calls `exit()` on missmatch */
void match(const enum token expect) {
    enum token read = yylex();
    if (read != expect) {
        fprintf(stderr, "Error in %d: read '%s', expected '%s'\n",
                yylineno, token_string(read), token_string(expect));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {

    while (true) {
        enum token t = yylex();

        if (t == _EOF) {
            break;
        }

        print(t);
    }

    return EXIT_SUCCESS;
}
