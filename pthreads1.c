#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "mandelbrot.h"

typedef struct {
    unsigned char *image;
    const Config *cfg;
    int start_row;
    int end_row;
} ThreadArgs1;

static void *worker1(void *arg) {
    ThreadArgs1 *targs = (ThreadArgs1 *)arg;
    int width = targs->cfg->width;

    for (int y = targs->start_row; y < targs->end_row; y++) {
        for (int x = 0; x < width; x++) {
            targs->image[(size_t)y * (size_t)width + (size_t)x] = mandelbrot_pixel(x, y, targs->cfg);
        }
    }
    return NULL;
}

double run_pthreads1(unsigned char *image, const Config *cfg) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int n = cfg->num_threads;
    pthread_t *threads = malloc((size_t)n * sizeof(pthread_t));
    ThreadArgs1 *args = malloc((size_t)n * sizeof(ThreadArgs1));
    if (threads == NULL || args == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para as threads (pthreads1).\n");
        free(threads);
        free(args);
        exit(EXIT_FAILURE);
    }

    int base_rows = cfg->height / n;
    int remainder = cfg->height % n;
    int next_row = 0;

    for (int i = 0; i < n; i++) {
        int rows_for_this_thread = base_rows + (i < remainder ? 1 : 0);
        args[i].image = image;
        args[i].cfg = cfg;
        args[i].start_row = next_row;
        args[i].end_row = next_row + rows_for_this_thread;
        next_row += rows_for_this_thread;

        if (pthread_create(&threads[i], NULL, worker1, &args[i]) != 0) {
            fprintf(stderr, "Erro: falha ao criar a thread %d (pthreads1).\n", i);
            free(threads);
            free(args);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < n; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Erro: falha ao aguardar a thread %d (pthreads1).\n", i);
            free(threads);
            free(args);
            exit(EXIT_FAILURE);
        }
    }

    free(threads);
    free(args);

    clock_gettime(CLOCK_MONOTONIC, &end);
    return (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
}
