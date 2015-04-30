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
        <th>Ident / Value<br></th>
        <th>uses next<br></th>
    </tr>
    <tr>
        <td>program</td>
        <td>NODE_PROGRAM</td>
        <td>identifier<br>identListType (optional)<br>stmtList<br></td>
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
        <td>identListType</td>
        <td>NODE_IDENTLISTTYPE</td>
        <td>identList<br>type<br></td>
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
        <td>num (optional)<br>num (optional)<br>simpleType<br></td>
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
        <td>simpleExpr<br>relOp (optional)<br>simpleExpr (optional)<br></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>simpleExpr</td>
        <td>NODE_SIMPLEEXPR</td>
        <td>term<br>addOp (optional)<br>simpleExpr<br></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>term</td>
        <td>NODE_TERM</td>
        <td>factor<br>mulOp (optional)<br>simpleExpr (optional)<br></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>statement</td>
        <td>STMT_ASSIGN</td>
        <td>identifier<br>expr (optional)<br>expr<br></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>statement</td>
        <td>STMT_IF</td>
        <td>expr<br>stmtList<br>stmtList (optional)<br></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>statement</td>
        <td>STMT_WHILE</td>
        <td>expr<br>stmtList<br></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>statement</td>
        <td>STMT_FOR</td>
        <td>identifier<br>expr<br>toPart<br>expr<br>stmtList<br></td>
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
        <td>factor, num<br></td>
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
        <td>FACTOR_BOOL<br></td>
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
        <td>factor<br></td>
        <td>FACTOR_IDENT_SUBSCR</td>
        <td>identifier<br>expr<br></td>
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
        <td>toPart</td>
        <td>TOPART_TO</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>toPart</td>
        <td>TOPART_DOWNTO</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>relOp</td>
        <td>RELOP_EQ</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>relOp</td>
        <td>RELOP_GEQ</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>relOp</td>
        <td>RELOP_GT</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>relOp</td>
        <td>RELOP_LEQ</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>relOp</td>
        <td>RELOP_LT</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>relOp</td>
        <td>RELOP_NEQ</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>addOp</td>
        <td>ADDOP_PLUS</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>addOp</td>
        <td>ADDOP_MINUS</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>addOp</td>
        <td>ADDOP_OR</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>mulOp</td>
        <td>MULOP_ASTR</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>mulOp</td>
        <td>MULOP_SLASH</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>mulOp</td>
        <td>MULOP_DIV</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>mulOp</td>
        <td>MULOP_MOD</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
    <tr>
        <td>mulOp</td>
        <td>MULOP_AND</td>
        <td></td>
        <td></td>
        <td>no</td>
    </tr>
</table>