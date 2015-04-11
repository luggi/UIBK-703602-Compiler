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

## Building

Make should do everything for you, note that [Python Jinja] is required:

    $ make

[Python Jinja]: <http://jinja.pocoo.org/>

## Running

The parser reads from *stdin*, therefore:

    $ ./main < sample.pas

The parser will tell you whether your input has been accepted or the location
where it got stuck. It may also hint a solution for the error if you are lucky.
