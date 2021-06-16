#include "aabb.h"
#include "util.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//
// Returns a pointer to a newly created AABB.
//
AABB *aabb_create(vec3 min, vec3 max) {
    AABB *box = (AABB *)malloc(sizeof(AABB));
    assert(box != NULL);

    box->min = min;
    box->max = max;

    return box;
}

//
// Initializes a bounding box w/o dyanmically allocating any memory.
//
AABB aabb_init(vec3 min, vec3 max) {
    AABB box;
    box.min = min;
    box.max = max;

    return box;
}

//
// Deletes an AABB.
//
void aabb_delete(AABB **box) {
    if (*box) {
        free(*box);
        *box = NULL;
    }
    return;
}

//
// Returns true if a ray hit an AABB, false otherwise.
//
bool aabb_hit(AABB aabb, ray r, double t_min, double t_max) {
    // Comptue t-intervals along the x-axis
    double invD = 1.0 / r.dir.x;
    double t0 = (aabb.min.x - r.orig.x) * invD;
    double t1 = (aabb.max.x - r.orig.x) * invD;

    if (invD < 0.0) {
        swap_double(&t0, &t1);
    }

    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;

    if (t_max <= t_min) {
        return false;
    }

    // Comptue t-intervals along the y-axis
    invD = 1.0 / r.dir.y;
    t0 = (aabb.min.y - r.orig.y) * invD;
    t1 = (aabb.max.y - r.orig.y) * invD;

    if (invD < 0.0) {
        swap_double(&t0, &t1);
    }

    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;

    if (t_max <= t_min) {
        return false;
    }

    // Comptue t-intervals along the z-axis
    invD = 1.0 / r.dir.z;
    t0 = (aabb.min.z - r.orig.z) * invD;
    t1 = (aabb.max.z - r.orig.z) * invD;

    if (invD < 0.0) {
        swap_double(&t0, &t1);
    }

    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;

    if (t_max <= t_min) {
        return false;
    }

    return true;
}

//
// Computes the bounding box of two boxes.
//
AABB surrounding_box(AABB box0, AABB box1) {
    vec3 small = v3_init(fmin(box0.min.x, box1.min.x), fmin(box0.min.y, box1.min.y),
                         fmin(box0.min.z, box1.min.z));
    vec3 big = v3_init(fmax(box0.max.x, box1.max.x), fmax(box0.max.y, box1.max.y),
                       fmax(box0.max.z, box1.max.z));
    AABB surrounding_box = {small, big};
    return surrounding_box;
}

//
// Prints the lower and upper bound of the given AABB.
//
void aabb_print(AABB box) {
    printf("Bounding box -> ");
    printf("Min: (%f, %f, %f), Max: (%f, %f, %f)\n", box.min.x, box.min.y, box.min.z,
           box.max.x, box.max.y, box.max.z);
    return;
}
