#ifndef VALIDATION_H
#define VALIDATION_H

int is_digit(char c);
int is_space(char c);
int is_operator(char c);
int is_valid_char(char c);
int is_unary_operator(char c);
int validate(const char *expression);
char *tildas(const char *expression);
int is_function_token(const char *token);
int check_syntax(const char *expression);
int check_valid_chars(const char *expression);
int check_balanced_brackets(const char *expression);
int starts_with(const char *str, const char *prefix);
int process_operand(const char *expression, int *i, int length, int *expect_operand, int *last_was_operator);
int skip_spaces(const char *expression, int *i, int length, int valid);
int process_subexpression(const char *expression, int *i, int length, int *success);
int check_function_argument(const char *expression, int start, int end);
int process_operator_or_close(const char *expression, int *i, int length, int *expect_operand,
                              int *last_was_operator);
int process_number(const char *expression, int *i, int length, int *success);
int process_function(const char *expression, int *i, int length, int *success);
int process_current_char(const char *expression, int *i, int length, int *expect_operand,
                         int *last_was_operator, int valid);
#endif