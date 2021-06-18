#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "hit.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "scene.h"
#include "sphere.h"
#include "util.h"
#include "vec3.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image/stb_image_write.h"

// TODO: Every time an object is added to the scene, keep track of it's material
// using a LL, then when scene_delete() is called, free that memory.

//
// Returns the color a given ray is pointing at
//
color ray_color(Scene *scene, BVHNode *bvh, ray r, uint32_t depth) {
    HitRecord rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth == 0) {
        return v3_init(0, 0, 0);
    }

    // Check if ray hits an object in our scene
    if (bvh_hit(bvh, r, 0.001, INFINITY, &rec)) {
        ray scattered;
        color attenuation;
        if (scatter(rec.material, r, &rec, &attenuation, &scattered)) {
            return v3_hadamard(attenuation, ray_color(scene, bvh, scattered, depth - 1));
        }
        return v3_init(0, 0, 0);
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
    const double aspect_ratio = 3.0 / 2.0;
    const uint32_t image_width = 400;
    const uint32_t image_height = (uint32_t)(image_width / aspect_ratio);
    // Buffer for storing image data
    uint8_t *image = (uint8_t *)calloc(image_width * image_height * 3, sizeof(uint8_t));
    uint32_t samples_per_pixel = 100;
    uint32_t max_depth = 50;

    // Camera settings
    vec3 vup = v3_init(0, 1, 0);
    vec3 look_from = v3_init(0, 2, 2);
    vec3 look_at = v3_init(0, 0, -1);
    double dist_to_focus = v3_length(v3_sub(look_from, look_at));
    double aperture = 0.1;
    Camera *cam =
        cam_create(vup, look_from, look_at, aspect_ratio, 65, aperture, dist_to_focus);

    // Scene settings
    /*Scene *scene = random_scene();*/
    Scene *scene = scene_create();

    Material *mat = create_lambertian(v3_init(0.8, 0.8, 0.9));
    scene_add_sphere(scene, 0, 1, -1, 1.0, mat);

    BVHNode *bvh = bvh_create(scene, 0, scene->object_count - 1);

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
                pixel_color =
                    v3_add(pixel_color, ray_color(scene, bvh, view_ray, max_depth));
            }
            // Write color to final image
            write_color(image, pixel_color, i, samples_per_pixel);
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
