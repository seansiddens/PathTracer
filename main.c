
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image/stb_image_write.h"

#define WIDTH 512
#define HEIGHT 512

int main(void) {
    // Buffer for storing image data
    uint8_t image[WIDTH * HEIGHT * 3] = {0};

    // Iterate over each pixel in the image
    for (int y = 0; y < HEIGHT; y++) {
        if (y % 50 == 0) {
            printf("Rows remaining: %d\n", HEIGHT - y);
        }
        for (int x = 0; x < WIDTH; x++) {
            // Get index into buffer from x and y coordinates
            uint32_t i = y * WIDTH * 3 + x * 3;

            double r = (double)x / (WIDTH - 1);
            double g = (double)y / (HEIGHT - 1);
            double b = 0.25;

            // Set value of color channels at the pixel
            image[i] = (uint8_t)(255 * r);
            image[i + 1] = (uint8_t)(255 * g);
            image[i + 2] = (uint8_t)(255 * b);
        }
    }
    printf("Done!\n");

    // Write contents of image buffer out to PNG
    char *image_name = "out.png";
    if ((stbi_write_png(image_name, WIDTH, HEIGHT, 3, image, WIDTH * 3)) == 0) {
        fprintf(stderr, "Failed to write image out to file\n");
        exit(1);
    }

    return 0;
}
