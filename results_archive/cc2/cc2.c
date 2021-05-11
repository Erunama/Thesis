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