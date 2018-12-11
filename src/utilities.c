#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "utilities.h"

boolean metropolis(double cost_current, double cost_next, double T) {

    if (fabs(T) < 1e-15)
        return cost_next < cost_current;
    double random = rand() / (double)RAND_MAX;
    return fmin(1, exp((cost_current - cost_next) / T)) >= random;

}

// load_tsp loads the question from a file and
// store points in variable points.
//
// This function will override points size,
// and the callee must manually free those
// variables in its context.
//
// The return value is the current best cost
// presenting in the file.
double load_tsp(const char *path, Point ***points, int *size) {

    FILE *f = fopen(path, "r");

    double best = 0;
    fscanf(f, "%lf", &best);
    
    fscanf(f, "%d", size);

    *points = (Point **)malloc(sizeof(Point *) * *size);
    int index = 0;

    while (!feof(f)) {
        int num;
        double x, y;
        fscanf(f, "%d%lf%lf", &num, &x, &y);
        (*points)[index] = (Point *)malloc(sizeof(Point));
        (*points)[index]->x = x;
        (*points)[index]->y = y;
        index++;
    }

    return best;

}

// knuth_shuffle randomly shuffles an integer
// array with knuth shuffle algorithm.
void knuth_shuffle(int *arr, int size) {

    for (int i = 0; i < size - 1; i++) {
        int pos = rand() % (size - i) + i;
        int tmp = arr[i];
        arr[i] = arr[pos];
        arr[pos] = tmp;
    }

}

// random_process randomly picks a function
// to process the arr.
void random_process(int *arr, int size) {

    if (rand() % 2) {
        random_swap(arr, size);
    } else {
        random_reverse(arr, size);
    }

}

// random_swap randomly swaps two elements
// in an integer arr.
void random_swap(int *arr, int size) {

    int posX = rand() % (size - 1);
    int posY = rand() % (size - posX) + posX;
    int tmp = arr[posX];
    arr[posX] = arr[posY];
    arr[posY] = tmp;

}

// random_reverse randomly reverses a slice of
// an array.
void random_reverse(int *arr, int size) {

    int posX = rand() % (size - 1);
    int posY = rand() % (size - posX) + posX;
    int len = posY - posX + 1;
    int *tmp = (int *)malloc(sizeof(int) * len);
    for (int i = 0; i < len; i++)
        tmp[i] = arr[posY - i];
    memcpy(arr + posX, tmp, sizeof(int) * len);
    free(tmp);

}

// cost computes the total distance based on a
// permutation.
double cost(Point **points, int *perm, int size) {

    double distance = 0;
    for (int i = 1; i < size; i++) {
        Point *p = points[perm[i]];
        Point *prev_p = points[perm[i - 1]];
        distance += sqrt(pow(p->x - prev_p->x, 2) + pow(p->y - prev_p->y, 2));
    }
    Point *p = points[perm[size - 1]];
    Point *prev_p = points[perm[0]];
    distance += sqrt(pow(p->x - prev_p->x, 2) + pow(p->y - prev_p->y, 2));
    return distance;

}
