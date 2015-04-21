#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>

struct array {
    void **start;
    unsigned long int size;
};

/* Returns new instance of array or NULL on failure. */
struct array *array_create(void);

/* Calls free on array instance but not its content. */
void array_destroy(struct array *array);

/* Change size of the array to hold exactly new_size elements. Drops elements
 * if new_size is greater than current size. Returns true iff resize was
 * sucessful. Old data will still be present on failure.
 */
bool array_resize(struct array *array, unsigned long int new_size);

/* Calls resize with current size * 2. */
bool array_grow(struct array *array);

/* Calls resize with current size / 2. */
bool array_shrink(struct array *array);

/* Return element from array at position index. */
void *array_get(const struct array *array, unsigned long int index);

/* Set element in array at position index to data. */
bool array_set(struct array *array, unsigned long int index, void *data);

/* Resize array to initial size and set everything to NULL. Returns true on
 * success.
 */
bool array_clear(struct array *array);

#endif
