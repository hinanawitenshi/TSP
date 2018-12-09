#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "utilities.h"

#define TERMINATED 1e5

int main() {

    int seed = time(NULL);
    srand(seed);
    printf("# %d\n", seed);

    clock_t start_clock;
    start_clock = clock();

    Point **points = NULL;
    int n_points = 0;
    double best = load_tsp("res/a280.tsp", &points, &n_points);

    int *state = (int *)malloc(sizeof(int) * n_points);
    for (int i = 0; i < n_points; i++) state[i] = i;
    knuth_shuffle(state, n_points);
    double C = cost(points, state, n_points);
    int age = 0;
    int count = 0;

    printf("# HILL_CLIMBING\n");
    printf("# iteration,cost\n");

    while (age < TERMINATED) {

        if (!(count % 1000)) {
            printf("%d,%.3f\n", count, C);
        }

        int *tmp_state = (int *)malloc(sizeof(int) * n_points);
        memcpy(tmp_state, state, sizeof(int) * n_points);
        random_process(tmp_state, n_points);
        double cost_new = cost(points, tmp_state, n_points);
        if (cost_new < C) {
            C = cost_new;
            free(state);
            state = tmp_state;
            age = 0;
        } else {
            free(tmp_state);
        }

        age++;
        count++;

    }

    printf("\n");

    for (int i = 0; i < n_points; i++) {
        printf("# %.3f,%.3f\n", points[state[i]]->x, points[state[i]]->y);
    }
    printf("# %.3f,%.3f\n", points[state[0]]->x, points[state[0]]->y);

    printf("\n# %.3f/%.3f\n", C, best);
    printf("# %.3f\n", (double)(clock() - start_clock)/CLOCKS_PER_SEC);

    return 0;

}
