#pragma once

#include "vec3.h"
#include "ray.h"

// Struct definition for a hit record. Records the point at which the hit occurred,
// surface normal of the hit, and the t value of the hit
typedef struct {
    vec3 p;
    vec3 normal;
    double t;
} hit_rec;
