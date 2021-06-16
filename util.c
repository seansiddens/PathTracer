#include "util.h"

#include <math.h>
#include <stdlib.h>

double degrees_to_radians(double degrees) { return degrees * M_PI / 180.0; }

// Returns a random real in [0, 1)
double random_uniform() { return (double)random() / (double)((long int)RAND_MAX + 1); }

// Returns a random real in [min, max)
double random_double(double min, double max) {
    return min + (max - min) * random_uniform();
}

// A simple clamp function
double clamp(double x, double min, double max) {
    if (x < min) {
        return min;
    }
    if (x > max) {
        return max;
    }
    return x;
}

void swap_double(double *x, double *y) {
    double temp = *y;
    *y = *x;
    *x = temp;
    return;
}
