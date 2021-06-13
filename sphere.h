#pragma once

#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include <stdbool.h>

// Struct definition for a hittable sphere object
typedef struct {
    vec3 center;
    double radius;
} sphere;

sphere sphere_init(vec3, double);

bool sphere_hit(sphere, ray, double, double, hit_rec *);
