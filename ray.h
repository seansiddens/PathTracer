#pragma once

#include "vec3.h"

// Struct definition for a ray in 3D space.
// Defined by it's origin and direction.
typedef struct {
    vec3 orig;
    vec3 dir;
} ray;

double ray_at(ray, double);
