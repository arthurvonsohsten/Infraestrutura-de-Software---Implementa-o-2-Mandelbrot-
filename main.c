#include <stdlib.h>
#include "mandelbrot.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    Config cfg;
    cfg.width = 800;
    cfg.height = 600;
    cfg.max_iter = 1000;
    cfg.num_threads = 4;

    unsigned char *image = alloc_image(&cfg);

    run_serial(image, &cfg);
    write_pgm("mandelbrot_alvs_serial.pgm", image, &cfg);

    run_openmp(image, &cfg);
    write_pgm("mandelbrot_alvs_openmp.pgm", image, &cfg);

    run_pthreads1(image, &cfg);
    write_pgm("mandelbrot_alvs_pthreads1.pgm", image, &cfg);

    run_pthreads2(image, &cfg);
    write_pgm("mandelbrot_alvs_pthreads2.pgm", image, &cfg);

    free(image);

    return 0;
}
