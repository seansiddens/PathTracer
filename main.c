#include "hit.h"
#include "ray.h"
#include "scene.h"
#include "sphere.h"
#include "vec3.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image/stb_image_write.h"

// Returns a background color given a ray
color ray_color(ray r, Scene *scene) {
    HitRecord rec;
    // Check if ray hits an object in our scene
    if (scene_intersect(scene, r, 0, INFINITY, &rec)) {
        vec3 N = rec.normal;
        return v3_scale(v3_init(N.x + 1, N.y + 1, N.z + 1), 0.5);
    }

    // If not, return background color
    vec3 unit_direction = v3_unit_vector(r.dir);
    double t = 0.5 * (unit_direction.y + 1.0);
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

    // Scene settings
    Scene *scene = scene_create();
    scene_add_sphere(scene, 0, 0, -1, 0.5);
    scene_add_sphere(scene, 0, -100.5, -1, 100);
    scene_print(scene);

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
            color col = ray_color(r, scene);

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

    // Free allocated memory
    free(image);
    scene_delete(&scene);

    return 0;
}
