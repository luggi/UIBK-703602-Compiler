/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "list.h"

enum symbol_type {
    SYMBOL_SCALAR,
    SYMBOL_ARRAY,
};

enum data_type {
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_BOOLEAN,
};

struct symbol_entry {
    enum symbol_type etype;
    enum data_type dtype;

    /* symbol is used to store text of identifier */
    char *symbol;
};

struct symbol_entry *symbol_entry_create(const enum symbol_type etype,
        const enum data_type dtype);

void symbol_entry_destroy(struct symbol_entry *entry);

struct list_head *symbol_table_create(void);

/* frees the symbol table and its contents */
void symbol_table_destroy(struct list_head *table);

/* returns entry with name ident or NULL */
struct symbol_entry *symbol_table_lookup(const struct list_head *table,
        const char *ident);

#endif
