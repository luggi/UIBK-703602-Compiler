/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#include "tokens.h"

const char *token_type_string(const enum token_type type) {
    switch (type) {
        case REAL: return "real";
        case THEN: return "then";
        case PAR_R: return ")";
        case READ: return "read";
        case WHILE: return "while";
        case PROGRAM: return "program";
        case DDOT: return "..";
        case TRUE: return "true";
        case PAR_L: return "(";
        case DOT: return ".";
        case NEQ: return "<>";
        case SEMCO: return ";";
        case ASGN: return ":=";
        case NUM: return "number";
        case PLUS: return "+";
        case COMMA: return ",";
        case ASTR: return "*";
        case GT: return ">";
        case WRITE: return "write";
        case SLASH: return "/";
        case VAR: return "var";
        case ARRAY: return "array";
        case FALSE: return "false";
        case OF: return "of";
        case LEQ: return "<=";
        case BOOLEAN: return "boolean";
        case OR: return "or";
        case CARET: return "^";
        case DO: return "do";
        case _EOF: return "EOF";
        case DIV: return "div";
        case GEQ: return ">=";
        case DOWNTO: return "downto";
        case MINUS: return "-";
        case TO: return "to";
        case LT: return "<";
        case COLON: return ":";
        case INTEGER: return "integer";
        case IDENT: return "identifier";
        case END: return "end";
        case STRING: return "string";
        case FOR: return "for";
        case ELSE: return "else";
        case EQ: return "=";
        case IF: return "if";
        case AND: return "and";
        case _BEGIN: return "begin";
        case BRA_R: return "]";
        case BRA_L: return "[";
        case NOT: return "not";
        case MOD: return "mod";
        default: return "unknown symbol";
    }
}
