#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "utilities.h"

// #define TEST

#define TSP_PATH "res/a280.tsp"
#define POP_SIZE 40 // the size of the population
#define N_GEN 500 // the number of generations(iterations)
#define PROP_X 0.8 // the probability of crossover
#define PROP_M 0.01 // the probability of mutation
#define TOURNAMENT_P 0.5
#define HC_TERMINATE 1e5
#define SA_N_EXT 1000
#define SA_N_INN 10000
#define SA_TEMP 100
#define M_METHOD m_random_process // the method of mutation
#define X_METHOD RNDX // the method of crossover

typedef int* Individual;
typedef int** Population;
typedef int (*FUNC_X)(Individual, Individual, Individual **, int);
typedef void (*FUNC_M)(Point **, Individual, int);

double fitness(Point **points, Individual x, int npoints);
void gen_random_pop(Population pop, int npop, int npoints);
void gen_near_pop(Population pop, int npop, int npoints);
void crossover(Point **points, Population *pop, int *npop, int npoints,
    FUNC_X x_method);
void mutate(Population pop, int npop, Point **points, int npoints,
    FUNC_M m_method);
void selection(Point **points, int npoints, Population *pop, int *npop,
    int dst_npop);

Individual bestof(Point **points, int npoints, Population pop, int npop);
double bestfitnessof(Point **points, int npoints, Population pop, int npop);
double meanof(Point **points, int npoints, Population pop, int npop); 

int PMX(Individual x, Individual y, Individual **offspring, int size);
int OX(Individual x, Individual y, Individual **offspring, int size);
int PBX(Individual x, Individual y, Individual **offspring, int size);
int OBX(Individual x, Individual y, Individual **offspring, int size);
int CX(Individual x, Individual y, Individual **offspring, int size);
int RNDX(Individual x, Individual y, Individual **offspring, int size);

void m_random_process(Point **points, Individual x, int size);
void m_hill_climbing(Point **points, Individual x, int size);
void m_sa(Point **points, Individual x, int size);

void m_sa(Point **points, Individual x, int size) {
    double T = SA_TEMP;
    double C = cost(points, x, size);
    for (int i = 0; i < SA_N_EXT; i++) {
        for (int j = 0; j < SA_N_INN; j++) {
            int *tmp_state = (int *)malloc(sizeof(int) * size);
            memcpy(tmp_state, x, sizeof(int) * size);
            random_process(tmp_state, size);
            double cost_next = cost(points, tmp_state, size);
            if (metropolis(C, cost_next, T)) {
                memcpy(x, tmp_state, sizeof(int) * size);
                C = cost_next;
            }
            free(tmp_state);
        }
        T *= 0.9;
    }
}

double fitness(Point **points, Individual x, int npoints) {
    return 1 / (cost(points, x, npoints)) * 1000;
}

void m_hill_climbing(Point **points, Individual x, int size) {
    int terminated = HC_TERMINATE;
    double C = cost(points, x, size);
    int age = 0;
    int count = 0;
    while (age < terminated) {
        int *tmp_state = (int *)malloc(sizeof(int) * size);
        memcpy(tmp_state, x, sizeof(int) * size);
        random_process(tmp_state, size);
        double cost_new = cost(points, tmp_state, size);
        if (cost_new < C) {
            C = cost_new;
            memcpy(x, tmp_state, sizeof(int) * size);
            age = 0;
        }
        free(tmp_state);
        age++;
        count++;
    }
}

void m_random_process(Point **points, Individual x, int size) {
    random_process(x, size);
}


int RNDX(Individual x, Individual y, Individual **offspring, int size) {
    FUNC_X pool[] = {PMX, OX, OBX};
    int pool_size = 3;
    int pick = rand() % pool_size;
    return pool[pick](x, y, offspring, size);
}

int CX(Individual x, Individual y, Individual **offspring, int size) {
    // TODO
}

int OBX(Individual x, Individual y, Individual **offspring, int size) {
    int pos_set_size = rand() % (size - 2) + 1;
    int *pos_set = (int *)malloc(sizeof(int) * pos_set_size);
    pos_set[0] = rand() % size;
    for (int i = 1; i < pos_set_size; i++) {
        int found = 1;
        while (found) {
            found = 0;
            pos_set[i] = rand() % size;
            for (int j = 0; j < i; j++) {
                if (pos_set[i] == pos_set[j]) {
                    found = 1;
                }
            }
        }
    }
    *offspring = (Individual *)malloc(sizeof(Individual) * 1);
    (*offspring)[0] = (Individual)malloc(sizeof(int) * size);
    int pos_set_pos = 0;
    for (int i = 0; i < size; i++) {
        int found = 0;
        for (int j = 0; j < pos_set_size; j++) {
            if (y[i] == x[pos_set[j]]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            (*offspring)[0][i] = y[i];
        } else {
            (*offspring)[0][i] = x[pos_set[pos_set_pos++]];
        }
    }
    free(pos_set);
    return 1;
}

int PBX(Individual x, Individual y, Individual **offspring, int size) {
    // TODO
}

int OX(Individual x, Individual y, Individual **offspring, int size) {
    int posX = rand() % (size - 1);
    int posY = rand() % (size - posX - 1) + posX + 1;
    *offspring = (Individual *)malloc(sizeof(Individual) * 1);
    (*offspring)[0] = (Individual)malloc(sizeof(int) * size);
    int y_pos = 0;
    for (int i = posX; i <= posY; i++) {
        (*offspring)[0][i] = x[i];
    }
    for (int i = 0; i < size; i++) {
        if (posX <= i && i <= posY) continue;
        int found = 1;
        while (found) {
            found = 0;
            for (int j = posX; j <= posY; j++) {
                if (y[y_pos] == (*offspring)[0][j]) {
                    found = 1;
                }
            }
            if (found) {
                y_pos++;
            }
        }
        (*offspring)[0][i] = y[y_pos++];
    }
    return 1;
}

int PMX(Individual x, Individual y, Individual **offspring, int size) {
    int posX = rand() % (size - 1);
    int posY = rand() % (size - posX - 1) + posX + 1;
    *offspring = (Individual *)malloc(sizeof(Individual) * 2);
    (*offspring)[0] = (Individual)malloc(sizeof(int) * size);
    (*offspring)[1] = (Individual)malloc(sizeof(int) * size);
    for (int i = posX; i <= posY; i++) {
        (*offspring)[0][i] = y[i];
        (*offspring)[1][i] = x[i];
    }
    for (int i = 0; i < size; i++) {
        if (posX <= i && i <= posY) {
            continue;
        }
        int foundX = 1;
        int foundY = 1;
        int candX = x[i];
        int candY = y[i];
        while (foundX || foundY) {
            foundX = 0;
            foundY = 0;
            for (int j = posX; j <= posY; j++) {
                if (candX == (*offspring)[0][j]) {
                    foundX = 1;
                    candX = (*offspring)[1][j];
                }
                if (candY == (*offspring)[1][j]) {
                    foundY = 1;
                    candY = (*offspring)[0][j];
                }
                if (foundX && foundY) {
                    break;
                }
            }
        }
        (*offspring)[0][i] = candX;
        (*offspring)[1][i] = candY;
    }
    return 2;
}

double meanof(Point **points, int npoints, Population pop, int npop) {
    double total = 0.0;
    for (int i = 0; i < npop; i++) {
        total += fitness(points, pop[i], npoints);
    }
    return total / npop;
}

double bestfitnessof(Point **points, int npoints, Population pop, int npop) {
    double max_fitness = 0;
    for (int i = 0; i < npop; i++) {
        double c = fitness(points, pop[i], npoints);
        if (c > max_fitness) {
            max_fitness = c;
        }
    }
    return max_fitness;
}

Individual bestof(Point **points, int npoints, Population pop, int npop) {
    double max_fitness = 0;
    int max_pos = 0;
    for (int i = 0; i < npop; i++) {
        double c = fitness(points, pop[i], npoints);
        if (c > max_fitness) {
            max_fitness = c;
            max_pos = i;
        }
    }
    return pop[max_pos];
}

void tournament(Point **points, int npoints, Population *pop, int *npop,
    int dst_npop) {
    Population result = (Population)malloc(sizeof(Individual) * dst_npop);
    int result_pos = 0;
    double *props = (double *)malloc(sizeof(double) * *npop);
    for (int i = 0; i < *npop; i++) {
        props[i] = fitness(points, (*pop)[i], npoints);
    }
    int *desc = (int *)malloc(sizeof(int) * *npop);
    for (int i = 0; i < *npop; i++) desc[i] = i;
    for (int i = 0; i < *npop - 1; i++) {
        for (int j = i + 1; j < *npop; j++) {
            if (props[desc[j]] > props[desc[i]]) {
                int tmp = desc[i];
                desc[i] = desc[j];
                desc[j] = tmp;
            }
        }
    }
    double p = TOURNAMENT_P;
    for (int i = 0; i < *npop; i++) {
        props[desc[i]] = p * pow(1-p, i);
    }
    for (int i = 1; i < *npop; i++) {
        props[i] += props[i - 1];
    }
    free(desc);
    
    // pickup
    for (int i = 0; i < dst_npop; i++) {
        double rnd = rand() / (double)RAND_MAX * props[*npop - 1];
        if (rnd <= props[0]) {
            int has = 0;
            for (int i = 0; i < result_pos; i++) {
                if (result[i] == (*pop)[0]) {
                    has = 1;
                    break;
                }
            }
            if (has) {
                result[result_pos] = (Individual)malloc(
                    sizeof(int) * npoints);
                memcpy(result[result_pos], (*pop)[0],
                    sizeof(int) * npoints);
            } else {
                result[result_pos] = (*pop)[0];
            }
            result_pos++;
        } else {
            for (int j = 0; j < *npop - 1; j++) {
                if (props[j] <= rnd && rnd <= props[j + 1]) {
                    int has = 0;
                    for (int i = 0; i < result_pos; i++) {
                        if (result[i] == (*pop)[j + 1]) {
                            has = 1;
                            break;
                        }   
                    }
                    if (has) {
                        result[result_pos] = (Individual)malloc(
                            sizeof(int) * npoints);
                        memcpy(result[result_pos], (*pop)[j + 1],
                            sizeof(int) * npoints);
                    } else {
                        result[result_pos] = (*pop)[j + 1];
                    }
                    result_pos++;
                }
            }
        }
    }
    for (int i = 0; i < *npop; i++) {
        int found = 0;
        for (int j = 0; j < dst_npop; j++) {
            if ((*pop)[i] == result[j]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            free((*pop)[i]);
        }
    }
    Population tmp = *pop;
    *pop = result;
    *npop = dst_npop;
    free(props);
    free(tmp);
}

void selection(Point **points, int npoints, Population *pop, int *npop,
    int dst_npop) {
    Population result = (Population)malloc(sizeof(Individual) * dst_npop);
    int result_pos = 0;
    double *props = (double *)malloc(sizeof(double) * *npop);
    double total_cost = 0;
    // compute costs and total_cost
    for (int i = 0; i < *npop; i++) {
        props[i] = fitness(points, (*pop)[i], npoints);
        total_cost += props[i];
    }
    // compute cdf
    props[0] = props[0] / total_cost;
    for (int i = 1; i < *npop; i++) {
        props[i] = props[i] / total_cost;
        props[i] += props[i - 1];
    }
    // pickup
    for (int i = 0; i < dst_npop; i++) {
        double rnd = rand() / (double)RAND_MAX;
        if (rnd <= props[0]) {
            int has = 0;
            for (int i = 0; i < result_pos; i++) {
                if (result[i] == (*pop)[0]) {
                    has = 1;
                    break;
                }   
            }
            if (has) {
                result[result_pos] = (Individual)malloc(
                    sizeof(int) * npoints);
                memcpy(result[result_pos], (*pop)[0],
                    sizeof(int) * npoints);
            } else {
                result[result_pos] = (*pop)[0];
            }
            result_pos++;
        } else {
            for (int j = 0; j < *npop - 1; j++) {
                if (props[j] <= rnd && rnd <= props[j + 1]) {
                    int has = 0;
                    for (int i = 0; i < result_pos; i++) {
                        if (result[i] == (*pop)[j + 1]) {
                            has = 1;
                            break;
                        }   
                    }
                    if (has) {
                        result[result_pos] = (Individual)malloc(
                            sizeof(int) * npoints);
                        memcpy(result[result_pos], (*pop)[j + 1],
                            sizeof(int) * npoints);
                    } else {
                        result[result_pos] = (*pop)[j + 1];
                    }
                    result_pos++;
                }
            }
        }
    }
    for (int i = 0; i < *npop; i++) {
        int found = 0;
        for (int j = 0; j < dst_npop; j++) {
            if ((*pop)[i] == result[j]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            free((*pop)[i]);
        }
    }
    Population tmp = *pop;
    *pop = result;
    *npop = dst_npop;
    free(props);
    free(tmp);
}

void mutate(Population pop, int npop, Point **points, int npoints,
    FUNC_M m_method) {
    for (int i = 0; i < npop; i++) {
        double rnd = rand() / (double)RAND_MAX;
        if (rnd < PROP_M) {
            m_method(points, pop[i], npoints);
        }
    }
}

void crossover(Point **points, Population *pop, int *npop, int npoints,
    FUNC_X x_method) {
    int times = *npop;
    Population new_pop = (Population)malloc(sizeof(Individual) * *npop * 2);
    int new_pop_size = 0;
    while (times--) {
        double rnd = rand() / (double)RAND_MAX;
        if (rnd < PROP_X) {
            Population tmp = (Population)malloc(sizeof(Individual) * *npop);
            int tmp_size = *npop;
            for (int i = 0; i < *npop; i++) {
                tmp[i] = (Individual)malloc(sizeof(int) * npoints);
                memcpy(tmp[i], (*pop)[i], sizeof(int) * npoints);
            }
            selection(points, npoints, &tmp, &tmp_size, 2);
            Individual *offspring = NULL;
            int n_offspring = x_method(tmp[0], tmp[1],
                &offspring, npoints);
            for (int i = 0; i < n_offspring; i++) {
                new_pop[new_pop_size++] = offspring[i];
            }
            free(offspring);
            for (int i = 0; i < tmp_size; i++) free(tmp[i]);
            free(tmp);
        }
    }
    mutate(new_pop, new_pop_size, points, npoints, M_METHOD);
    Population all_pop = (Population )malloc(sizeof(Individual) *
        (*npop + new_pop_size));
    memcpy(all_pop, *pop, sizeof(Individual) * *npop);
    memcpy(all_pop + *npop, new_pop, sizeof(Individual) * new_pop_size);
    *npop += new_pop_size;
    Population tmp = *pop;
    *pop = all_pop;
    free(tmp);
    free(new_pop);
}

void gen_random_pop(Population pop, int npop, int npoints) {
    for (int i = 0; i < npop; i++) {
        pop[i] = (Individual)malloc(sizeof(int) * npoints);
        for (int j = 0; j < npoints; j++) pop[i][j] = j;
        knuth_shuffle(pop[i], npoints);
    }
}

void gen_near_pop(Population pop, int npop, int npoints) {
    pop[0] = (Individual)malloc(sizeof(int) * npoints);
    for (int i = 0; i < npoints; i++) pop[0][i] = i;
    for (int i = 1; i < npop; i++) {
        pop[i] = (Individual)malloc(sizeof(int) * npoints);
        for (int j = 0; j < npoints; j++) pop[i][j] = j;
        random_process(pop[i], npoints);
    }
}

#ifndef TEST
int main() {
    // set and display seed, title, columns
    int seed = time(NULL);
    srand(seed);
    printf("# %d\n", seed);
    printf("# GA\n");
    printf("# generation,best,mean,global_best\n");
    // set program timer
    clock_t start_clock;
    start_clock = clock();
    // load points with the advised cost of the problem
    Point **points = NULL;
    int npoints = 0;
    double best = load_tsp(TSP_PATH, &points, &npoints);
    double global_best_fitness = 0.0;
    Individual global_best = NULL;

    // initialize the population
    Population population = (Population)malloc(sizeof(Individual) * POP_SIZE);
    gen_random_pop(population, POP_SIZE, npoints);

    // iterate
    int pop_size = POP_SIZE;
    for (int gen = 0; gen < N_GEN; gen++) {
        crossover(points, &population, &pop_size, npoints, X_METHOD);
        selection(points, npoints, &population, &pop_size, POP_SIZE);
        Individual cur_best_i = bestof(points, npoints, population, pop_size);
        double cur_best = fitness(points, cur_best_i, npoints);
        if (cur_best > global_best_fitness) {
            global_best_fitness = cur_best;
            if (global_best) {
                free(global_best);
            }
            global_best = (Individual)malloc(sizeof(int) * npoints);
            memcpy(global_best, cur_best_i, sizeof(int) * npoints);
        }
        double mean = meanof(points, npoints, population, pop_size);
        printf("%d,%f,%f,%f\n", gen, cur_best, mean, global_best_fitness);
    }

    // output
    printf("\n");
    Individual state = global_best;
    for (int i = 0; i < npoints; i++) {
        printf("# %.3f,%.3f\n", points[state[i]]->x, points[state[i]]->y);
    }
    printf("# %.3f,%.3f\n", points[state[0]]->x, points[state[0]]->y);
    double C = cost(points, state, npoints);
    printf("\n# %.3f/%.3f\n", C, best);
    printf("# %.3f\n", (double)(clock() - start_clock)/CLOCKS_PER_SEC);

    for (int i = 0; i < POP_SIZE; i++)
        free(population[i]);
    free(population);

    return 0;
}
#endif
