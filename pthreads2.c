#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "mandelbrot.h"

typedef struct {
    unsigned char *image;
    const Config *cfg;
    int thread_id;
    int num_threads;
} ThreadArgs2;

static void *worker2(void *arg) {
    ThreadArgs2 *targs = (ThreadArgs2 *)arg;
    int width = targs->cfg->width;
    int height = targs->cfg->height;

    for (int y = targs->thread_id; y < height; y += targs->num_threads) {
        for (int x = 0; x < width; x++) {
            targs->image[(size_t)y * (size_t)width + (size_t)x] = mandelbrot_pixel(x, y, targs->cfg);
        }
    }
    return NULL;
}

double run_pthreads2(unsigned char *image, const Config *cfg) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int n = cfg->num_threads;
    pthread_t *threads = malloc((size_t)n * sizeof(pthread_t));
    ThreadArgs2 *args = malloc((size_t)n * sizeof(ThreadArgs2));
    if (threads == NULL || args == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para as threads (pthreads2).\n");
        free(threads);
        free(args);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        args[i].image = image;
        args[i].cfg = cfg;
        args[i].thread_id = i;
        args[i].num_threads = n;

        if (pthread_create(&threads[i], NULL, worker2, &args[i]) != 0) {
            fprintf(stderr, "Erro: falha ao criar a thread %d (pthreads2).\n", i);
            free(threads);
            free(args);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < n; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Erro: falha ao aguardar a thread %d (pthreads2).\n", i);
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
