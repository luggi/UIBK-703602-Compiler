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

enum ast_node_type {
        NODE_PROGRAM,
        NODE_VARDEC,
        NODE_VARDECLIST,
        NODE_IDENTLISTTYPE,
        NODE_IDENTLIST,
        NODE_TYPE,
        NODE_SIMPLETYPE,
        NODE_COMPSTMT,
        NODE_STMTLIST,
        NODE_STMT_ASSIGN,
        NODE_STMT_IF,
        NODE_STMT_WHILE,
        NODE_STMT_FOR,
        NODE_STMT_READ,
        NODE_STMT_WRITE,
        NODE_TOPART,
        NODE_EXPR,
        NODE_EXPRLIST,
        NODE_SIMPLEEXPR,
        NODE_TERM,
        NODE_FACTOR_NOT,
        NODE_FACTOR_MINUS,
        NODE_FACTOR_EXPR,
        NODE_IDENT,
        NODE_INT,
        NODE_REAL,
        NODE_STR,
        NODE_BOOL,
        NODE_RELOP,
        NODE_ADDOP,
        NODE_MULOP,
        NODE_IDENT_SUBSCR,
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

/* returns a new node with set type, calls exit() on failure */
struct ast_node *node_create(enum ast_node_type type);

/* recursivly free node */
void node_destroy(struct ast_node *node);

/* AST back to prascal */
void print_as_prascal(struct ast_node *node);

/* AST as graphviz */
void print_as_graphviz(struct ast_node *node);

/* returns string representation of node type */
const char *node_type_string(const enum ast_node_type);

#endif
