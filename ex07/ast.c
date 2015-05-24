/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ast_node *node_create(const enum ast_node_type type) {
    struct ast_node *node = (struct ast_node *) malloc(sizeof(struct ast_node));
    if (node == NULL) {
        fprintf(stderr, "could not allocate memory for AST node\n");
        exit(EXIT_FAILURE);
    }
    memset(node, 0, sizeof(struct ast_node));
    node->type = type;
    return node;
}

void node_destroy(struct ast_node *node) {
    if (node == NULL) {
        return;
    }

    switch (node->type) {
        case NODE_STR:
            free(node->sValue);
            break;

        default:
            break;
    }

    if (node_uses_body(node)) {
        for (unsigned int i = 0; i < AST_NODE_BODY_MAX_SIZE; i++) {
            node_destroy(node->body[i]);
        }
    }

    if (node->next != NULL) {
        node_destroy(node->next);
    }

    /* this node */
    free(node);
}

const char *node_type_string(const enum ast_node_type type) {
    switch (type) {
        case NODE_PROGRAM       : return "PROGRAM";
        case NODE_VARDEC        : return "VARDEC";
        case NODE_VARDECLIST    : return "VARDECLIST";
        case NODE_IDENTLISTTYPE : return "IDENTLISTTYPE";
        case NODE_IDENTLIST     : return "IDENTLIST";
        case NODE_TYPE          : return "TYPE";
        case NODE_SIMPLETYPE    : return "SIMPLETYPE";
        case NODE_COMPSTMT      : return "COMPSTMT";
        case NODE_STMTLIST      : return "STMTLIST";
        case NODE_STMT_ASSIGN   : return "STMT_ASSIGN";
        case NODE_STMT_IF       : return "STMT_IF";
        case NODE_STMT_WHILE    : return "STMT_WHILE";
        case NODE_STMT_FOR      : return "STMT_FOR";
        case NODE_STMT_READ     : return "STMT_READ";
        case NODE_STMT_WRITE    : return "STMT_WRITE";
        case NODE_TOPART        : return "TOPART";
        case NODE_EXPR          : return "EXPR";
        case NODE_EXPRLIST      : return "EXPRLIST";
        case NODE_SIMPLEEXPR    : return "SIMPLEEXPR";
        case NODE_TERM          : return "TERM";
        case NODE_FACTOR_NOT    : return "FACTOR_NOT";
        case NODE_FACTOR_MINUS  : return "FACTOR_MINUS";
        case NODE_FACTOR_EXPR   : return "FACTOR_EXPR";
        case NODE_IDENT         : return "IDENT";
        case NODE_INT           : return "INT";
        case NODE_REAL          : return "REAL";
        case NODE_STR           : return "STR";
        case NODE_BOOL          : return "BOOL";
        case NODE_RELOP         : return "RELOP";
        case NODE_ADDOP         : return "ADDOP";
        case NODE_MULOP         : return "MULOP";
        case NODE_IDENT_SUBSCR  : return "IDENT_SUBSCR";
        default                 : return "Unknown Node Type";
    }
}

bool node_uses_body(const struct ast_node *node) {
    switch (node->type) {
        case NODE_PROGRAM:
        case NODE_VARDEC:
        case NODE_VARDECLIST:
        case NODE_IDENTLISTTYPE:
        case NODE_IDENTLIST:
        case NODE_TYPE:
        case NODE_COMPSTMT:
        case NODE_STMTLIST:
        case NODE_STMT_ASSIGN:
        case NODE_STMT_IF:
        case NODE_STMT_WHILE:
        case NODE_STMT_FOR:
        case NODE_STMT_READ:
        case NODE_STMT_WRITE:
        case NODE_EXPR:
        case NODE_EXPRLIST:
        case NODE_SIMPLEEXPR:
        case NODE_TERM:
        case NODE_FACTOR_NOT:
        case NODE_FACTOR_MINUS:
        case NODE_FACTOR_EXPR:
        case NODE_IDENT_SUBSCR:
            return true;

        default:
            return false;
    }
}
