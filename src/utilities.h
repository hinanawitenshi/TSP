#ifndef UTILITIES_H
#define UTILITIES_H

#define true 1
#define false 0

typedef struct {
    int x;
    int y;
} Point;

typedef int boolean;

double load_tsp(const char *path, Point ***points, int *size);
void knuth_shuffle(int *arr, int size);
void random_swap(int *state, int size);
double cost(Point **points, int *perm, int size);

#endif
