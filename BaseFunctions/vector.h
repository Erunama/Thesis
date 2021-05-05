#ifndef VECTOR_H
#define VECTOR_H
#include <stdio.h>
#include <stdlib.h>
struct vector {
    int size;
    char* data;
} vector;
/* FLOAT ARRAYS OR SOMETHING. STILL WANNA DEFINE SOME FUNCTIONS */

struct vector *v_new( int size);
struct vector *v_fill( int size, char value);
struct vector *v_from_array(int size, char *array);
struct vector *v_copy(struct vector *vector);

// matrix_t *vec_transpose(const matrix_t *matrix);
void v_add(struct vector *vectorA, struct vector *vectorB);
void v_mod(struct vector *vector, int value);
void v_capped(struct vector *vector);
char v_rank(struct vector *vector);

struct vector *v_mul_array(struct vector *vector, char *array);
struct vector *v_sub(const struct vector *vectorA, const struct vector *vectorB);
struct vector *v_mul(const struct vector *vectorA, const struct vector *vectorB);
struct vector *v_scalar_mul(const struct vector *vectorA, const char x);

/* Get & Set functions IDK if needed at all */
void v_set(struct vector *vector, int i, char value);
char v_get(struct vector *vector, int i);

void v_free(struct vector *vector);

/* Matrix functions ? */

#endif