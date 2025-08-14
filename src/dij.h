#ifndef DIJ_H
#define DIJ_H

int is_digitw(char c);
int is_alphaw(char c);
int is_spacew(char c);
int precedence(const char *op);
int is_operatorw(const char *token);
int is_function(const char *token);
void append_postfix(char *postfix, const char *token);
struct stack *pop_while(struct stack *op_stack, char *postfix, const char *op);
void handle_number(const char *infix, int *i, int n, char *postfix);
void handle_function(const char *infix, int *i, int n, struct stack **op_stack);
void handle_right_paren(struct stack **op_stack, char *postfix);
void parse_token(const char *infix, int *i, int n, struct stack **op_stack, char *postfix);
int infix_to_postfix(const char *infix, char *postfix);
#endif