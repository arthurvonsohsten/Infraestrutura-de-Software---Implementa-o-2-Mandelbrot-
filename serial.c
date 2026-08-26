#include <time.h>
#include "mandelbrot.h"

double run_serial(unsigned char *image, const Config *cfg) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int y = 0; y < cfg->height; y++) {
        for (int x = 0; x < cfg->width; x++) {
            image[(size_t)y * (size_t)cfg->width + (size_t)x] = mandelbrot_pixel(x, y, cfg);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    return (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
}
