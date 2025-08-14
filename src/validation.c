/*
    char *tildas(const char *expression) - проверяет строку на валидность (1 -
   валид, 0 - инвалид) int validate(const char *expression) - возвращает
   отформатированную строку с тильдами
*/

#include "validation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_digit(char c) {
    int result = c >= '0' && c <= '9';
    return result;
}

int is_space(char c) {
    int result = c == ' ';
    return result;
}

int is_operator(char c) {
    int result = c == '+' || c == '-' || c == '*' || c == '/';
    return result;
}

// int is_unary_operator(char c) {
//   int result = c == '+' || c == '-' || c == '~';
//   return result;
// }

int is_valid_char(char c) {
    int result = is_digit(c) || c == '.' || c == 'x' || c == '(' || c == ')' || c == '+' || c == '-' ||
                 c == '*' || c == '/' || c == '~' || c == ' ' || c == 's' || c == 'i' || c == 'n' ||
                 c == 'c' || c == 'o' || c == 't' || c == 'a' || c == 'g' || c == 'q' || c == 'r' || c == 'l';
    return result;
}

int starts_with(const char *str, const char *prefix) {
    int result = 0;
    if (str && prefix) {
        result = strncmp(str, prefix, strlen(prefix)) == 0;
    }
    return result;
}

int check_balanced_brackets(const char *expression) {
    int result = 1;
    if (expression) {
        int balance = 0;
        for (int i = 0; expression[i] != '\0' && result; i++) {
            if (expression[i] == '(')
                balance++;
            else if (expression[i] == ')')
                balance--;
            result = balance >= 0;
        }
        result = result && (balance == 0);
    }
    return result;
}

int is_function_token(const char *token) {
    int result = starts_with(token, "sin(") || starts_with(token, "cos(") || starts_with(token, "tan(") ||
                 starts_with(token, "ctg(") || starts_with(token, "sqrt(") || starts_with(token, "ln(");
    return result;
}

int check_function_argument(const char *expression, int start, int end) {
    int result = 0;
    for (int k = start; k < end && !result; k++) {
        result = !is_space(expression[k]);
    }
    return result;
}

int process_function_token(const char *str, int token_len, int *new_index) {
    const char *arg_start = str + token_len;
    int pos = 0;
    int balance = 1;
    int result = 0;

    while (arg_start[pos] != '\0' && balance > 0) {
        if (arg_start[pos] == '(')
            balance++;
        else if (arg_start[pos] == ')')
            balance--;
        pos++;
    }

    if (balance == 0 && check_function_argument(arg_start, 0, pos - 1)) {
        char *subexpression = strndup(arg_start, pos - 1);
        if (subexpression) {
            result = check_syntax(subexpression);
            free(subexpression);
        }
    }

    if (result) *new_index = token_len + pos;
    return result;
}

int try_match_token(const char *str, int *step) {
    const char *valid_tokens[] = {"0", "1",    "2",    "3",    "4",    "5",     "6",   "7", "8",
                                  "9", "sin(", "cos(", "tan(", "ctg(", "sqrt(", "ln(", "x", "(",
                                  ")", "+",    "-",    "*",    "/",    ".",     "~",   " "};
    const int num_tokens = sizeof(valid_tokens) / sizeof(valid_tokens[0]);
    int success = 0;
    int new_index = 0;

    for (int j = 0; j < num_tokens && !success; j++) {
        if (starts_with(str, valid_tokens[j])) {
            const int token_len = strlen(valid_tokens[j]);
            if (is_function_token(valid_tokens[j])) {
                success = process_function_token(str, token_len, &new_index);
            } else {
                success = 1;
                new_index = token_len;
            }
        }
    }

    if (success) {
        *step = new_index;
    }
    return success;
}

int check_valid_chars(const char *expression) {
    int valid = 1;
    if (!expression) return 1;

    int i = 0;
    while (valid && expression[i] != '\0') {
        int step = 0;

        if (try_match_token(expression + i, &step)) {
            i += step;
        } else if (is_valid_char(expression[i])) {
            i++;
        } else {
            valid = 0;
        }
    }
    return valid;
}

int process_number(const char *expression, int *i, int length, int *success) {
    int has_decimal = 0;
    int start = *i;
    int error = 0;

    while (*i < length && (is_digit(expression[*i]) || (expression[*i] == '.' && !has_decimal))) {
        if (expression[*i] == '.') {
            has_decimal = 1;
        }
        (*i)++;
    }

    if (*success) {
        if (*i == start)
            error = 1;

        else if (expression[*i - 1] == '.')
            error = 1;

        else if (*i < length && expression[*i] == '.' && has_decimal)
            error = 1;
    }

    if (error) *success = 0;

    return *success;
}

int process_subexpression(const char *expression, int *i, int length, int *success) {
    int result = 0;
    if (*i < length && expression[*i] == '(') {
        int start = ++(*i);
        int balance = 1;
        while (*i < length && balance > 0) {
            if (expression[*i] == '(')
                balance++;
            else if (expression[*i] == ')')
                balance--;
            (*i)++;
        }
        if (balance == 0 && check_function_argument(expression, start, *i - 1)) {
            char *subexpression = strndup(expression + start, *i - start - 1);
            if (subexpression) {
                result = check_syntax(subexpression);
                free(subexpression);
            } else {
                *success = 0;
            }
        } else {
            *success = 0;
        }
    } else {
        *success = 0;
    }
    return result;
}

int process_function(const char *expression, int *i, int length, int *success) {
    int result = 0;
    int token_len = starts_with(expression + *i, "sin(")    ? 4
                    : starts_with(expression + *i, "cos(")  ? 4
                    : starts_with(expression + *i, "tan(")  ? 4
                    : starts_with(expression + *i, "ctg(")  ? 4
                    : starts_with(expression + *i, "sqrt(") ? 5
                    : starts_with(expression + *i, "ln(")   ? 3
                                                            : 0;

    if (token_len) {
        *i += token_len;
        int start = *i;
        int balance = 1;
        while (*i < length && balance > 0) {
            if (expression[*i] == '(')
                balance++;
            else if (expression[*i] == ')')
                balance--;
            (*i)++;
        }
        if (balance == 0 && check_function_argument(expression, start, *i - 1)) {
            char *subexpression = strndup(expression + start, *i - start - 1);
            if (subexpression) {
                result = check_syntax(subexpression);
                free(subexpression);
            } else {
                *success = 0;
            }
        } else {
            *success = 0;
        }
    } else {
        *success = 0;
    }
    return result;
}

static int handle_unary(const char *expression, int *i, int length, int *expect_operand,
                        int *last_was_operator, int *ret, int *success) {
    if (expression[*i] != '~') return 0;
    (*i)++;
    while (*i < length && is_space(expression[*i])) (*i)++;
    if (*i >= length) {
        *success = 0;
        *ret = 0;
        return 1;
    }
    *ret = process_operand(expression, i, length, expect_operand, last_was_operator);
    return 1;
}

static int handle_number(const char *expression, int *i, int length, int *ret, int *success) {
    if (!is_digit(expression[*i])) return 0;
    *ret = process_number(expression, i, length, success);
    return 1;
}

static int handle_variable(const char *expression, int *i, int length, int *ret, int *success) {
    if (length == 0) {
        ret = 0;
    }
    (void)success;
    if (expression[*i] != 'x') return 0;
    (*i)++;
    *ret = 1;
    return 1;
}

static int handle_subexpr(const char *expression, int *i, int length, int *ret, int *success) {
    if (expression[*i] != '(') return 0;
    *ret = process_subexpression(expression, i, length, success);
    return 1;
}

static int handle_function(const char *expression, int *i, int length, int *ret, int *success) {
    int res = 1;
    const char *funcs[] = {"sin(", "cos(", "tan(", "ctg(", "sqrt(", "ln("};
    for (size_t idx = 0; idx < sizeof(funcs) / sizeof(funcs[0]); idx++) {
        if (starts_with(expression + *i, funcs[idx])) {
            *ret = process_function(expression, i, length, success);
            res = 1;
        }
    }
    return res;
}

int process_operand(const char *expression, int *i, int length, int *expect_operand, int *last_was_operator) {
    int ret = 0;
    int success = 1;
    int handled = 0;
    if (length == 0)

        // Пропуск пробелов
        while (*i < length && is_space(expression[*i])) (*i)++;
    if (*i >= length) return 0;

    // Последовательные попытки обработки операндов
    handled = handle_unary(expression, i, length, expect_operand, last_was_operator, &ret, &success);
    if (!handled) handled = handle_number(expression, i, length, &ret, &success);
    if (!handled) handled = handle_variable(expression, i, length, &ret, &success);
    if (!handled) handled = handle_subexpr(expression, i, length, &ret, &success);
    if (!handled) handled = handle_function(expression, i, length, &ret, &success);

    // Обработка неизвестного формата
    if (!handled) {
        success = 0;
        ret = 0;
    }

    // Обновление состояния парсера
    if (success && ret) {
        *expect_operand = 0;
        *last_was_operator = 0;
    }

    return ret && success;
}

int process_operator_or_close(const char *expression, int *i, int length, int *expect_operand,
                              int *last_was_operator) {
    int result = 0;
    if (length == 0) {
        result = 0;
    }
    if (is_operator(expression[*i])) {
        if (!*last_was_operator) {
            (*i)++;
            *expect_operand = 1;
            *last_was_operator = 1;
            result = 1;
        }
    } else if (expression[*i] == ')') {
        if (!*last_was_operator) {
            (*i)++;
            *expect_operand = 0;
            *last_was_operator = 0;
            result = 1;
        }
    }
    return result;
}

int check_syntax(const char *expression) {
    int result = 0;
    if (expression) {
        int length = strlen(expression);
        int i = 0;
        int expect_operand = 1;
        int last_was_operator = 0;
        int valid = 1;

        while (i < length && valid) {
            valid = skip_spaces(expression, &i, length, valid);

            if (i >= length || !valid) {
                valid = 0;
            } else {
                valid =
                    process_current_char(expression, &i, length, &expect_operand, &last_was_operator, valid);
            }
        }

        result = valid && !expect_operand && !last_was_operator;
    }
    return result;
}

int skip_spaces(const char *expression, int *i, int length, int valid) {
    while (*i < length && is_space(expression[*i])) {
        (*i)++;
    }
    return valid;
}

int process_current_char(const char *expression, int *i, int length, int *expect_operand,
                         int *last_was_operator, int valid) {
    if (*expect_operand) {
        valid = process_operand(expression, i, length, expect_operand, last_was_operator);
    } else {
        valid = process_operator_or_close(expression, i, length, expect_operand, last_was_operator);
    }
    return valid;
}

char *tildas(const char *expression) {
    char *result = NULL;
    if (expression) {
        result = strdup(expression);
        if (result) {
            for (size_t i = 0; i < strlen(result); i++) {
                if ((result[i] == '-' || result[i] == '+') &&
                    (i == 0 || result[i - 1] == '(' || is_operator(result[i - 1]))) {
                    result[i] = '~';
                }
            }
        }
    }
    return result;
}

int validate(const char *expression) {
    int result = 0;
    if (expression && strlen(expression) > 0) {
        char *modified = tildas(expression);
        if (modified) {
            int balanced = check_balanced_brackets(modified);
            int valid_chars = check_valid_chars(modified);
            int syntax_ok = check_syntax(modified);
            free(modified);
            result = balanced && valid_chars && syntax_ok;
        }
    }
    return result;
}