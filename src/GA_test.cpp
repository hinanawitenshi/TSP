#include <gtest/gtest.h>
#include <math.h>
#include <time.h>
#include "utilities.h"

#define POP_SIZE 2 // the size of the population
#define N_GEN 100 // the number of generations(iterations)

typedef int* Individual;
typedef int** Population;
typedef int (*FUNC_X)(Individual, Individual, Individual **, int);
typedef void (*FUNC_M)(Individual, int);

void gen_pop(Population pop, int npop, int npoints);
void crossover(Population *pop, int *npop, int npoints, FUNC_X x_method);
void mutate(Population pop, int npop, int npoints, FUNC_M m_method);
void selection(Point **points, int npoints, Population *pop, int *npop,
    int dst_npop);

Individual bestof(Point **points, int npoints, Population pop, int npop);
double bestcostof(Point **points, int npoints, Population pop, int npop);
double meanof(Point **points, int npoints, Population pop, int npop);

int PMX(Individual x, Individual y, Individual **offspring, int size);
int OX(Individual x, Individual y, Individual **offspring, int size);
int PBX(Individual x, Individual y, Individual **offspring, int size);
int OBX(Individual x, Individual y, Individual **offspring, int size);
int CX(Individual x, Individual y, Individual **offspring, int size);
int RNDX(Individual x, Individual y, Individual **offspring, int size);

void m_swap(Individual x, int size);
void m_inverse(Individual x, int size);
void m_heuristic(Individual x, int size);

namespace {

class GATest : public ::testing::Test {
protected:
    // (0, 0), (1, 0), (2, 0), (3, 0), (4, 0)
    Point **points;
    int npoints;
    Population pop;
    int npop;
    int seed;
    void SetUp() {
        load_tsp("res/test20.tsp", &points, &npoints);
        printf("[----------] TEST ON");
        for (int i = 0; i < npoints; i++) {
            printf(" (%.2f,%.2f)", points[i]->x, points[i]->y);
        }
        printf("\n");
        npop = 2;
        pop = (Population)malloc(sizeof(Individual) * POP_SIZE);
        pop[0] = (Individual)malloc(sizeof(Individual) * npoints);
        pop[1] = (Individual)malloc(sizeof(Individual) * npoints);
        for (int i = 0; i < npoints; i++) pop[0][i] = i;
        knuth_shuffle(pop[0], npoints);
        for (int i = 0; i < npoints; i++) pop[1][i] = i;
        knuth_shuffle(pop[1], npoints);
        printf("[----------] With Populations:\n");
        for (int i = 0; i < POP_SIZE; i++) {
            printf("[----------]");
            for (int j = 0; j < npoints; j++) {
                printf(" %d", pop[i][j]);
            }
            double c = cost(points, pop[i], npoints);
            printf(" (%.2f)\n", c);
        }
    }
    ~GATest() {
        for (int i = 0; i < npoints; i++) {
            free(points[i]);
        }
        free(points);
        for (int i = 0; i < POP_SIZE; i++) {
            free(pop[i]);
        }
        free(pop);
    }
};

TEST_F(GATest, GenPop) {
    Population pop = (Population)malloc(sizeof(Individual) * POP_SIZE);
    gen_pop(pop, POP_SIZE, npoints);
    for (int i = 0; i < POP_SIZE; i++) {
        ASSERT_NE(pop[i], nullptr) << "position " << i << " is NULL";
    }
    for (int i = 0; i < POP_SIZE; i++) {
        free(pop[i]);
    }
    free(pop);
}

TEST_F(GATest, CrossMethods) {
    // PMX: 14-17
    FUNC_X methods[] = {PMX, OX, OBX};
    for (int i = 0; i < 3; i++) {
        Individual x = pop[0], y = pop[1];
        Individual *offspring = nullptr;
        int n_offspring = methods[i](x, y, &offspring, npoints);
        if (methods[i] == PMX)
            EXPECT_EQ(n_offspring, 2);
        else
            EXPECT_EQ(n_offspring, 1);
        EXPECT_NE(offspring, nullptr);
        printf("[----------] offspring[0]:");
        for (int i = 0; i < npoints; i++) {
            printf(" %d", offspring[0][i]);
        }
        printf("\n");
        if (methods[i] == PMX) {
            printf("[----------] offspring[1]:");
            for (int i = 0; i < npoints; i++) {
                printf(" %d", offspring[1][i]);
            }
            printf("\n");
        }
    }
}

TEST_F(GATest, CrossOver) {
    crossover(&pop, &npop, npoints, PMX);
    EXPECT_EQ(npop, 6);
    printf("[----------] Populations:\n");
    for (int i = 0; i < npop; i++) {
        printf("[----------]");
        for (int j = 0; j < npoints; j++) {
            printf(" %d", pop[i][j]);
        }
        double c = cost(points, pop[i], npoints);
        printf(" (%.2f)\n", c);
    }
}

TEST_F(GATest, selection) {
    crossover(&pop, &npop, npoints, PMX);
    printf("[----------] Populations:\n");
    for (int i = 0; i < npop; i++) {
        printf("[----------]");
        for (int j = 0; j < npoints; j++) {
            printf(" %d", pop[i][j]);
        }
        double c = cost(points, pop[i], npoints);
        printf(" (%.2f)\n", c);
    }
    selection(points, npoints, &pop, &npop, POP_SIZE);
    EXPECT_EQ(npop, POP_SIZE);
    printf("[----------] Selected Populations:\n");
    for (int i = 0; i < npop; i++) {
        printf("[----------]");
        for (int j = 0; j < npoints; j++) {
            printf(" %d", pop[i][j]);
        }
        double c = cost(points, pop[i], npoints);
        printf(" (%.2f)\n", c);
    }
}

} // namespace

int main(int argc, char *argv[]) {
    srand(time(NULL));
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
