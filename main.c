#include "aarect.h"
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

#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image/stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image/stb_image.h"

#define MULITHREAD true
#define NUM_THREADS 8

// TODOS: 
// ----------------------------------------------------------------------------
// TODO: Deallocate material memory.

unsigned char *skybox;
int sky_width, sky_height, sky_channels;

typedef struct {
    Scene *scene;
    Camera *cam;
    BVHNode *bvh;
    uint32_t image_width, image_height, samples_per_pixel, max_depth;
    uint8_t *image;
    uint32_t thread_id;
} RenderArgs;

//
// Given a unit vector, return a color.
//
color get_background_color(vec3 dir) {
    color col; // Final bg color to return

    int bg_type = 1;
    switch (bg_type) {
    case 1: {
        // Set background to a gradient between two colors
        double t = 0.5 * (dir.y + 1.0);
        color start_color = v3_init(0.9, 0.9, 1.0);
        color end_color = v3_init(0.1, 0.1, 1.0);
        col = v3_lerp(start_color, end_color, t);
        break;
    }
    case 2: {
        // Set background to a skybox image
        // Get uv coords of skybox
        double u = 0.5 + (atan2(dir.x, dir.z) / (2 * M_PI));
        double v = 0.5 + (asin(dir.y) / M_PI);

        uint32_t x = u * sky_width;
        uint32_t y = (1.0 - v) * sky_height;

        uint32_t i = y * sky_width * sky_channels + x * sky_channels;

        col.x = skybox[i] / (double)255;
        col.y = skybox[i + 1] / (double)255;
        col.z = skybox[i + 2] / (double)255;

        col = SRGB_to_linear(col);
        break;
    }
    default:
        // Just set bg to black
        col = v3_init(0, 0, 0);
        break;
    }

    return col;
}


//
// Returns the color a given ray is pointing at
//
color ray_color(Scene *scene, BVHNode *bvh, ray r, uint32_t depth) {
    HitRecord rec;
    rec.t = INFINITY;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth == 0) {
        return v3_init(0, 0, 0);
    }

    // Check if ray hits an object in our scene
    if (!bvh_hit(bvh, r, 0.001, INFINITY, &rec)) {
        // If ray hits nothing, return background color
        vec3 unit_direction = v3_unit_vector(r.dir);
        return get_background_color(unit_direction);
    }

    // Ray has hit an object
    color red = {1.0, 0.0, 0.0};
    return red;

    ray scattered;
    color attenuation;
    color emitted_col = emitted(rec.material, rec.u, rec.v, rec.p);

    if (!scatter(rec.material, r, &rec, &attenuation, &scattered)) {
        return emitted_col;
    }

    return v3_add(emitted_col,
                  v3_hadamard(attenuation, ray_color(scene, bvh, scattered, depth - 1)));
}

void *render(void *thread_args) {
    RenderArgs *args = (RenderArgs *)thread_args;

    printf("Thread %d start!\n", args->thread_id);

    // Render chunk of image buffer from start_idx to end_idx
    for (uint32_t i = args->thread_id * 3;
         i <= args->image_width * args->image_height * 3; i += (NUM_THREADS * 3)) {
        uint32_t x = (i / 3) % args->image_width;
        uint32_t y = i / args->image_width / 3;

        // Take multiple samples per pixel
        color pixel_color = v3_init(0, 0, 0);
        for (uint32_t s = 0; s < args->samples_per_pixel; s++) {
            // Map image coordinates to normalized (u, v) coordinates,
            // offset by random amount for antialiasing
            double u = (double)(x + random_uniform()) / (args->image_width - 1);
            double v = 1.0 - ((double)(y + random_uniform()) / (args->image_height - 1));

            // Get view ray from camera to viewport
            ray view_ray = get_view_ray(args->cam, u, v);

            // Accumulate color of what ray is looking at
            pixel_color = v3_add(pixel_color, ray_color(args->scene, args->bvh, view_ray,
                                                        args->max_depth));
        }
        // Write color to final image
        write_color(args->image, pixel_color, i, args->samples_per_pixel);
    }

    printf("Thread %d done!\n", args->thread_id);

    return NULL;
}

int main(void) {
    // Image Settings
    const double aspect_ratio = 1.0;
    const uint32_t image_width = 400;
    const uint32_t image_height = (uint32_t)(image_width / aspect_ratio);
    // Buffer for storing image data
    uint8_t *image = (uint8_t *)calloc(image_width * image_height * 3, sizeof(uint8_t));
    uint32_t samples_per_pixel = 2;
    uint32_t max_depth = 50;

    // Camera settings
    vec3 vup = v3_init(0, 1, 0);
    vec3 look_from = v3_init(-10, 0, 0);
    vec3 look_at = v3_init(0, 0, 0);
    double dist_to_focus = v3_length(v3_sub(look_from, look_at));
    double aperture = 0.05;
    double vfov = 40;
    Camera *cam =
        cam_create(vup, look_from, look_at, aspect_ratio, vfov, aperture, dist_to_focus);

    // Scene settings
    Scene *scene = scene_create();
    
    Material *red = create_lambertian(v3_init(0.65, 0.05, 0.05));
    Material *white = create_lambertian(v3_init(0.73, 0.73, 0.73));
    Material *green = create_lambertian(v3_init(0.12, 0.45, 0.15));
    Material *light = create_diffuse_light(v3_init(15, 15, 15));

    /*scene_add_yz_rect(scene, 0, 555, 0, 555, 555, green);*/
    /*scene_add_xy_rect(scene, 0, 555, 0, 555, 555, white);*/
    /*scene_add_xz_rect(scene, 0, 555, 0, 555, 555, white);*/

    /*scene_add_xy_rect(scene, -1, 1, -1, 1, 0, green);*/
    scene_add_yz_rect(scene, -1, 1, -1, 1, 0, green);

    // Load skybox asset
    skybox = stbi_load("assets/parched_canal_4k.hdr", &sky_width, &sky_height,
                       &sky_channels, 0);
    printf("Skybox: %d x %d, channels = %d\n", sky_width, sky_height, sky_channels);
    if (skybox == NULL) {
        fprintf(stderr, "ERROR: Failed to load skybox HDRI image!\n");
        exit(1);
    }

    // Construct BVH
    BVHNode *bvh = bvh_create(scene, 0, scene->object_count - 1);

    if (MULITHREAD) {
        // Thread setup
        pthread_t threads[NUM_THREADS];
        RenderArgs thread_args[NUM_THREADS];

        // Initialize thread creation arguments for each thread
        for (int thread = 0; thread < NUM_THREADS; thread++) {
            thread_args[thread].bvh = bvh;
            thread_args[thread].cam = cam;
            thread_args[thread].scene = scene;
            thread_args[thread].image_width = image_width;
            thread_args[thread].image_height = image_height;
            thread_args[thread].samples_per_pixel = samples_per_pixel;
            thread_args[thread].max_depth = max_depth;
            thread_args[thread].image = image;
            thread_args[thread].thread_id = thread;
        }

        // Spawn each thread
        for (int thread = 0; thread < NUM_THREADS; thread++) {
            int rc = pthread_create(&threads[thread], NULL, render,
                                    (void *)&thread_args[thread]);
            if (rc) {
                fprintf(stderr, "ERROR: Return code from pthread_create() is %d\n", rc);
                exit(1);
            }
        }

        // Join threads
        for (int thread = 0; thread < NUM_THREADS; thread++) {
            pthread_join(threads[thread], NULL);
        }

    } else {
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
                    double v =
                        1.0 - ((double)(y + random_uniform()) / (image_height - 1));

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
    }

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
