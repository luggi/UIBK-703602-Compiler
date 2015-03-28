#!/usr/bin/env python2


import sys


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


def replace(inputfilename, outputfilename, old, new):
    with open(inputfilename, 'r') as fin:
        with open(outputfilename, 'w') as fout:
            for line in fin:
                fout.write(line.replace(old, new))


if __name__ == '__main__':

    tokens = ''
    for name,string in TOKENS.iteritems():
        tokens += "        case {}: return \"{}\";\n".format(name, string)
    tokens += "        default: return \"unknown symbol\";\n"

    replace('tokens.c.in', 'tokens.c', '{{{TOKENS}}}', tokens)

    tokens = ''
    for name in TOKENS:
        tokens += "    {},\n".format(name)

    replace('tokens.h.in', 'tokens.h', '{{{TOKENS}}}', tokens)

    rules = ''

    for name in RULES:
        rules += "void {}(void);\n".format(name)

    rules += '\n'

    for name,options in RULES.iteritems():

        rules += "void {}(void) {{\n".format(name)
        rules += "    enum token t = yylex();\n"
        rules += "    switch (t) {\n"

        for line in filter(None, options):
            words = line.split(' ')
            rules += "        case {}:\n".format(words[0])
            for w in words[1:]:
                if w in TOKENS:
                    rules += "            expect({});\n".format(w)
                elif w in RULES:
                    rules += "            {}();\n".format(w)
                else:
                    print "Error: {} neither TOKEN nor RULE".format(w)
                    sys.exit(1)

            rules += "            break;\n"

        if '' not in options:
            rules += "        default:\n"
            rules += "            no_match(t);\n"
        else:
            rules += "        default:\n"
            rules += "            break;\n"

        rules += "    }\n"
        rules += "}\n"
        rules += "\n"

    replace('main.c.in', 'main.c', '{{{RULES}}}', rules)
