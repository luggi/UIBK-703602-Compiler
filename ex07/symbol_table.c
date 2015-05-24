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
    free(entry);
}
