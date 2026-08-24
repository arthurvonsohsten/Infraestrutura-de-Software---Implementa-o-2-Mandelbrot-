#ifndef MANDELBROT_H
#define MANDELBROT_H

typedef struct {
    int width;
    int height;
    int max_iter;
    int num_threads;
} Config;

/* Calcula o pixel (px, py) e ja devolve a intensidade normalizada (0-255) */
unsigned char mandelbrot_pixel(int px, int py, const Config *cfg);

/* Aloca o buffer da imagem (1 byte por pixel) */
unsigned char *alloc_image(const Config *cfg);

/* Cada implementacao preenche 'image' e devolve o tempo gasto em segundos */
double run_serial(unsigned char *image, const Config *cfg);
double run_openmp(unsigned char *image, const Config *cfg);
double run_pthreads1(unsigned char *image, const Config *cfg);
double run_pthreads2(unsigned char *image, const Config *cfg);

/* Escreve o .pgm sem cabecalho (valores separados por espaco, 1 linha por linha de imagem) */
void write_pgm(const char *filename, const unsigned char *image, const Config *cfg);

#endif
