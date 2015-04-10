/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

struct list_node {
    struct list_node *next;
    struct list_node *prev;
    void *data;
};

struct list_head {
    struct list_node *first;
    struct list_node *last;
    unsigned long int size;
};

struct list_head *list_create(void);

void list_destroy(struct list_head *list);

unsigned long int list_append(struct list_head *list, void *data);

unsigned long int list_prepend(struct list_head *list, void *data);

unsigned long int list_put(struct list_head *list, unsigned long int index,
        void *data);

void *list_remove(struct list_head *list, unsigned long int index);

void *list_get(const struct list_head *list, unsigned long int index);

struct list_node *list_get_node(const struct list_head *list, unsigned long int index);

void list_clear(struct list_head *list);

bool list_contains(const struct list_head *list, void *data);

bool list_is_empty(const struct list_head *list);

#endif
