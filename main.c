#include "camera.h"
#include "color.h"
#include "hit.h"
#include "ray.h"
#include "scene.h"
#include "sphere.h"
#include "vec3.h"
#include "util.h"

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
    uint32_t samples_per_pixel = 100;

    // Camera settings
    Camera *cam = cam_create(v3_init(0, 0, 0), aspect_ratio, 1.0);

    // Scene settings
    Scene *scene = scene_create();
    scene_add_sphere(scene, 0, 0, -1, 0.5);
    scene_add_sphere(scene, 0, -100.5, -1, 100);

    // Iterate over each pixel in the image
    for (uint32_t y = 0; y < image_height; y++) {
        // Print progress
        if (y % 50 == 0) {
            printf("Rows remaining: %d\n", image_height - y);
        }
        for (uint32_t x = 0; x < image_width; x++) {
            // Get index into buffer from x and y coordinates
            uint32_t i = y * image_width * 3 + x * 3;

            // Take multiple samples per pixel
            color pixel_color = v3_init(0, 0, 0);
            for (uint32_t s = 0; s < samples_per_pixel; s++) {
                // Map image coordinates to normalized (u, v) coordinates, 
                // offset by random amount for antialiasing
                double u = (double)(x + random_uniform()) / (image_width - 1);
                double v = 1.0 - ((double)(y + random_uniform()) / (image_height - 1));
                
                // Get view ray from camera to viewport
                ray view_ray = get_view_ray(cam, u, v);

                // Accumulate color of what ray is looking at
                pixel_color = v3_add(pixel_color, ray_color(view_ray, scene));
               
            }
            // Write color to final image 
            write_color(image, pixel_color , i, samples_per_pixel);
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
    cam_delete(&cam);

    return 0;
}
