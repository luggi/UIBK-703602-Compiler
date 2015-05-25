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

struct list_head *symbol_table_create(void) {
    return list_create();
}

void symbol_table_destroy(struct list_head *table) {
    for (struct list_node *node = table->first; node != NULL; node = node->next) {
        symbol_entry_destroy((struct symbol_entry *) node->data);
    }

    list_destroy(table);
}

struct symbol_entry *symbol_table_lookup(const struct list_head *table,
        const char *ident) {
    for (struct list_node *node = table->first; node != NULL; node = node->next) {
        struct symbol_entry *entry = (struct symbol_entry *) node->data;
        if (strcmp(entry->symbol, ident) == 0) {
            return entry;
        }
    }
    return NULL;
}
