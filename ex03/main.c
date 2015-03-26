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

void print(char *token) {
    printf("%3d :  %10s   %s\n", yylineno, token, yytext);
}

int main(int argc, char *argv[]) {

    while (true) {
        switch (yylex()) {

            case  AND:      print("AND");      break;
            case  ARRAY:    print("ARRAY");    break;
            case  _BEGIN:   print("_BEGIN");   break;
            case  DIV:      print("DIV");      break;
            case  DO:       print("DO");       break;
            case  ELSE:     print("ELSE");     break;
            case  END:      print("END");      break;
            case  FOR:      print("FOR");      break;
            case  IF:       print("IF");       break;
            case  MOD:      print("MOD");      break;
            case  NOT:      print("NOT");      break;
            case  OF:       print("OF");       break;
            case  PROGRAM:  print("PROGRAM");  break;
            case  THEN:     print("THEN");     break;
            case  TO:       print("TO");       break;
            case  VAR:      print("VAR");      break;
            case  WHILE:    print("WHILE");    break;

            case  ASTR:     print("ASTR");     break;
            case  PLUS:     print("PLUS");     break;
            case  COMMA:    print("COMMA");    break;
            case  MINUS:    print("MINUS");    break;
            case  DOT:      print("DOT");      break;
            case  DDOT:     print("DDOT");     break;
            case  SLASH:    print("SLASH");    break;
            case  COLON:    print("COLON");    break;
            case  ASGN:     print("ASGN");     break;
            case  SEMCO:    print("SEMCO");    break;
            case  LT:       print("LT");       break;
            case  LEQ:      print("LEQ");      break;
            case  NEQ:      print("NEQ");      break;
            case  EQ:       print("EQ");       break;
            case  GT:       print("GT");       break;
            case  GEQ:      print("GEQ");      break;
            case  CARET:    print("CARET");    break;

            case  PAR_L:    print("PAR_L");    break;
            case  PAR_R:    print("PAR_R");    break;
            case  BRA_L:    print("BRA_L");    break;
            case  BRA_R:    print("BRA_R");    break;

            case  BOOL:     print("BOOL");     break;
            case  INTEGER:  print("INTEGER");  break;
            case  REAL:     print("REAL");     break;

            case  FALSE:    print("FALSE");    break;
            case  TRUE:     print("TRUE");     break;
            case  NUM:      print("NUM");      break;
            case  STR:      print("STR");      break;

            case  IDENT:    print("IDEN");     break;

            case  _EOF:     print("EOF");      exit(0);

            default:        print("unknown symbol");

        }
    }

    return EXIT_SUCCESS;
}
