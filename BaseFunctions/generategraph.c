#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

​

#define NUM_GRAPHS 10
#define MIN_GRAPH_SIZE 10
#define MAX_GRAPH_SIZE 32

​

void genGraph(char* filename, int seed, double edge_density) {
    FILE* fp = fopen(filename, "w");
    srand(seed);
    int graph_size = MIN_GRAPH_SIZE + rand() % (MAX_GRAPH_SIZE+1 - MIN_GRAPH_SIZE);
    int **am;
    am = (int**)malloc(graph_size * sizeof(int*));
    for (int i = 0; i < graph_size; i++) {
        am[i] = (int*)malloc(graph_size * sizeof(int));
    }
    fprintf(fp, "%d\n", graph_size);
    for (int i = 0; i < graph_size; i++){
        for (int j = i; j < graph_size; j++) {
            if (i == j) {
                am[i][j] = 0;
            }
            else if (edge_density > ((double)rand())/RAND_MAX) {
                am[i][j] = 1;
                am[j][i] = 1;
            }
        }
    }
    for (int i = 0; i < graph_size; i++) {
        for (int j = 0; j < graph_size; j++) {
            if (am[i][j] == 1) {
                fprintf(fp, "%d %d\n", i+1, j+1);
            }
        }
    }
    fclose(fp);
    for (int i = 0; i < graph_size; i++) {
        free(am[i]);
    }
    free(am);
}

​
int main (int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    double edge_density = 0.1;
    for (int i = 0; i < NUM_GRAPHS; i++) {
        for (int j = 0; j < NUM_GRAPHS; j++) {
            char filename[128];
            sprintf(filename, "./input/%d_testGraph_%d.txt\n", (int) (edge_density*100), j);
            genGraph(filename, j, edge_density);
        }
        edge_density = edge_density + 0.1;
    }
}

