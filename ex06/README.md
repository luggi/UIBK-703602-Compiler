# Homework 6 (20 Points)

1. Extend the Yacc-based LALR(1) parser with an attribute grammar for creating
   the abstract syntax tree (AST) representation of the input program.
2. Verify the correctness of the AST by traversing it in pre-order and
   generating a new source program semantically equivalent with the original
   source code.

## Abstract Syntax Tree (AST)

<table width="100%">
    <tr>
        <th>Super Type<br></th>
        <th>Node Type</th>
        <th>Body<br></th>
        <th>Ident / Value/ Token</th>
        <th>uses next</th>
    </tr>
    <tr>
        <td>program</td>
        <td>NODE_PROGRAM</td>
        <td>identifier<br>identListType (optional)<br>stmtList</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>identListType</td>
        <td>NODE_IDENTLISTTYPE</td>
        <td>identList<br>type</td>
        <td></td>
        <td>yes</td>
    </tr>
    <tr>
        <td>identList</td>
        <td>NODE_IDENTLIST</td>
        <td>identifier</td>
        <td></td>
        <td>yes</td>
    </tr>
    <tr>
        <td>type</td>
        <td>NODE_TYPE</td>
        <td>num (optional)<br>num (optional)<br>simpleType</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>stmtList</td>
        <td>NODE_STMTLIST</td>
        <td>statement</td>
        <td></td>
        <td>yes</td>
    </tr>
    <tr>
        <td>exprList</td>
        <td>NODE_EXPRLIST</td>
        <td>expr</td>
        <td></td>
        <td>yes</td>
    </tr>
    <tr>
        <td>expr</td>
        <td>NODE_EXPR</td>
        <td>simpleExpr<br>relOp (optional)<br>simpleExpr (optional)</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>simpleExpr</td>
        <td>NODE_SIMPLEEXPR</td>
        <td>term<br>addOp (optional)<br>simpleExpr</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>term</td>
        <td>NODE_TERM</td>
        <td>factor<br>mulOp (optional)<br>simpleExpr (optional)</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>identifier</td>
        <td>NODE_IDENTIFIER</td>
        <td></td>
        <td>text</td>
        <td>no</td>
    </tr>
    <tr>
        <td>simpleType</td>
        <td>NODE_SIMPLETYPE</td>
        <td></td>
        <td>type</td>
        <td>no</td>
    </tr>
    <tr>
        <td>toPart</td>
        <td>NODE_TOPART</td>
        <td></td>
        <td>TO / DOWNTO</td>
        <td>no</td>
    </tr>
    <tr>
        <td>relOp</td>
        <td>NODE_RELOP</td>
        <td></td>
        <td>EQ / GEQ / GT / LEQ / LT NEQ</td>
        <td>no</td>
    </tr>
    <tr>
        <td>addOp</td>
        <td>NODE_ADDOP</td>
        <td></td>
        <td>PLUS / MINUS / OR</td>
        <td>no</td>
    </tr>
    <tr>
        <td>mulOp</td>
        <td>NODE_MULOP</td>
        <td></td>
        <td>AST / SLASH / DIV / MOD / AND</td>
        <td>no</td>
    </tr>
    <tr>
        <td>statement</td>
        <td>STMT_ASSIGN</td>
        <td>identifier<br>expr (optional)<br>expr</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>statement</td>
        <td>STMT_IF</td>
        <td>expr<br>stmtList<br>stmtList (optional)</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>statement</td>
        <td>STMT_WHILE</td>
        <td>expr<br>stmtList</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>statement</td>
        <td>STMT_FOR</td>
        <td>identifier<br>expr<br>toPart<br>expr<br>stmtList</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>statement</td>
        <td>STMT_READ</td>
        <td>exprList</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>statement</td>
        <td>STMT_WRITE</td>
        <td>exprList</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>factor, num</td>
        <td>FACTOR_INT</td>
        <td></td>
        <td>integer</td>
        <td>no</td>
    </tr>
    <tr>
        <td>factor, num</td>
        <td>FACTOR_FLOAT</td>
        <td></td>
        <td>float</td>
        <td>no</td>
    </tr>
    <tr>
        <td>factor</td>
        <td>FACTOR_BOOL</td>
        <td></td>
        <td>boolean</td>
        <td>no</td>
    </tr>
    <tr>
        <td>factor</td>
        <td>FACTOR_STR</td>
        <td></td>
        <td>string</td>
        <td>no</td>
    </tr>
    <tr>
        <td>factor</td>
        <td>FACTOR_IDENT_SUBSCR</td>
        <td>identifier<br>expr</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>factor</td>
        <td>FACTOR_NOT</td>
        <td>factor</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>factor</td>
        <td>FACTOR_MINUS</td>
        <td>factor</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>factor</td>
        <td>FACTOR_EXPR</td>
        <td>expr</td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>identifier</td>
        <td>NODE_IDENTIFIER</td>
        <td></td>
        <td>text</td>
        <td>no</td>
    </tr>
    <tr>
        <td>toPart</td>
        <td>NODE_TOPART</td>
        <td></td>
        <td>TO / DOWNTO</td>
        <td>no</td>
    </tr>
    <tr>
        <td>relOp</td>
        <td>NODE_RELOP</td>
        <td></td>
        <td>EQ / GEQ / GT / LEQ / LT NEQ</td>
        <td>no</td>
    </tr>
    <tr>
        <td>addOp</td>
        <td>NODE_ADDOP</td>
        <td></td>
        <td>PLUS / MINUS / OR</td>
        <td>no</td>
    </tr>
    <tr>
        <td>mulOp</td>
        <td>NODE_MULOP</td>
        <td></td>
        <td>AST / SLASH / DIV / MOD / AND</td>
        <td>no</td>
    </tr>
</table>
