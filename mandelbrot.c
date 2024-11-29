// ALBERTO FACCHIN SM3201282

#include "mandelbrot.h"
#include "pgm.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <complex.h>
#include <math.h>
#include <omp.h>

/* 
* The mandelbrot function takes as input the maximum number of iterations(M), a complex double number(c) and 
* the radius(r), and returns i (the divergence speed) if the point is outside the Mandelbrot set, otherwise
* returns M, but it could return also other values e.g. 0 or 1, etc. (I chose M)
* The function basically does a check if |Zn| is greather (or equal) than the radius and returns the
* appropriate value.
*/
int mandelbrot(int M, complex double c, double r){
    complex double z = 0;
    for (int i = 1; i <= M; i++) {
        z = z*z + c;
        if (cabs(z) >= r)    // check if |Zn| >= r
            return i;    // return i is equal to return n (the divergence speed)
    }
    return M;
}


/*
* The calculateMandelbrotSet procedure takes as input the maximum number of iterations(M), the number of
* rows(n_rows) and columns(n_cols), the radius(r), and the pgm file where the Mandelbrot set has to be drawn.
* At the beginning there are defined the lower bound and the upper bound for both the real and the imaginary
* part. Subsequently it has been calculated the incremental step and the logarithm of M, because it is
* sufficient to calculate it just once (it will be used to determine the color of the pixel).
* Then I decided to collapse the two innested for cycles using the OpenMP library.
* Since it is a procedure (void) and not a function it does not return any value.
*/
void calculateMandelbrotSet(int M, int n_rows, int n_cols, double r, pgm_ptr image) {
    double r_lower = -2.0;    // real lower bound
    double r_upper = 1.0;    // real upper bound
    double i_lower = -1.0;    // imaginary lower bound
    double i_upper = 1.0;    // imaginary upper bound

    double r_step = (r_upper - r_lower)/(n_cols - 1);    // real progression step
    double i_step = (i_upper - i_lower)/(n_rows - 1);    // imaginary progression step

    double log_M = log(M);    // it is more efficient to calculate log(M) once
    double real = 0.0;    // real part of the complex number
    double imag = 0.0;    // imaginary part of the complex number
    double complex c = 0.0;    // complex number

#pragma omp for collapse(2)
    // in "collapse(2)", the number 2 is used to indicated that the two outermost for cycles are collapsed
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            real = r_lower + j*r_step;
            imag = i_lower + i*i_step;
            c = real + imag*I;

            if (mandelbrot(M, c, r) == M)    // if the point is inside the Mandelbrot set
                image->data[i*n_cols + j + image->offset] = 255;    // set the pixel's colour to white
            else
                image->data[i*n_cols + j + image->offset] = 255*log(mandelbrot(M, c, r))/log_M;
        }
    }
}