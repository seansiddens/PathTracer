#include "scene.h"
#include "material.h"
#include "sphere.h"
#include "util.h"
#include "vec3.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

// Constructor for a scene
Scene *scene_create(void) {
    Scene *scene = (Scene *)malloc(sizeof(Scene));
    assert(scene != NULL);

    scene->objects = ll_create(false);

    return scene;
}

//
// Create and initialize a randomized scene
//
Scene *random_scene(void) {
    Scene *scene = (Scene *)malloc(sizeof(Scene));
    assert(scene != NULL);

    scene->objects = ll_create(false);

    Material *ground_material = create_lambertian(v3_init(0.5, 0.5, 0.5));
    scene_add_sphere(scene, 0, -1000, 0, 1000, ground_material);

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_uniform();
            vec3 center =
                v3_init(a + 0.9 * random_uniform(), 0.2, b + 0.9 * random_uniform());

            if (v3_length(v3_sub(center, v3_init(4, 0.2, 0))) > 0.9) {
                Material *sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    color albedo = v3_hadamard(v3_random_uniform(), v3_random_uniform());
                    sphere_material = create_lambertian(albedo);
                    scene_add_sphere(scene, center.x, center.y, center.z, 0.2,
                                     sphere_material);
                } else if (choose_mat < 0.95) {
                    // metal
                    color albedo = v3_random_range(0.5, 1.0);
                    double fuzz = random_double(0, 0.5);
                    sphere_material = create_metal(albedo, fuzz);
                    scene_add_sphere(scene, center.x, center.y, center.z, 0.2,
                                     sphere_material);
                } else {
                    // glass
                    sphere_material = create_dielectric(1.5);
                    scene_add_sphere(scene, center.x, center.y, center.z, 0.2,
                                     sphere_material);
                }
            }
        }
    }

    Material *material1 = create_dielectric(1.5);
    scene_add_sphere(scene, 0, 1, 0, 1.0, material1);

    Material *material2 = create_lambertian(v3_init(0.4, 0.2, 0.1));
    scene_add_sphere(scene, -4, 1, 0, 1.0, material2);

    Material *material3 = create_metal(v3_init(0.7, 0.6, 0.5), 0.0);
    scene_add_sphere(scene, 4, 1, 0, 1.0,  material3);

    return scene;
}

// Destructor for a scene
void scene_delete(Scene **scene) {
    if (*scene) {
        ll_delete(&(*scene)->objects);
        free(*scene);
        *scene = NULL;
    }
    return;
}

// Adds a sphere to the scene
void scene_add_sphere(Scene *scene, double x, double y, double z, double r,
                      Material *material) {
    Sphere *s = sphere_create(v3_init(x, y, z), r, material);
    ll_insert(scene->objects, s, SPHERE);
}

// Intersects a ray with our scene. Returns true if it hits any object
// in the scene. Modiefies the hit record with information about the
// closest intersection.
bool scene_intersect(Scene *scene, ray r, double t_min, double t_max, HitRecord *rec) {
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    // Iterate over every object and intersect w/ it
    HittableList *objects = scene->objects;
    Node *curr_node = objects->head->next;
    while (curr_node != objects->tail) {
        switch (curr_node->type) {
        case SPHERE:
            if (sphere_intersect(*((Sphere *)curr_node->object), r, t_min, closest_so_far,
                                 &temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                *rec = temp_rec;
            }
            break;
        }
        curr_node = curr_node->next;
    }

    return hit_anything;
}

// Print the objects in our scene
void scene_print(Scene *scene) { ll_print(scene->objects); }
