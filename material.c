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
        ray_scattered->orig = rec->p;
        ray_scattered->dir = scatter_direction;
        *attenuation = ((Lambertian *)(mat->material))->albedo;
        return true;
    } else if (mat->type == METAL) {

    } else {
        fprintf(stderr,
                "ERROR: Unknown material type encountered in scatter()!\n");
        exit(1);
    }
}
