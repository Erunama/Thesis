/* CC2 the first optimization of the reference */
int cc2(struct matrix *matrix)
{
    struct vector *vector = v_fill(matrix->cols, 0);
    *(vector->data) = 1;
    char d = 0;
    for (int i = 0; i < matrix->rows; i++)
    {
        struct vector *res = mul_matrix_vector_optimized(matrix, vector);
        for (int i = 0; i < vector->size; i++)
        {
            *(vector->data + i) += *(res->data + i);
            *(vector->data + i) = *(vector->data + i) != 0 ? (*(vector->data + i) / *(vector->data + i)) : 0;
            d += *(vector->data + i);
        }
        if (d == vector->size)
        {
            free(vector->data);
            free(vector);
            free(res->data);
            free(res);
            return 0;
        }
        free(res->data);
        free(res);
        d = 0;
    }
    free(vector->data);
    free(vector);
    return 1;
}