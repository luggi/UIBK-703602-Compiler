# Homework 4 (10 Points)

Extend your recursive descendent parser with error detection and recovery
functionality as follows:

1. Calculate by hand for each nonterminal of the EBNF grammar the FIRST and the
   FOLLOW sets using the algorithms presented at the lecture.
2. Display a meaningful error message if the next lookahead token is not in the
   FIRST set of the expected nonterminal.
3. In case of syntactic errors, implement a panic-mode error recovery that
   scans ahead until:
    1. it reaches a token in the First set of the current nonterminal or
    2. it reaches a token in the Follow set of the current nonterminal, and
       then continues parsing.
