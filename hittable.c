#include "hittable.h"
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
        case SPHERE:
        {
            Sphere s = *((Sphere *)(h.object));
            hit = sphere_intersect(s, r, t_min, t_max, rec);
            break;
        }
        default:
            fprintf(stderr, "ERROR: Unknown object type encountered in hittable_intersect()!\n");
            exit(1);
            break;
    }

    return hit;
}

//
// Constructs a bounding box for a hittable, passed back through the AABB pointer
// output_box. True is returned because the object has a definable bounding box.
//
bool hittable_bounding_box(Hittable h, AABB* output_box) {
    vec3 min;
    vec3 max;
    switch(h.type) {
        case SPHERE: {
            Sphere *s = ((Sphere *)(h.object));
            min = v3_sub(s->center, v3_init(s->radius, s->radius, s->radius));
            max = v3_add(s->center, v3_init(s->radius, s->radius, s->radius));

            output_box->min = min;
            output_box->max = max;

            return true;
            break;
        }
        default:
            fprintf(stderr, "ERROR: Invalid hittable type encountered in hittable_bounding_box()\n");
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
