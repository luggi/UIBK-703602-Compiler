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

int main(int argc, char *argv[]) {

    while (true) {
        switch (yylex()) {

            case  AND:      puts("AND");      break;
            case  ARRAY:    puts("ARRAY");    break;
            case  _BEGIN:   puts("_BEGIN");   break;
            case  DIV:      puts("DIV");      break;
            case  DO:       puts("DO");       break;
            case  ELSE:     puts("ELSE");     break;
            case  END:      puts("END");      break;
            case  FOR:      puts("FOR");      break;
            case  IF:       puts("IF");       break;
            case  MOD:      puts("MOD");      break;
            case  NOT:      puts("NOT");      break;
            case  OF:       puts("OF");       break;
            case  PROGRAM:  puts("PROGRAM");  break;
            case  THEN:     puts("THEN");     break;
            case  TO:       puts("TO");       break;
            case  VAR:      puts("VAR");      break;
            case  WHILE:    puts("WHILE");    break;

            case  ASTR:     puts("ASTR");     break;
            case  PLUS:     puts("PLUS");     break;
            case  COMMA:    puts("COMMA");    break;
            case  MINUS:    puts("MINUS");    break;
            case  DOT:      puts("DOT");      break;
            case  DDOT:     puts("DDOT");     break;
            case  SLASH:    puts("SLASH");    break;
            case  COLON:    puts("COLON");    break;
            case  ASGN:     puts("ASGN");     break;
            case  SEMCO:    puts("SEMCO");    break;
            case  LT:       puts("LT");       break;
            case  LEQ:      puts("LEQ");      break;
            case  NEQ:      puts("NEQ");      break;
            case  EQ:       puts("EQ");       break;
            case  GT:       puts("GT");       break;
            case  GEQ:      puts("GEQ");      break;
            case  CARET:    puts("CARET");    break;

            case  PAR_L:    puts("PAR_L");    break;
            case  PAR_R:    puts("PAR_R");    break;
            case  BRA_L:    puts("BRA_L");    break;
            case  BRA_R:    puts("BRA_R");    break;

            case  BOOL:     puts("BOOL");     break;
            case  INTEGER:  puts("INTEGER");  break;
            case  REAL:     puts("REAL");     break;

            case  FALSE:    puts("FALSE");    break;
            case  TRUE:     puts("TRUE");     break;
            case  NUM:      puts("NUM");      break;
            case  STR:      puts("STR");      break;

            case  IDENT:    puts("IDEN");     break;

            case  _EOF:     puts("EOF");      exit(0);

            default:        puts("unknown symbol");

        }
    }

    return EXIT_SUCCESS;
}
