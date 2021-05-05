#include <stdio.h>
#include <stdlib.h>
#include "matrices.h"
#include <string.h>
#include "vector.h"

/* Matrices.c
 * This file contains an implementation of the defined functions on matrices.h
 * This file will be used for a bachelor thesis project.
 *
 * By: Daniël Boelman
 * Date: 20-4-2021
 */

//TODO: Setup Testsuite,
// TODO: Setup Git Repo

/* - Matrix creation functions - */

/* Creates a new float based on given rows and cols size.
 * Outputs a float matrix.
 */
struct matrix *m_new(int rows, int cols) {
    struct matrix *matrix = malloc(sizeof(struct matrix));
    if (!matrix) {
        return NULL;
    }
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = malloc(rows * cols * sizeof(char));
    if (!matrix->data) {
        return NULL;
    }
    return matrix;
}

struct matrix *m_fill(int rows, int cols, char value) {
    struct matrix *matrix = m_new(rows,cols);
    memset(matrix->data, value, sizeof(matrix->data));
    return matrix;
}


/* Creates a new Matrix based on the size and values of the passed matrix.
 * Outputs a float Matrix.
 */
struct matrix *m_copy(struct matrix *matrix) {
    struct matrix *copy = m_new(matrix->rows, matrix->cols);
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            m_set(copy, i, j, m_get(matrix, i, j));
        }
    }
    return copy;
}

/* - Arithmetic Functions - */

/* Creates the transpose of the given matrix.
 * Outputs a float matrix
 */
struct matrix *m_transpose(struct matrix *matrix) {
    int rows = matrix->rows;
    int cols = matrix->cols;
    struct matrix *copy = m_new(cols, rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m_set(copy, j, i, m_get(matrix, i, j));
        }
    }
    return copy;
}

// /* Tiny secret helper */
int check_same_size(struct matrix *matrixA, struct matrix *matrixB) {
    return (matrixA->rows == matrixB->rows && matrixA->cols == matrixB->cols) ? 0 : 1;
}

struct matrix *m_add(struct matrix *matrixA, struct matrix *matrixB) {
    if (!check_same_size(matrixA, matrixB) ) {
        return NULL;
    }
    struct matrix *sum = m_new(matrixA->rows,matrixA->cols);
    for (int i = 0; i < matrixA->rows; i++) {
        for (int j = 0; j < matrixA->cols; j++) {
            m_set(sum, i, j, m_get(matrixA, i, j) + m_get(matrixB, i, j));
        }
    }
    return sum;

}


struct matrix *m_sub(struct matrix *matrixA, struct matrix *matrixB) {
    if (!check_same_size(matrixA, matrixB) ) {
        return NULL;
    }
    struct matrix *sum = m_new(matrixA->rows,matrixA->cols);
    for (int i = 0; i < matrixA->rows; i++) {
        for (int j = 0; j < matrixA->cols; j++) {
            m_set(sum, i, j, m_get(matrixA, i, j) - m_get(matrixB, i, j));
        }
    }
    return sum;

}


struct matrix *m_mul(struct matrix *matrixA, struct matrix *matrixB) {
    //Check if the matrices are compatible to do the multiplication
    if (matrixA->cols != matrixB->rows) {
        return NULL;
    }
    struct matrix *product = m_new(matrixA->rows, matrixB->cols);
    for (int i = 0; i < matrixA->rows; i++) {
        for (int j = 0; j < matrixA->cols; j++) {
            char sum = 0;
            for (int k = 0; k < matrixB->cols; k++) {
                sum += m_get(matrixA, i, k) * m_get(matrixB, k, j);
            }
            m_set(product, i, j, sum);
        }
    }
    return product;
}

char *m_return_row(struct matrix *matrix, int row) {
    if (row > matrix->rows && row < 0) {
        return NULL;
    }
    char* array = malloc(matrix->cols * sizeof(char));
    for (int j = 0; j < matrix->cols; j++) {
        *(array +j) = m_get(matrix, row, j);
    }
    return array;
}

char *m_return_col(struct matrix *matrix, int col) {
    if (col > matrix->cols && col < 0) {
        return NULL;
    }
    char* array = malloc(matrix->cols * sizeof(char));
    for (int j = 0; j < matrix->cols; j++) {
        *(array +j) =m_get(matrix, j, col);
    }
    return array;
}


/* - Get & Set functions - */

/* In the passed matrix it sets the value on position (i,j) to the value of x.
 */
void m_set(struct matrix *matrix, int i,  int j, char x) {
    *(matrix->data + i * matrix->rows + j) = x;
}

/* Returns the value in the passed matrix at (i,j).
 */
char m_get(struct matrix *matrix, int i, int j) {
    return *(matrix->data + i * matrix->rows + j);
}

/* - IO and utility functions - */

void *m_free(struct matrix *matrix) {
    free(matrix->data);
    free(matrix);
}

void *m_fprint(struct matrix *matrix, FILE *stream)
{
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->cols; j++)
        {
            fprintf(stream, "%5.2d ", m_get(matrix, i, j));
        }
        fprintf(stream, "\n");
    }
}

void *m_print(struct matrix *matrix) {
    m_fprint(matrix, stdout);
}