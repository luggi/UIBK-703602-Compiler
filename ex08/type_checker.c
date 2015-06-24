#include "type_checker.h"

#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "symbol_table.h"

void type_checker_run(void) {
    node_walk(SYNTAX_TREE, type_checker_dispatch)
}

/* do typechecking based on node->type */
static void type_checker_dispatch(struct ast_node *node) {
    switch (node->type) {
        default:
            break;
    }
}

/* returns the resuling type of an expression (also does conversion) */
static enum data_type type_of_expr(struct ast_node *node) {

    /* factor */

    if (node->type == NODE_IDENT) {
        return node->symbol->dtype;
    }

    if (node->type == NODE_IDENT_SUBSCR) {
        /* check subscript */
        if (type_of_expr(node->body[1]) != TYPE_INTEGER) {
            fprintf(stderr, "Error: subscript is not an integer\n");
            exit(EXIT_FAILURE);
        }
        return node->body[0]->symbol->dtype;
    }

    if (node->type == NODE_INT) {
        return TYPE_INTEGER;
    }

    if (node->type == NODE_REAL) {
        return TYPE_REAL;
    }

    if (node->type == NODE_BOOL) {
        return TYPE_BOOLEAN;
    }

    if (node->type == NODE_STR) {
        return TYPE_STRING;
    }

    if (node->type == NODE_FACTOR_NOT) {
        /* subsequent type has to be bool */
        if (type_of_expr(node->body[0]) != TYPE_BOOLEAN) {
            fprintf(stderr, "Error: NOT works only on boolean\n");
            exit(EXIT_FAILURE);
        }
        return TYPE_BOOLEAN;
    }

    if (node->type == NODE_FACTOR_MINUS) {
        /* subsequent type has to be int or real */
        if (type_of_expr(node->body[0] != TYPE_INTEGER) && type_of_expr(node->body[0]) != TYPE_REAL) {
            fprintf(stderr, "Error: MINUS works only on int and real\n");
            exit(EXIT_FAILURE);
        }
    }

    if (node->type == NODE_FACTOR_EXPR) {
        return type_of_expr(node->body[0]);
    }

    /* term */

    if (node->type == NODE_TERM && node->body[1]) {
        enum data_type lhs = type_of_expr(node->body[0]);
        enum data_type rhs = type_of_expr(node->body[2]);

        /* check mulOp */
        if (node->body[1]->token == AND) {
            if (lhs != TYPE_BOOLEAN || rhs != TYPE_BOOLEAN) {
                fprintf(stderr, "Error: AND works only on bool\n");
                exit(EXIT_FAILURE);
            }
        } else {
            /* convert int + real to real */
            if ((lhs == TYPE_REAL && rhs == TYPE_INTEGER) || (lhs == TYPE_INTEGER && rhs == TYPE_REAL)) {
                return TYPE_REAL;
            }
        }
    }

    if (node->type == NODE_TERM) {
        return type_of_expr(node->body[0]);
    }
}
