#Homework 4 (10 points)
Extend your recursive descendent parser with error detection and recovery functionality as follows:

1. Calculate by hand for each nonterminal of the EBNF grammar the First and the Follow sets using the algorithms presented at the lecture
2. Display a meaningful error message if the next lookahead token is not in the First set of the expected nonterminal.
3. In case of syntactic errors, implement a panic-mode error recovery that scans ahead until:
	a. it reaches a token in the First set of the current nonterminal or
	b. it reaches a token in the Follow set of the current nonterminal, and then continues parsing.