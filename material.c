#include "material.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

enum MaterialType { LAMBERTIAN, METAL };
typedef enum MaterialType MaterialType;

struct Material {
    MaterialType type;
    void *material;
};

typedef struct {
    color albedo;
} Lambertian;

typedef struct {
    color albedo;
} Metal;

//
// Returns a pointer to a newly created Lambertian material.
// The albedo is roughly the color of the material.
//
Material *create_lambertian(color albedo) {
    Material *mat = (Material *)malloc(sizeof(Material));
    assert(mat != NULL);

    // Set the material type to Lambertian
    mat->type = LAMBERTIAN;

    // Initialize the material w/ the supplied albedo
    Lambertian *lambert = (Lambertian *)malloc(sizeof(Lambertian));
    assert(lambert != NULL);
    lambert->albedo = albedo;

    mat->material = lambert;

    return mat;
}

//
// Returns a pointer to a newly created metal material.
// Metal reflects incoming rays and has a surface albedo.
//
Material *create_metal(color albed) {
    Material *mat = (Material *)malloc(sizeof(Material));
    assert(mat != NULL);

    // Set material type to metal
    mat->type = METAL;

    // Initialize the material w/ supplied albedo
    Metal *metal = (Metal *)malloc(sizeof(Metal));
    assert(metal != NULL);

    mat->material = metal;

    return mat;
}

//
// Deallocates material memory.
//
void mat_delete(Material **mat) {
    if (*mat) {
        if ((*mat)->material) {
            free((*mat)->material);
            (*mat)->material = NULL;
        }
        free(*mat);
        *mat = NULL;
    }
    return;
}

//
// Given an incoming ray and the material type, returns true if a ray is
// scattered and false otherwise. The scattered ray is passed back in the
// pointer ray_scattered. The light attenuation is passed back through the color
// pointer attenuation.
//
bool scatter(Material *mat, ray ray_in, HitRecord *rec, color *attenuation,
             ray *ray_scattered) {
    if (mat->type == LAMBERTIAN) {
        vec3 scatter_direction = v3_add(rec->normal, random_unit_vector());

        // Catch degenerate scatter direction - if the random unit vector
        // generated is exactly opposite to the surface normal, then they will
        // sum to zero, resulting in a zero scatter direction vector. This will
        // lead to issues later on.
        if (v3_near_zero(scatter_direction)) {
            scatter_direction = rec->normal;
        }
        ray_scattered->orig = rec->p;
        ray_scattered->dir = scatter_direction;
        *attenuation = ((Lambertian *)(mat->material))->albedo;
        return true;
    } else if (mat->type == METAL) {
        v3_print(ray_in.orig);
        return false;
    } else {
        fprintf(stderr,
                "ERROR: Unknown material type encountered in scatter()!\n");
        exit(1);
    }
}
