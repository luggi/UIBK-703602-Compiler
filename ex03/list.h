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

/* returns a new list head or NULL on failure */
struct list_head *list_create(void);

/* frees all nodes and the list head but not your data */
void list_destroy(struct list_head *list);

/* add new node to the end of the list, returns index or -1 on failure */
unsigned long int list_append(struct list_head *list, void *data);


/* add new node to the beginning of the list, returns index or -1 on failure */
unsigned long int list_prepend(struct list_head *list, void *data);

/* add new node to position index, returns index or -1 on failure */
unsigned long int list_put(struct list_head *list, unsigned long int index, void *data);

/* removes node at position index from list, returns its data or NULL on failure */
void *list_remove(struct list_head *list, unsigned long int index);

/* returns the data held by node at position index or NULL on failure */
void *list_get(const struct list_head *list, unsigned long int index);

/* returns whole node at position index or NULL on failure */
struct list_node *list_get_node(const struct list_head *list, unsigned long int index);

/* free all nodes in list and reset list head */
void list_clear(struct list_head *list);

/* true iff node in list contains data */
bool list_contains(const struct list_head *list, void *data);

/* true iff list is empty */
bool list_is_empty(const struct list_head *list);

#endif
