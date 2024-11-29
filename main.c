// ALBERTO FACCHIN SM3201282

#include "mandelbrot.h"
#include "pgm.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

// function that checks if the number of rows is even or if it is <1
int checkNumberRows(int n_rows) {
  if (n_rows < 1 || (n_rows%2 != 0))
    return -1;
  return 0;
}

int main(int argc, char **argv) {
  // check if the number of arguments is correct
  if (argc == 4) {
    char *filename = argv[1];
    int M = atoi(argv[2]);      // number of iterations
    int n_rows = atoi(argv[3]); // vertical resoultion

    if (checkNumberRows(n_rows) == -1) {
      printf("Invalid number of rows (odd or <1 or string)\n");
      return 0;
    }
    
    int n_cols = n_rows * 1.5;  // horizontal resolution
    double r = 2;

    strcat(filename, ".pgm"); // string concatenation between the filename and the extension type
    // printf("filename = %s\nM = %d\nn.rows = %d\nn.cols = %d\n", filename, M, n_rows, n_cols);

    pgm image;
    int err = empty_image(filename, &image, n_cols, n_rows);
    if (err != 0) {
      printf("Unable to open image: %d\n", -err);
      return 0;
    }

    // calculating the execution time of the mandelbrot set function
    double start = omp_get_wtime();
    calculateMandelbrotSet(M, n_rows, n_cols, r, &image);
    double end = omp_get_wtime();
    printf("Execution time: %fms\n", (end - start) * 1000);

    close_image(&image);  // closing the image and deallocating the memory
  } 
  else
    printf("Wrong number of args: %d instead of 4\n", argc);

  return 0;
}