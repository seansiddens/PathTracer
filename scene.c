#include "scene.h"
#include "sphere.h"
#include "vec3.h"

#include <stdbool.h>
#include <stdlib.h>

// Constructor for a scene
Scene *scene_create(void) {
    Scene *scene = (Scene *)malloc(sizeof(Scene));
    if (scene) {
        scene->objects = ll_create(false);
    }
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
void scene_add_sphere(Scene *scene, double x, double y, double z, double r) {
    Sphere *s = sphere_create(v3_init(x, y, z), r);
    ll_insert(scene->objects, s, SPHERE);
}

// Intersects a ray with our scene. Returns true if it hits any object
// in the scene. Modiefies the hit record with information about the
// closest intersection.
bool scene_intersect(Scene *scene, ray r, double t_min, double t_max,
                     HitRecord *rec) {
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    // Iterate over every object and intersect w/ it
    HittableList *objects = scene->objects;
    Node *curr_node = objects->head->next;
    while (curr_node != objects->tail) {
        switch (curr_node->type) {
        case SPHERE:
            if (sphere_intersect(*((Sphere *)curr_node->object), r, t_min,
                                 closest_so_far, &temp_rec)) {
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
