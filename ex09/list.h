#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdlib.h>

struct list_node {
    struct list_node *next;
    struct list_node *prev;
    void *data;
};

struct list_head {
    struct list_node *first;
    struct list_node *last;
    size_t size;
};

/* Returns a new list head or NULL on failure. */
struct list_head *list_create(void);

/* Frees all nodes and the list head but not your data. */
void list_destroy(struct list_head *list);

/* Add new node to the end of the list, returns index or -1 on failure. */
size_t list_append(struct list_head *list, void *data);

/* Add new node to the beginning of the list, returns index or -1 on failure. */
size_t list_prepend(struct list_head *list, void *data);

/* Add new node to position index, returns index or -1 on failure. */
size_t list_put(struct list_head *list, size_t index, void *data);

/* Removes node at position index from list, returns its data or NULL on
 * failure. */
void *list_remove(struct list_head *list, size_t index);

/* Returns the data held by node at position index or NULL on failure. */
void *list_get(const struct list_head *list, size_t index);

/* Returns whole node at position index or NULL on failure. */
struct list_node *list_get_node(const struct list_head *list, size_t index);

/* Free all nodes in list and reset list head. */
void list_clear(struct list_head *list);

/* Returns index of first occurence of data in list or -1 if not found. */
size_t list_index_of(const struct list_head *list, void* data);

/* Returns index of last occurence of data in list or -1 if not found. */
size_t list_index_of_last(const struct list_head *list, void* data);

/* walk through list and call cmp with data, if cmp returns true the current
 * index is returned. Returns -1 if the whole list has been traversed. */
size_t list_find(const struct list_head *list, bool (*cmp)(void *data));

/* true iff node in list contains data */
bool list_contains(const struct list_head *list, void *data);

/* true iff list is empty */
bool list_is_empty(const struct list_head *list);

#endif
