#ifndef MANDELBROT_H
#define MANDELBROT_H

/*
* Macros to calculate the square of a complex number
*/
#define SQPOWER(zr, zi) (zr * zr + zi * zi)

/*
* Function to calculate the number of iterations for a given complex number
* to escape from the Mandelbrot set
* real: the real part of the complex number
* imag: the imaginary part of the complex number
* max_iterations: the maximum number of iterations to be performed
* return: the number of iterations it took for the complex number to escape
*/
int mandelbrot(double real, double imag, int max_iterations);

/*
* Function to generate the Mandelbrot set in a given region of the complex plane
* n_x: the number of pixels in the x-direction
* n_y: the number of pixels in the y-direction
* local_ny_start: the starting index of the y-direction for the current process
* local_ny_stop: the stopping index of the y-direction for the current process
* x_L: the leftmost x-coordinate of the complex plane
* y_L: the lowest y-coordinate of the complex plane
* x_R: the rightmost x-coordinate of the complex plane
* y_R: the highest y-coordinate of the complex plane
* I_max: the maximum number of iterations to be performed
* imgPtr: a pointer to the memory region that contains the image
*/
void generate_mandelbrot(int n_x, int n_y,
                        int local_ny_start, int local_ny_stop,
                        double x_L, double y_L, 
                        double x_R, double y_R,
                        int I_max, void *imgPtr);

/*
* Function to write a PGM image
* image        : a pointer to the memory region that contains the image
* maxval       : either 255 or 65536
* xsize, ysize : x and y dimensions of the image
* image_name   : the name of the file to be written
*/
void write_pgm_image( void *image, int maxval, int xsize, int ysize, const char *image_name);

#endif // MANDELBROT_H
