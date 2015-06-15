# Homework 9 (20 Points)

Implement a code generator that translates the mini-Pascal programming language
into MIPS32 assembly language using the following rules:

1. Traverse the symbol table to generate the data declaration section
   containing both scalar and array variables
2. Load variable references into register `$t0` (integers and booleans) or
   `$f0` (reals)
3. Evaluate binary operations in expressions using two registers as follows
    - Recursively evaluate the left operand and make the result available in
      register `$t0 / $f0`
    - Push the result from register `$t0 / $f0` onto the stack
    - Recursively evaluate the right operand and make the result available in
      register `$t0 / $f0`
    - Pop the left operand from the stack into register `$t1 / $f1`
    - Perform the operation is performed using the operands from `$t1` and
      `$t0`
4. Store the value from register `$t0`, respectively `$f0`, into the memory
   reference for assignment statements
5. Use branch and jump instructions for the for, while and if statements (see
   the lecture)
6. Test the generated code using the SPIM simulator available at:
   <http://spimsimulator.sourceforge.net/>.
