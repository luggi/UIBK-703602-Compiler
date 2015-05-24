#include "list.h"

#include <stdbool.h>
#include <stdlib.h>

struct list_head *list_create(void) {
    struct list_head *list = (struct list_head *) malloc(
            sizeof(struct list_head));
    if (list == NULL) {
        return NULL;
    }
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    return list;
}

void list_destroy(struct list_head *list) {
    list_clear(list);
    free(list);
}

size_t list_append(struct list_head *list, void *data) {
    return list_put(list, list->size, data);
}

size_t list_prepend(struct list_head *list, void *data) {
    return list_put(list, 0, data);
}

size_t list_put(struct list_head *list, size_t index, void *data) {
    if (index > list->size) {
        return -1;
    }

    /* create new node */
    struct list_node *node = (struct list_node *) malloc(
            sizeof(struct list_node));
    if (node == NULL) {
        return -1;
    }
    node->data = data;

    if (list_is_empty(list)) {
        /* handle empty list */
        node->prev = NULL;
        node->next = NULL;
    } else if (index == list->size) {
        /* handle append */
        node->prev = list->last;
        node->next = NULL;
        node->prev->next = node;
    } else {
        struct list_node *target = list_get_node(list, index);

        /* set node pointers */
        node->prev = target->prev;
        node->next = target;

        /* update chain */
        if (node->prev != NULL) {
            node->prev->next = node;
        }
        if (node->next != NULL) {
            node->next->prev = node;
        }
    }

    /* update head */
    if (index == 0) {
        list->first = node;
    }
    if (index == list->size) {
        list->last = node;
    }

    list->size++;
    return index;
}

void *list_remove(struct list_head *list, size_t index) {
    if (index >= list->size) {
        return NULL;
    }

    /* find target */
    struct list_node *target = list_get_node(list, index);

    /* update chain */
    if (target->prev != NULL) {
        target->prev->next = target->next;
    }
    if (target->next != NULL) {
        target->next->prev = target->prev;
    }

    /* update head */
    if (index == 0) {
        list->first = target->next;
    }
    if (index == list->size) {
        list->last = target->prev;
    }

    void *data = target->data;
    free(target);
    return data;
}

void *list_get(const struct list_head *list, size_t index) {
    struct list_node *node = list_get_node(list, index);
    if (node == NULL) {
        return NULL;
    }
    return node->data;
}

struct list_node *list_get_node(const struct list_head *list, size_t index) {
    if (index >= list->size) {
        return NULL;
    }

    /* find target */
    struct list_node *target = NULL;
    if (index < list->size / 2) {
        target = list->first;
        for (size_t i = 0; i < index; i++) {
            target = target->next;
        }
    } else {
        target = list->last;
        for (size_t i = list->size; i > index + 1; i--) {
            target = target->prev;
        }
    }

    return target;
}

void list_clear(struct list_head *list) {
    for (struct list_node *node = list->first; node != NULL;) {
        struct list_node *node2 = node;
        node = node->next;
        free(node2);
    }
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
}

size_t list_index_of(const struct list_head *list, void* data) {
    if (list->size == 0) {
        return -1;
    }

    struct list_node *node = list->first;
    for (size_t i = 0; i < list->size; i++) {
        if (node->data == data) {
            return i;
        }
        node = node->next;
    }

    return -1;
}

size_t list_index_of_last(const struct list_head *list, void* data) {
    if (list->size == 0) {
        return -1;
    }

    struct list_node *node = list->last;
    for (size_t i = list->size - 1; i >= 0; i++) {
        if (node->data == data) {
            return i;
        }
        node = node->prev;
    }

    return -1;
}

size_t list_find(const struct list_head *list, bool (*cmp)(void *data)) {
    if (list->size == 0) {
        return -1;
    }

    struct list_node *node = list->first;
    for (size_t i = 0; i < list->size; i++) {
        if (cmp(node->data)) {
            return i;
        }
        node = node->next;
    }

    return -1;
}

bool list_contains(const struct list_head *list, void *data) {
    return list_index_of(list, data) != -1;
}

bool list_is_empty(const struct list_head *list) {
    return list->size == 0;
}
