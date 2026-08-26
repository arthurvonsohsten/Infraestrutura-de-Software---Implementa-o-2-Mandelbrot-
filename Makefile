CC = gcc
CFLAGS = -Wall -Wextra -O3 -std=c11 -fopenmp -pthread
LDFLAGS = -lm

SRCS = main.c core.c serial.c openmp.c pthreads1.c pthreads2.c pgm_writer.c
OBJS = $(SRCS:.c=.o)
BIN = mandelbrot

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LDFLAGS)

%.o: %.c mandelbrot.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BIN) *.pgm times.txt

.PHONY: all clean
