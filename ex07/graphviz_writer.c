/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#include "graphviz_writer.h"

#include <stdio.h>
#include <stdlib.h>

static void print_ast_as_graphviz_rec(const struct ast_node *node);
static void print_node_as_graphviz(const struct ast_node *node);

void print_ast_as_graphviz(const struct ast_node *node) {
    puts("digraph ast {");
    puts("\tnode [color=lightblue2, style=filled];");
    print_ast_as_graphviz_rec(node);
    puts("}");
}

static void print_ast_as_graphviz_rec(const struct ast_node *node) {
    if (node_uses_body(node)) {
        for (unsigned int i = 0; i < AST_NODE_BODY_MAX_SIZE; i++) {
            if (node->body[i] == NULL) {
                continue;
            }
            printf("\t");
            print_node_as_graphviz(node);
            printf(" -> ");
            print_node_as_graphviz(node->body[i]);
            printf(";\n");
            print_ast_as_graphviz_rec(node->body[i]);
        }
    }

    if (node->next) {
        printf("\t");
        print_node_as_graphviz(node);
        printf(" -> ");
        print_node_as_graphviz(node->next);
        printf(";\n");
        print_ast_as_graphviz_rec(node->next);
    }
}

static void print_node_as_graphviz(const struct ast_node *node) {
    switch (node->type) {
        case NODE_IDENT:
            printf("\"(%p)\\n%s\\n%s\"", node, node_type_string(node->type),
                    node->ident);
            break;

        case NODE_STR:
            printf("\"(%p)\\n%s\\n\\\"%s\\\"\"", node,
                    node_type_string(node->type), node->sValue);
            break;

        case NODE_INT:
            printf("\"(%p)\\n%s\\n%ld\"", node, node_type_string(node->type),
                    node->iValue);
            break;

        case NODE_REAL:
            printf("\"(%p)\\n%s\\n%f\"", node, node_type_string(node->type),
                    node->fValue);
            break;

        case NODE_BOOL:
            if (node->bValue) {
                printf("\"(%p)\\n%s\\nTrue\"", node,
                        node_type_string(node->type));
            } else {
                printf("\"(%p)\\n%s\\nFalse\"", node,
                        node_type_string(node->type));
            }
            break;

        case NODE_RELOP: {
            printf("\"(%p)\\n%s\\n", node,
                    node_type_string(node->type));
            switch (node->token) {
                case LT  : printf("<")  ; break ;
                case LEQ : printf("<=") ; break ;
                case GT  : printf(">")  ; break ;
                case GEQ : printf(">=") ; break ;
                case EQ  : printf("=")  ; break ;
                case NEQ : printf("<>") ; break ;
                default:
                    fprintf(stderr, "\n\nunknown token in RELOP\n");
                    exit(EXIT_FAILURE);
            }
            printf("\"");
            break;
        }

        case NODE_ADDOP: {
            printf("\"(%p)\\n%s\\n", node,
                    node_type_string(node->type));
            switch (node->token) {
                case PLUS  : printf("+")  ; break ;
                case MINUS : printf("-")  ; break ;
                case OR    : printf("OR") ; break ;
                default:
                    fprintf(stderr, "\n\nunknown token in ADDOP\n");
                    exit(EXIT_FAILURE);
            }
            printf("\"");
            break;
        }

        case NODE_MULOP: {
            printf("\"(%p)\\n%s\\n", node,
                    node_type_string(node->type));
            switch (node->token) {
                case ASTR  : printf("*")   ; break ;
                case SLASH : printf("/")   ; break ;
                case DIV   : printf("DIV") ; break ;
                case MOD   : printf("MOD") ; break ;
                case AND   : printf("AND") ; break ;
                default:
                    fprintf(stderr, "\n\nunknown token in MULOP\n");
                    exit(EXIT_FAILURE);
            }
            printf("\"");
            break;
        }

        default:
            printf("\"(%p)\\n%s\"", node, node_type_string(node->type));
            break;
    }
}
