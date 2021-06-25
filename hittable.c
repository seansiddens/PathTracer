#include "hittable.h"
#include "aarect.h"
#include "sphere.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//
// Constructor for a hittable.
//
Hittable *hittable_create(void *object, HittableType type) {
    Hittable *hittable = (Hittable *)malloc(sizeof(Hittable));
    assert(hittable != NULL);
    hittable->object = object;
    hittable->type = type;
    return hittable;
}

//
// Destructor for a hittable.
//
void hittable_delete(Hittable **hittable) {
    if (*hittable) {
        if ((*hittable)->type == SPHERE) {
            Sphere *s = (Sphere *)(*hittable)->object;
            sphere_delete(&s);
        }
        free(*hittable);
        *hittable = NULL;
    }
    return;
}

bool hittable_intersect(Hittable h, ray r, double t_min, double t_max, HitRecord *rec) {
    bool hit = false;
    switch (h.type) {
    case SPHERE: {
        hit = sphere_intersect(*((Sphere *)(h.object)), r, t_min, t_max, rec);
        break;
    }
    case XYRECT: {
        hit = xy_rect_intersect(*((XYRect *)(h.object)), r, t_min, t_max, rec);
        break;
    }
    default:
        fprintf(stderr,
                "ERROR: Unknown object type encountered in hittable_intersect()!\n");
        exit(1);
        break;
    }

    return hit;
}

//
// Constructs a bounding box for a hittable, passed back through the AABB pointer
// output_box. True is returned because the object has a definable bounding box.
//
bool hittable_bounding_box(Hittable h, AABB *output_box) {
    switch (h.type) {
    case SPHERE:
        return sphere_bounding_box(*((Sphere *)(h.object)), output_box);
        break;
    case XYRECT:
        return xy_rect_bounding_box(*((XYRect *)(h.object)), output_box);
        break;
    default:
        fprintf(stderr,
                "ERROR: Invalid hittable type encountered in hittable_bounding_box()\n");
        exit(1);
        break;
    }
}

//
// Prints a hittable struct - calls the corresponding print function of the object type.
//
void hittable_print(Hittable *h) {
    if (h) {
        switch (h->type) {
        case SPHERE:
            sphere_print((Sphere *)h->object);
            break;
        default:
            fprintf(stderr,
                    "ERRROR: Unknown object type encountered in hittable_print()!\n");
            break;
        }
    }
}
