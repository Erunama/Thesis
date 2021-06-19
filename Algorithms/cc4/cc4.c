int cc4(char *matrix) {
    char *vector = calloc(GRAPH_SIZE, sizeof(char));
    char *result = calloc(GRAPH_SIZE, sizeof(char));
    if(!result || !vector) {
        return 1;
    }
    *(vector) = 1;
    char d = 1;
    char iters = 0;
    do {
        for (int j = 0; j < GRAPH_SIZE; j++) {
            for (int i = 0; i < GRAPH_SIZE; i++) {
                *(result + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
            }
            d &= (*(vector + j) | *(result + j));
        }
        if (d) {
            free(vector);
            free(result);
            return 0;
        }
        for (int i = 0; i < GRAPH_SIZE; i++) {
            *(vector + i) = (*(vector + i) | *(result + i));
            *(result + i) = 0;
        }
        d = 1;
        iters++;
    }
    while (iters < GRAPH_SIZE);
    free(vector);
    free(result);
    return 1;
}
