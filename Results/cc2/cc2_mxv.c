/* Basic CC algorithm based on bfs */
int cc2_mxv_timed(struct matrix *matrix, FILE *fout, int size, int density)
{
    struct vector *vector = v_fill(matrix->cols, 0);
    *(vector->data) = 1;
    char d = 0;
    for (int i = 0; i < matrix->rows; i++)
    {
        struct timespec begin, end;
        double cpu_time_used;
        clock_gettime(CLOCK_MONOTONIC, &begin);

        struct vector *res = mul_matrix_vector_optimized(matrix, vector);
        clock_gettime(CLOCK_MONOTONIC, &end);

        long seconds = end.tv_sec - begin.tv_sec;
        long nseconds = end.tv_nsec - begin.tv_nsec;
        cpu_time_used = seconds + nseconds * 1e-9;
        fprintf(fout, "1,%d,%d,%d,%.10f\n", size, density, i, cpu_time_used);

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