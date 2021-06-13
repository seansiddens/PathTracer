#include "ray.h"
#include "vec3.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image/stb_image_write.h"

double hit_sphere(vec3 center, double radius, ray r) {
    vec3 oc = v3_sub(r.orig, center);
    double a = v3_dot(r.dir, r.dir);
    double b = 2.0 * v3_dot(oc, r.dir);
    double c = v3_dot(oc, oc) - (radius * radius);
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }

}

// Returns a background color given a ray
color ray_color(ray r) {
    // Check if ray hit sphere
    double t = hit_sphere(v3_init(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        // Get surface normal of hit
        vec3 N = v3_unit_vector(v3_sub(ray_at(r, t), v3_init(0, 0, -1)));
        return v3_scale(v3_init(N.x + 1, N.y + 1, N.z + 1), 0.5);
    }

    // If not, return background color
    vec3 unit_direction = v3_unit_vector(r.dir);
    t = 0.5 * (unit_direction.y + 1.0);
    color start_color = v3_init(1.0, 1.0, 1.0);
    color end_color = v3_init(0.5, 0.7, 1.0);
    return v3_lerp(start_color, end_color, t);
}

int main(void) {
    // Image Settings
    const double aspect_ratio = 4.0 / 3.0;
    const uint32_t image_width = 400;
    const uint32_t image_height = (uint32_t)(image_width / aspect_ratio);
    // Buffer for storing image data
    uint8_t *image =
        (uint8_t *)calloc(image_width * image_height * 3, sizeof(uint8_t));

    // Camera settings
    double viewport_height = 2.0;
    double viewport_width = viewport_height * aspect_ratio;
    double focal_length = 1.0;

    vec3 origin = {0, 0, 0};
    vec3 horizontal = {viewport_width, 0, 0};
    vec3 vertical = {0, viewport_height, 0};
    vec3 focal = {0, 0, focal_length};
    vec3 lower_left_corner =
        v3_sub(v3_sub(v3_sub(origin, focal), v3_scale(horizontal, 0.5)),
               v3_scale(vertical, 0.5));

    // Iterate over each pixel in the image
    for (uint32_t y = 0; y < image_height; y++) {
        // Print progress
        if (y % 50 == 0) {
            printf("Rows remaining: %d\n", image_height - y);
        }
        for (uint32_t x = 0; x < image_width; x++) {
            // Get index into buffer from x and y coordinates
            uint32_t i = y * image_width * 3 + x * 3;

            // Map image coordinates to normalized (u, v) coordinates
            double u = x / (double)(image_width - 1);
            double v = 1.0 - (y / (double)(image_height - 1));

            // Get direction from camera origin to point on viewport
            vec3 dir =
                v3_add(v3_add(lower_left_corner, v3_scale(horizontal, u)),
                       v3_scale(vertical, v));

            // View ray
            ray r = {origin, dir};

            // Get color of what ray is looking at
            color col = ray_color(r);

            // Set value of color channels at the pixel
            image[i] = (uint8_t)(255 * col.x);
            image[i + 1] = (uint8_t)(255 * col.y);
            image[i + 2] = (uint8_t)(255 * col.z);
        }
    }
    printf("Done!\n");

    // Write contents of image buffer out to PNG
    char *image_name = "out.png";
    if ((stbi_write_png(image_name, image_width, image_height, 3, image,
                        image_width * 3)) == 0) {
        fprintf(stderr, "Failed to write image out to file\n");
        exit(1);
    }

    return 0;
}
