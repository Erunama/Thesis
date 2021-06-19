int cc7_mxv_timed(char *matrix, FILE *fout, int size, int density)
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
            /* First ten entries because min graph size is 10 */
            *(vector + j) |= (*(vector) & *(matrix + j * GRAPH_SIZE));
            *(vector + j) |= (*(vector + 1) & *(matrix + j * GRAPH_SIZE + 1));
            *(vector + j) |= (*(vector + 2) & *(matrix + j * GRAPH_SIZE + 2));
            *(vector + j) |= (*(vector + 3) & *(matrix + j * GRAPH_SIZE + 3));
            *(vector + j) |= (*(vector + 4) & *(matrix + j * GRAPH_SIZE + 4));
            *(vector + j) |= (*(vector + 5) & *(matrix + j * GRAPH_SIZE + 5));
            *(vector + j) |= (*(vector + 6) & *(matrix + j * GRAPH_SIZE + 6));
            *(vector + j) |= (*(vector + 7) & *(matrix + j * GRAPH_SIZE + 7));
            *(vector + j) |= (*(vector + 8) & *(matrix + j * GRAPH_SIZE + 8));
            *(vector + j) |= (*(vector + 9) & *(matrix + j * GRAPH_SIZE + 9));

            int i = 10;
            for (; i < loop_size - 1; i += 3)
            {
                *(vector + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
                *(vector + j) |= (*(vector + i + 1) & *(matrix + j * GRAPH_SIZE + i + 1));
                *(vector + j) |= (*(vector + i + 2) & *(matrix + j * GRAPH_SIZE + i + 2));
            }
            for (; i < loop_size; i++)
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