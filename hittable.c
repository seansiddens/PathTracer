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
void node_delete(Hittable **hittable) {
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
