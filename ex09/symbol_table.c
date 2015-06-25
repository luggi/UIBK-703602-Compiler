/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#include "symbol_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "list.h"

/* TODO: since the symbole table is generated after parsing yylineno is useless
 * */
extern int yylineno;

struct list_head *SYMBOL_TABLE;

static void symbol_table_build(struct ast_node *node);
static void symbol_table_process_ident(struct ast_node *ident);
static void symbol_table_process_identListType(struct ast_node *ident);

struct symbol_entry *symbol_entry_create(const enum symbol_type etype,
        const enum data_type dtype) {
    struct symbol_entry *entry;
    entry = (struct symbol_entry *) malloc(sizeof(struct symbol_entry));
    if (entry == NULL) {
        fprintf(stderr, "could not allocate memory for symbol_entry\n");
        exit(EXIT_FAILURE);
    }
    memset(entry, 0, sizeof(struct symbol_entry));
    entry->etype = etype;
    entry->dtype = dtype;
    return entry;
}

void symbol_entry_destroy(struct symbol_entry *entry) {
    /* symbol has been allocated by the parser using strndup */
    free(entry->symbol);

    free(entry);
}

void symbol_table_create(struct ast_node *root) {
    SYMBOL_TABLE = list_create();
    node_walk(root, symbol_table_build);
}

void symbol_table_destroy(void) {
    for (struct list_node *node = SYMBOL_TABLE->first; node != NULL; node = node->next) {
        symbol_entry_destroy((struct symbol_entry *) node->data);
    }

    list_destroy(SYMBOL_TABLE);
}

struct symbol_entry *symbol_table_lookup(const char *ident) {
    for (struct list_node *node = SYMBOL_TABLE->first; node != NULL; node = node->next) {
        struct symbol_entry *entry = (struct symbol_entry *) node->data;
        if (strcmp(entry->symbol, ident) == 0) {
            return entry;
        }
    }
    return NULL;
}

void print_symbol_table(void) {
    for (struct list_node *node = SYMBOL_TABLE->first; node != NULL; node = node->next) {
        struct symbol_entry *e = (struct symbol_entry *) node->data;
        if(e == NULL){
            return;
        }
        switch(e->etype){
        /*case SYMBOL_TABLE->:
            switch(e->dtype) {
            case TYPE_INTEGER:
                printf("%s:\t\t.word\t\n",e->symbol);
                break;
            case TYPE_BOOLEAN:
                printf("%s:\t\t.word\t\n",e->symbol);
                break;
            case TYPE_REAL:
                printf("%s:\t\t.float\t\n",e->symbol);
                break;
            }
            default:
            break;*/
        case SYMBOL_SCALAR:
            switch(e->dtype) {
            case TYPE_INTEGER:
            case TYPE_BOOLEAN:
                printf("%s:\t\t.word\t0\n",e->symbol);
                break;
            case TYPE_REAL:
                printf("%s:\t\t.float\t0.0\n",e->symbol);
                break;
            }
            default:
            break;
        case SYMBOL_ARRAY:
        //TODO size of array accessor
            printf("%s:\t\t.space\t%d\n",e->symbol, 4*4);
            break;
        default:
            printf("error, no etype %i\n", e->etype);
            break;
  }
        printf("%p: %s (%d, %d)\n", e, e->symbol, e->etype, e->dtype);
    }
}

/* adds identifiers in identListType node to symbol table with set type. a
 * reference is stored in the node. */
static void symbol_table_build(struct ast_node *node) {
    switch (node->type) {
        case NODE_IDENTLISTTYPE:
            symbol_table_process_identListType(node);
            break;

        case NODE_IDENT:
            symbol_table_process_ident(node);
            break;

        default:
            break;
    }
}

/* looks up the corresponding entry in the symbol table and rewrites the node
 * to hold a reference to the entry. calls exit if entry is not present. */
static void symbol_table_process_ident(struct ast_node *ident) {
    struct symbol_entry *entry = symbol_table_lookup(ident->ident);

    if (entry == NULL) {
        fprintf(stderr, "Error: Line %d: unknown symbol '%s'\n", yylineno,
                ident->ident);
        exit(EXIT_FAILURE);
    }

    free(ident->ident);
    ident->symbol = entry;
}

static void symbol_table_process_identListType(struct ast_node *identListType) {
    struct ast_node* type_node = identListType->body[1];

    /* process entry type */
    enum symbol_type etype;
    if (type_node->body[0] == NULL) {
        etype = SYMBOL_SCALAR;
    } else {
        etype = SYMBOL_ARRAY;
    }

    /* process data type */
    enum data_type dtype;
    if (type_node->body[2]->token == INTEGER) {
        dtype = TYPE_INTEGER;
    } else if (type_node->body[2]->token == REAL) {
        dtype = TYPE_REAL;
    } else if (type_node->body[2]->token == BOOLEAN) {
        dtype = TYPE_BOOLEAN;
    } else {
        fprintf(stderr, "Error: Line %d: unknown type in syntax tree\n",
                yylineno);
        exit(EXIT_FAILURE);
    }

    /* process ident list */
    for (struct ast_node *identList = identListType->body[0]; identList != NULL; identList = identList->next) {
        struct ast_node *ident = identList->body[0];

        /* check if ident already present */
        if (symbol_table_lookup(ident->ident)) {
            fprintf(stderr, "Error: Line %d: symbol already declared '%s'\n",
                    yylineno, ident->ident);
            exit(EXIT_FAILURE);
        }

        /* new entry */
        struct symbol_entry *e = symbol_entry_create(etype, dtype);
        e->symbol = strdup(ident->ident);

        /* store reference in node */
        /* ident->symbol = e; */

        list_append(SYMBOL_TABLE, e);
    }
}
