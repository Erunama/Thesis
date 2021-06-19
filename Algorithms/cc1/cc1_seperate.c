int cc(struct matrix *matrix)
{
    struct vector *vector = v_fill(matrix->cols, 0);
    v_set(vector, 0, 1);
    for (int i = 0; i < matrix->rows; i++)
    {
        struct vector *res = mul_matrix_vector(matrix, vector);
        v_add(vector, res);
        v_free(res);
        v_capped(vector);
        if (v_rank(vector) == vector->size)
        {
            v_free(vector);
            return 0;
        }
    }
    v_free(vector);
    return 1;
}
