#include "aabb.h"
#include "util.h"

#include <assert.h>
#include <stdlib.h>

struct AABB {
    vec3 min, max;
};

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
bool aabb_hit(AABB *aabb, ray r, double t_min, double t_max) {
    // Comptue t-intervals along the x-axis
    double invD = 1.0 / r.dir.x;
    double t0 = (aabb->min.x - r.orig.x) * invD;
    double t1 = (aabb->max.x - r.orig.x) * invD;

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
    t0 = (aabb->min.y - r.orig.y) * invD;
    t1 = (aabb->max.y - r.orig.y) * invD;

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
    t0 = (aabb->min.z - r.orig.z) * invD;
    t1 = (aabb->max.z - r.orig.z) * invD;

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
