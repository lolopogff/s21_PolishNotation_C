#include "stack.h"

#include <stdlib.h>
#include <string.h>

struct stack *push(struct stack *current, char value[]) {
    struct stack *new = malloc(sizeof(struct stack));
    if (new != NULL) {
        strcpy(new->value, value);
        new->prev = current;
    }
    return new;
}

struct stack *pop(struct stack *current, char value[]) {
    struct stack *res = NULL;
    if (current == NULL) {
        value[0] = '1';
    } else {
        strcpy(value, current->value);
        res = current->prev;
        free(current);
    }
    return res;
}
void view(struct stack *current, char value[]) {
    if (current == NULL) {
        value[0] = '1';
    } else {
        strcpy(value, current->value);
    }
}

void destroy(struct stack *current) {
    while (current != NULL) {
        struct stack *buf = current->prev;
        free(current);
        current = buf;
    }
}
