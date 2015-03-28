#!/usr/bin/env python2

import sys

from jinja2 import Template

TOKENS = {
    # EOF = 0
    '_EOF' : 'EOF',

    # keywords
    'AND'     : 'and',
    'ARRAY'   : 'array',
    '_BEGIN'  : 'begin',
    'DIV'     : 'div',
    'DO'      : 'do',
    'ELSE'    : 'else',
    'END'     : 'end',
    'FOR'     : 'for',
    'IF'      : 'if',
    'MOD'     : 'mod',
    'NOT'     : 'not',
    'OF'      : 'of',
    'PROGRAM' : 'program',
    'THEN'    : 'then',
    'TO'      : 'to',
    'VAR'     : 'var',
    'WHILE'   : 'while',

    # opreators
    'ASTR'  : '*',
    'PLUS'  : '+',
    'COMMA' : ',',
    'MINUS' : '-',
    'DOT'   : '.',
    'DDOT'  : '..',
    'SLASH' : '/',
    'COLON' : ':',
    'ASGN'  : ':=',
    'SEMCO' : ';',
    'LT'    : '<',
    'LEQ'   : '<=',
    'NEQ'   : '<>',
    'EQ'    : '=',
    'GT'    : '>',
    'GEQ'   : '>=',
    'CARET' : '^',

    # parentheses
    'PAR_L' : '(',
    'PAR_R' : ')',
    'BRA_L' : '[',
    'BRA_R' : ']',

    # types
    'BOOL'    : 'boolean',
    'INTEGER' : 'integer',
    'REAL'    : 'real',

    # literals
    'FALSE' : 'false',
    'TRUE'  : 'true',
    'NUM'   : 'number',
    'STR'   : 'string',

    # identifier
    'IDENT' : 'identifier',
}

RULES = {

    'start': [
        'PROGRAM IDENT SEMCO varDec DOT',
        ],

    'varDec': [
        'VAR varDecList',
        '',
        ],

    'varDecList': [
        'SEMCO identListType SEMCO',
        ],

    'identListType': [
        'IDENT COLON INTEGER',
        ],

    }

if __name__ == '__main__':
    print Template(sys.stdin.read()).render(RULES=RULES, TOKENS=TOKENS)
