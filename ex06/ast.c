#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ast_node *node_create(enum ast_node_type type) {
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
            /* free all in body */
            for (unsigned int i = 0; i < AST_NODE_BODY_MAX_SIZE; i++) {
                node_destroy(node->body[i]);
            }
            break;

        default:
            break;
    }

    if (node->next != NULL) {
        node_destroy(node->next);
    }

    /* this node */
    free(node);
}

/* print functions */
static void print_NODE_PROGRAM(struct ast_node *node);
static void print_NODE_VARDEC(struct ast_node *node);
static void print_NODE_VARDECLIST(struct ast_node *node);
static void print_NODE_IDENTLISTTYPE(struct ast_node *node);
static void print_NODE_IDENTLIST(struct ast_node *node);
static void print_NODE_TYPE(struct ast_node *node);
static void print_NODE_SIMPLETYPE(struct ast_node *node);
static void print_NODE_COMPSTMT(struct ast_node *node);
static void print_NODE_STMTLIST(struct ast_node *node);
static void print_NODE_STMT_ASSIGN(struct ast_node *node);
static void print_NODE_STMT_IF(struct ast_node *node);
static void print_NODE_STMT_WHILE(struct ast_node *node);
static void print_NODE_STMT_FOR(struct ast_node *node);
static void print_NODE_STMT_READ(struct ast_node *node);
static void print_NODE_STMT_WRITE(struct ast_node *node);
static void print_NODE_TOPART(struct ast_node *node);
static void print_NODE_EXPRLIST(struct ast_node *node);
static void print_NODE_EXPR(struct ast_node *node);
static void print_NODE_SIMPLEEXPR(struct ast_node *node);
static void print_NODE_TERM(struct ast_node *node);
static void print_NODE_FACTOR_NOT(struct ast_node *node);
static void print_NODE_FACTOR_MINUS(struct ast_node *node);
static void print_NODE_FACTOR_EXPR(struct ast_node *node);
static void print_NODE_IDENT_SUBSCR(struct ast_node *node);
static void print_NODE_NUM(struct ast_node *node);
static void print_NODE_STR(struct ast_node *node);
static void print_NODE_BOOL(struct ast_node *node);
static void print_NODE_IDENT(struct ast_node *node);
static void print_NODE_RELOP(struct ast_node *node);
static void print_NODE_ADDOP(struct ast_node *node);
static void print_NODE_MULOP(struct ast_node *node);

void print_as_prascal(struct ast_node *node) {
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
        case NODE_NUM           : print_NODE_NUM(node)           ; break ;
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

static void print_NODE_PROGRAM(struct ast_node *node) {
    printf(" PROGRAM ");
    print_as_prascal(node->body[0]);
    printf(" ; ");
    if (node->body[1]) {
        print_as_prascal(node->body[1]);
    }
    print_as_prascal(node->body[2]);
    printf(" .\n");
}

static void print_NODE_VARDEC(struct ast_node *node) {
    printf(" VAR ");
    print_as_prascal(node->body[0]);
}

static void print_NODE_VARDECLIST(struct ast_node *node) {
    print_as_prascal(node->body[0]);
    printf(" ; ");
    if (node->next) {
        print_NODE_VARDECLIST(node->next);
    }
}

static void print_NODE_IDENTLISTTYPE(struct ast_node *node) {
    print_as_prascal(node->body[0]);
    printf(" : ");
    print_as_prascal(node->body[1]);
}

static void print_NODE_IDENTLIST(struct ast_node *node) {
    print_as_prascal(node->body[0]);
    if (node->next) {
        printf(" , ");
        print_NODE_IDENTLIST(node->next);
    }
}

static void print_NODE_TYPE(struct ast_node *node) {
    if (node->body[0]) {
        printf(" ARRAY [ ");
        print_as_prascal(node->body[0]);
        printf(" .. ");
        print_as_prascal(node->body[1]);
        printf(" ] OF ");
    }
    print_as_prascal(node->body[2]);
}

static void print_NODE_SIMPLETYPE(struct ast_node *node) {
    switch (node->token) {
        case INTEGER : printf(" INTEGER ") ; break ;
        case REAL    : printf(" REAL ")    ; break ;
        case BOOLEAN : printf(" BOOLEAN ") ; break ;
        default:
            fprintf(stderr, "\n\nunknown token in SIMPLETYPE\n");
            exit(EXIT_FAILURE);
    }
}

static void print_NODE_COMPSTMT(struct ast_node *node) {
    printf(" BEGIN ");
    print_as_prascal(node->body[0]);
    printf(" END ");
}

static void print_NODE_STMTLIST(struct ast_node *node) {
    print_as_prascal(node->body[0]);
    if (node->next) {
        printf(" ; ");
        print_NODE_STMTLIST(node->next);
    }
}

static void print_NODE_STMT_ASSIGN(struct ast_node *node) {
    print_as_prascal(node->body[0]);
    if (node->body[1]) {
        printf(" [ ");
        print_as_prascal(node->body[1]);
        printf(" ] ");
    }
    printf(" := ");
    print_as_prascal(node->body[2]);
}

static void print_NODE_STMT_IF(struct ast_node *node) {
    printf(" IF ");
    print_as_prascal(node->body[0]);
    printf(" THEN ");
    print_as_prascal(node->body[1]);
    if (node->body[2]) {
        printf(" ELSE ");
        print_as_prascal(node->body[2]);
    }
}

static void print_NODE_STMT_WHILE(struct ast_node *node) {
    printf(" WHILE ");
    print_as_prascal(node->body[0]);
    printf(" DO ");
    print_as_prascal(node->body[1]);
}

static void print_NODE_STMT_FOR(struct ast_node *node) {
    printf(" FOR ");
    print_as_prascal(node->body[0]);
    printf(" := ");
    print_as_prascal(node->body[1]);
    print_as_prascal(node->body[2]);
    print_as_prascal(node->body[3]);
    printf(" DO ");
    print_as_prascal(node->body[4]);
}

static void print_NODE_STMT_READ(struct ast_node *node) {
    printf(" READ ( ");
    print_as_prascal(node->body[0]);
    printf(" ) ");
}

static void print_NODE_STMT_WRITE(struct ast_node *node) {
    printf(" WRITE ( ");
    print_as_prascal(node->body[0]);
    printf(" ) ");
}

static void print_NODE_TOPART(struct ast_node *node) {
    switch (node->token) {
        case TO     : printf(" TO ")     ; break ;
        case DOWNTO : printf(" DOWNTO ") ; break ;
        default:
            fprintf(stderr, "\n\nunknown token in TOPART\n");
            exit(EXIT_FAILURE);
    }
}

static void print_NODE_EXPRLIST(struct ast_node *node) {
    print_as_prascal(node->body[0]);
    if (node->next) {
        printf(" , ");
        print_NODE_EXPRLIST(node->next);
    }
}

static void print_NODE_EXPR(struct ast_node *node) {
    print_as_prascal(node->body[0]);
    if (node->body[1]) {
        print_as_prascal(node->body[1]);
        print_as_prascal(node->body[2]);
    }
}

static void print_NODE_SIMPLEEXPR(struct ast_node *node) {
    print_as_prascal(node->body[0]);
    if (node->body[1]) {
        print_as_prascal(node->body[1]);
        print_as_prascal(node->body[2]);
    }
}

static void print_NODE_TERM(struct ast_node *node) {
    print_as_prascal(node->body[0]);
    if (node->body[1]) {
        print_as_prascal(node->body[1]);
        print_as_prascal(node->body[2]);
    }
}

static void print_NODE_FACTOR_NOT(struct ast_node *node) {
    printf(" NOT ");
    print_as_prascal(node->body[0]);
}

static void print_NODE_FACTOR_MINUS(struct ast_node *node) {
    printf(" - ");
    print_as_prascal(node->body[0]);
}

static void print_NODE_FACTOR_EXPR(struct ast_node *node) {
    printf(" ( ");
    print_as_prascal(node->body[0]);
    printf(" ) ");
}

static void print_NODE_IDENT_SUBSCR(struct ast_node *node) {
    print_as_prascal(node->body[0]);
    printf(" [ ");
    print_as_prascal(node->body[1]);
    printf(" ] ");
}

static void print_NODE_IDENT(struct ast_node *node) {
    printf(" %s ", node->ident);
}

static void print_NODE_NUM(struct ast_node *node) {
    printf(" %ld ", node->iValue);
}

static void print_NODE_STR(struct ast_node *node) {
    printf(" \"%s\" ", node->sValue);
}

static void print_NODE_BOOL(struct ast_node *node) {
    if (node->bValue) {
        printf(" TRUE ");
    } else {
        printf(" FALSE ");
    }
}

static void print_NODE_RELOP(struct ast_node *node) {
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

static void print_NODE_ADDOP(struct ast_node *node) {
    switch (node->token) {
        case PLUS  : printf(" + ")  ; break ;
        case MINUS : printf(" - ")  ; break ;
        case OR    : printf(" OR ") ; break ;
        default:
            fprintf(stderr, "\n\nunknown token in ADDOP\n");
            exit(EXIT_FAILURE);
    }
}

static void print_NODE_MULOP(struct ast_node *node) {
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
