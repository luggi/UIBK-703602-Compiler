#include "array.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

const unsigned long int INITIAL_SIZE = 8;

struct array *array_create(void) {
    /* allocate instance */
    struct array *array = (struct array *) malloc(sizeof(struct array));
    if (array == NULL) {
        return NULL;
    }

    /* initialize */
    array->size = INITIAL_SIZE;

    /* allocate segment */
    array->start = malloc(sizeof(void *) * array->size);
    if (array->start == NULL) {
        free(array);
        return NULL;
    }

    /* initialize segment */
    memset(array->start, 0, sizeof(void *) * array->size);

    return array;
}

void array_destroy(struct array *array) {
    free(array->start);
    free(array);
}

bool array_resize(struct array *array, unsigned long int new_size) {
    /* do not allow to resize smaller than initial */
    if (new_size < INITIAL_SIZE) {
        return false;
    }
    /* accept if new_size is current size */
    if (new_size == array->size) {
        return true;
    }
    /* allocate new segment */
    void **new_start = realloc(array->start, sizeof(void *) * new_size);
    if (new_start == NULL) {
        /* old segment untouched */
        return false;
    }

    /* initialize new part */
    if (new_size > array->size) {
        memset(new_start + array->size, 0,
                sizeof(void *) * (new_size - array->size));
    }

    array->start = new_start;
    array->size = new_size;
    return true;
}

bool array_grow(struct array *array) {
    return array_resize(array, array->size * 2);
}

bool array_shrink(struct array *array) {
    return array_resize(array, array->size / 2);
}

void *array_get(const struct array *array, unsigned long int index) {
    if (index > array->size) {
        return NULL;
    }
    return *(array->start + index);
}

bool array_set(struct array *array, unsigned long int index, void *data) {
    if (index > array->size) {
        return false;
    }
    array->start[index] = data;
    return true;
}

bool array_clear(struct array *array) {
    if (!array_resize(array, INITIAL_SIZE)) {
        return false;
    }
    array->size = INITIAL_SIZE;
    memset(array->start, 0, sizeof(void *) * array->size);
    return true;
}
