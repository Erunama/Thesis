int cc4_mxv_timed(char *matrix, FILE *fout, int size, int density)
{
    char *vector = calloc(GRAPH_SIZE, sizeof(char));
    char *result = calloc(GRAPH_SIZE, sizeof(char));
    if (!result || !vector)
    {
        return 1;
    }
    *(vector) = 1;
    char d = 1;
    char iters = 0;
    do
    {
        struct timespec begin, end;
        double cpu_time_used;
        clock_gettime(CLOCK_MONOTONIC, &begin);
        for (int j = 0; j < GRAPH_SIZE; j++)
        {
            for (int i = 0; i < GRAPH_SIZE; i++)
            {
                *(result + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
            }
            d &= (*(vector + j) | *(result + j));
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        long seconds = end.tv_sec - begin.tv_sec;
        long nseconds = end.tv_nsec - begin.tv_nsec;
        cpu_time_used = seconds + nseconds * 1e-9;
        fprintf(fout, "1,%d,%d,%d,%.10f\n", size, density, i, cpu_time_used);
        if (d)
        {
            free(vector);
            free(result);
            return 0;
        }
        for (int i = 0; i < GRAPH_SIZE; i++)
        {
            *(vector + i) = (*(vector + i) | *(result + i));
            *(result + i) = 0;
        }
        d = 1;
        iters++;
    } while (iters < GRAPH_SIZE);
    free(vector);
    free(result);
    return 1;
}