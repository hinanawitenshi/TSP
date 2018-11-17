#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "utilities.h"

#define RESTART_TIMES 10000
#define TERMINATED 1e5

int main() {

    int seed = time(NULL);
    srand(seed);
    printf("# %d\n", seed);

    clock_t start_clock;
    start_clock = clock();

    Point **points = NULL;
    int n_points = 0;
    double best = load_tsp("res/att48.tsp", &points, &n_points);

    int *state = (int *)malloc(sizeof(int) * n_points);
    for (int i = 0; i < n_points; i++) state[i] = i;
    knuth_shuffle(state, n_points);
    double C = cost(points, state, n_points);
    int age = 0;
    int count = 0;
    int restart_times = RESTART_TIMES;
    int *best_state = (int *)malloc(sizeof(int) * n_points);
    memcpy(best_state, state, sizeof(int) * n_points);

    printf("# HILL_CLIMBING\n");
    printf("# interation,cost\n");

    while (restart_times--) {
        while (age < TERMINATED) {

            int *tmp_state = (int *)malloc(sizeof(int) * n_points);
            memcpy(tmp_state, state, sizeof(int) * n_points);
            random_swap(tmp_state, n_points);
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
        if (C < cost(points, best_state, n_points)) {
            memcpy(best_state, state, sizeof(int) * n_points);
        }
        printf("%d,%.3f\n", RESTART_TIMES - restart_times,
            cost(points, best_state, n_points));
        knuth_shuffle(state, n_points);
        C = cost(points, state, n_points);
        age = 0;
        count = 0;
    }

    printf("\n");

    for (int i = 0; i < n_points; i++) {
        printf("# %d,%d\n", points[best_state[i]]->x, points[best_state[i]]->y);
    }

    printf("\n# %.3f/%.3f\n", cost(points, best_state, n_points), best);
    printf("# %.3f\n", (double)(clock() - start_clock)/CLOCKS_PER_SEC);

    return 0;

}
