#pragma once

#include "aabb.h"
#include "hit.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

#include <stdbool.h>

// Struct definition for a hittable sphere object
typedef struct {
    vec3 center;
    double radius;
    Material *material;
} Sphere;

Sphere *sphere_create(vec3, double, Material *);

void sphere_delete(Sphere **);

bool sphere_intersect(Sphere, ray, double, double, HitRecord *);

bool sphere_bounding_box(Sphere s, AABB *output_box);
