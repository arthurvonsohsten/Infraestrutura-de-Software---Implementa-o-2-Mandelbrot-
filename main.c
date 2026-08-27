#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "mandelbrot.h"

static int parse_positive_int(const char *str, const char *name) {
    char *endptr;
    errno = 0;
    long value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        fprintf(stderr, "Erro: '%s' invalido para %s. Deve ser um numero inteiro.\n", str, name);
        exit(EXIT_FAILURE);
    }
    if (errno == ERANGE || value > INT_MAX || value < INT_MIN) {
        fprintf(stderr, "Erro: valor de %s fora do intervalo permitido.\n", name);
        exit(EXIT_FAILURE);
    }
    if (value <= 0) {
        fprintf(stderr, "Erro: %s deve ser um inteiro positivo (recebido: %ld).\n", name, value);
        exit(EXIT_FAILURE);
    }
    return (int)value;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s largura altura max_iteracoes num_threads\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Config cfg;
    cfg.width = parse_positive_int(argv[1], "largura");
    cfg.height = parse_positive_int(argv[2], "altura");
    cfg.max_iter = parse_positive_int(argv[3], "max_iteracoes");
    cfg.num_threads = parse_positive_int(argv[4], "num_threads");

    unsigned char *image = alloc_image(&cfg);

    double t_serial = run_serial(image, &cfg);
    write_pgm("mandelbrot_alvs_serial.pgm", image, &cfg);

    double t_openmp = run_openmp(image, &cfg);
    write_pgm("mandelbrot_alvs_openmp.pgm", image, &cfg);

    double t_pthreads1 = run_pthreads1(image, &cfg);
    write_pgm("mandelbrot_alvs_pthreads1.pgm", image, &cfg);

    double t_pthreads2 = run_pthreads2(image, &cfg);
    write_pgm("mandelbrot_alvs_pthreads2.pgm", image, &cfg);

    free(image);

    FILE *times_fp = fopen("times.txt", "w");
    if (times_fp == NULL) {
        fprintf(stderr, "Erro: nao foi possivel criar o arquivo times.txt.\n");
        exit(EXIT_FAILURE);
    }

    if (fprintf(times_fp, "Serial: %.6f s\n", t_serial) < 0 ||
        fprintf(times_fp, "OpenMP: %.6f s\n", t_openmp) < 0 ||
        fprintf(times_fp, "Pthreads1: %.6f s\n", t_pthreads1) < 0 ||
        fprintf(times_fp, "Pthreads2: %.6f s\n", t_pthreads2) < 0) {
        fprintf(stderr, "Erro: falha ao escrever em times.txt.\n");
        fclose(times_fp);
        exit(EXIT_FAILURE);
    }

    if (fclose(times_fp) != 0) {
        fprintf(stderr, "Erro: falha ao fechar times.txt.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
