int cc8(unsigned int *matrix)
{
    unsigned int vector = 0;
    vector |= 1 << (GRAPH_SIZE - 1);
    char iters = 0;
    unsigned int temp = vector;
    do
    {
        vector |= (((vector & *(matrix + 0)) > 0) << 0);
        vector |= (((vector & *(matrix + 1)) > 0) << 1);
        vector |= (((vector & *(matrix + 2)) > 0) << 2);
        vector |= (((vector & *(matrix + 3)) > 0) << 3);
        vector |= (((vector & *(matrix + 4)) > 0) << 4);
        vector |= (((vector & *(matrix + 5)) > 0) << 5);
        vector |= (((vector & *(matrix + 6)) > 0) << 6);
        vector |= (((vector & *(matrix + 7)) > 0) << 7);
        vector |= (((vector & *(matrix + 8)) > 0) << 8);
        vector |= (((vector & *(matrix + 9)) > 0) << 9);
        for (int j = 10; j < (GRAPH_SIZE - 10); j++)
        {
            vector |= (((vector & *(matrix + j)) > 0) << j);
        }

        if ((vector + 1) == (1 << GRAPH_SIZE))
        {
            return 0;
        }

        if (temp == vector)
        {
            return 1;
        }

        temp = vector;
        iters++;
    } while (iters < GRAPH_SIZE);
    return 1;
}