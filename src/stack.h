#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
struct stack {
    char value[20];
    struct stack *prev;
};

struct stack *push(struct stack *current, char value[]);
struct stack *pop(struct stack *current, char value[]);
void view(struct stack *current, char value[]);
void destroy(struct stack *current);
void debug_print_stack(struct stack *s);

#endif