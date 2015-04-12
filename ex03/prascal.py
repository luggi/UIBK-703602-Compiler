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
    'DOWNTO'  : 'downto',
    'ELSE'    : 'else',
    'END'     : 'end',
    'FOR'     : 'for',
    'IF'      : 'if',
    'MOD'     : 'mod',
    'NOT'     : 'not',
    'OF'      : 'of',
    'OR'      : 'or',
    'PROGRAM' : 'program',
    'READ'    : 'read',
    'THEN'    : 'then',
    'TO'      : 'to',
    'VAR'     : 'var',
    'WHILE'   : 'while',
    'WRITE'   : 'write',

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
    'BOOLEAN' : 'boolean',
    'INTEGER' : 'integer',
    'REAL'    : 'real',

    # literals
    'FALSE'  : 'false',
    'TRUE'   : 'true',
    'NUM'    : 'number',
    'STRING' : 'string',

    # identifier
    'IDENT' : 'identifier',
}

RULES = {

    'start': [
        [ 'PROGRAM', 'IDENT', 'SEMCO', 'varDec', 'compStmt', 'DOT' ],
        ],

    'varDec': [
        [ 'VAR', 'varDecList' ],
        [],
        ],

    'varDecList': [
        [ 'identListType', 'SEMCO', 'varDecList_' ],
        ],

    'varDecList_': [
        [ 'varDecList' ],
        [],
        ],

    'identListType': [
        [ 'identList', 'COLON', 'type' ],
        ],

    'identList': [
        [ 'IDENT', 'identList_' ],
        ],

    'identList_': [
        [ 'COMMA', 'identList' ],
        [],
        ],

    'type': [
        [ 'simpleType' ],
        [ 'ARRAY', 'BRA_L', 'NUM', 'DDOT', 'NUM', 'BRA_R', 'OF', 'simpleType' ],
        ],

    'simpleType': [
        [ 'INTEGER' ],
        [ 'REAL' ],
        [ 'BOOLEAN' ],
        ],

    'compStmt': [
        [ '_BEGIN', 'stmtList', 'END' ],
        ],

    'stmtList': [
        [ 'statement', 'stmtList_' ],
        ],

    'stmtList_': [
        [ 'SEMCO', 'stmtList' ],
        [],
        ],

    'statement': [
        [ 'assignStmt' ],
        [ 'compStmt' ],
        [ 'ifStmt' ],
        [ 'whileStmt' ],
        [ 'forStmt' ],
        [ 'READ', 'PAR_L', 'exprList', 'PAR_R' ],
        [ 'WRITE', 'PAR_L', 'exprList', 'PAR_R' ],
        ],

    'assignStmt': [
        [ 'IDENT', 'assignStmt_' ],
        ],

    'assignStmt_': [
        [ 'ASGN', 'expr' ],
        [ 'BRA_L', 'expr', 'BRA_R', 'ASGN', 'expr' ],
        ],

    'ifStmt': [
        [ 'IF', 'expr', 'THEN', 'statement', 'elsePart' ],
        ],

    'elsePart': [
        [ 'ELSE', 'statement' ],
        [],
        ],

    'whileStmt': [
        [ 'WHILE', 'expr', 'DO', 'statement' ],
        ],

    'forStmt': [
        [ 'FOR', 'IDENT', 'ASGN', 'expr', 'toPart', 'expr', 'DO', 'statement' ],
        ],

    'toPart': [
        [ 'TO' ],
        [ 'DOWNTO' ],
        ],

    'expr': [
        [ 'simpleExpr', 'expr_' ],
        ],

    'expr_': [
        [ 'relOp', 'simpleExpr' ],
        [],
        ],

    'exprList': [
        [ 'expr', 'exprList_' ],
        ],

    'exprList_': [
        [ 'COMMA', 'exprList' ],
        [],
        ],

    'simpleExpr': [
        [ 'term', 'simpleExpr_' ],
        ],

    'simpleExpr_': [
        [ 'addOp', 'simpleExpr' ],
        [],
        ],

    'term': [
        [ 'factor', 'term_' ],
        ],

    'term_': [
        [ 'mulOp', 'term' ],
        [],
        ],

    'factor': [
        [ 'NUM' ],
        [ 'STRING' ],
        [ 'FALSE' ],
        [ 'TRUE' ],
        [ 'IDENT', 'factor_' ],
        [ 'NOT', 'factor' ],
        [ 'MINUS', 'factor' ],
        [ 'PAR_L', 'expr', 'PAR_R' ],
        ],

    'factor_': [
        [ 'BRA_L', 'expr', 'BRA_R' ],
        [],
        ],

    'relOp': [
        [ 'LT' ],
        [ 'LEQ' ],
        [ 'GT' ],
        [ 'GEQ' ],
        [ 'EQ' ],
        [ 'NEQ' ],
        ],

    'addOp': [
        [ 'PLUS' ],
        [ 'MINUS' ],
        [ 'OR' ],
        ],

    'mulOp': [
        [ 'ASTR' ],
        [ 'SLASH' ],
        [ 'DIV' ],
        [ 'MOD' ],
        [ 'AND' ],
        ],

    }

if __name__ == '__main__':
    print Template(sys.stdin.read()).render(RULES=RULES, TOKENS=TOKENS)
