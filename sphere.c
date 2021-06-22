#include "sphere.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Constructor for a sphere
Sphere *sphere_create(vec3 center, double radius, Material *material) {
    Sphere *s = (Sphere *)malloc(sizeof(Sphere));
    if (s) {
        s->center = center;
        s->radius = radius;
        s->material = material;
    }
    return s;
}

void sphere_delete(Sphere **s) {
    if (*s) {
        free(*s);
        *s = NULL;
    }
    return;
}

// Ray-sphere intersection function. Returns true if a hit occurred and false
// otherwise. If a hit occurs, the hit information is kept track of within
// the hit_rec pointer.
bool sphere_intersect(Sphere s, ray r, double t_min, double t_max, HitRecord *rec) {
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
    rec->material = s.material;

    return true;
}

//
// Constructs a bounding box for a sphere, passed back through the AABB pointer
// output_box. True is returned because the object has a definable bounding box.
//
bool sphere_bounding_box(Sphere s, AABB *output_box) {
    vec3 min = v3_sub(s.center, v3_init(s.radius, s.radius, s.radius));
    vec3 max = v3_add(s.center, v3_init(s.radius, s.radius, s.radius));

    output_box->min = min;
    output_box->max = max;

    return true;
}

//
// Given a point on the unit sphere centered at the origin, computes the (u, v)
// coordinates.
//
void get_sphere_uv(Sphere s, vec3 p, double *u, double *v) {
    double theta = acos(-p.y);
    double phi = atan2(-p.z, p.x) + M_PI;

    *u = phi / 2 * M_PI;
    *v = theta / M_PI;

    return;
}

//
// Prints the information about a given sphere.
//
void sphere_print(Sphere *s) {
    if (s) {
        printf("Sphere: (%f, %f, %f), r = %f\n", s->center.x, s->center.y, s->center.z,
               s->radius);
    }
    return;
}
