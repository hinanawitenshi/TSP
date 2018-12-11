#ifndef UTILITIES_H
#define UTILITIES_H

#define true 1
#define false 0

typedef struct {
    double x;
    double y;
} Point;

typedef int boolean;

boolean metropolis(double cost_current, double cost_next, double T);
double load_tsp(const char *path, Point ***points, int *size);
void knuth_shuffle(int *arr, int size);
void random_process(int *arr, int size);
void random_swap(int *arr, int size);
void random_reverse(int *arr, int size);
double cost(Point **points, int *perm, int size);

#endif
