#include "stack.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct stack *stack_create(void) {
    /* allocate instance */
    struct stack *stack = (struct stack *) malloc(sizeof(struct stack));
    if (stack == NULL) {
        return NULL;
    }

    /* allocate underlying datastructure */
    stack->array = array_create();
    if (stack->array == NULL) {
        free(stack);
        return NULL;
    }

    /* initialize */
    stack->size = 0;

    return stack;
}

void stack_destroy(struct stack *stack) {
    array_destroy(stack->array);
    free(stack);
}

bool stack_push(struct stack *stack, void *data) {
    /* check size and resize if necessary */
    if (stack->size == stack->array->size) {
        if (!array_grow(stack->array)) {
            return false;
        }
    }

    /* add element */
    if (!array_set(stack->array, stack->size, data)) {
        return false;
    }

    /* update size */
    stack->size++;
    return true;
}

void *stack_pop(struct stack *stack) {
    if (stack->size == 0) {
        return NULL;
    }
    stack->size--;
    if (stack->size <= stack->array->size / 2) {
        array_shrink(stack->array);
    }
    return array_get(stack->array, stack->size);
}

void *stack_peek(const struct stack *stack) {
    if (stack->size == 0) {
        return NULL;
    }
    return array_get(stack->array, stack->size - 1);
}

bool stack_clear(struct stack *stack) {
    if (!array_clear(stack->array)) {
        return false;
    }
    stack->size = 0;
    return true;
}
