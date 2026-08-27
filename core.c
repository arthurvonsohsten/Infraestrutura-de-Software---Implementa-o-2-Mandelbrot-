#include "mandelbrot.h"

static const double REAL_MIN = -2.0;
static const double REAL_MAX = 1.0;
static const double IMAG_MIN = -1.5;
static const double IMAG_MAX = 1.5;

unsigned char mandelbrot_pixel(int px, int py, const Config *cfg) {
    double cr = REAL_MIN + (REAL_MAX - REAL_MIN) * (double)px / (double)cfg->width;
    double ci = IMAG_MIN + (IMAG_MAX - IMAG_MIN) * (double)py / (double)cfg->height;

    double zr = 0.0;
    double zi = 0.0;
    int iter = 0;

    while (zr * zr + zi * zi <= 4.0 && iter < cfg->max_iter) {
        double zr_new = zr * zr - zi * zi + cr;
        double zi_new = 2.0 * zr * zi + ci;
        zr = zr_new;
        zi = zi_new;
        iter++;
    }

    long value = ((long)iter * 255L) / (long)cfg->max_iter;
    if (value > 255) {
        value = 255;
    }
    return (unsigned char)value;
}
