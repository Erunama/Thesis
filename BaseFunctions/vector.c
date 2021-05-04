#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include <string.h>

#define arraySize(x) (sizeof(x) / sizeof((x)[0]))


struct vector *v_new(int size) {
    struct vector *vector = malloc(sizeof(struct vector));

    if(!vector) {
        /* Throw Error */
        return NULL;
    }
    vector->size = size;
    vector->data = malloc(size * sizeof(float));
    if(!vector->data) {
        /* Throw Error */
        return NULL;
    }
    return vector;
}
struct vector *v_fill(int size, float value) {
    struct vector *vector = v_new(size);
    for(int i = 0; i < size; i++) {
        v_set(vector, i, value);
    }
    return vector;
}
struct vector *v_from_array(int size, float *array) {
    struct vector *vector = malloc(sizeof(struct vector));
    if(!vector) {
        /* Throw Error */
        return NULL;
    }
    vector->size = size;
    vector->data = array;
    return vector;
}

struct vector *v_copy(struct vector *vector) {
    struct vector *copy = v_new(vector->size);
    for (int i = 0; i < vector->size; i++) {
        v_set(copy,i, v_get(vector, i));
    }
    return copy;
}
int equalLength(struct vector *vectorA, struct vector *vectorB) {
    return (vectorA->size - vectorB->size);
}

void v_add(struct vector *vectorA, struct vector *vectorB) {
    if (vectorA->size - vectorB->size != 0) {
        return;
    }
    for (int i = 0; i < vectorA->size; i++) {
        v_set(vectorA, i, (v_get(vectorA, i) + v_get(vectorB, i)));
    }
    return;
}
void v_mod(struct vector *vector, int value) {
    for (int i = 0; i < vector->size; i++) {
        v_set(vector, i, ((int)v_get(vector, i) % value));
    }
}
void v_capped(struct vector *vector) {
    for (int i = 0; i < vector->size; i++) {
        v_set(vector, i, v_get(vector, i) != 0.0 ? (v_get(vector, i) / v_get(vector, i)) : 0.0);
    }
}
float v_rank(struct vector *vector) {
    float sum = 0;
    for (int i = 0; i < vector->size; i++) {
        sum += v_get(vector, i);
    }
    return sum;
}
void v_set(struct vector *vector, int i, float value){
    *(vector->data + i) = value;
}
float v_get(struct vector *vector, int i) {
    return *(vector->data + i);
}
// float *v_sub(const float *vectorA, const float *vectorB);
// float *v_mul(const float *vectorA, const float *vectorB);
// float *v_scalar_mul(const float *vectorA, const float x);


// void *v_free(matrix_t *matrix);
// void *v_print(const matrix_t *matrix);