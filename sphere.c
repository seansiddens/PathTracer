#include "sphere.h"

#include <math.h>

// Init function for a sphere
sphere sphere_init(vec3 center, double radius) {
    sphere s = {center, radius};
    return s;
}

// Ray-sphere intersection function. Returns true if a hit occurred and false
// otherwise. If a hit occurs, the hit information is kept track of within
// the hit_rec pointer.
bool hit_sphere(sphere s, ray r, double t_min, double t_max, hit_rec *rec) {
    // Solve quadratic
    vec3 oc = v3_sub(r.orig, s.center);
    double a = v3_length_squared(r.dir);
    double half_b = v3_dot(oc, r.dir);
    double c = v3_length_squared(oc) - s.radius * s.radius;

    double discriminant = half_b * half_b - a * c;
    // No real solutions, so hit did not occur
    if (discriminant < 0) {
        return false;
    }
    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max) {
        // Root lies outside acceptable range, so check other root
        root = (-half_b + sqrtd) / a;
        if (root < t_min || root > t_max) {
            // Other root also lies outside range, so hit did not occur.
            return false;
        }
    }

    // Hit occurred, so record hit information
    rec->t = root;
    rec->p = ray_at(r, rec->t);
    vec3 outward_normal = v3_scale(v3_sub(rec->p, s.center), 1.0 / s.radius);
    set_face_normal(rec, r, outward_normal);

    return true;
}
