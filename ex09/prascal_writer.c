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

int labelc;
char t0;
char t1;

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
    printf(" .data\n");
    if (node->body[0]) {
        print_ast_as_prascal(node->body[1]);
    }

    //intermediate section
    printf("\n .text\n");
    printf(" .globl main\n");
    printf("main:\n");
    print_ast_as_prascal(node->body[1]);
    printf("\n\t#Exit\n");
    printf("\tli\t$v0, 10\n");
    printf("\tsyscall\n");
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
    print_ast_as_prascal(node->body[0]);
    printf(" \n ");
}

static void print_NODE_STMTLIST(const struct ast_node *node) {
    print_ast_as_prascal(node->body[0]);
    if (node->next) {
        //printf(" ; ");
        print_NODE_STMTLIST(node->next);
    }
}

static void print_NODE_STMT_ASSIGN(const struct ast_node *node) {
    //eval expr
    printf("\t# assign\n");
    print_ast_as_prascal(node->body[2]);
    print_ast_as_prascal(node->body[0]);
    //result of expr is in $t0
       //print code to store result of expr
    if (node->body[1]) {
        //printf(" [ ");
        //save value of expr in $t2
        printf("\tmove\t$t2, $t0 #store expr val in $t2\n");
        //load array index expr value
        print_ast_as_prascal(node->body[1]);
        //calculate the index //shift left <<2 = *4
        printf("\tsll\t$t0, $t0, 2 #index calculation\n");
        //load array andress in $t1
        printf("\tla\t$t1, %s #load array address\n", node->body[1]->symbol);
        //add offset to adress
        printf("\tadd\t$t1, $t1, $t0\n");
        //store the value of $t2 at the adress of the array in $t1
        printf("\tsw\t$t2, ($t1) #store value in array adress\n");
        //printf(" ] ");
    }else{
         printf("\tsw\t$t0, %s #store result\n", node->body[0]->symbol);
    }
    
}

static void print_NODE_STMT_IF(const struct ast_node *node) {
    printf("\t# if\n");
      labelc = symbolNumb;
      symbolNumb += 2;
      //eval if cond (expr)
    print_ast_as_prascal(node->body[0]);
    //compare res of $t0
    //equal zero means false
    printf("\tbeqz\t$t0, nif_%03d\n",labelc);
    //code if true
    print_ast_as_prascal(node->body[1]);
    //jump over else
    printf("\tj\teif_%03d\n", labelc+1);
    //gen else lable
    printf("nif_%03d:\n", labelc);
    //print_ast_as_prascal(node->body[1]);
    if (node->body[2]) {
        //printf(" ELSE ");
        print_ast_as_prascal(node->body[2]);
    }
    //gen end label
    printf("eif_%03d:\n", labelc+1);
}

static void print_NODE_STMT_WHILE(const struct ast_node *node) {
    printf("\t# while\n");
      labelc = symbolNumb;
      symbolNumb += 2;
      //gen lable to jump back
      printf("while_%03d:\n", labelc);
      //evaluate expr
    print_ast_as_prascal(node->body[0]);
    //evaluate $t0
      //equal zero means false
      printf("\tbeqz\t$t0, ewhile_%03d\n",labelc+1);
      //while code
    print_ast_as_prascal(node->body[1]);
    //jump to while label
      printf("\tj\twhile_%03d\n",labelc);
      //generate end label
      printf("ewhile_%03d:\n", labelc+1);
}

static void print_NODE_STMT_FOR(const struct ast_node *node) {
    /FOR IDENTIFIER ASSIGN expr TO expr DO statement
    printf("\t# for\n");
    labelc = symbolNumb;
    symbolNumb += 2;
    //load value of identifier expr into $t0 + save val in t2
    print_ast_as_prascal(node->body[1]);
    printf("\tmove\t$t2, $t0 #store identifier val in $t2\n"); 
    //assign value to i
    printf("\tsw\t$t2, %s\n", n->body[0]->symbol);
    //save upper bound in $t3
    print_ast_as_prascal(node->body[3]);
    printf("\tmove\t$t3, $t0 #store upper bound in $t3\n"); 
    //create label for for
    printf("for_%03d:\n",labelc);
    //branch on greater than equal
    printf("\tbge\t$t2, $t3, efor_%03d\n",labelc+1);
    //code
    print_ast_as_prascal(node->body[4]);
    //load value of i into t1 + save val in t2
    printf("\tlw\t$t2, %s\n", n->body[0]->symbol->symbol.scalar);
    //printf("\tmove\t$t2, $t0 #store identifier val in $t2\n"); 
    //increase and store i 
    printf("\taddi\t$t2, $t2, 1 #increase counter\n");
    printf("\tsw\t$t2, %s #store i\n", n->body[0]->symbol);
    //save upper bound in $t3
    generateNode(n->body[3]);
    printf("\tmove\t$t3, $t0 #store upper bound in $t3\n"); 
    //jump to for label
    printf("\tj\tfor_%03d\n",labelc);
    //end label
    printf("efor_%03d:\n", labelc+1);
    
      print_ast_as_prascal(node->body[1]);
    print_ast_as_prascal(node->body[2]);
}

static void print_NODE_STMT_READ(const struct ast_node *node) {
    printf("\t# read\n");
    print_ast_as_prascal(node->body[0]);
    //printf(" ) ");
}

static void print_NODE_STMT_WRITE(const struct ast_node *node) {
     printf("\t# write\n");
    print_ast_as_prascal(node->body[0]);
    //printf(" ) ");
}

static void print_NODE_TOPART(const struct ast_node *node) {
    switch (node->token) {
        case TO     : printf(" ")     ; break ;
        case DOWNTO : printf(" ") ; break ;
        default:
            fprintf(stderr, "\n\nunknown token in TOPART\n");
            exit(EXIT_FAILURE);
    }
}

static void print_NODE_EXPRLIST(const struct ast_node *node) {
    print_ast_as_prascal(node->body[0]);
    if (node->next) {
        //printf(" , ");
        print_NODE_EXPRLIST(node->next);
    }
}

static void print_NODE_EXPR(const struct ast_node *node) {

    print_ast_as_prascal(node->body[0]);
    if (node->body[1]) {
        //push the result from left in $t0 onto the stack
        printf("\taddi\t$sp, $sp, -4\t#push\n");
        printf("\tsw\t$t0, 0($sp)\n");
        //print_ast_as_prascal(node->body[1]);
        print_ast_as_prascal(node->body[2]);
        //pop from the stack
        printf("\tlw\t$t1, 0($sp)\t#pop\n");
        printf("\taddi\t$sp, $sp, 4\n");
        //apply the operation
        //$t0 = $t0 op $t1
        t0= "$t0";
        t1= "$t1";
        print_ast_as_prascal(node->body[1]);
        //printf("\tadd\t$t0, $t0, $t1\n");
        //n->body[1]->op

         //store the result in %t0
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
    //load array index
          print_ast_as_prascal(node->body[1]);
          //calculate the index //shift left <<2 = *4
          printf("\tsll\t$t0, $t0, 2 #index calculation\n");
          //load array adress in $t1
          printf("\tla\t$t1, %d #load array address\n",node->symbol->iValue);
          //add offset to adress and store everything in $t1
          printf("\tadd\t$t1, $t1, $t0\n");
          //load value of array in $a0
          printf("\tlw\t$a0, 0($t1) #load array value of adress in t1\n"); 
          //load print call to print value at adress from t0
          printf("\tli\t$v0, 1\t#load sys-call to print int\n");
          //print_ast_as_prascal(node->body[0]);
          printf("\tsyscall\n");
    //printf(" [ ");
    //printf(" ] ");
}

static void print_NODE_IDENT(const struct ast_node *node) {
    switch(node->symbol->dtype){
            case TYPE_INTEGER:
              printf("\tli\t$v0, 1\t#load sys-call to print int\n");
              printf("\tlw\t$a0, %d #load int adress into a0\n", node->symbol->iValue);
              break;
            case TYPE_BOOLEAN:
              printf("\tli\t$v0, 1\t#load sys-call to print int\n");
              printf("\tlw\t$a0, %d #load int adress into a0\n", node->symbol->bValue);
              break;
            case TYPE_REAL:
              printf("\tli\t$v0, 2\t#load sys-call to print float\n");
              printf("\tl.s\t$f12, %f #load single prec into f12\n", node->symbol->fValue);
              break;
          };
    printf("\tsyscall\n");      
}

static void print_NODE_INT(const struct ast_node *node) {
     printf("\tli\t$v0, 5\t#load sys-call to read int\n");
     printf("\tsyscall\n");
     printf("\tsw\t$v0, %ld #store read value from v0\n", node->iValue);
}

static void print_NODE_REAL(const struct ast_node *node) {
    printf(" %f ", node->fValue);
}

static void print_NODE_STR(const struct ast_node *node) {
     printf("\tli\t$v0, 6\t#load sys-call to read float\n");
     printf("\tsyscall\n");
     printf("\ts.s\t$f0, %s #store single prec from f0\n", node->sValue);
}

static void print_NODE_BOOL(const struct ast_node *node) {
     printf("\tli\t$v0, 5\t#load sys-call to read int\n");
     printf("\tsyscall\n");
     printf("\tsw\t$v0, %s #store read value from v0\n", node->bValue);
}

static void print_NODE_RELOP(const struct ast_node *node, char t0, char t1) {
    switch (node->token) {
        case LT  : printf("\tslt\t%s, %s, %s\n", t0, t1, t0); break ;
        case LEQ : printf("\tsle\t%s, %s, %s\n", t0, t1, t0); break ;
        case GT  : printf("\tsgt\t%s, %s, %s\n", t0, t1, t0); break ;
        case GEQ : printf("\tsge\t%s, %s, %s\n", t0, t1, t0); break ;
        case EQ  : printf("\tseq\t%s, %s, %s\n", t0, t1, t0); break ;
        case NEQ : printf("\tsne\t%s, %s, %s\n", t0, t1, t0); break ;
        default:
            fprintf(stderr, "\n\nunknown token in RELOP\n");
            exit(EXIT_FAILURE);
    }
}

static void print_NODE_ADDOP(const struct ast_node *node, char t0, char t1) {
    switch (node->token) {
        case PLUS  : printf("\tadd\t%s, %s, %s\n", t0, t0, t1); break ;
        case MINUS : printf("\tsub\t%s, %s, %s\n", t0, t1, t0); break ;
        case OR    : printf("\tor\t%s, %s, %s\n", t0, t1, t0); break ;
        default:
            fprintf(stderr, "\n\nunknown token in ADDOP\n");
            exit(EXIT_FAILURE);
    }
}

static void print_NODE_MULOP(const struct ast_node *node) {
    switch (node->token) {
        case ASTR  : printf("\tmul\t%s, %s, %s\n", t0, t0, t1); break ;
        case SLASH : printf("\tdiv\t%s, %s, %s\n", t0, t1, t0); break ;
        case DIV   : printf("\tdiv\t%s, %s, %s\n", t0, t1, t0); break ;
        case MOD   : printf("\tdiv\t%s, %s\n", t1, t0);
                     //copy hi in t0
                     printf("\tmfhi\t%s\n", t0);   
                     break ;
        case AND   : printf("\tand\t%s, %s, %s\n", t0, t1, t0); break ;
        default:
            fprintf(stderr, "\n\nunknown token in MULOP\n");
            exit(EXIT_FAILURE);
    }
}
