#pragma once

#include "hit.h"
#include "ray.h"
#include "vec3.h"
#include <stdbool.h>

// Struct definition for a hittable sphere object
typedef struct {
    vec3 center;
    double radius;
} Sphere;

Sphere *sphere_create(vec3, double);

void sphere_delete(Sphere **);

bool sphere_intersect(Sphere, ray, double, double, HitRecord *);
