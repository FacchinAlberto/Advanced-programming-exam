// ALBERTO FACCHIN SM3201282

#include "pgm.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/*
* The open_image function is used to open the image and map it into the memory. It takes as input the path
* and the image, and returns -1 in case of error during the opening, -2 in case of error during the scan
* of the dimension (width and height), -3 in case of error during the mapping, and 0 in case of success.
*/
int open_image(char * path, pgm_ptr img)
{
  img->fd = fopen(path, "r+");  // opening the file
  if (img->fd == NULL) {
    return -1;
  }
  struct stat sbuf;
  stat(path, &sbuf);  // obtaining file dimension
  img->size = sbuf.st_size;
  if (fscanf(img->fd, "P5\n%d %d\n255\n", &img->width, &img->height) != 2) {
    fclose(img->fd);
    return -2;
  }
  img->offset = ftell(img->fd);  // setting the offset
  img->data = mmap((void *)0, img->size, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(img->fd), 0);  //fileno() returns the file descriptor as a number (int)
  if (img->data == MAP_FAILED) {  // MAP_FAILED is returned if it has been impossible to map the file
    fclose(img->fd);
    return -3;
  }
  return 0;
}

/*
* The empy_image function creates the image with the given width and height, and changing the dimension with
* the ftruncate() function.
* It returns -1 in case of error during the file opening, open_image() in case of success.
*/
int empty_image(char * path, pgm_ptr img, int width, int height)
{
  FILE * fd = fopen(path, "w+");
  if (fd == NULL) {
    return -1;
  }
  int written = fprintf(fd, "P5\n%d %d\n255\n", width, height);
  ftruncate(fileno(fd), written + width * height);  // changing file dimension
  fclose(fd);
  return open_image(path, img);
}


/*
* The function close_image closes the image.
* It uses also the munmap() function to free the memory allocated to the image.
* Returns 0 in case of success, -1 in case of error.
*/
int close_image(pgm_ptr img)
{
  if (img == NULL) {
    return -1;
  }
  munmap(img->data, img->size);  // unmapping
  fclose(img->fd);  // closing the file
  return 0;
}
