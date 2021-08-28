#pragma once

#include "aabb.h"
#include "hit.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

#include <stdbool.h>

typedef struct Sphere Sphere;

// Struct definition for a hittable sphere object
struct Sphere {
    vec3 center;
    double radius;
    Material *material;
};

Sphere *sphere_create(vec3 center, double radius, Material *);

void sphere_delete(Sphere **);

bool sphere_intersect(Sphere s, ray r, double t_min, double t_max, HitRecord *rec);

bool sphere_bounding_box(Sphere s, AABB *output_box);

void get_sphere_uv(Sphere s, vec3 p, double *u, double *v);

void sphere_print(Sphere *s);
