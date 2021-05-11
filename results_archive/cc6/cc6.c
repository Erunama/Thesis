int cc6(char *matrix) {
    char *vector = calloc(GRAPH_SIZE, sizeof(char));
    if(!vector) {
        return 1;
    }
    *(vector) = 1;
    char d = 1;
    char iters = 0;
    char loop_size = GRAPH_SIZE - 10;
    do {
        for (int j = 0; j < GRAPH_SIZE; j++) {
            int i = 0;
            for (; i < loop_size - 1; i+=2) {
                *(vector + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
                *(vector + j) |= (*(vector + i + 1) & *(matrix + j * GRAPH_SIZE + i + 1));
            }
            if (i < loop_size) {
                *(vector + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
            }
            d &= *(vector + j);
        }
        if (d) {
            free(vector);
            return 0;
        }
        d = 1;
        iters++;
    }
    while (iters < GRAPH_SIZE);
    free(vector);
    return 1;
}