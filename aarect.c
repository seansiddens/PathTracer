#include "aarect.h"

#include <stdlib.h>

// Constructor for a sphere
XYRect *xy_rect_create(double x0, double x1, double y0, double y1, double z,
                       Material *material) {
    XYRect *rect = (XYRect *)malloc(sizeof(XYRect));
    if (rect) {
        rect->x0 = x0;
        rect->x1 = x1;
        rect->y0 = y0;
        rect->y1 = y1;
        rect->z = z;
        rect->material = material;
    }
    return rect;
}

void xy_rect_delete(XYRect **rect) {
    if (*rect) {
        free(*rect);
        *rect = NULL;
    }
    return;
}

//
// Creates the bounding box around an xy-aligned rectangle. The bounding box must have
// a non-zero width in each dimension, so pad the Z dimension a small amount.
//
bool xy_rect_bounding_box(XYRect rect, AABB *output_box) {
    output_box->min = v3_init(rect.x0, rect.y0, rect.z - 0.0001);
    output_box->max = v3_init(rect.x1, rect.y1, rect.z + 0.0001);

    return true;
}

//
// Intersects a ray with an xy-aligned rectangle. Hit information is recorded in
// the HitRecord pointer. Returns true if hit occurred, false otherwise.
//
bool xy_rect_intersect(XYRect rect, ray r, double t_min, double t_max, HitRecord *rec) {
    // Get t-value of where hit occurred
    double t = (rect.z - r.orig.z) / r.dir.z;
    if (t < t_min || t > t_max) {
        // Outside of t-range
        return false;
    }

    // Figure out where hit occurred
    double x = r.orig.x + t * r.dir.x;
    double y = r.orig.y + t * r.dir.y;
    if (x < rect.x0 || x > rect.x1 || y < rect.y0 || y > rect.y1) {
        // Missed rect
        return false;
    }

    // Is a valid hit, record hit info
    rec->u = (x - rect.x0) / (rect.x1 - rect.x0);
    rec->v = (y - rect.y0) / (rect.y1 - rect.y0);
    rec->t = t;
    vec3 outward_normal = v3_init(0, 0, 1);
    set_face_normal(rec, r, outward_normal);
    rec->material = rect.material;
    rec->p = ray_at(r, t);
    return true;
}
