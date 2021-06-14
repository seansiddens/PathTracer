#include "color.h"
#include "util.h"

#include <math.h>

// Writes out the color of the pixel given an index into the image buffer
void write_color(uint8_t *image, color pixel_color, uint32_t i,
                 uint32_t samples_per_pixel) {

    // Scale color based on the number of samples taken per pixel and
    // gamma-correct for gamma=2.0.
    double scale = 1.0 / (double)samples_per_pixel;
    pixel_color.x = sqrt(scale * pixel_color.x);
    pixel_color.y = sqrt(scale * pixel_color.y);
    pixel_color.z = sqrt(scale * pixel_color.z);

    // Set value of color channels at the pixel
    image[i] = (uint8_t)(255 * clamp(pixel_color.x, 0.0, 1.0));
    image[i + 1] = (uint8_t)(255 * clamp(pixel_color.y, 0.0, 1.0));
    image[i + 2] = (uint8_t)(255 * clamp(pixel_color.z, 0.0, 1.0));
}
