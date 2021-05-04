#include "matrices.h"
#include "vector.h"
#include <time.h>

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

/* Pass function to time the call */
double timing(int (*f)(struct matrix *), struct matrix *matrix) {
    clock_t t;
    t = clock();
    (*f)(matrix);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    return time_taken;
}


int main(int argc, char *argv[]) {
    struct matrix *arr = m_new(2,2);
    struct matrix *arr2 = m_fill(22,22,0);
    struct vector *vector = v_fill(3, 1);
    struct matrix *arr3 = m_transpose(arr2);
    double time = timing(cc, arr3);
    printf("CC algorithm took %f seconds.\n", time);


    return 0;
}