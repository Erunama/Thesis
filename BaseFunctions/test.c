#include "matrices.h"
#include "vector.h"
#include <time.h>
#include <string.h>
#define NUM_GRAPHS 10
#define MIN_GRAPH_SIZE 10
#define MAX_GRAPH_SIZE 32

// TODO: Freeze Data
// TODO: Freeze version

int GRAPH_SIZE = 0;
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

struct vector *mul_matrix_vector_optimized(struct matrix *matrix, struct vector *vector){
    if (matrix->cols != vector->size) {
        return NULL;
    }
    int rowz = matrix->rows;
    if (!rowz) {
        return NULL;
    }
    struct vector *result = v_fill(rowz, 0);
    for (int j = 0; j < rowz; j++) {
        for (int i = 0; i < vector->size; i++) {
            *(result->data + j) += (*(vector->data + i) * *(matrix->data + j * rowz + i));
        }
    }
    return result;
}

/* Basic CC algorithm based on bfs */
int cc2(struct matrix *matrix) {
    struct vector *vector = v_fill(matrix->cols, 0);
    *(vector->data) = 1;
    char d = 0;
    for (int i = 0; i < matrix->rows; i++) {
        struct vector *res = mul_matrix_vector_optimized(matrix, vector);
        for (int i = 0; i < vector->size; i++) {
            *(vector->data + i) += *(res->data + i);
            *(vector->data + i) = *(vector->data + i) != 0 ? (*(vector->data + i) / *(vector->data + i)) : 0;
            d += *(vector->data + i);
        }
        if (d == vector->size) {
            free(vector->data);
            free(vector);
            v_free(res);
            return 0;
        }
        v_free(res);
        d = 0;
    }
    free(vector->data);
    free(vector);
    return 1;
}

/* Basic CC algorithm based on bfs */
int cc3(struct matrix *matrix) {
    char *vector = calloc(matrix->cols, sizeof(char));
    char *result = calloc(matrix->cols, sizeof(char));
    if(!result || !vector) {
        return 1;
    }
    *(vector) = 1;
    char d = 0;
    char iters = 0;
    do {
        for (int j = 0; j < matrix->rows; j++) {
            for (int i = 0; i < matrix->cols; i++) {
                *(result + j) += (*(vector + i) * *(matrix->data + j * matrix->rows + i));
            }
            d += ((*(vector + j) + *(result + j)) != 0 ? 1 : 0);
        }
        if (d == matrix->cols) {
            free(vector);
            free(result);
            return 0;
        }

        for (int i = 0; i < matrix->cols; i++) {
            *(vector + i) = (*(vector + i) + *(result + i)) != 0 ? 1 : 0;
            *(result + i) = 0;
        }
        d = 0;
        iters++;
    }
    while (iters < matrix->rows);
    free(vector);
    free(result);
    return 1;
}

int cc4(char *matrix) {
    char *vector = calloc(GRAPH_SIZE, sizeof(char));
    char *result = calloc(GRAPH_SIZE, sizeof(char));
    if(!result || !vector) {
        return 1;
    }
    *(vector) = 1;
    char d = 1;
    char iters = 0;
    do {
        for (int j = 0; j < GRAPH_SIZE; j++) {
            for (int i = 0; i < GRAPH_SIZE; i++) {
                *(result + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
            }
            d &= (*(vector + j) | *(result + j));
        }
        if (d) {
            free(vector);
            free(result);
            return 0;
        }
        for (int i = 0; i < GRAPH_SIZE; i++) {
            *(vector + i) = (*(vector + i) | *(result + i));
            *(result + i) = 0;
        }
        d = 1;
        iters++;
    }
    while (iters < GRAPH_SIZE);
    free(vector);
    free(result);
    return 1;
}

/* 6 iteration of my CCa
 * This one is the most abstract: uses only 2 allocated data structures based on the graph size
 */
int cc5(char *matrix) {
    char *vector = calloc(GRAPH_SIZE, sizeof(char));
    if(!vector) {
        return 1;
    }
    *(vector) = 1;
    char d = 1;
    char iters = 0;
    do {
        for (int j = 0; j < GRAPH_SIZE; j++) {
            for (int i = 0; i < GRAPH_SIZE; i++) {
                *(vector + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
            }
            d &= *(vector + j);
        }
        if (d) {
            free(vector);
            return 0;
        }
        d = 1;
        iters++;
    }
    while (iters < GRAPH_SIZE);
    free(vector);
    return 1;
}

int cc6(char *matrix) {
    char *vector = calloc(GRAPH_SIZE, sizeof(char));
    if(!vector) {
        return 1;
    }
    *(vector) = 1;
    char d = 1;
    char iters = 0;
    char loop_size = GRAPH_SIZE - 10;
    do {
        for (int j = 0; j < GRAPH_SIZE; j++) {
            int i = 0;
            for (; i < loop_size - 1; i+=2) {
                *(vector + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
                *(vector + j) |= (*(vector + i + 1) & *(matrix + j * GRAPH_SIZE + i + 1));
            }
            if (i < loop_size) {
                *(vector + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
            }
            d &= *(vector + j);
        }
        if (d) {
            free(vector);
            return 0;
        }
        d = 1;
        iters++;
    }
    while (iters < GRAPH_SIZE);
    free(vector);
    return 1;
}

/* BEST CASE CURRENTLY */
int cc7(char *matrix) {
    char *vector = calloc(GRAPH_SIZE, sizeof(char));
    if(!vector) {
        return 1;
    }
    *(vector) = 1;
    char d = 1;
    char iters = 0;
    char loop_size = GRAPH_SIZE - 10;
    do {
        for (int j = 0; j < GRAPH_SIZE; j++) {
            /* First ten entries because min graph size is 10 */
            *(vector + j) |= (*(vector ) & *(matrix + j * GRAPH_SIZE));
            *(vector + j) |= (*(vector + 1) & *(matrix + j * GRAPH_SIZE + 1));
            *(vector + j) |= (*(vector + 2) & *(matrix + j * GRAPH_SIZE + 2));
            *(vector + j) |= (*(vector + 3) & *(matrix + j * GRAPH_SIZE + 3));
            *(vector + j) |= (*(vector + 4) & *(matrix + j * GRAPH_SIZE + 4));
            *(vector + j) |= (*(vector + 5) & *(matrix + j * GRAPH_SIZE + 5));
            *(vector + j) |= (*(vector + 6) & *(matrix + j * GRAPH_SIZE + 6));
            *(vector + j) |= (*(vector + 7) & *(matrix + j * GRAPH_SIZE + 7));
            *(vector + j) |= (*(vector + 8) & *(matrix + j * GRAPH_SIZE + 8));
            *(vector + j) |= (*(vector + 9) & *(matrix + j * GRAPH_SIZE + 9));

            int i = 10;
            for (; i < loop_size - 1; i+=3) {
                *(vector + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
                *(vector + j) |= (*(vector + i + 1) & *(matrix + j * GRAPH_SIZE + i + 1));
                *(vector + j) |= (*(vector + i + 2) & *(matrix + j * GRAPH_SIZE + i + 2));
            }
            for (; i < loop_size; i++) {
                *(vector + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
            }
            d &= *(vector + j);
        }
        if (d) {
            free(vector);
            return 0;
        }
        d = 1;
        iters++;
    }
    while (iters < GRAPH_SIZE);
    free(vector);
    return 1;
}

struct matrix *readGraph(char* filename) {
    FILE* fp = fopen(filename, "r");
    int src;
    int dest;
    GRAPH_SIZE = 0;
    fscanf(fp, "%d", &GRAPH_SIZE);
    // printf("Gs: %d\n", GRAPH_SIZE);
    struct matrix *am = m_fill(GRAPH_SIZE, GRAPH_SIZE, 0);
    while (fscanf(fp, "%d %d", &src, &dest) != EOF) {
        m_set(am, src-1, dest-1, 1);
        m_set(am, dest-1, src-1, 1);
    }
    fclose(fp);
    return am;
}

char *readGraph2(char* filename) {
    FILE* fp = fopen(filename, "r");
    int src;
    int dest;

    fscanf(fp, "%d", &GRAPH_SIZE);
    // printf("Gs: %d\n", GRAPH_SIZE);
    char *matrix = calloc(GRAPH_SIZE * GRAPH_SIZE, sizeof(char));
    while (fscanf(fp, "%d %d", &src, &dest) != EOF) {
        *(matrix + (src-1) * GRAPH_SIZE + (dest-1)) = 1;
        *(matrix + (dest-1) * GRAPH_SIZE + (src-1)) = 1;
    }
    fclose(fp);
    return matrix;
}

int main (int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    int iterations = 10000;
    int cca = 0;
    int cca_num = 1;

    char outname[256];


    sprintf(outname, "./results/results_cc%d.csv", cca_num);
    FILE* fout = fopen(outname, "w");
    fprintf(fout, "cca,size,density,total_time\n");

    for (int size = MIN_GRAPH_SIZE; size < MAX_GRAPH_SIZE; size++) {
        double edge_density = 0.1;
        for (int j = 0; j < 2; j++) {
            for (int inputfile = 0; inputfile < 5; inputfile++) {
                int density = (int)(edge_density*100);
                double total_time = 0.0;
                char filename[256];
                sprintf(filename, "./input/%dsize_%ddens_%d.txt", size, density, inputfile);

                struct matrix *am = readGraph(filename);
                // char *am = readGraph2(filename);
                cca = cc8(am);
                for (int i = 0; i < iterations; i++) {
                    struct timespec begin, end;
                    double cpu_time_used;
                    clock_gettime(CLOCK_REALTIME, &begin);
                    cca = cc1(am);
                    clock_gettime(CLOCK_REALTIME, &end);
                    long seconds = end.tv_sec - begin.tv_sec;
                    long nseconds = end.tv_nsec - begin.tv_nsec;
                    cpu_time_used = seconds + nseconds*1e-9;
                    total_time = total_time + cpu_time_used;
                }
                m_free(am);
                // free(am);
                // fprintf(fout, "Input file: %s\n", filename);
                // fprintf(fout, "Graph size: %d\nEdge density: %d\n", size, density);
                fprintf(fout, "%d,%d,%d,%.10f\n", cca_num, size, density, total_time);
                // printf("%.10f\n", total_time);
                // fprintf(fout, "Average time per iteration: %.10f\n", total_time/iterations);
                // if (!cca) {
                //     fprintf(fout, "Graph is connected!\n");
                // }
                // else {
                //     fprintf(fout, "Graph is not connected!\n");
                // }


            }
            edge_density = edge_density + 0.1;
        }
    }
    fclose(fout);
}