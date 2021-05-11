struct matrix *readGraph(char* filename) {
    FILE* fp = fopen(filename, "r");
    int src;
    int dest;
    GRAPH_SIZE = 0;
    fscanf(fp, "%d", &GRAPH_SIZE);
    // printf("Gs: %d\n", GRAPH_SIZE);
    struct matrix *am = m_fill(GRAPH_SIZE, GRAPH_SIZE, 0);
    while (fscanf(fp, "%d %d", &src, &dest) != EOF) {
        m_set(am, src-1, dest-1, 1);
        m_set(am, dest-1, src-1, 1);
    }
    fclose(fp);
    return am;
}

char *readGraph2(char* filename) {
    FILE* fp = fopen(filename, "r");
    int src;
    int dest;

    fscanf(fp, "%d", &GRAPH_SIZE);
    // printf("Gs: %d\n", GRAPH_SIZE);
    char *matrix = calloc(GRAPH_SIZE * GRAPH_SIZE, sizeof(char));
    while (fscanf(fp, "%d %d", &src, &dest) != EOF) {
        *(matrix + (src-1) * GRAPH_SIZE + (dest-1)) = 1;
        *(matrix + (dest-1) * GRAPH_SIZE + (src-1)) = 1;
    }
    fclose(fp);
    return matrix;
}