#pragma once

#include "ray.h"
#include "vec3.h"

#include <stdbool.h>

// Struct definition for a hit record. Records the point at which the hit
// occurred, surface normal of the hit, and the t value of the hit.
// We also always want the normal to face opposite of the intersected ray.
// This is done using the set_face_normal function. If the incoming ray
// was outside the object, front_face is set to true. If not, it is
// set to false.
typedef struct {
    vec3 p;
    vec3 normal;
    double t;
    bool front_face;
} HitRecord;

void set_face_normal(HitRecord *, ray, vec3);
