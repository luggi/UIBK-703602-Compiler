#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#include "array.h"

struct stack {
    struct array *array;
    unsigned long size;
};

/* Returns new instance of stack or NULL on failure. */
struct stack *stack_create(void);

/* Calls free on stack instance but not its content. */
void stack_destroy(struct stack *stack);

/* Pushes data on top of stack. Returns true on success. */
bool stack_push(struct stack *stack, void *data);

/* Pops data from top of stack. Returns NULL on failure. */
void *stack_pop(struct stack *stack);

/* Returns data from top of stack but does not pop it. Returns NULL on failure. */
void *stack_peek(const struct stack *stack);

/* Empty stack and prepares it for reuse. Returns true on success. */
bool stack_clear(struct stack *stack);

#endif
