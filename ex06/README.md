# Homework 6 (20 Points)

1. Extend the Yacc-based LALR(1) parser with an attribute grammar for creating
   the abstract syntax tree (AST) representation of the input program.
2. Verify the correctness of the AST by traversing it in pre-order and
   generating a new source program semantically equivalent with the original
   source code.

Use the following data types and structures to represent the AST. The mapping
of grammar non-terminals to the AST data structure is described in the table
below.

```c
typedef enum {
    PROGRAM, ASSIGN, IF, WHILE, STATEMENT, CONST, VAR, TYPE, EXPR, INT_CONST,
    REAL_CONST, BOOL_CONST, STRING_CONST, IDENTIFIER, OP
} node_type;

typedef enum {
    PLUS, MINUS, MUL, DIV, MOD, LT, LE, GT, GE, EQ, NE, AND, OR
} operator;

typedef struct _node {
    node_type type;
    union {
        int iValue;         /* integer, true, false, compOp, addOp, mulOp */
        float fValue;       /* number */
        char *identifier;   /* identifier */
        /* list of BNF right-hand side symbols of nonterminal type */
        struct _node *body;
    };
    struct _node *next;     /* decl-list, stmt-list */
} node;
```

<table width="100%">
    <tr >
        <th></th>
        <th>type</th>
        <th>iValue</th>
        <th>fValue</th>
        <th>identifier</th>
        <th>body</th>
        <th>next</th>
    </tr>
    <tr>
        <th>program</th>
        <td>PROGRAM</td>
        <td></td>
        <td></td>
        <td></td>
        <td>
            <ol>
                <li>decl-list</li>
                <li>statement</li>
            </ol>
        </td>
        <td></td>
    </tr>
    <tr>
        <th>statement</th>
        <td>ASSIGN</td>
        <td></td>
        <td></td>
        <td></td>
        <td>
            <ol>
                <li>IDENTIFIER</li>
                <li>index (optional)</li>
                <li>expr</li>
            </ol>
        </td>
        <td></td>
    </tr>
    <tr>
        <th>statement</th>
        <td>IF</td>
        <td></td>
        <td></td>
        <td></td>
        <td>
            <ol>
                <li>expr</li>
                <li>statement</li>
                <li>else-part (optional)</li>
            </ol>
        </td>
        <td></td>
    </tr>
    <tr>
        <th>statement</th>
        <td>WHILE</td>
        <td></td>
        <td></td>
        <td></td>
        <td>
            <ol>
                <li>expr</li>
                <li>statement</li>
            </ol>
        </td>
        <td></td>
    </tr>
    <tr>
        <th>statement</th>
        <td>FOR</td>
        <td></td>
        <td></td>
        <td></td>
        <td>
            <ol>
                <li>IDENTIFIER</li>
                <li>expr</li>
                <li>expr</li>
                <li>statement</li>
            </ol>
        </td>
        <td></td>
    </tr>
    <tr>
        <th>statement</th>
        <td>READ WRITE</td>
        <td></td>
        <td></td>
        <td></td>
        <td>io-list</td>
        <td></td>
    </tr>
    <tr>
        <th>statement</th>
        <td>STATEMENT</td>
        <td></td>
        <td></td>
        <td></td>
        <td>stmt-list</td>
        <td></td>
    </tr>
    <tr>
        <th>decl-list</th>
        <td>declaration</td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td>decl-list</td>
    </tr>
    <tr>
        <th>declaration</th>
        <td>CONST</td>
        <td></td>
        <td></td>
        <td></td>
        <td>
            <ol>
                <li>IDENTIFIER</li>
                <li>expr</li>
            </ol>
        </td>
        <td></td>
    </tr>
    <tr>
        <th>declaration</th>
        <td>VAR</td>
        <td></td>
        <td></td>
        <td></td>
        <td>
            <ol>
                <li>IDENTIFIER</li>
                <li>index (optional)</li>
                <li>expr</li>
            </ol>
        </td>
        <td></td>
    </tr>
    <tr>
        <th>type</th>
        <td>TYPE</td>
        <td></td>
        <td></td>
        <td><b>IDENTIFIER</b></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <th>index</th>
        <td>expr</td>
        <td></td>
        <td></td>
        <td></td>
        <td>expr</td>
        <td></td>
    </tr>
    <tr>
        <th>else-part</th>
        <td>statement</td>
        <td></td>
        <td></td>
        <td></td>
        <td>statement</td>
        <td></td>
    </tr>
    <tr>
        <th>io-list</th>
        <td>IDENTIFIER STRING_CONST</td>
        <td></td>
        <td></td>
        <td></td>
        <td>
            <ol>
                <li>IDENTIFIER / STRING_CONST</li>
                <li>index (optional)</li>
            </ol>
        </td>
        <td>io-list</td>
    </tr>
    <tr>
        <th>stmt-list</th>
        <td>statement</td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td>stmt-list</td>
    </tr>
    <tr>
        <th>expr</th>
        <td>EXPR</td>
        <td></td>
        <td></td>
        <td></td>
        <td>
            <ol>
                <li>expr</li>
                <li>compOp, andOp, mulOp (optional)</li>
                <li>expr (optional)</li>
            </ol>
        </td>
        <td></td>
    </tr>
    <tr>
        <th>simpleExpr</th>
        <td>expr</td>
        <td></td>
        <td></td>
        <td></td>
        <td>expr</td>
        <td></td>
    </tr>
    <tr>
        <th>term</th>
        <td>expr</td>
        <td></td>
        <td></td>
        <td></td>
        <td>expr</td>
        <td></td>
    </tr>
    <tr>
        <th>factor</th>
        <td>INT_CONST</td>
        <td><b>NUMBER</b></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <th>factor</th>
        <td>REAL_CONST</td>
        <td></td>
        <td><b>NUMBER</b></td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <th>factor</th>
        <td>BOOL_CONST</td>
        <td><b>TRUE, FALSE</b></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <th>factor</th>
        <td>IDENTIFIER</td>
        <td></td>
        <td></td>
        <td><b>IDENTIFIER</b></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <th>factor</th>
        <td>expr</td>
        <td></td>
        <td></td>
        <td></td>
        <td>expr</td>
        <td></td>
    </tr>
    <tr>
        <th>compOp, addOp, mulOP</th>
        <td>OP</td>
        <td>operator</td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
</table>
