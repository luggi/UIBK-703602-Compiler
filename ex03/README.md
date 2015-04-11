# Homework 3 (10 points)

Recursive descendent parsing Study the BNF grammar attached to this exercise
sheet and do the following tasks:

1. Check if the grammar correctly defines the syntax of the language proposed
   in the sample.pas file and discuss in the OLAT forum eventual corrections
   and changes.
2. Transform the grammar into a form which suitable for a recursive-descent
   parsing as follows:
   1. Eliminate the left recursion
   2. Left factorize the productions
   3. Convert the grammar into the EBNF form
3. Declare the set of tokens in an enumerated type and modify the lexical
   analyser from the Homework 1 to return the current token when a regular
   expression is matched.
4. Write a recursive-descent parser that uses the lexical analyser implemented
   in the previous task to get the next lookahead token.
5. Stop the parse at the first syntactic error with a meaningful error message
   including the line number.

## About

This parser is based on [Roman Redziejowski's proposal] and is generated
according to the grammar defined in `prascal.py`. The main template for the
generation can be viewed in `main.c.jin`. [Python Jinja] is required in order
to build this parser, installing it via [Python Pip] is recommended.

[Roman Redziejowski's proposal]: <https://www2.informatik.hu-berlin.de/~hs/Aktivitaeten/2006_CSP/CSP06_28.pdf>
[Python Jinja]: <http://jinja.pocoo.org/>
[Python Pip]: <https://pip.pypa.io/en/stable/>

## Building

Make should do everything for you, note that [Python Jinja] is required:

    $ make

## Running

The parser reads from *stdin*, therefore:

    $ ./main < sample.pas

The parser will tell you whether your input has been accepted or the location
where it got stuck. It may also hint a solution for the error if you are lucky.
