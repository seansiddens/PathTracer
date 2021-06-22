#include "material.h"
#include "util.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

enum MaterialType { LAMBERTIAN, METAL, DIELECTRIC, DIFFUSE_LIGHT};
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
    double fuzz;
} Metal;

typedef struct {
    double index_of_refraction;
} Dielectric;

typedef struct {
    color emittted;
} DiffuseLight;

//
// Use Schlick's approximation for reflectance
//
static double reflectance(double cosine, double ref_idx) {
    double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}

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
// Fuzz modifies how "fuzzy" the reflections are. A value of zero will have no
// pertubation.
//
Material *create_metal(color albedo, double fuzz) {
    Material *mat = (Material *)malloc(sizeof(Material));
    assert(mat != NULL);

    // Set material type to metal
    mat->type = METAL;

    // Initialize the material w/ supplied albedo
    Metal *metal = (Metal *)malloc(sizeof(Metal));
    assert(metal != NULL);
    metal->albedo = albedo;
    metal->fuzz = clamp(fuzz, 0.0, 1.0); // Ensure fuzziness factor is in [0, 1]

    mat->material = metal;

    return mat;
}

//
// Returns a pointer to a newly created dielectric material.
//
Material *create_dielectric(double index_of_refraction) {
    Material *mat = (Material *)malloc(sizeof(Material));
    assert(mat != NULL);

    // Set material type to metal
    mat->type = DIELECTRIC;

    // Initialize the material w/ supplied albedo
    Dielectric *dielectric = (Dielectric *)malloc(sizeof(Metal));
    assert(dielectric != NULL);
    dielectric->index_of_refraction = index_of_refraction;

    mat->material = dielectric;

    return mat;
}

Material *create_diffuse_light(color emitted) {
    Material *mat = (Material *)malloc(sizeof(Material));
    assert(mat != NULL);

    mat->type = DIFFUSE_LIGHT;

    DiffuseLight *diffuse_light = (DiffuseLight *)malloc(sizeof(DiffuseLight));
    assert(diffuse_light != NULL);
    diffuse_light->emittted = emitted;

    mat->material = diffuse_light;

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
        // Lambertian scattering.
        vec3 scatter_direction = v3_add(rec->normal, random_unit_vector());

        // Catch degenerate scatter direction - if the random unit vector
        // generated is exactly opposite to the surface normal, then they will
        // sum to zero, resulting in a zero scatter direction vector. This will
        // lead to issues later on.
        if (v3_near_zero(scatter_direction)) {
            scatter_direction = rec->normal;
        }

        // Initialize scattered ray.
        ray_scattered->orig = rec->p;
        ray_scattered->dir = scatter_direction;

        // Reflected light is attenuated by the surface color.
        *attenuation = ((Lambertian *)(mat->material))->albedo;
        return true;
    } else if (mat->type == METAL) {
        // Reflect incoming ray.
        vec3 reflected = v3_reflect(v3_unit_vector(ray_in.dir), rec->normal);

        // Initialize scattered ray - direction is offset by fuzz factor
        ray_scattered->orig = rec->p;
        ray_scattered->dir =
            v3_add(reflected,
                   v3_scale(random_in_unit_sphere(), ((Metal *)(mat->material))->fuzz));

        // Reflected light is attenuated by the surface color.
        *attenuation = ((Metal *)(mat->material))->albedo;

        // Return true if reflected ray is in same hemisphere as normal
        return (v3_dot(ray_scattered->dir, rec->normal) > 0);

    } else if (mat->type == DIELECTRIC) {
        Dielectric *dielectric = (Dielectric *)mat->material;
        double ir = dielectric->index_of_refraction;
        *attenuation = v3_init(1.0, 1.0, 1.0);
        double refraction_ratio = rec->front_face ? (1.0 / ir) : ir;

        vec3 unit_dir = v3_unit_vector(ray_in.dir);
        double cos_theta = fmin(v3_dot(v3_scale(unit_dir, -1), rec->normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = (refraction_ratio * sin_theta) > 1.0;
        vec3 direction;
        if (cannot_refract ||
            reflectance(cos_theta, refraction_ratio) > random_uniform()) {
            direction = v3_reflect(unit_dir, rec->normal);
        } else {
            direction = v3_refract(unit_dir, rec->normal, refraction_ratio);
        }

        ray_scattered->orig = rec->p;
        ray_scattered->dir = direction;
        return true;
    
    } else if (mat->type == DIFFUSE_LIGHT) {
        return false; 
    } else {
        fprintf(stderr, "ERROR: Unknown material type encountered in scatter()!\n");
        exit(1);
    }
}

//
// 
color emitted(Material *mat, double u, double v, vec3 p) {
    color col;
    switch (mat->type) {
        case LAMBERTIAN:
            col = v3_init(0, 0, 0);
            break;
        case METAL:
            col = v3_init(0, 0, 0);
            break;
        case DIELECTRIC:
            col = v3_init(0, 0, 0);
            break;
        case DIFFUSE_LIGHT:
            col = ((DiffuseLight *)mat->material)->emittted;
            break;
    }
    return col;
}
