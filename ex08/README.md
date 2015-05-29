# Homework 8 (10 Points)

Consider the following semantic rules in our language:

- Expressions of type integer and real can be combined. The type of the
  resulting expression is real, if at least one operand is real; otherwise, it
  is integer.
- If the variable in an assign statement is real and the right hand side
  expression is integer, the rhs is implicitly converted to real.
- If the variable in an assign statement is integer and the right hand side
  expression is real, the rhs is implicitly converted to integer.

Extend your Yacc-based parser with semantic analysis that performs the
following semantic checks and outputs corresponding error messages (containing
line numbers).

1. variable not declared
2. variable already declared
3. wrong array index type
    - ex: `a[true]`, `a[3.14]`
    - consider also expressions and variables as index
4. negative array size declaration
5. type mismatch in assignment
    - ex: `b: real; b := false`
    - ex: assuming `a` is an array and `b` is a scalar variable:
        1. `b := a` is not allowed
        2. `a := b` is allowed (assigns `b` to every element of `a`)
6. illegal operand type (does not match operator)
    - ex: `(3.14 and 2.0)`
7. consider not boolean (in if and while statements)
    - ex: `if (4-3) then `
