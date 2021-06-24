#include "color.h"
#include "util.h"

#include <assert.h>
#include <math.h>

//
// Returns the gamma corrected version of the color
//
color gamma_correct(color col, double gamma) {
    col.x = pow(col.x, gamma);
    col.y = pow(col.y, gamma);
    col.z = pow(col.z, gamma);

    return col;
}

//
// Transforms color from linear to SRGB.
// Source(s):
//      - https://stackoverflow.com/questions/34472375/linear-to-srgb-conversion
//      - https://www.ryanjuckett.com/rgb-color-space-conversion/
//
color linear_to_SRGB(color rgb) {
    if (rgb.x > 0.0031308) {
        rgb.x = 1.055 * pow(rgb.x, 1.0 / 2.4) - 0.055;
    } else {
        rgb.x = 12.92 * rgb.x;
    }
    if (rgb.y > 0.0031308) {
        rgb.y = 1.055 * pow(rgb.y, 1.0 / 2.4) - 0.055;
    } else {
        rgb.y = 12.92 * rgb.y;
    }
    if (rgb.z > 0.0031308) {
        rgb.z = 1.055 * pow(rgb.z, 1.0 / 2.4) - 0.055;
    } else {
        rgb.z = 12.92 * rgb.z;
    }

    return rgb;
}

color SRGB_to_linear(color rgb) {
    if (rgb.x > 0.04045) {
        rgb.x = pow((rgb.x + 0.055) / 1.055, 2.4);
    } else {
        rgb.x = rgb.x / 12.92;
    }
    if (rgb.y > 0.04045) {
        rgb.y = pow((rgb.y + 0.055) / 1.055, 2.4);
    } else {
        rgb.y = rgb.y / 12.92;
    }
    if (rgb.z > 0.04045) {
        rgb.z = pow((rgb.z + 0.055) / 1.055, 2.4);
    } else {
        rgb.z = rgb.z / 12.92;
    }
    return rgb;
}

//
// ACES tone mapping curve fit to go from HDR to LDR
// https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
//
color ACES_film(color rgb) {
    double a = 2.51;
    double b = 0.03;
    double c = 2.43;
    double d = 0.59;
    double e = 0.14;

    rgb.x = clamp((rgb.x * (a * rgb.x + b)) / (rgb.x * (c * rgb.x + d) + e), 0.0, 1.0);
    rgb.y = clamp((rgb.y * (a * rgb.y + b)) / (rgb.y * (c * rgb.y + d) + e), 0.0, 1.0);
    rgb.z = clamp((rgb.z * (a * rgb.z + b)) / (rgb.z * (c * rgb.z + d) + e), 0.0, 1.0);

    return rgb;
}

//
// Writes out the color of the pixel given an index into the image buffer. Applies
// appropriate transformations from linear to non-linear color space.
//
void write_color(uint8_t *image, color pixel_color, uint32_t i,
                 uint32_t samples_per_pixel) {

    // Scale color based on the number of samples taken per pixel and then
    // gamma-correct.
    double scale = 1.0 / (double)samples_per_pixel;
    pixel_color = v3_scale(pixel_color, scale);

    // apply exposure
    double exposure = 1.0;
    pixel_color = v3_scale(pixel_color, exposure);

    // convert unbounded HDR color range to SDR color range
    pixel_color = ACES_film(pixel_color);

    // Convert from linear to sRGB for display
    pixel_color = linear_to_SRGB(pixel_color);

    // Set value of color channels at the pixel
    image[i] = (uint8_t)(255 * clamp(pixel_color.x, 0.0, 1.0));
    image[i + 1] = (uint8_t)(255 * clamp(pixel_color.y, 0.0, 1.0));
    image[i + 2] = (uint8_t)(255 * clamp(pixel_color.z, 0.0, 1.0));
}
