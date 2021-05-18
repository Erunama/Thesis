int cc3_mxv_timed(struct matrix *matrix, FILE *fout, int size, int density)
{
    char *vector = calloc(matrix->cols, sizeof(char));
    char *result = calloc(matrix->cols, sizeof(char));
    if (!result || !vector)
    {
        return 1;
    }
    *(vector) = 1;
    char d = 0;
    char iters = 0;
    do
    {
        struct timespec begin, end;
        double cpu_time_used;
        clock_gettime(CLOCK_MONOTONIC, &begin);
        for (int j = 0; j < matrix->rows; j++)
        {

            for (int i = 0; i < matrix->cols; i++)
            {
                *(result + j) += (*(vector + i) * *(matrix->data + j * matrix->rows + i));
            }

            d += ((*(vector + j) + *(result + j)) != 0 ? 1 : 0);
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        long seconds = end.tv_sec - begin.tv_sec;
        long nseconds = end.tv_nsec - begin.tv_nsec;
        cpu_time_used = seconds + nseconds * 1e-9;
        fprintf(fout, "1,%d,%d,%d,%.10f\n", size, density, i, cpu_time_used);

        if (d == matrix->cols)
        {
            free(vector);
            free(result);
            return 0;
        }

        for (int i = 0; i < matrix->cols; i++)
        {
            *(vector + i) = (*(vector + i) + *(result + i)) != 0 ? 1 : 0;
            *(result + i) = 0;
        }
        d = 0;
        iters++;
    } while (iters < matrix->rows);
    free(vector);
    free(result);
    return 1;
}