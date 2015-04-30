/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#ifndef AST_H
#define AST_H

#include <stdbool.h>

#define AST_NODE_BODY_MAX_SIZE 5

/* This enum defines all possible types for a node.
 *
 * Comments directly *below* a node type indicate the content of the node's
 * body. This list may contain optional parts, but MUST NOT contain more
 * entries than AST_NODE_BODY_MAX_SIZE (including all optional entries).
 *
 * Comments directly to the *right* of a node type indicate the content of the
 * node's value.
 *
 * A Comment above a group of node types states the "super type" of these
 * types. This should be used as follows:
 *
 *      A node of type `NODE_STMTLIST` contains a node of type `statement` in
 *      its body. Since `statement` is here used as a "super type", the
 *      underlying node can be of type STMT_ASSIGN, STMT_IF, STMT_WHILE,
 *      STMT_FOR, STMT_READ or STMT_WRITE. Note that the prefix of a node type
 *      *can* indicate this relation.
 *
 */
enum ast_node_type {

    /* program */
        NODE_PROGRAM,
            /* identifier
             * identListType (optional)
             * stmtList
             */

    /* identifier */
        NODE_IDENTIFIER,       /* store text in ident */

    /* identListType */
        NODE_IDENTLISTTYPE,
            /* identList
             * type
             */

    /* identList */
        NODE_IDENTLIST,
            /* identifier
             */

    /* type */
        NODE_TYPE,
            /* num (optional)
             * num (optional)
             * simpleType
             */

    /* stmtList */
        NODE_STMTLIST,
            /* statement
             */

    /* exprList */
        NODE_EXPRLIST,
            /* expr
             */

    /* expr */
        NODE_EXPR,
            /* simpleExpr
             * relOp (optional)
             * simpleExpr (optional)
             */

    /* simpleExpr */
        NODE_SIMPLEEXPR,
            /* term
             * addOp (optional)
             * simpleExpr (optional)
             */

    /* term */
        NODE_TERM,
            /* factor
             * mulOp (optional)
             * simpleExpr (optional)
             */

    /* statement */
        STMT_ASSIGN,
            /* identifier
             * expr (optional)
             * expr
             */

        STMT_IF,
            /* expr
             * stmtList
             * stmtList (optional)
             */

        STMT_WHILE,
            /* expr
             * stmtList
             */

        STMT_FOR,
            /* identifier
             * expr
             * toPart
             * expr
             * stmtList
             */

        STMT_READ,
            /* exprList
             */

        STMT_WRITE,
            /* exprList
             */

    /* factor */
        /* num */
            FACTOR_INT,         /* store value in iValue */
            FACTOR_FLOAT,       /* store value in fValue */

        FACTOR_BOOL,            /* store value in bValue */

        FACTOR_STR,             /* store value in sValue */

        FACTOR_IDENT_SUBSCR,
            /* identifier
             * expr
             */

        FACTOR_NOT,
            /* factor
             */

        FACTOR_MINUS,
            /* factor
             */

        FACTOR_EXPR,
            /* expr
             */

    /* toPart */
        TOPART_TO,
        TOPART_DOWNTO,

    /* relOp */
        RELOP_EQ,
        RELOP_GEQ,
        RELOP_GT,
        RELOP_LEQ,
        RELOP_LT,
        RELOP_NEQ,

    /* addOp */
        ADDOP_PLUS,
        ADDOP_MINUS,
        ADDOP_OR,

    /* mulOp */
        MULOP_ASTR,
        MULOP_SLASH,
        MULOP_DIV,
        MULOP_MOD,
        MULOP_AND,

};

struct ast_node {

    /* type of this node */
    enum ast_node_type type;

    /* pointer to next node, only used in lists */
    struct ast_node *next;

    /* content of this node */
    union {

        /* body is a list of pointers to underlying nodes */
        struct ast_node *body[AST_NODE_BODY_MAX_SIZE];

        /* ident is used to store text of identifier */
        char *ident;

        /* value is used to store data */
        long int iValue;
        double fValue;
        bool bValue;
        char *sValue;

    };

};

#endif
