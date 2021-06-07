#include "matrices.h"
#include <immintrin.h>
#include <smmintrin.h>
#include "vector.h"
#include <time.h>
#include <string.h>
#define NUM_GRAPHS 10
#define MIN_GRAPH_SIZE 10
#define MAX_GRAPH_SIZE 32

// TODO: Freeze Data
// TODO: Freeze version
void print_bit(unsigned int number)
{
    for (int i = 0; i < 32; ++i)
    {
        if (number >> i & 0x1)
            putchar('1');
        else
            putchar('0');
    }

    putchar('\n');
}

int GRAPH_SIZE = 0;
/* multiply a matrix and vector and return a vector */
struct vector *mul_matrix_vector(struct matrix *matrix, struct vector *vector)
{
    if (matrix->cols != vector->size)
    {
        return NULL;
    }
    int rowz = matrix->rows;
    if (!rowz)
    {
        return NULL;
    }
    struct vector *result = v_new(rowz);
    for (int j = 0; j < rowz; j++)
    {
        char *row = m_return_row(matrix, j);
        for (int i = 0; i < vector->size; i++)
        {
            v_set(result, j, (v_get(vector, i) * *(row + i) + v_get(result, j)));
        }
        free(row);
    }
    return result;
}

/* Basic CC algorithm based on bfs */
int cc1(struct matrix *matrix)
{
    struct vector *vector = v_fill(matrix->cols, 0);
    v_set(vector, 0, 1);
    for (int i = 0; i < matrix->rows; i++)
    {
        struct vector *res = mul_matrix_vector(matrix, vector);
        v_add(vector, res);
        v_free(res);
        v_capped(vector);
        if (v_rank(vector) == vector->size)
        {
            v_free(vector);
            return 0;
        }
    }
    v_free(vector);
    return 1;
}

/* Basic CC algorithm based on bfs */
double cc1_mxv_timed(struct matrix *matrix)
{
    double total_time = 0.0;
    struct vector *vector = v_fill(matrix->cols, 0);
    v_set(vector, 0, 1);
    int i = 0;
    for (; i < matrix->rows; i++)
    {
        // RUN TIMERS
        struct timespec begin, end;
        double cpu_time_used;
        clock_gettime(CLOCK_MONOTONIC, &begin);
        // FUNCTION CALL
        struct vector *res = mul_matrix_vector(matrix, vector);
        // CALL END
        clock_gettime(CLOCK_MONOTONIC, &end);
        long seconds = end.tv_sec - begin.tv_sec;
        long nseconds = end.tv_nsec - begin.tv_nsec;
        cpu_time_used = seconds + nseconds * 1e-9;
        total_time += cpu_time_used;
        // END TIMERS
        // RETURN VALUE?

        v_add(vector, res);
        v_free(res);
        v_capped(vector);
        if (v_rank(vector) == vector->size)
        {
            v_free(vector);
            return i > 0 ? total_time / i : total_time;
        }
    }
    v_free(vector);
    return i > 0 ? total_time / i : total_time;
}

struct vector *mul_matrix_vector_optimized(struct matrix *matrix, struct vector *vector)
{
    if (matrix->cols != vector->size)
    {
        return NULL;
    }
    int rowz = matrix->rows;
    if (!rowz)
    {
        return NULL;
    }
    struct vector *result = v_fill(rowz, 0);
    for (int j = 0; j < rowz; j++)
    {
        for (int i = 0; i < vector->size; i++)
        {
            *(result->data + j) += (*(vector->data + i) * *(matrix->data + j * rowz + i));
        }
    }
    return result;
}

/* Basic CC algorithm based on bfs */
int cc2(struct matrix *matrix)
{
    struct vector *vector = v_fill(matrix->cols, 0);
    *(vector->data) = 1;
    char d = 0;
    for (int i = 0; i < matrix->rows; i++)
    {
        struct vector *res = mul_matrix_vector_optimized(matrix, vector);
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
            free(res->data);
            free(res);
            return 0;
        }
        free(res->data);
        free(res);
        d = 0;
    }
    free(vector->data);
    free(vector);
    return 1;
}

/* Basic CC algorithm based on bfs */
double cc2_mxv_timed(struct matrix *matrix)
{
    struct vector *vector = v_fill(matrix->cols, 0);
    *(vector->data) = 1;
    char d = 0;
    double total_time = 0.0;
    int i = 0;
    for (; i < matrix->rows; i++)
    {
        struct timespec begin, end;
        double cpu_time_used;
        clock_gettime(CLOCK_MONOTONIC, &begin);

        struct vector *res = mul_matrix_vector_optimized(matrix, vector);
        clock_gettime(CLOCK_MONOTONIC, &end);

        long seconds = end.tv_sec - begin.tv_sec;
        long nseconds = end.tv_nsec - begin.tv_nsec;
        cpu_time_used = seconds + nseconds * 1e-9;
        total_time += cpu_time_used;

        for (int j = 0; j < vector->size; j++)
        {
            *(vector->data + j) += *(res->data + j);
            *(vector->data + j) = *(vector->data + j) != 0 ? (*(vector->data + j) / *(vector->data + j)) : 0;
            d += *(vector->data + j);
        }
        if (d == vector->size)
        {
            free(vector->data);
            free(vector);
            v_free(res);
            return i > 0 ? total_time / i : total_time;
        }
        v_free(res);
        d = 0;
    }
    free(vector->data);
    free(vector);
    return i > 0 ? total_time / i : total_time;
}

/* Basic CC algorithm based on bfs */
int cc3(struct matrix *matrix)
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
        for (int j = 0; j < matrix->rows; j++)
        {
            for (int i = 0; i < matrix->cols; i++)
            {
                *(result + j) += (*(vector + i) * *(matrix->data + j * matrix->rows + i));
            }
            d += ((*(vector + j) + *(result + j)) != 0 ? 1 : 0);
        }
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

/* Basic CC algorithm based on bfs */
double cc3_mxv_timed(struct matrix *matrix)
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
    double total_time = 0.0;
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
        // fprintf(fout, "1,%d,%d,%d,%.10f\n", size, density, iters, cpu_time_used);
        total_time += cpu_time_used;

        if (d == matrix->cols)
        {
            free(vector);
            free(result);
            return iters > 0 ? total_time / iters : total_time;
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
    return iters > 0 ? total_time / iters : total_time;
}

int cc4(char *matrix)
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
        for (int j = 0; j < GRAPH_SIZE; j++)
        {
            for (int i = 0; i < GRAPH_SIZE; i++)
            {
                *(result + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
            }
            d &= (*(vector + j) | *(result + j));
        }
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

double cc4_mxv_timed(char *matrix)
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
    double total_time = 0.0;
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
        total_time += cpu_time_used;
        if (d)
        {
            free(vector);
            free(result);
            return iters > 0 ? total_time / iters : total_time;
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
    return iters > 0 ? total_time / iters : total_time;
}

/* 6 iteration of my CCa
 * This one is the most abstract: uses only 2 allocated data structures based on the graph size
 */
int cc5(char *matrix)
{
    char *vector = calloc(GRAPH_SIZE, sizeof(char));
    if (!vector)
    {
        return 1;
    }
    *(vector) = 1;
    char d = 1;
    char iters = 0;
    do
    {
        for (int j = 0; j < GRAPH_SIZE; j++)
        {
            for (int i = 0; i < GRAPH_SIZE; i++)
            {
                *(vector + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
            }
            d &= *(vector + j);
        }
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

double cc5_mxv_timed(char *matrix)
{
    char *vector = calloc(GRAPH_SIZE, sizeof(char));
    if (!vector)
    {
        return 1;
    }
    *(vector) = 1;
    char d = 1;
    char iters = 0;
    double total_time = 0.0;
    do
    {
        struct timespec begin, end;
        double cpu_time_used;
        clock_gettime(CLOCK_MONOTONIC, &begin);
        for (int j = 0; j < GRAPH_SIZE; j++)
        {
            for (int i = 0; i < GRAPH_SIZE; i++)
            {
                *(vector + j) |= (*(vector + i) & *(matrix + j * GRAPH_SIZE + i));
            }
            d &= *(vector + j);
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        long seconds = end.tv_sec - begin.tv_sec;
        long nseconds = end.tv_nsec - begin.tv_nsec;
        cpu_time_used = seconds + nseconds * 1e-9;
        total_time += cpu_time_used;
        if (d)
        {
            free(vector);
            return iters > 0 ? total_time / iters : total_time;
        }
        d = 1;
        iters++;
    } while (iters < GRAPH_SIZE);
    free(vector);
    return iters > 0 ? total_time / iters : total_time;
}

int cc6(char *matrix)
{
    char *vector = calloc(GRAPH_SIZE, sizeof(char));
    if (!vector)
    {
        return 1;
    }
    *(vector) = 1;
    char d = 1;
    char iters = 0;
    char loop_size = GRAPH_SIZE;
    do
    {
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

double cc6_mxv_timed(char *matrix)
{
    char *vector = calloc(GRAPH_SIZE, sizeof(char));
    if (!vector)
    {
        return 1;
    }
    *(vector) = 1;
    char d = 1;
    char iters = 0;
    char loop_size = GRAPH_SIZE;
    double total_time = 0.0;
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
        total_time += cpu_time_used;
        if (d)
        {
            free(vector);
            return iters > 0 ? total_time / iters : total_time;
        }
        d = 1;
        iters++;
    } while (iters < GRAPH_SIZE);
    free(vector);
    return iters > 0 ? total_time / iters : total_time;
}
/* BEST CASE CURRENTLY */
int cc7(char *matrix)
{
    char *vector = calloc(GRAPH_SIZE, sizeof(char));
    if (!vector)
    {
        return 1;
    }
    *(vector) = 1;
    char d = 1;
    char iters = 0;
    char loop_size = GRAPH_SIZE;
    do
    {
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

/* BEST CASE CURRENTLY */
double cc7_mxv_timed(char *matrix)
{
    char *vector = calloc(GRAPH_SIZE, sizeof(char));
    if (!vector)
    {
        return 1;
    }
    *(vector) = 1;
    char d = 1;
    char iters = 0;
    char loop_size = GRAPH_SIZE;
    double total_time = 0.0;
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
        total_time += cpu_time_used;
        // fprintf(fout, "1,%d,%d,%d,%.10f\n", size, density, iters, cpu_time_used);
        if (d)
        {
            free(vector);
            return iters > 0 ? total_time / iters : total_time;
        }
        d = 1;
        iters++;
    } while (iters < GRAPH_SIZE);
    free(vector);
    return iters > 0 ? total_time / iters : total_time;
}

int cc8(unsigned int *matrix)
{
    unsigned int vector = 0;
    vector |= 1;
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
        for (int j = 10; j < (GRAPH_SIZE); j++)
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

int cc8_timed(unsigned int *matrix)
{
    unsigned int vector = 0;
    vector |= 1 << (GRAPH_SIZE - 1);
    char iters = 0;
    unsigned int temp = vector;
    double total_time = 0.0;
    do
    {
        struct timespec begin, end;
        double cpu_time_used;
        clock_gettime(CLOCK_MONOTONIC, &begin);
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
        clock_gettime(CLOCK_MONOTONIC, &end);

        long seconds = end.tv_sec - begin.tv_sec;
        long nseconds = end.tv_nsec - begin.tv_nsec;
        cpu_time_used = seconds + nseconds * 1e-9;
        total_time += cpu_time_used;

        printf("1,%d,%.10f\n", iters, cpu_time_used);

        if ((vector + 1) == (1 << GRAPH_SIZE))
        {
            return iters > 0 ? total_time / iters : total_time;
        }

        if (temp == vector)
        {
            return iters > 0 ? total_time / iters : total_time;
        }

        temp = vector;
        iters++;
    } while (iters < GRAPH_SIZE);
    return iters > 0 ? total_time / iters : total_time;
}

int cc9(unsigned int *matrix)
{
    unsigned int vector = *(matrix + 0);
    // vector |= 1 << (GRAPH_SIZE - 1);
    vector |= *(matrix + 1);
    vector |= *(matrix + 2);
    vector |= *(matrix + 3);
    vector |= *(matrix + 4);
    vector |= *(matrix + 5);
    vector |= *(matrix + 6);
    vector |= *(matrix + 7);
    vector |= *(matrix + 8);
    vector |= *(matrix + 9);
    for (int j = 10; j < GRAPH_SIZE; j++)
    {
        vector |= *(matrix + j);
    }
    if ((vector + 1) == (1 << GRAPH_SIZE))
    {
        return 0;
    }
    return 1;
}

/*
unsigned int hsum_epi32_avx(__m128i x)
{
    __m128i hi64 = _mm_unpackhi_epi64(x, x); // 3-operand non-destructive AVX lets us save a byte without needing a movdqa
    __m128i sum64 = _mm_add_epi32(hi64, x);
    __m128i hi32 = _mm_shuffle_epi32(sum64, _MM_SHUFFLE(2, 3, 0, 1)); // Swap the low two elements
    __m128i sum32 = _mm_add_epi32(sum64, hi32);
    return _mm_cvtsi128_si32(sum32); // movd
}

// only needs AVX2
unsigned int hsum_8x32(__m256i v)
{
    __m128i sum128 = _mm_add_epi32(
        _mm256_castsi256_si128(v),
        _mm256_extracti128_si256(v, 1)); // silly GCC uses a longer AXV512VL instruction if AVX512 is enabled :/
    return hsum_epi32_avx(sum128);
}

int cc11(unsigned int *matrix)
{

    int matrixt[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int vectort = 3;
    int n_values = 256 / (8 * sizeof(int));
    // Load matrix into SIMD datatype
    __m256i simd_matrix = _mm256_loadu_si256((__m256i *)matrixt);
    // Load vector as a constant into the SIMD datatype

    // Multiply matrix and vector (mullo because we dont care about overflow)
    // _mm256_mul_epi32 would also multiply, but it would use 64 bits for the result.

    _mm256_storeu_si256((__m256i *)matrixt, simd_result);
    for (int i = 0; i < n_values; i++)
    {
        printf("%d\n", matrixt[i]);
    }
    __m256i simd_vector = _mm256_set1_epi32(vectort);
    for (int i = 0; i < GRAPH_SIZE - 1; i += 8)
    {
        __m256i simd_matrix = _mm256_loadu_si256((__m256i *)matrix + i);
        __m256i simd_result = _mm256_mullo_epi32(simd_matrix, simd_vector);
    }
    // Store result back into matrix.
    unsigned int tester = hsum_8x32(simd_result);

    printf("%u\n", tester);

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
*/
struct matrix *readGraph(char *filename)
{
    FILE *fp = fopen(filename, "r");
    int src;
    int dest;
    GRAPH_SIZE = 0;
    fscanf(fp, "%d", &GRAPH_SIZE);
    // printf("Gs: %d\n", GRAPH_SIZE);
    struct matrix *am = m_fill(GRAPH_SIZE, GRAPH_SIZE, 0);
    while (fscanf(fp, "%d %d", &src, &dest) != EOF)
    {
        m_set(am, src - 1, dest - 1, 1);
        m_set(am, dest - 1, src - 1, 1);
    }
    fclose(fp);
    return am;
}

char *readGraph2(char *filename)
{
    FILE *fp = fopen(filename, "r");
    int src;
    int dest;

    fscanf(fp, "%d", &GRAPH_SIZE);
    // printf("Gs: %d\n", GRAPH_SIZE);
    char *matrix = calloc(GRAPH_SIZE * GRAPH_SIZE, sizeof(char));
    while (fscanf(fp, "%d %d", &src, &dest) != EOF)
    {
        *(matrix + (src - 1) * GRAPH_SIZE + (dest - 1)) = 1;
        *(matrix + (dest - 1) * GRAPH_SIZE + (src - 1)) = 1;
    }
    fclose(fp);
    return matrix;
}
unsigned int *readGraph3(char *filename)
{
    FILE *fp = fopen(filename, "r");
    int src;
    int dest;

    fscanf(fp, "%d", &GRAPH_SIZE);
    // printf("Gs: %d\n", GRAPH_SIZE);
    unsigned int *matrix = calloc(GRAPH_SIZE, sizeof(unsigned int));
    while (fscanf(fp, "%d %d", &src, &dest) != EOF)
    {
        *(matrix + (src - 1)) |= 1 << (dest - 1);
        // printf("%u\n", 1 << (dest));
        // printf("%d %d\n", src, (GRAPH_SIZE - dest));
        *(matrix + (dest - 1)) |= 1 << (src - 1);
    }
    // printf("-----%u\n", *(matrix));

    fclose(fp);
    return matrix;
}

unsigned int *readGraph4(char *filename)
{
    FILE *fp = fopen(filename, "r");
    int src;
    int dest;

    fscanf(fp, "%d", &GRAPH_SIZE);
    // printf("Gs: %d\n", GRAPH_SIZE);
    unsigned int *matrix = calloc(32, sizeof(unsigned int));
    while (fscanf(fp, "%d %d", &src, &dest) != EOF)
    {
        *(matrix + (src - 1)) |= 1 << (dest - 1);
        *(matrix + (dest - 1)) |= 1 << (src - 1);
    }
    fclose(fp);
    return matrix;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    int iterations = 1000000;
    double cca = 0;
    int cca_num = 8;

    char outname[256];

    sprintf(outname, "./results/results_new_cc%d.csv", cca_num);
    FILE *fout = fopen(outname, "w");
    // fprintf(fout, "cca,size,density,iteration,mxv_time\n");
    fprintf(fout, "cca,size,density,total_time\n");

    for (int size = MIN_GRAPH_SIZE; size < MAX_GRAPH_SIZE + 1; size++)
    {
        double edge_density = 0.1;
        for (int j = 0; j < 2; j++)
        {
            for (int inputfile = 0; inputfile < 10; inputfile++)
            {
                int density = (int)(edge_density * 100);
                double total_time = 0.0;
                char filename[256];
                sprintf(filename, "./input/%dsize_%ddens_%d.txt", size, density, inputfile);

                // struct matrix *am = readGraph(filename);
                // char *am = readGraph2(filename);
                int *am = readGraph3(filename);
                // for (int i = 0; i < GRAPH_SIZE; i++)
                // {
                //     print_bit(*(am + (i)));
                // }
                // printf("--\n");
                cca = cc8(am);
                for (int i = 0; i < iterations; i++)
                {
                    // printf("--\n");
                    struct timespec begin, end;
                    double cpu_time_used;
                    clock_gettime(CLOCK_MONOTONIC, &begin);
                    // cca = cc11(am);
                    cca = cc8(am);
                    // printf("%f\n", cca);
                    clock_gettime(CLOCK_MONOTONIC, &end);
                    long seconds = end.tv_sec - begin.tv_sec;
                    long nseconds = end.tv_nsec - begin.tv_nsec;
                    cpu_time_used = seconds + nseconds * 1e-9;
                    total_time = total_time + cpu_time_used;
                }
                // m_free(am);
                free(am);
                // fprintf(fout, "Input file: %s\n", filename);
                // fprintf(fout, "Graph size: %d\nEdge density: %d\n", size, density);
                fprintf(fout, "%d,%d,%d,%.10f\n", cca_num, size, density, total_time);
                // fprintf(fout, "%d,%d,%d,%.10f\n", cca_num, size, density, cca);
                // printf("%d,%d,%d,%.10f\n", cca_num, size, density, cca);
                // fprintf(fout, "Average time per iteration: %.10f\n", total_time / iterations);
                // if (!cca) {
                //     fprintf(fout, "Graph is connected!\n");
                // }
                // else {
                //     fprintf(fout, "Graph is not connected!\n");
                // }
            }
            edge_density = edge_density + 0.1;
        }
    }
    fclose(fout);
}