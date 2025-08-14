#include "dij.h"

#include <stdio.h>
#include <string.h>

#include "stack.h"
#define MAX_LEN 20
int is_digitw(char c) { return c >= '0' && c <= '9'; }
int is_alphaw(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
int is_spacew(char c) { return c == ' '; }
int precedence(const char *op) {
    int res = 0;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) res = 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0) res = 2;
    if (strcmp(op, "~") == 0) res = 3;
    if (strcmp(op, "sin") == 0 || strcmp(op, "cos") == 0 || strcmp(op, "tan") == 0 ||
        strcmp(op, "ctg") == 0 || strcmp(op, "sqrt") == 0 || strcmp(op, "ln") == 0)
        res = 4;
    return res;
}
int is_function(const char *token) {
    return strcmp(token, "sin") == 0 || strcmp(token, "cos") == 0 || strcmp(token, "tan") == 0 ||
           strcmp(token, "ctg") == 0 || strcmp(token, "sqrt") == 0 || strcmp(token, "ln") == 0;
}
void append_postfix(char *postfix, const char *token) {
    if (strlen(postfix) > 0) strcat(postfix, " ");
    strcat(postfix, token);
}
struct stack *pop_while(struct stack *op_stack, char *postfix, const char *op) {
    char top[MAX_LEN] = "";
    view(op_stack, top);
    while (op_stack != NULL && precedence(top) >= precedence(op)) {
        op_stack = pop(op_stack, top);
        append_postfix(postfix, top);
        view(op_stack, top);
    }
    return op_stack;
}
void handle_number(const char *infix, int *i, int n, char *postfix) {
    char token[MAX_LEN];
    int j = 0;
    while (*i < n && (is_digitw(infix[*i]) || infix[*i] == '.' || infix[*i] == 'x'))
        token[j++] = infix[(*i)++];
    token[j] = '\0';
    append_postfix(postfix, token);
}
void handle_function(const char *infix, int *i, int n, struct stack **op_stack) {
    char token[MAX_LEN];
    int j = 0;
    while (*i < n && is_alphaw(infix[*i])) token[j++] = infix[(*i)++];
    token[j] = '\0';
    *op_stack = push(*op_stack, token);
}
void handle_right_paren(struct stack **op_stack, char *postfix) {
    char top[MAX_LEN];
    view(*op_stack, top);
    while (*op_stack && strcmp(top, "(") != 0) {
        *op_stack = pop(*op_stack, top);
        append_postfix(postfix, top);
        view(*op_stack, top);
    }
    if (*op_stack) *op_stack = pop(*op_stack, top);
    view(*op_stack, top);
    if (*op_stack && is_function(top)) {
        *op_stack = pop(*op_stack, top);
        append_postfix(postfix, top);
    }
}
void parse_token(const char *infix, int *i, int n, struct stack **op_stack, char *postfix) {
    char token[MAX_LEN];
    if (is_spacew(infix[*i]))
        (*i)++;
    else if (is_digitw(infix[*i]) || infix[*i] == 'x')
        handle_number(infix, i, n, postfix);
    else if (is_alphaw(infix[*i]))
        handle_function(infix, i, n, op_stack);
    else if (infix[*i] == '(') {
        token[0] = '(';
        token[1] = '\0';
        *op_stack = push(*op_stack, token);
        (*i)++;
    } else if (infix[*i] == ')') {
        (*i)++;
        handle_right_paren(op_stack, postfix);
    } else {
        token[0] = infix[(*i)++];
        token[1] = '\0';
        *op_stack = pop_while(*op_stack, postfix, token);
        *op_stack = push(*op_stack, token);
    }
}

int infix_to_postfix(const char *infix, char *postfix) {
    struct stack *op_stack = NULL;
    char top[MAX_LEN];
    int i = 0, n = strlen(infix);
    postfix[0] = '\0';

    while (i < n) parse_token(infix, &i, n, &op_stack, postfix);

    view(op_stack, top);
    while (op_stack) {
        op_stack = pop(op_stack, top);
        append_postfix(postfix, top);
        view(op_stack, top);
    }
    destroy(op_stack);
    return 0;
}