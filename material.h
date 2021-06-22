#pragma once

#include "hit.h"
#include "ray.h"
#include "vec3.h"
#include "texture.h"

#include <stdbool.h>

typedef struct Material Material;

Material *create_lambertian(color albedo);

Material *create_metal(color albedo, double fuzz);

Material *create_dielectric(double index_of_refraction);

Material *create_diffuse_light(color emitted);

void mat_delete(Material **mat);

bool scatter(Material *mat, ray ray_in, HitRecord *rec, color *attenuation,
             ray *ray_scattered);

color emitted(Material *, double u, double v, vec3 p);
