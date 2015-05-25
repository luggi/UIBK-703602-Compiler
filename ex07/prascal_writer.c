/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#include "prascal_writer.h"

#include <stdio.h>
#include <stdlib.h>

#include "symbol_table.h"

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
    struct symbol_entry *entry = (struct symbol_entry *) node->symbol;
    printf(" %s ", entry->symbol);
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
