#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <float.h>

#include "utilities.h"

#define TEMP 100
#define FACTOR 0.99
#define N_EXT 1000
#define N_INN 10000
#define TSP_PATH "res/a280.tsp"

boolean metropolis(double cost_current, double cost_next, double T);

int main() {

    int seed = time(NULL);
    srand(seed);
    printf("# %d\n", seed);

    clock_t start_clock;
    start_clock = clock();

    Point **points = NULL;
    int n_points = 0;
    double best = load_tsp(TSP_PATH, &points, &n_points);

    int *state = (int *)malloc(sizeof(int) * n_points);
    for (int i = 0; i < n_points; i++) state[i] = i;
    knuth_shuffle(state, n_points);
    double T = TEMP;
    int age = 0;
    double C = cost(points, state, n_points);
    

    printf("# SA\n# %.3f\n", T);
    printf("# ext_iteration,temperature,cost\n");

    for (int i = 0; i < N_EXT; i++) {
        for (int j = 0; j < N_INN; j++) {
            int *tmp_state = (int *)malloc(sizeof(int) * n_points);
            memcpy(tmp_state, state, sizeof(int) * n_points);
            random_process(tmp_state, n_points);
            double cost_next = cost(points, tmp_state, n_points);
            if (metropolis(C, cost_next, T)) {
                free(state);
                state = tmp_state;
                C = cost_next;
                age = 0;
            } else {
                free(tmp_state);
            }
            age++;
        }
        T *= 0.99;
        printf("%d,%.3f,%.3f\n", i, T, C);
        // if (age > N_INN * 10) {
        //     break;
        // }
    }

    printf("\n");

    for (int i = 0; i < n_points; i++) {
        printf("# %.3f,%.3f\n", points[state[i]]->x, points[state[i]]->y);
    }

    printf("\n# %.3f/%.3f\n", C, best);
    printf("# %.3f\n", (double)(clock() - start_clock)/CLOCKS_PER_SEC);

    free(state);
    for (int i = 0; i < n_points; i++) free(points[i]);
    free(points);

    return 0;

}

boolean metropolis(double cost_current, double cost_next, double T) {

    if (fabs(T) < 1e-15)
        return cost_next < cost_current;
    double random = rand() / (double)RAND_MAX;
    return fmin(1, exp((cost_current - cost_next) / T)) >= random;

}
