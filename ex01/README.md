# Homework 1 (10 Points)

Consider the `sample.pas` program and do following tasks using the lexical
analysis tool `lex`:

1. Identify and enumerate all tokens in the program
2. Write a lexical analyser that recognises all tokens
3. Report lexical errors where they occur (line numbers)
4. Generate a new source program which
    1. Uppercase all keywords
    2. Lowercase all identifiers
    3. Remove redundant whitespace characters
    4. Transform all comments into old style Pascal comments
5. Write a new program (min. 100 loc in Pascal?) that implements a
   well-known algorithm. Modify it to contain at least five lexical errors
   reported by the lexical analyser.
 
 =======
 
 ERRORs in algorithm_error.pas 
 
 1. line 12: 0Asize not a valid string name
 2. line 23: @ is not a valid character
 3. line 45: != is not a valid operator in pascal
 4. line 76: >>>>> not a valid operator
 5. line 116: /\* invalid comment */
