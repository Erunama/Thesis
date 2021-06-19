int cc(struct matrix *matrix)
{
    struct vector *vector = v_fill(matrix->cols, 0);
    v_set(vector, 0, 1);
    for (int i = 0; i < matrix->rows; i++)
    {
        struct vector *res = mul_matrix_vector(matrix, vector);
        if (vector->size - res->size != 0)
        {
            return 1;
        }
        for (int i = 0; i < vector->size; i++)
        {
            *(vector->data + i) += *(res->data + i);
        }
        v_free(res);
        for (int i = 0; i < vector->size; i++)
        {
            char c = *(vector->data + i);
            *(vector->data + i) = c != 0 ? (c / c) : 0;
        }
        char sum = 0;
        for (int i = 0; i < vector->size; i++)
        {
            sum += *(vector->data + i);
        }

        if (sum == vector->size)
        {
            v_free(vector);
            return 0;
        }
    }
    v_free(vector);
    return 1;
}
