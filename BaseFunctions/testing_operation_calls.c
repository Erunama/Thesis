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
    int rowz = matrix->rows;
    if (!rowz) {
        return NULL;
    }
    struct vector *result = v_new(rowz);
    for (int j = 0; j < rowz; j++) {
        char *row = m_return_row(matrix, j);
        for (int i = 0; i < vector->size; i++) {
            v_set(result, j, (v_get(vector, i) * *(row + i) + v_get(result, j)));
        }
        free(row);
    }
    return result;

}

/* Basic CC algorithm based on bfs */
int cc(struct matrix *matrix) {
    struct vector *vector = v_fill(matrix->cols, 0);
    v_set(vector, 0, 1);
    for (int i = 0; i < matrix->rows; i++) {
        struct vector *res = mul_matrix_vector(matrix, vector);
        v_add(vector, res);
        v_free(res);
        v_capped(vector);
        if (v_rank(vector) == vector->size) {
            v_free(vector);
            return 0;
        }
    }
    v_free(vector);
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
    struct matrix *am = m_fill(MAX_GRAPH_SIZE, MAX_GRAPH_SIZE, 0);
    while (fscanf(fp, "%d %d", &src, &dest) != EOF) {
        m_set(am, src-1, dest-1, 1);
        m_set(am, dest-1, src-1, 1);
    }
    fclose(fp);
    return am;
}

int main (int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    struct vector *vector = v_new(10);
    v_set(vector, 0, 1);
    double total_time = 0;
    struct timespec begin, end;
    double cpu_time_used;
    int a = 1;
    int b = 0;

    clock_gettime(CLOCK_REALTIME, &begin);
    a & b;
    clock_gettime(CLOCK_REALTIME, &end);
    printf("Integer mul: %d", a & b);
    long seconds = end.tv_sec - begin.tv_sec;
    long nseconds = end.tv_nsec - begin.tv_nsec;
    cpu_time_used = seconds + nseconds*1e-9;
    total_time += cpu_time_used;
    printf("Total time used %.10f\n", total_time);
}