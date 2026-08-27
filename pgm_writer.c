#include <stdio.h>
#include <stdlib.h>
#include "mandelbrot.h"

unsigned char *alloc_image(const Config *cfg) {
    unsigned char *image = malloc((size_t)cfg->width * (size_t)cfg->height);
    if (image == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para a imagem.\n");
        exit(EXIT_FAILURE);
    }
    return image;
}

void write_pgm(const char *filename, const unsigned char *image, const Config *cfg) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Erro: nao foi possivel criar o arquivo '%s'.\n", filename);
        exit(EXIT_FAILURE);
    }

    for (int y = 0; y < cfg->height; y++) {
        for (int x = 0; x < cfg->width; x++) {
            const char *fmt = (x == 0) ? "%d" : " %d";
            if (fprintf(fp, fmt, image[(size_t)y * (size_t)cfg->width + (size_t)x]) < 0) {
                fprintf(stderr, "Erro: falha ao escrever no arquivo '%s'.\n", filename);
                fclose(fp);
                exit(EXIT_FAILURE);
            }
        }
        if (fprintf(fp, "\n") < 0) {
            fprintf(stderr, "Erro: falha ao escrever no arquivo '%s'.\n", filename);
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }

    if (fclose(fp) != 0) {
        fprintf(stderr, "Erro: falha ao fechar o arquivo '%s'.\n", filename);
        exit(EXIT_FAILURE);
    }
}
