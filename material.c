#include "material.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
