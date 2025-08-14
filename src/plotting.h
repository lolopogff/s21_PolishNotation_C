/* plotting.h */
#ifndef PLOTTING_H
#define PLOTTING_H

double apply_unary(const char *op, double a);
double apply_binary(const char *op, double a, double b);
double evaluate_rpn(const char *postfix, double x);
void plot_function(const char *postfix, double xmin, double xmax, double ymin, double ymax);
#endif