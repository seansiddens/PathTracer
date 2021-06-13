#include "ray.h"

// Returns the point at t along the ray r
// R(t) = orig + t * dir
vec3 ray_at(ray r, double t) {
    vec3 p = v3_add(r.orig, v3_scale(r.dir, t));
    return p;
}
