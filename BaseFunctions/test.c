#include "matrices.h"
#include "vector.h"
#include <time.h>
#define NUM_GRAPHS 10
#define MIN_GRAPH_SIZE 10
#define MAX_GRAPH_SIZE 32

/* multiply a matrix and vector and return a vector */
struct vector *mul_matrix_vector(struct matrix *matrix, struct vector *vector){
    if (matrix->cols != vector->size) {
        return NULL;
    }
    struct vector *result = v_new(matrix->rows);
    for (int j = 0; j < matrix->rows; j++) {
        float *row = m_return_row(matrix, j);
        for (int i = 0; i < vector->size; i++) {
            v_set(result, j, (v_get(vector, i) * *(row + i) + v_get(result, j)));
        }
    }
    return result;

}

/* Basic CC algorithm based on bfs */
int cc(struct matrix *matrix) {
    struct vector *vector = v_fill(matrix->cols, 0);
    v_set(vector, 0, 1);
    for (int i = 0; i < matrix->rows; i++) {
        v_add(vector, mul_matrix_vector(matrix, vector));
        v_capped(vector);
        if (v_rank(vector) == vector->size) {
            return 0;
        }
    }
    return 1;
}

// /* Pass function to time the call */
// double timing(int (*f)(struct matrix *), struct matrix *matrix) {
//     clock_t t;
//     t = clock();
//     (*f)(matrix);
//     t = clock() - t;
//     double time_taken = ((double)t)/CLOCKS_PER_SEC;
//     return time_taken;
// }

struct matrix *readGraph(char* filename) {
    FILE* fp = fopen(filename, "r");
    int src;
    int dest;
    int graph_size;
    fscanf(fp, "%d", &graph_size);
    struct matrix *am = m_fill(graph_size, graph_size, 0.0);
    while (fscanf(fp, "%d %d", &src, &dest) != EOF) {
        m_set(am, src-1, dest-1, 1.0);
        m_set(am, dest-1, src-1, 1.0);
    }
    fclose(fp);
    return am;
}

int main (int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    int iterations = 1000;
    int cca;
    for (int size = MIN_GRAPH_SIZE; size < MAX_GRAPH_SIZE; size++) {
        double edge_density = 0.1;
        for (int j = 0; j < 2; j++) {
            for (int inputfile = 0; inputfile < 5; inputfile++) {
                int density = (int)(edge_density*100);
                double total_time = 0.0;
                char filename[256];
                sprintf(filename, "./input/%dsize_%ddens_%d.txt", size, density, inputfile);
                char outname[256];
                sprintf(outname, "./results/%dsize_%ddens_%d.out", size, density, inputfile);
                FILE* fout = fopen(outname, "w");
                struct matrix *am = readGraph(filename);
                for (int i = 0; i < iterations; i++) {

                    // printAdjacencyMatrix(am);
                    struct timespec begin, end;
                    double cpu_time_used;
                    clock_gettime(CLOCK_REALTIME, &begin);
                    cca = cc(am);
                    clock_gettime(CLOCK_REALTIME, &end);
                    long seconds = end.tv_sec - begin.tv_sec;
                    long nseconds = end.tv_nsec - begin.tv_nsec;
                    cpu_time_used = seconds + nseconds*1e-9;
                    total_time = total_time + cpu_time_used;
                }
                m_free(am);
                fprintf(fout, "Input file: %s\n", filename);
                fprintf(fout, "Graph size: %d\nEdge density: %d\n", size, density);
                fprintf(fout, "Total time used for %d iterations: %.10f\n", iterations, total_time);
                fprintf(fout, "Average time per iteration: %.10f\n", total_time/iterations);
                if (cca) {
                    fprintf(fout, "Graph is connected!\n");
                }
                else {
                    fprintf(fout, "Graph is not connected!\n");
                }
            }
            edge_density = edge_density + 0.1;
        }
    }
}