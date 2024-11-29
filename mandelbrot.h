// ALBERTO FACCHIN SM3201282

#ifndef _MANDELBROT_H
#define _MANDELBROT_H

#include "pgm.h"
#include <complex.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int mandelbrot(int M, complex double c, double r);

void calculateMandelbrotSet(int M, int n_rows, int n_cols, double r, pgm_ptr image);

#endif