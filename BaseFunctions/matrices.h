#ifndef MATRICES_H
#define MATRICES_H
#include <stdio.h>
#include <stdlib.h>


struct matrix {
    int rows;
    int cols;
    float* data;
} matrix;

/* Matrix creation functions */
struct matrix *m_new(int rows, int cols);
struct matrix *m_fill(int rows, int cols, float value);
// float*m_identity(const int rows, const int cols);

struct matrix *m_copy(struct matrix *matrix);

/* Arithmetic Functions */
struct matrix *m_transpose(struct matrix *matrix);
struct matrix *m_add(struct matrix *matrixA, struct matrix *matrixB);
struct matrix *m_sub(struct matrix *matrixA, struct matrix *matrixB);
struct matrix *m_mul(struct matrix *matrixA, struct matrix *matrixB);
struct matrix *m_scalar_mul(struct matrix *matrixA, float x);

float *m_return_row(struct matrix *matrix, int row);
float *m_return_col(struct matrix *matrix, int col);



/* Get & Set functions */
void m_set(struct matrix *matrix, int i, int j, float x);
float m_get(struct matrix *matrix, int i, int j);

/* IO and utility functions */
void *m_free(struct matrix *matrix);
void *m_fprint( struct matrix *matrix, FILE *stream);
void *m_print( struct matrix *matrix);



#endif