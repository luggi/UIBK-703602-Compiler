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

static bool node_uses_body(const struct ast_node *node);
static void print_ast_as_graphviz_rec(const struct ast_node *node);
static void print_node_as_graphviz(const struct ast_node *node);
static void print_NODE_PROGRAM(const struct ast_node *node);
static void print_NODE_VARDEC(const struct ast_node *node);
static void print_NODE_VARDECLIST(const struct ast_node *node);
static void print_NODE_IDENTLISTTYPE(const struct ast_node *node);
static void print_NODE_IDENTLIST(const struct ast_node *node);
static void print_NODE_TYPE(const struct ast_node *node);
static void print_NODE_SIMPLETYPE(const struct ast_node *node);
static void print_NODE_COMPSTMT(const struct ast_node *node);
static void print_NODE_STMTLIST(const struct ast_node *node);
static void print_NODE_STMT_ASSIGN(const struct ast_node *node);
static void print_NODE_STMT_IF(const struct ast_node *node);
static void print_NODE_STMT_WHILE(const struct ast_node *node);
static void print_NODE_STMT_FOR(const struct ast_node *node);
static void print_NODE_STMT_READ(const struct ast_node *node);
static void print_NODE_STMT_WRITE(const struct ast_node *node);
static void print_NODE_TOPART(const struct ast_node *node);
static void print_NODE_EXPRLIST(const struct ast_node *node);
static void print_NODE_EXPR(const struct ast_node *node);
static void print_NODE_SIMPLEEXPR(const struct ast_node *node);
static void print_NODE_TERM(const struct ast_node *node);
static void print_NODE_FACTOR_NOT(const struct ast_node *node);
static void print_NODE_FACTOR_MINUS(const struct ast_node *node);
static void print_NODE_FACTOR_EXPR(const struct ast_node *node);
static void print_NODE_IDENT_SUBSCR(const struct ast_node *node);
static void print_NODE_INT(const struct ast_node *node);
static void print_NODE_REAL(const struct ast_node *node);
static void print_NODE_STR(const struct ast_node *node);
static void print_NODE_BOOL(const struct ast_node *node);
static void print_NODE_IDENT(const struct ast_node *node);
static void print_NODE_RELOP(const struct ast_node *node);
static void print_NODE_ADDOP(const struct ast_node *node);
static void print_NODE_MULOP(const struct ast_node *node);

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
        case NODE_IDENT:
            free(node->ident);
            break;

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

void print_ast_as_graphviz(const struct ast_node *node) {
    puts("digraph ast {");
    puts("\tnode [color=lightblue2, style=filled];");
    print_ast_as_graphviz_rec(node);
    puts("}");
}

static bool node_uses_body(const struct ast_node *node) {
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

        default:
            printf("\"(%p)\\n%s\"", node, node_type_string(node->type));
            break;
    }
}

void print_ast_as_prascal(const struct ast_node *node) {
    if (node == NULL) {
        fprintf(stderr, "\n\nnode is NULL\n");
        exit(EXIT_FAILURE);
    }

    switch (node->type) {
        case NODE_PROGRAM       : print_NODE_PROGRAM(node)       ; break ;
        case NODE_VARDEC        : print_NODE_VARDEC(node)        ; break ;
        case NODE_VARDECLIST    : print_NODE_VARDECLIST(node)    ; break ;
        case NODE_IDENTLISTTYPE : print_NODE_IDENTLISTTYPE(node) ; break ;
        case NODE_IDENTLIST     : print_NODE_IDENTLIST(node)     ; break ;
        case NODE_TYPE          : print_NODE_TYPE(node)          ; break ;
        case NODE_SIMPLETYPE    : print_NODE_SIMPLETYPE(node)    ; break ;
        case NODE_COMPSTMT      : print_NODE_COMPSTMT(node)      ; break ;
        case NODE_STMTLIST      : print_NODE_STMTLIST(node)      ; break ;
        case NODE_STMT_ASSIGN   : print_NODE_STMT_ASSIGN(node)   ; break ;
        case NODE_STMT_IF       : print_NODE_STMT_IF(node)       ; break ;
        case NODE_STMT_WHILE    : print_NODE_STMT_WHILE(node)    ; break ;
        case NODE_STMT_FOR      : print_NODE_STMT_FOR(node)      ; break ;
        case NODE_STMT_READ     : print_NODE_STMT_READ(node)     ; break ;
        case NODE_STMT_WRITE    : print_NODE_STMT_WRITE(node)    ; break ;
        case NODE_TOPART        : print_NODE_TOPART(node)        ; break ;
        case NODE_EXPR          : print_NODE_EXPR(node)          ; break ;
        case NODE_EXPRLIST      : print_NODE_EXPRLIST(node)      ; break ;
        case NODE_SIMPLEEXPR    : print_NODE_SIMPLEEXPR(node)    ; break ;
        case NODE_TERM          : print_NODE_TERM(node)          ; break ;
        case NODE_FACTOR_NOT    : print_NODE_FACTOR_NOT(node)    ; break ;
        case NODE_FACTOR_MINUS  : print_NODE_FACTOR_MINUS(node)  ; break ;
        case NODE_FACTOR_EXPR   : print_NODE_FACTOR_EXPR(node)   ; break ;
        case NODE_IDENT         : print_NODE_IDENT(node)         ; break ;
        case NODE_INT           : print_NODE_INT(node)           ; break ;
        case NODE_REAL          : print_NODE_REAL(node)          ; break ;
        case NODE_STR           : print_NODE_STR(node)           ; break ;
        case NODE_BOOL          : print_NODE_BOOL(node)          ; break ;
        case NODE_RELOP         : print_NODE_RELOP(node)         ; break ;
        case NODE_ADDOP         : print_NODE_ADDOP(node)         ; break ;
        case NODE_MULOP         : print_NODE_MULOP(node)         ; break ;
        case NODE_IDENT_SUBSCR  : print_NODE_IDENT_SUBSCR(node)  ; break ;
        default:
            fprintf(stderr, "\n\nunknown type\n");
            exit(EXIT_FAILURE);
    }
}

static void print_NODE_PROGRAM(const struct ast_node *node) {
    printf(" PROGRAM ");
    print_ast_as_prascal(node->body[0]);
    printf(" ; ");
    if (node->body[1]) {
        print_ast_as_prascal(node->body[1]);
    }
    print_ast_as_prascal(node->body[2]);
    printf(" .\n");
}

static void print_NODE_VARDEC(const struct ast_node *node) {
    printf(" VAR ");
    print_ast_as_prascal(node->body[0]);
}

static void print_NODE_VARDECLIST(const struct ast_node *node) {
    print_ast_as_prascal(node->body[0]);
    printf(" ; ");
    if (node->next) {
        print_NODE_VARDECLIST(node->next);
    }
}

static void print_NODE_IDENTLISTTYPE(const struct ast_node *node) {
    print_ast_as_prascal(node->body[0]);
    printf(" : ");
    print_ast_as_prascal(node->body[1]);
}

static void print_NODE_IDENTLIST(const struct ast_node *node) {
    print_ast_as_prascal(node->body[0]);
    if (node->next) {
        printf(" , ");
        print_NODE_IDENTLIST(node->next);
    }
}

static void print_NODE_TYPE(const struct ast_node *node) {
    if (node->body[0]) {
        printf(" ARRAY [ ");
        print_ast_as_prascal(node->body[0]);
        printf(" .. ");
        print_ast_as_prascal(node->body[1]);
        printf(" ] OF ");
    }
    print_ast_as_prascal(node->body[2]);
}

static void print_NODE_SIMPLETYPE(const struct ast_node *node) {
    switch (node->token) {
        case INTEGER : printf(" integer ") ; break ;
        case REAL    : printf(" real ")    ; break ;
        case BOOLEAN : printf(" boolean ") ; break ;
        default:
            fprintf(stderr, "\n\nunknown token in SIMPLETYPE\n");
            exit(EXIT_FAILURE);
    }
}

static void print_NODE_COMPSTMT(const struct ast_node *node) {
    printf(" BEGIN ");
    print_ast_as_prascal(node->body[0]);
    printf(" END ");
}

static void print_NODE_STMTLIST(const struct ast_node *node) {
    print_ast_as_prascal(node->body[0]);
    if (node->next) {
        printf(" ; ");
        print_NODE_STMTLIST(node->next);
    }
}

static void print_NODE_STMT_ASSIGN(const struct ast_node *node) {
    print_ast_as_prascal(node->body[0]);
    if (node->body[1]) {
        printf(" [ ");
        print_ast_as_prascal(node->body[1]);
        printf(" ] ");
    }
    printf(" := ");
    print_ast_as_prascal(node->body[2]);
}

static void print_NODE_STMT_IF(const struct ast_node *node) {
    printf(" IF ");
    print_ast_as_prascal(node->body[0]);
    printf(" THEN ");
    print_ast_as_prascal(node->body[1]);
    if (node->body[2]) {
        printf(" ELSE ");
        print_ast_as_prascal(node->body[2]);
    }
}

static void print_NODE_STMT_WHILE(const struct ast_node *node) {
    printf(" WHILE ");
    print_ast_as_prascal(node->body[0]);
    printf(" DO ");
    print_ast_as_prascal(node->body[1]);
}

static void print_NODE_STMT_FOR(const struct ast_node *node) {
    printf(" FOR ");
    print_ast_as_prascal(node->body[0]);
    printf(" := ");
    print_ast_as_prascal(node->body[1]);
    print_ast_as_prascal(node->body[2]);
    print_ast_as_prascal(node->body[3]);
    printf(" DO ");
    print_ast_as_prascal(node->body[4]);
}

static void print_NODE_STMT_READ(const struct ast_node *node) {
    printf(" READ ( ");
    print_ast_as_prascal(node->body[0]);
    printf(" ) ");
}

static void print_NODE_STMT_WRITE(const struct ast_node *node) {
    printf(" WRITE ( ");
    print_ast_as_prascal(node->body[0]);
    printf(" ) ");
}

static void print_NODE_TOPART(const struct ast_node *node) {
    switch (node->token) {
        case TO     : printf(" TO ")     ; break ;
        case DOWNTO : printf(" DOWNTO ") ; break ;
        default:
            fprintf(stderr, "\n\nunknown token in TOPART\n");
            exit(EXIT_FAILURE);
    }
}

static void print_NODE_EXPRLIST(const struct ast_node *node) {
    print_ast_as_prascal(node->body[0]);
    if (node->next) {
        printf(" , ");
        print_NODE_EXPRLIST(node->next);
    }
}

static void print_NODE_EXPR(const struct ast_node *node) {
    print_ast_as_prascal(node->body[0]);
    if (node->body[1]) {
        print_ast_as_prascal(node->body[1]);
        print_ast_as_prascal(node->body[2]);
    }
}

static void print_NODE_SIMPLEEXPR(const struct ast_node *node) {
    print_ast_as_prascal(node->body[0]);
    if (node->body[1]) {
        print_ast_as_prascal(node->body[1]);
        print_ast_as_prascal(node->body[2]);
    }
}

static void print_NODE_TERM(const struct ast_node *node) {
    print_ast_as_prascal(node->body[0]);
    if (node->body[1]) {
        print_ast_as_prascal(node->body[1]);
        print_ast_as_prascal(node->body[2]);
    }
}

static void print_NODE_FACTOR_NOT(const struct ast_node *node) {
    printf(" NOT ");
    print_ast_as_prascal(node->body[0]);
}

static void print_NODE_FACTOR_MINUS(const struct ast_node *node) {
    printf(" - ");
    print_ast_as_prascal(node->body[0]);
}

static void print_NODE_FACTOR_EXPR(const struct ast_node *node) {
    printf(" ( ");
    print_ast_as_prascal(node->body[0]);
    printf(" ) ");
}

static void print_NODE_IDENT_SUBSCR(const struct ast_node *node) {
    print_ast_as_prascal(node->body[0]);
    printf(" [ ");
    print_ast_as_prascal(node->body[1]);
    printf(" ] ");
}

static void print_NODE_IDENT(const struct ast_node *node) {
    printf(" %s ", node->ident);
}

static void print_NODE_INT(const struct ast_node *node) {
    printf(" %ld ", node->iValue);
}

static void print_NODE_REAL(const struct ast_node *node) {
    printf(" %f ", node->fValue);
}

static void print_NODE_STR(const struct ast_node *node) {
    printf(" \"%s\" ", node->sValue);
}

static void print_NODE_BOOL(const struct ast_node *node) {
    if (node->bValue) {
        printf(" TRUE ");
    } else {
        printf(" FALSE ");
    }
}

static void print_NODE_RELOP(const struct ast_node *node) {
    switch (node->token) {
        case LT  : printf(" < ")  ; break ;
        case LEQ : printf(" <= ") ; break ;
        case GT  : printf(" > ")  ; break ;
        case GEQ : printf(" >= ") ; break ;
        case EQ  : printf(" = ")  ; break ;
        case NEQ : printf(" <> ") ; break ;
        default:
            fprintf(stderr, "\n\nunknown token in RELOP\n");
            exit(EXIT_FAILURE);
    }
}

static void print_NODE_ADDOP(const struct ast_node *node) {
    switch (node->token) {
        case PLUS  : printf(" + ")  ; break ;
        case MINUS : printf(" - ")  ; break ;
        case OR    : printf(" OR ") ; break ;
        default:
            fprintf(stderr, "\n\nunknown token in ADDOP\n");
            exit(EXIT_FAILURE);
    }
}

static void print_NODE_MULOP(const struct ast_node *node) {
    switch (node->token) {
        case ASTR  : printf(" * ")   ; break ;
        case SLASH : printf(" / ")   ; break ;
        case DIV   : printf(" DIV ") ; break ;
        case MOD   : printf(" MOD ") ; break ;
        case AND   : printf(" AND ") ; break ;
        default:
            fprintf(stderr, "\n\nunknown token in MULOP\n");
            exit(EXIT_FAILURE);
    }
}
