/* Basic CC algorithm based on bfs */
int cc3(struct matrix *matrix) {
    char *vector = calloc(matrix->cols, sizeof(char));
    char *result = calloc(matrix->cols, sizeof(char));
    if(!result || !vector) {
        return 1;
    }
    *(vector) = 1;
    char d = 0;
    char iters = 0;
    do {
        for (int j = 0; j < matrix->rows; j++) {
            for (int i = 0; i < matrix->cols; i++) {
                *(result + j) += (*(vector + i) * *(matrix->data + j * matrix->rows + i));
            }
            d += ((*(vector + j) + *(result + j)) != 0 ? 1 : 0);
        }
        if (d == matrix->cols) {
            free(vector);
            free(result);
            return 0;
        }

        for (int i = 0; i < matrix->cols; i++) {
            *(vector + i) = (*(vector + i) + *(result + i)) != 0 ? 1 : 0;
            *(result + i) = 0;
        }
        d = 0;
        iters++;
    }
    while (iters < matrix->rows);
    free(vector);
    free(result);
    return 1;
}