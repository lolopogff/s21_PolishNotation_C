#include "plotting.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "dij.h"
#include "stack.h"

#define WIDTH 80
#define HEIGHT 25
#define MAX_TOK_LEN 20

double apply_unary(const char *op, double a) {
    double res = NAN;
    if (strcmp(op, "~") == 0) res = -a;
    if (strcmp(op, "sin") == 0) res = sin(a);
    if (strcmp(op, "cos") == 0) res = cos(a);
    if (strcmp(op, "tan") == 0) res = tan(a);
    if (strcmp(op, "ctg") == 0) res = 1.0 / tan(a);
    if (strcmp(op, "sqrt") == 0) res = (a >= 0) ? sqrt(a) : NAN;
    if (strcmp(op, "ln") == 0) res = (a > 0) ? log(a) : NAN;
    return res;
}

double apply_binary(const char *op, double a, double b) {
    double res = NAN;
    if (strcmp(op, "+") == 0) res = a + b;
    if (strcmp(op, "-") == 0) res = a - b;
    if (strcmp(op, "*") == 0) res = a * b;
    if (strcmp(op, "/") == 0) res = (b != 0) ? a / b : NAN;
    return res;
}

double evaluate_rpn(const char *postfix, double x) {
    struct stack *s = NULL;
    char *rest = (char *)postfix, token[MAX_TOK_LEN];
    double result = NAN;
    while (sscanf(rest, "%19s", token) == 1) {
        rest += strlen(token);
        while (*rest == ' ') rest++;
        if (strcmp(token, "x") == 0) {
            char buf[MAX_TOK_LEN];
            snprintf(buf, sizeof(buf), "%.10g", x);
            s = push(s, buf);
        } else if ((token[0] >= '0' && token[0] <= '9') ||
                   (token[0] == '-' && (token[1] >= '0' && token[1] <= '9')))
            s = push(s, token);
        else {
            char va[MAX_TOK_LEN];
            double a = 0, res;
            int unary = (strcmp(token, "~") == 0 || is_function(token));
            if (unary) {
                s = pop(s, va);
                a = atof(va);
                res = apply_unary(token, a);
            } else {
                char vb[MAX_TOK_LEN];
                s = pop(s, vb);
                s = pop(s, va);
                a = atof(va);
                double b = atof(vb);
                res = apply_binary(token, a, b);
            }
            char buf[MAX_TOK_LEN];
            snprintf(buf, sizeof(buf), "%.10g", res);
            s = push(s, buf);
        }
    }
    if (s) {
        char final_val[MAX_TOK_LEN];
        s = pop(s, final_val);
        result = atof(final_val);
        destroy(s);
    }
    return result;
}

void plot_function(const char *postfix, double xmin, double xmax, double ymin, double ymax) {
    char grid[HEIGHT][WIDTH];
    memset(grid, '.', sizeof(grid));
    for (int col = 0; col < WIDTH; col++) {
        double x = xmin + (xmax - xmin) * col / (WIDTH - 1);
        double y = evaluate_rpn(postfix, x);
        y = round(y * 1e10) / 1e10;
        int plot_row = (int)round((y - ymin) / (ymax - ymin) * (HEIGHT - 1));
        if (plot_row >= 0 && plot_row < HEIGHT) {
            grid[plot_row][col] = '*';
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            putchar(grid[i][j]);
        }
        putchar('\n');
    }
}
