#!/usr/bin/env python2

import sys

from collections import defaultdict
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
        [ 'varDecList', 'identListType', 'SEMCO' ],
        [ 'identListType', 'SEMCO' ],
        ],

    'identListType': [
        [ 'identList', 'COLON', 'type' ],
        ],

    'identList': [
        [ 'identList', 'COMMA', 'IDENT' ],
        [ 'IDENT' ],
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
        [ 'stmtList', 'SEMCO', 'statement' ],
        [ 'statement' ],
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
        [ 'IDENT', 'ASGN', 'expr' ],
        [ 'IDENT', 'BRA_L', 'expr', 'BRA_R', 'ASGN', 'expr' ],

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
        [ 'simpleExpr', 'relOp', 'simpleExpr' ],
        [ 'simpleExpr' ],
        ],

    'exprList': [
        [ 'exprList', 'COMMA', 'expr' ],
        [ 'expr' ],
        ],

    'simpleExpr': [
        [ 'simpleExpr', 'addOp', 'term' ],
        [ 'term' ],
        ],


    'term': [
        [ 'term', 'mulOp', 'factor' ],
        [ 'factor' ],
        ],

    'factor': [
        [ 'NUM' ],
        [ 'STRING' ],
        [ 'FALSE' ],
        [ 'TRUE' ],
        [ 'IDENT', 'BRA_L', 'expr', 'BRA_R' ],
        [ 'IDENT' ],
        [ 'NOT', 'factor' ],
        [ 'MINUS', 'factor' ],
        [ 'PAR_L', 'expr', 'PAR_R' ],
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


def remove_left_recursion(rules):

    def get_immediate_alphas(name, options):
        return [x[1:] for x in options if x != [] and x[0] == name]

    def get_immediate_betas(name, options):
        return [x for x in options if x == [] or x[0] != name]

    def remove_immediate_left_recursion(name, options):
        alphas = get_immediate_alphas(name, options)
        if len(alphas) > 0:
            for beta in get_immediate_betas(name, options):
                new_rules[name].insert(0, beta + [name + '_'])
            for alpha in alphas:
                new_rules[name + '_'].insert(0, alpha + [name + '_'])
            new_rules[name + '_'].append([])
        else:
            new_rules[name] = options

    new_rules = defaultdict(list)

    seen = set()
    for i in rules:
        seen.add(i)
        for j in seen:
            if i == j:
                continue
            for line in [x for x in rules[i] if x != [] and x[0] == j]:
                for alpha in rules[j]:
                    new_rules[i].insert(0, alpha + line[1:])

        remove_immediate_left_recursion(i, rules[i])

    return new_rules


if __name__ == '__main__':
    rules = remove_left_recursion(RULES)
    print Template(sys.stdin.read()).render(RULES=rules, TOKENS=TOKENS)
