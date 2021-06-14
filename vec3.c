#include "vec3.h"
#include "util.h"

#include <math.h>
#include <stdio.h>

//
// Returns a newly initialized vector
//
vec3 v3_init(double x, double y, double z) {
    vec3 v = {x, y, z};
    return v;
}

//
// Returns the squared length of a given vector
//
double v3_length_squared(vec3 v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

//
// Returns the length of a given vector
//
double v3_length(vec3 v) { return sqrt(v3_length_squared(v)); }

//
// Returns the sum of two vectors
//
vec3 v3_add(vec3 u, vec3 v) {
    vec3 w;
    w.x = u.x + v.x;
    w.y = u.y + v.y;
    w.z = u.z + v.z;
    return w;
}

//
// Returns u minus v
//
vec3 v3_sub(vec3 u, vec3 v) {
    vec3 w;
    w.x = u.x - v.x;
    w.y = u.y - v.y;
    w.z = u.z - v.z;
    return w;
}

//
// Returns the dot product between two vectors
//
double v3_dot(vec3 u, vec3 v) { return u.x * v.x + u.y * v.y + u.z * v.z; }

//
// Returns the cross product of u and v
//
vec3 v3_cross(vec3 u, vec3 v) {
    vec3 w;
    w.x = u.y * v.z - v.y * u.z;
    w.y = v.x * u.z - u.x * v.z;
    w.z = u.x * v.y - v.x * u.y;
    return w;
}

//
// Returns the product of the vector v and scalar a
//
vec3 v3_scale(vec3 v, double a) {
    vec3 w;
    w.x = v.x * a;
    w.y = v.y * a;
    w.z = v.z * a;
    return w;
}

//
// Returns the unit vector of a given vector
//
vec3 v3_unit_vector(vec3 v) { return v3_scale(v, 1.0 / v3_length(v)); }

//
// Normalizes the given vector
//
void v3_normalize(vec3 *v) {
    double l = v3_length(*v);
    v->x = v->x / l;
    v->y = v->y / l;
    v->z = v->z / l;
    return;
}

//
// Returns a vector linearly interpolated between two vectors given the
// parameter t.
//
// blended_value = (1 - t) * start_value + t * end_value
//
vec3 v3_lerp(vec3 start, vec3 end, double t) {
    return v3_add(v3_scale(start, 1.0 - t), v3_scale(end, t));
}

//
// Returns a new vec3 w/ random components in [0, 1).
//
vec3 v3_random_uniform() {
    return v3_init(random_uniform(), random_uniform(), random_uniform());
}

//
// Returns a new vec3 w/ random components in [min, max).
//
vec3 v3_random_range(double min, double max) {
    return v3_init(random_double(min, max), random_double(min, max),
                   random_double(min, max));
}

//
// Returns a random point in the unit sphere.
//
// Approximation of Lambertian reflectance
// We pick a random point the unit cube until we get a valid point within the
// unit sphere.
// Probability is higher close to the normal (scales w/ cos^3(Φ), where Φ is the
// angle from the normal).
//
vec3 random_in_unit_sphere() {
    while (1) {
        vec3 p = v3_random_range(-1, 1);
        if (v3_length_squared(p) >= 1.0) {
            continue;
        }
    }
}

//
// Prints a given vector.
//
void v3_print(vec3 v) {
    printf("(%f, %f, %f)\n", v.x, v.y, v.z);
    return;
}
