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

#include "parser.h"

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
            /* ident
             * varDecList (optional)
             * compStmt
             */

    /* varDecList */
        NODE_VARDECLIST,
            /* identListType
             */

    /* identListType */
        NODE_IDENTLISTTYPE,
            /* identList
             * type
             */

    /* identList */
        NODE_IDENTLIST,
            /* ident
             */

    /* type */
        NODE_TYPE,
            /* num (optional)
             * num (optional)
             * simpleType
             */

    /* simpleType */
        NODE_SIMPLETYPE,        /* store type in token */

    /* compStmt / stmtList */
        NODE_STMTLIST,
            /* statement
             */

    /* statement */
        STMT_ASSIGN,
            /* ident
             * expr (optional)
             * expr
             */

        STMT_IF,
            /* expr
             * statement
             * statement (optional)
             */

        STMT_WHILE,
            /* expr
             * statement
             */

        STMT_FOR,
            /* identifier
             * expr
             * toPart
             * expr
             * statement
             */

        STMT_READ,
            /* exprList
             */

        STMT_WRITE,
            /* exprList
             */

    /* toPart */
        NODE_TOPART,            /* store toPart in token */

    /* expr */
        NODE_EXPR,
            /* simpleExpr
             * relOp (optional)
             * simpleExpr (optional)
             */

    /* exprList */
        NODE_EXPRLIST,
            /* expr
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

    /* identifier */
        NODE_IDENT,             /* store text in ident */

    /* number */
        NODE_NUM,               /* store value in iValue */

    /* string */
        NODE_STR,               /* store value in sValue */

    /* boolean */
        NODE_BOOL,              /* store value in bValue */

    /* relOp */
        NODE_RELOP,             /* store operator in token */

    /* addOp */
        NODE_ADDOP,             /* store operator in token */

    /* mulOp */
        NODE_MULOP,             /* store operator in token */

    /* factor */
        NODE_IDENT_SUBSCR,
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

        /* token is used to store the corresponding operator */
        enum yytokentype token;

        /* value is used to store data */
        long int iValue;
        double fValue;
        bool bValue;
        char *sValue;

    };

};

struct ast_node *node_new(enum ast_node_type type);

void node_destroy(struct ast_node *node);

void print_as_prascal(struct ast_node *node);

#endif
