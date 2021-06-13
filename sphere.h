#pragma once

#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include <stdbool.h>

// Struct definition for a hittable sphere object
typedef struct {
    vec3 center;
    double radius;
} Sphere;

Sphere sphere_init(vec3, double);

bool sphere_hit(Sphere, ray, double, double, HitRecord *);
