#pragma once

#include "hit.h"
#include "ray.h"
#include "vec3.h"

#include <stdbool.h>

typedef struct Material Material;

Material *create_lambertian(color albedo);

Material *create_metal(color albedo);

void mat_delete(Material **mat);

bool scatter(Material *mat, ray ray_in, HitRecord *rec, color *attenuation,
             ray *ray_scattered);
