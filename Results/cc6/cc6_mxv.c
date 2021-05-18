int cc6_mxv_timed(char *matrix, FILE *fout, int size, int density)
{
    char *vector = calloc(GRAPH_SIZE, sizeof(char));
    if (!vector)
    {
        return 1;
    }
    *(vector) = 1;
    char d = 1;
    char iters = 0;
    char loop_size = GRAPH_SIZE - 10;
    do
    {
        struct timespec begin, end;
        double cpu_time_used;
        clock_gettime(CLOCK_MONOTONIC, &begin);
        for (int j = 0; j < GRAPH_SIZE; j++)
        {
            int i = 0;
            for (; i < loop_size - 1; i += 2)
            {
                *(vector + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
                *(vector + j) |= (*(vector + i + 1) & *(matrix + j * GRAPH_SIZE + i + 1));
            }
            if (i < loop_size)
            {
                *(vector + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
            }
            d &= *(vector + j);
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        long seconds = end.tv_sec - begin.tv_sec;
        long nseconds = end.tv_nsec - begin.tv_nsec;
        cpu_time_used = seconds + nseconds * 1e-9;
        fprintf(fout, "1,%d,%d,%d,%.10f\n", size, density, iters, cpu_time_used);
        if (d)
        {
            free(vector);
            return 0;
        }
        d = 1;
        iters++;
    } while (iters < GRAPH_SIZE);
    free(vector);
    return 1;
}