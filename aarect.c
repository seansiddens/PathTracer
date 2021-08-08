#include "aarect.h"

#include <stdlib.h>
#include <stdio.h>

// --------------------------------------------------------------------------------------
// XY-Aligned Rectangle
// --------------------------------------------------------------------------------------

//
// Constructor for an xy-aligned rectangle
//
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

// 
// Destructor for an xy-aligned rectangle
//
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

    /*printf("Hit xy-rect!\n");*/

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

//
// Prints the information about a given xy-rect
//
void xy_rect_print(XYRect *rect) {
    if (rect) {
        printf("XyRect: x0 = %f, x1 = %f, y0 = %f, y1 = %f, z = %f\n", rect->x0,  rect->x1, rect->y0, rect->y1, rect->z);
    }
    return;
}

// --------------------------------------------------------------------------------------
// XZ-Aligned Rectangle
// --------------------------------------------------------------------------------------

// Constructor for an xz-aligned rectangle
//
XZRect *xz_rect_create(double x0, double x1, double z0, double z1, double y,
                       Material *material) {
    XZRect *rect = (XZRect *)malloc(sizeof(XZRect));
    if (rect) {
        rect->x0 = x0;
        rect->x1 = x1;
        rect->z0 = z0;
        rect->z1 = z1;
        rect->y = y;
        rect->material = material;
    }
    return rect;
}

// 
// Destructor for an xz-aligned rectangle
//
void xz_rect_delete(XZRect **rect) {
    if (*rect) {
        free(*rect);
        *rect = NULL;
    }
    return;
}

//
// Creates the bounding box around an xz-aligned rectangle.
//
bool xz_rect_bounding_box(XZRect rect, AABB *output_box) {
    output_box->min = v3_init(rect.x0, rect.z0, rect.y - 0.0001);
    output_box->max = v3_init(rect.x1, rect.z1, rect.y + 0.0001);

    return true;
}

//
// Intersects a ray with an xz-aligned rectangle. Hit information is recorded in
// the HitRecord pointer. Returns true if hit occurred, false otherwise.
//
bool xz_rect_intersect(XZRect rect, ray r, double t_min, double t_max, HitRecord *rec) {
    // Get t-value of where hit occurred
    double t = (rect.y - r.orig.y) / r.dir.y;
    if (t < t_min || t > t_max) {
        // Outside of t-range
        return false;
    }

    // Figure out where hit occurred
    double x = r.orig.x + t * r.dir.x;
    double z = r.orig.z + t * r.dir.z;
    if (x < rect.x0 || x > rect.x1 || z < rect.z0 || z > rect.z1) {
        // Missed rect
        return false;
    }

    printf("Hit xz-rect!\n");

    // Is a valid hit, record hit info
    rec->u = (x - rect.x0) / (rect.x1 - rect.x0);
    rec->v = (z - rect.z0) / (rect.z1 - rect.z0);
    rec->t = t;
    vec3 outward_normal = v3_init(0, 1, 0);
    set_face_normal(rec, r, outward_normal);
    rec->material = rect.material;
    rec->p = ray_at(r, t);
    return true;
}

//
// Prints the information about a given xz-rect
//
void xz_rect_print(XZRect *rect) {
    if (rect) {
        printf("XZRect: x0 = %f, x1 = %f, z0 = %f, z1 = %f, y = %f\n", rect->x0,  rect->x1, rect->z0, rect->z1, rect->y);
    }
    return;
}

// --------------------------------------------------------------------------------------
// YZ-Aligned Rectangle
// --------------------------------------------------------------------------------------

//
// Constructor for an yz-aligned rectangle
//
YZRect *yz_rect_create(double y0, double y1, double z0, double z1, double x,
                       Material *material) {
    YZRect *rect = (YZRect *)malloc(sizeof(YZRect));
    if (rect) {
        rect->y0 = y0;
        rect->y1 = y1;
        rect->z0 = z0;
        rect->z1 = z1;
        rect->x = x;
        rect->material = material;
    }
    return rect;
}

// 
// Destructor for an yz-aligned rectangle
//
void yz_rect_delete(YZRect **rect) {
    if (*rect) {
        free(*rect);
        *rect = NULL;
    }
    return;
}

//
// Creates the bounding box around an yz-aligned rectangle. The bounding box must have
// a non-zero width in each dimension, so pad the X dimension a small amount.
//
bool yz_rect_bounding_box(YZRect rect, AABB *output_box) {
    output_box->min = v3_init(rect.y0, rect.z0, rect.x - 0.0001);
    output_box->max = v3_init(rect.y1, rect.z1, rect.x + 0.0001);

    return true;
}

//
// Intersects a ray with an yz-aligned rectangle. Hit information is recorded in
// the HitRecord pointer. Returns true if hit occurred, false otherwise.
//
bool yz_rect_intersect(YZRect rect, ray r, double t_min, double t_max, HitRecord *rec) {
    // Get t-value of where hit occurred
    double t = (rect.x - r.orig.x) / r.dir.x;
    if (t < t_min || t > t_max) {
        // Outside of t-range
        return false;
    }

    // Figure out where hit occurred
    double y = r.orig.y + t * r.dir.y;
    double z = r.orig.z + t * r.dir.z;
    if (y < rect.y0 || y > rect.y1 || z < rect.z0 || z > rect.z1) {
        // Missed rect
        return false;
    }

    printf("Hit xz-rect!\n");

    // Is a valid hit, record hit info
    rec->u = (y - rect.y0) / (rect.y1 - rect.y0);
    rec->v = (z - rect.z0) / (rect.z1 - rect.z0);
    rec->t = t;
    vec3 outward_normal = v3_init(1, 0, 0);
    set_face_normal(rec, r, outward_normal);
    rec->material = rect.material;
    rec->p = ray_at(r, t);
    return true;
}
