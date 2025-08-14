/**
 * @file graph.c
 * @brief Основной файл программы для построения графиков функций
 *
 * Программа принимает математическое выражение и строит его график
 * в терминале с использованием алгоритма Дейкстры для преобразования
 * выражения в польскую нотацию.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dij.h"
#include "plotting.h"
#include "validation.h"
#define XMIN 0
#define XMAX 4 * M_PI
#define YMIN 1
#define YMAX -1

int main() {
    char *input = NULL;
    size_t len = 0;
    if (getline(&input, &len, stdin) != -1) {
        input[strcspn(input, "\n")] = '\0';

        printf("%s\n", input);
        if (validate(input)) {
            char *out = tildas(input);
            char res[400];
            if (!infix_to_postfix(out, res)) {
                plot_function(res, XMIN, XMAX, YMIN, YMAX);
            }
            free(out);
        } else {
            printf("n/a");
        }
    }
    free(input);
}